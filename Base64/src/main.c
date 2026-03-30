#include "../../inc/libcypher_io.h"
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static const char BASE64_TABLE[]=
{
  'A','B','C','D','E','F','G','H',
  'I','J','K','L','M','N','O','P',
  'Q','R','S','T','U','V','W','X',
  'Y','Z','a','b','c','d','e','f',
  'g','h','i','j','k','l','m','n',
  'o','p','q','r','s','t','u','v',
  'w','x','y','z','0','1','2','3',
  '4','5','6','7','8','9','+','/'
};

static const int BASE64_INV_TABLE[]=
{
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,62,0,0,63,
  52,53,54,55,56,57,58,59,
  0,0,0,0,0,0,0,0,
  0,0,1,2,3,4,5,6,
  7,8,9,10,11,12,13,14,
  15,16,17,18,19,20,21,22,
  23,24,25,0,0,0,0,0,
  0,26,27,28,29,30,31,32,
  33,34,35,36,37,38,39,40,
  41,42,43,44,45,46,47,48,
  49,50,51
};

void addBase64Pading(CY_BUFF inbuff, CY_BUFF *outbuff)
{
  switch ((3 - inbuff.head.cy_len % 3) % 3)
  {
    case 0:
        outbuff->head.cy_len = inbuff.head.cy_len;
        outbuff->head.cy_pad_len = 0;
        outbuff->buffer = malloc(outbuff->head.cy_len);
      break;

    case 1:
        outbuff->head.cy_len = inbuff.head.cy_len + 1;
        outbuff->head.cy_pad_len = 1;
        outbuff->buffer = malloc(outbuff->head.cy_len);
        outbuff->buffer[outbuff->head.cy_len - 1] = 0;
      break;
    
    case 2:
        outbuff->head.cy_len = inbuff.head.cy_len + 2;
        outbuff->head.cy_pad_len = 2;
        outbuff->buffer = malloc(outbuff->head.cy_len);
        outbuff->buffer[outbuff->head.cy_len - 2] = 0;
        outbuff->buffer[outbuff->head.cy_len - 1] = 0;
      break;
  }
  memcpy(outbuff->buffer, inbuff.buffer, inbuff.head.cy_len);
}

void convertToBase64(CY_BUFF inbuff, CY_BUFF *outbuff)
{
  CY_BUFF tmp;
  addBase64Pading(inbuff, &tmp);
  size_t chunkz = tmp.head.cy_len / 3;
  outbuff->head.cy_pad_len = tmp.head.cy_pad_len;
  outbuff->head.cy_len = 4 * chunkz;
  outbuff->buffer = malloc(outbuff->head.cy_len);
  uint32_t num = 0;
  for (size_t i = 0; i < chunkz; i++)
  {
    num = (((uint32_t)tmp.buffer[3 * i]) << 0x10) | (((uint32_t)tmp.buffer[3 * i + 1]) << 0x08) | (((uint32_t)tmp.buffer[3 * i + 2]) << 0x00);
    outbuff->buffer[4 * i + 0] = BASE64_TABLE[(uint8_t)((num >> 18) & 0x3F)];
    outbuff->buffer[4 * i + 1] = BASE64_TABLE[(uint8_t)((num >> 12) & 0x3F)];
    outbuff->buffer[4 * i + 2] = BASE64_TABLE[(uint8_t)((num >> 6 ) & 0x3F)];
    outbuff->buffer[4 * i + 3] = BASE64_TABLE[(uint8_t)((num >> 0 ) & 0x3F)];
  }
  for (size_t i = 1; i <= outbuff->head.cy_pad_len; i++)
    outbuff->buffer[outbuff->head.cy_len - i] = '=';
  free(tmp.buffer);
}

void convertFromBase64(CY_BUFF inbuff, CY_BUFF *outbuff)
{
  inbuff.head.cy_pad_len = inbuff.buffer[inbuff.head.cy_len - 1] == '=' ? inbuff.buffer[inbuff.head.cy_len - 2] == '=' ? 2 : 1 : 0;
  size_t chunkz = (inbuff.head.cy_len) / 4;
  outbuff->head.cy_len = 3 * chunkz;
  outbuff->buffer = malloc(outbuff->head.cy_len);
  uint32_t num = 0;
  for (size_t i = 0; i < chunkz; i++)
  {
    num = (((uint32_t)BASE64_INV_TABLE[inbuff.buffer[4 * i + 0]]) << 18) |
          (((uint32_t)BASE64_INV_TABLE[inbuff.buffer[4 * i + 1]]) << 12) |
          (((uint32_t)BASE64_INV_TABLE[inbuff.buffer[4 * i + 2] == '=' ? 0 : inbuff.buffer[4 * i + 2]]) << 6 ) |
          (((uint32_t)BASE64_INV_TABLE[inbuff.buffer[4 * i + 3] == '=' ? 0 : inbuff.buffer[4 * i + 3]]) << 0 ) ;
    outbuff->buffer[3 * i + 0] = (uint8_t)((num >> 0x10) & 0xFF);
    outbuff->buffer[3 * i + 1] = (uint8_t)((num >> 0x08) & 0xFF);
    outbuff->buffer[3 * i + 2] = (uint8_t)((num >> 0x00) & 0xFF);
  }
  outbuff->head.cy_len -= inbuff.head.cy_pad_len;
  outbuff->buffer = realloc(outbuff->buffer, outbuff->head.cy_len);
}

int main(void)
{
  CY_BUFF inbuff, outbuff;
  int fd = open("doc/Base64/in.txt",O_RDONLY);
  cy_read(fd, &inbuff);
  close(fd);
  convertToBase64(inbuff, &outbuff);
  fd = open("doc/Base64/out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd < 0) cy_state("open out.txt", -1);
  cy_write(fd, outbuff);
  close(fd);
  return 0;
}