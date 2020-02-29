# learn opengl

## 蹒跚学步
### 绘制三角形
- display: displaymanager.h 
- pipeline: shader.h
  - vertex shader
  - fragment shader
- VAO&VBO&EBO: load.h AbstractLoader.class - > VAOLoader.class
- shaders and basic glsl
<img src="https://github.com/tingxia1028/learnopengl/blob/master/readmeimgs/first_practice_triangles.png" alt="triangles" width="200" height="150" />

### texture: AbstractLoader.class - > TextureLoader.class
- texture coordinates
- swapping and filtering
- mipmap

### transformation
- translate, rotate, scale : transformation.h
  - rotate
    - Euler
    - rotate around an arbitrary unit axis: 分解成垂直和水平于旋转轴的向量分别计算
- coordinates
  object space -> world space -> camera space -> clip space -> screen space 
  - object
    - local - world space: model matrix
       - vertex value
       - position
       - rotate
  
  - camera.h
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