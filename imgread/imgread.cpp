//
// std test
//

#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


int main(int argc, char *argv[])
{
  char *filename = "test.jpg";
  char *outfilename = "output.bmp";
  unsigned char *data;
  int x, y, n;

  if (argc > 1)
    filename = argv[1];
  if (argc > 2)
    outfilename = argv[2];

  data = stbi_load(filename, &x, &y, &n, 0);
  printf("image size = (%d, %d, %d)\n", x, y, n);

  int err = stbi_write_bmp(outfilename, x, y, n, data);

}
