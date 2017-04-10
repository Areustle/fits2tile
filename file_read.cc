#include <stdio.h>

int main() {
  FILE* fd = fopen("my_workspace/sparse_arrays/my_array_B/__484d7ede19a5139656956459200_1491841579143/__coords.tdb", "rb");
  float f;
  if(read(fd,&f,sizeof(f))==sizeof(f))
        printf("%f\n",f);
  else
        printf("done\n");
}
