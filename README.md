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
