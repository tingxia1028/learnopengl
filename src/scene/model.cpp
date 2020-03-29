
#include "model.h"
#include "../light/light.h"
#include "../renderengine/stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>

Model::Model(const std::string &path, Transformation &transformation)
    : transformation(transformation) {
  loadModel(path);
}

void Model::loadModel(const std::string &path) {
  Assimp::Importer importer;
  const aiScene *scene =
      importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs |
                                  aiProcess_CalcTangentSpace);
  // check for errors
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) // if is Not Zero
  {
    std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
    return;
  }
  // retrieve the directory path of the filepath
  directory = path.substr(0, path.find_last_of('/'));

  // process ASSIMP's root node recursively
  processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    // the node object only contains indices to index the actual objects in the
    // scene. the scene contains all the data, node is just to keep stuff
    // organized (like relations between nodes).
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(mesh, scene));
  }
  // after we've processed all of the meshes (if any) we then recursively
  // process each of the children nodes
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene);
  }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
  // data to fill
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

  // Walk through each of the mesh's vertices
  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;
    glm::vec3 vector;

    // positions
    vector.x = mesh->mVertices[i].x;
    vector.y = mesh->mVertices[i].y;
    vector.z = mesh->mVertices[i].z;
    vertex.position = vector;
    // normals
    vector.x = mesh->mNormals[i].x;
    vector.y = mesh->mNormals[i].y;
    vector.z = mesh->mNormals[i].z;
    vertex.normal = vector;
    // texture coordinates
    if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
    {
      glm::vec2 vec;
      // a vertex can contain up to 8 different texture coordinates. We thus
      // make the assumption that we won't use models where a vertex can have
      // multiple texture coordinates so we always take the first set (0).
      vec.x = mesh->mTextureCoords[0][i].x;
      vec.y = mesh->mTextureCoords[0][i].y;
      vertex.textureCoord = vec;
    } else {
      vertex.textureCoord = glm::vec2(0.0f, 0.0f);
    }
    // tangent
    vector.x = mesh->mTangents[i].x;
    vector.y = mesh->mTangents[i].y;
    vector.z = mesh->mTangents[i].z;
    vertex.tangent = vector;
    // bitangent
    vector.x = mesh->mBitangents[i].x;
    vector.y = mesh->mBitangents[i].y;
    vector.z = mesh->mBitangents[i].z;
    vertex.bitangent = vector;
    vertices.push_back(vertex);
  }

  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }

  // process materials
  aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
  Material dstMaterial = Material();
  std::vector<Texture> &textures = dstMaterial.textures;

  // 1. diffuse maps
  std::vector<Texture> diffuseMaps = loadMaterialTextures(
      material, aiTextureType_DIFFUSE, Texture::TextureType::DIFFUSE);
  if (diffuseMaps.size() > 0) {
    dstMaterial.hasDiffuseTex = true;
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
  }

  // 2. specular maps
  std::vector<Texture> specularMaps = loadMaterialTextures(
      material, aiTextureType_SPECULAR, Texture::TextureType::SPECULAR);
  if (specularMaps.size() > 0) {
    dstMaterial.hasSpecularTex = true;
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
  }

  // 3. normal maps
  std::vector<Texture> normalMaps = loadMaterialTextures(
      material, aiTextureType_HEIGHT, Texture::TextureType::NORMAL);
  if (normalMaps.size() > 0) {
    dstMaterial.hasNormalMap = true;
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
  }

  // 4. displacement maps
  //  std::vector<Texture> normalMaps = loadMaterialTextures(
  //      material, aiTextureType_DISPLACEMENT, TextureType::HEIGHT);
  //  if (normalMaps.size() > 0) {
  //    dstMaterial.hasDepthMap = true;
  //    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
  //  }

  aiColor3D color(0.0f, 0.0f, 0.0f);

  // 5. diffuse_color
  material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
  dstMaterial.diffuse = transformAIcolor(color);

  // 6. specular color
  material->Get(AI_MATKEY_COLOR_SPECULAR, color);
  dstMaterial.specular = transformAIcolor(color);

  // 7. shininess
  float floatParam;
  material->Get(AI_MATKEY_SHININESS, floatParam);
  dstMaterial.shininess = floatParam;

  // 8. opacity
  material->Get(AI_MATKEY_OPACITY, floatParam);
  dstMaterial.opacity = floatParam;

  // return a mesh object created from the extracted mesh data
  return Mesh(mesh->mName.C_Str(), vertices, indices,
              std::vector<Material>{dstMaterial});
}

// checks all material textures of a given type and loads the textures if
// they're not loaded yet. the required info is returned as a Texture struct.
std::vector<Texture>
Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                            Texture::TextureType typeName) {

  std::vector<Texture> textures;
  for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
    aiString str;
    mat->GetTexture(type, i, &str);
    // check if texture was loaded before and if so, continue to next iteration:
    // skip loading a new texture

    if (loadedTextures.count(str.C_Str()) <=
        0) { // if texture hasn't been loaded already, load it
      std::string filename = directory + '/' + std::string(str.C_Str());
      Texture texture(GL_TEXTURE_2D, filename, typeName);
      texture.load();
      textures.push_back(texture);
      loadedTextures[str.C_Str()] = texture;
    } else {
      textures.push_back(loadedTextures[str.C_Str()]);
    }
  }
  return textures;
}

glm::vec3 Model::transformAIcolor(aiColor3D aiColor3D) {
  return glm::vec3(aiColor3D.r, aiColor3D.g, aiColor3D.b);
}

void Model::draw(ShaderProgram &shaderProgram, std::vector<Light *> &lights,
                 bool withMaterials) {
  glm::mat4 modelTransform = transformation.getTransformationMat();
  shaderProgram.uniformSetMat4("model", modelTransform);

  for (unsigned int j = 0; j < meshes.size(); ++j) {
    meshes[j].draw(shaderProgram, withMaterials, lights);
  }
}
