# learn opengl

## 蹒跚学步
### 绘制三角形
- display: displaymanager.h 
- pipeline: shader.h
  - vertex shader
  - fragment shader
- VAO&VBO&EBO: load.h AbstractLoader.class - > VAOLoader.class
- shaders and basic glsl
<img src="https://github.com/tingxia1028/learnopengl/blob/master/readmeimgs/first_practice_triangles.png" alt="triangles" width="300" height="200" />

### texture
AbstractLoader.class - > TextureLoader.class
- texture coordinates
- swapping and filtering
- mipmap
<img src="https://github.com/tingxia1028/learnopengl/blob/master/readmeimgs/texture.png" alt="texture" width="300" height="200" />

### transformation
- translate, rotate, scale : transformation.h, rotate.h
  - rotate
    - Euler
    - rotate around an arbitrary unit axis
- coordinates
  object space -> world space -> camera space -> clip space -> screen space 
  - object : model.h
    - local - world space: model matrix
       - vertex value
       - position
       - rotate
  
  - camera : camera.h
    - world - > view space: view matrix
        - position
        - up axis
        - right axis
        - camera direction (front)
    - view - > clip space: perspective matrix
        - fov
        - width/height
        - near
        - far
  
  - screen
    - clip space - > screen space:  viewport transformation
       - start point
       - width
       - height
<img src="https://github.com/tingxia1028/learnopengl/blob/master/readmeimgs/transformation.png" alt="transformation" width="300" height="200" />

### light
- colors
  color of an object: the amount of each color component it reflects from a light source
- light :  light.h
  - ambient
  - diffuse
  - specular
  - Phong rendering & Gouraud rendering
- material
  - texture
  - ambient:  the color the surface reflects under ambient lighting
  - diffuse: the color of the surface under diffuse lighting
     diffuse color / diffuse texture
  - specular: the color of the specular highlight 
  - shininess: scattering/radius of the specular highlight
- lighting maps
  - diffuse maps
    texture image represents all of the object's diffuse colors
  - specular maps
    texture that defines the specular intensities of each part of the object
- light casters : extends from light
  - directional light : directionallight.h
    - light direction
  - point light : poinlight.h
    - attenuation
  - spotlight : spotlight.h
    - light dir
    - spot dir 
    - phi
    - theta (cos(theta) is better)
    - flashlight : flashlight.h
     its position and direction continually updated based on the player's position and orientation.
    - smooth/soft edges
      - inner cone and outer cone
      - interpolate the intensity 
      - glsl built-in clamp(value, min, max)
-  multiple lights
   - glsl  function must be declared before used.
   - glsl array
   - duplicated calculation
    eg. reflect vector, diffuse and specular terms, and sampling the material textures  
<img src="https://github.com/tingxia1028/learnopengl/blob/master/readmeimgs/light.png" alt="light" width="300" height="200" />

### model loading
- model file format
  - obj 
    - .obj
    - .mtl
- assimp
   scene (meshes, materials) -> node (meshIndices) -> mesh(vertices, normals, texCoords, materialIndex)
- scene -> model -> mesh 
	- mesh : mesh.h
	   - vertex
	   - texture : material.h  
	      add diffuseColor, specularColor to use if texture does not exist
	   - change from render.class to render all models 
		     render(Scene &scene, ShaderProgram &shaderProgram) 
		     to 
		     each model.class has draw(ShaderProgram &shaderProgram) function
		     (same with light.h)
	- model :  model.h
       - meshes
    - scene : scene.h
        - models
        - camera
        - lights
- optimization
	 - load texture only once
<img src="https://github.com/tingxia1028/learnopengl/blob/master/readmeimgs/modelloading.png" alt="modelloading" width="260" height="300" />

## more real walk
### depth testing
- non-linear z value calculated (1/z)
 z-value of the built-in gl_FragCoord vector in the fragment shader contains the depth value of that particular fragment
- z-fighting
  - small offset between near objects 
    - glPolygonOffset
  - set near plane far away
  - larger precision of depth buffer
  
### stencil testing
- stencil buffer
- object outlining
- more practical way to draw outline
  - glPolygonOffset
  - postprocessing

### blend
 - alpha
 - sorted all transparent models with distance to camera
   draw the furthest first and the nearest last
 - order-independent-transparency

### face culling
- winding order

### framebuffer
- what is
    - color buffer
    - depth buffer
    - stencil buffer
    - render buffer  write-only
- for what
    freely access each of the pixels of the completely rendered scene as a single texture image

### cubemaps
- why use cubemaps
  - the same as rendring a huge box filled in the camera space
  - has 6 face textures for different direction
  - first render models in the scene, then the skybox
  - vertex color of models can be indexed/sampled using a direction vector
- using framebuffer for dynamic environment maps
<img src="https://github.com/tingxia1028/learnopengl/blob/master/readmeimgs/skybox.png" alt="skybox" width="300" height="200" />

### advanced glsl
- vertex shader
    - gl_PointSize
    - gl_VertexID
- fragment shader
    - gl_FragCoord
    - gl_FrontFacing
    - gl_FragDepth  can be wrote
- interface blocks
- uniform buffer objects : share between shaders
   depend on index, offset

### geometry shader
takes as input a set of vertices that form a single primitive e.g. a point or a triangle.

<img src="https://github.com/tingxia1028/learnopengl/blob/master/readmeimgs/geometryshader.png" alt="gs" width="260" height="300" />

### instancing

### anti-aliasing
- why happened
  continuous -> discrete
- super sample anti-aliasing (SSAA)
  hight resolution  -> down sampling
- multisample anti-aliasing (MSAA)
  per pixel multi subsamples -> determine the color of the pixel
  - also use in depth test and stencil test
  - offScreen MSAA
  - custom anti-aliasing algorithm
- FXAA, SMAA, TXAA
