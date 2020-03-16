
#ifndef OPENGL_FILEUTILS_H
#define OPENGL_FILEUTILS_H

#include <glad/glad.h>
#include <string>

class FileUtils {
public:
  static void savePicture(GLbyte *arr, int size, const std::string &file);
};

#endif // OPENGL_FILEUTILS_H
