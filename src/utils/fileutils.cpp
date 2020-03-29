
#include "fileutils.h"
#include <iostream>

void FileUtils::savePicture(GLbyte *arr, int size, const std::string &file) {
  FILE *pFile = fopen(file.c_str(), "wt");
  if (!pFile) {
    std::cout << "picture " << file << " open failed" << std::endl;
    return;
  }

  for (int i = 0; i < size; i++) {
    if (arr[i] == -1) {
      arr[i] = 255;
    }
  }

  fwrite(arr, size, 1, pFile);
  fclose(pFile);
}