#include "../../inc/libcypher_io.h"
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>


int cy_read(int fd, CY_BUFF *buf)
{
  size_t fs = CY_BUFF_SIZE;
  buf->head.cy_len = 0;
  buf->buffer = malloc(fs);
  if(!buf->buffer) {perror(__func__); return 1;} 
  while (1)
  {
    ssize_t n = read(fd, buf->buffer + buf->head.cy_len, fs - buf->head.cy_len);
    if(n == 0) break;
    if(n < 0) {perror(__func__); return 1;}
    buf->head.cy_len += n;
    if(fs == buf->head.cy_len)
    {
      fs += CY_BUFF_SIZE;
      buf->buffer = realloc(buf->buffer, fs);
      if(!buf->buffer) {perror(__func__); return 1;}
    }
  }
  return 0;
}

int cy_read_line(int fd, CY_BUFF *buf)
{
  size_t fs = CY_BUFF_SIZE;
  buf->head.cy_len = 0;
  buf->buffer = malloc(fs);
  if(!buf->buffer) {perror(__func__); return 1;} 
  while (1)
  {
    ssize_t n = read(fd, buf->buffer + buf->head.cy_len, fs - buf->head.cy_len);
    if(n == 0) break;
    if(n < 0) {perror(__func__); return 1;}
    buf->head.cy_len += n;
    if(fs == buf->head.cy_len)
    {
      fs += CY_BUFF_SIZE;
      buf->buffer = realloc(buf->buffer, fs);
      if(!buf->buffer) {perror(__func__); return 1;}
    }
    if(buf->buffer[buf->head.cy_len - 1] == '\n') break;
  }
  return 0;
}

int cy_read_file(const char *path, string *__file)
{
  int fd = open(path, O_RDONLY);
  if(fd < 0) {perror(__func__); return 1;}
  CY_BUFF buff;
  if(cy_read(fd, &buff) != 0) {perror(__func__); return 1;}
  __file->str = buff.buffer;
  __file->len = buff.head.cy_len;
  close(fd);
  return 0;
}

int cy_write(int fd, const CY_BUFF buf)
{
  size_t fs = 0;
  if(!buf.buffer) {perror(__func__); return 1;}
  while (fs < buf.head.cy_len)
  {
    ssize_t n = write(fd, buf.buffer + fs, buf.head.cy_len - fs);
    if(n == 0) break;
    if(n < 0) {perror(__func__); return 1;}
    fs += n;
  }
  return 0;
}

int cy_write_file(const char *path, string __file)
{
  int fd = open(path, O_WRONLY);
  if(fd < 0) {perror(__func__); return 1;}
  CY_BUFF buff = (CY_BUFF){.buffer = __file.str, .head.cy_len = __file.len};
  if(cy_write(fd, buff) != 0) {perror(__func__); return 1;}
  close(fd);
  return 0;
}
