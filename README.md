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

##more real walk
###light
- colors
  color of an object: the amount of each color component it reflects from a light source
- light
  - ambient
  - diffuse
  - specular
- 

