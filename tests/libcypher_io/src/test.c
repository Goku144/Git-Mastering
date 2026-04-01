#include "../../../inc/libcypher_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
  string file;
  if(cy_read_file("libcypher_io/doc/test.in.txt",&file) != 0) return 1;
  if(cy_write_file("libcypher_io/doc/test.out.txt",file) != 0) return 1;
  string fileTest;
  if(cy_read_file("libcypher_io/doc/test.out.txt",&fileTest) != 0) return 1;
  if(file.len != fileTest.len) return 1;
  for (size_t i = 0; i < file.len; i++) 
    if(file.str[i] != fileTest.str[i]) 
      return 1;
  free(file.str);
  free(fileTest.str);
  return 0;
}