#include <cstdio>
#include <fstream>
#include <iostream>

int main() {

  /* std::FILE* fd = std::fopen("my_workspace/sparse_arrays/my_array_B/__484d7ede19a5139656956459200_1491841579143/__coords.tdb", "rb"); */
  std::FILE* fd = std::fopen("my_workspace/sparse_arrays/my_array_B/__b8e8560c1ea6140735133946624_1491844925200/__coords.tdb", "rb");
  /* std::vector<float> buf(); */
  float *buf = new float[2*150561];
  std::fread(&buf[0], sizeof buf[0], 2*150561, fd);
  for(int i=0; i<150561; ++i){
    std::cout << buf[2*i] << "," << buf[2*i+1] << std::endl;
  }
}
