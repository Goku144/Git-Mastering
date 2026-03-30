#include "../../inc/libcypher_io.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void cy_state(const char *str, int _errnum)
{
  if(errno == 0)
    fprintf(stderr, "%s\n", str);
  else
    perror(str);
  exit(_errnum);
}

void cy_read(int fd, CY_BUFF *buf)
{
  size_t fs = CY_BUFF_SIZE;
  buf->head.cy_len = 0;
  buf->buffer = malloc(fs);
  if(!buf->buffer) cy_state(__func__, -1); 
  while (1)
  {
    ssize_t n = read(fd, buf->buffer + buf->head.cy_len, fs - buf->head.cy_len);
    if(n == 0) break;
    if(n < 0) cy_state(__func__, -1);
    buf->head.cy_len += n;
    if(fs == buf->head.cy_len)
    {
      fs += CY_BUFF_SIZE;
      buf->buffer = realloc(buf->buffer, fs);
      if(!buf->buffer) cy_state(__func__, -1); 
    }
  }
}

void cy_read_line(int fd, CY_BUFF *buf)
{
  size_t fs = CY_BUFF_SIZE;
  buf->head.cy_len = 0;
  buf->buffer = malloc(fs);
  if(!buf->buffer) cy_state(__func__, -1); 
  while (1)
  {
    ssize_t n = read(fd, buf->buffer + buf->head.cy_len, fs - buf->head.cy_len);
    if(n == 0) break;
    if(n < 0) cy_state(__func__, -1);
    buf->head.cy_len += n;
    if(fs == buf->head.cy_len)
    {
      fs += CY_BUFF_SIZE;
      buf->buffer = realloc(buf->buffer, fs);
      if(!buf->buffer) cy_state(__func__, -1); 
    }
    if(buf->buffer[buf->head.cy_len - 1] == '\n') break;
  }
}

void cy_write(int fd, const CY_BUFF buf)
{
  size_t fs = 0;
  if(!buf.buffer) cy_state(__func__, -1); 
  while (fs < buf.head.cy_len)
  {
    ssize_t n = write(fd, buf.buffer + fs, buf.head.cy_len - fs);
    if(n == 0) break;
    if(n < 0) cy_state(__func__, -1);
    fs += n;
  }
}

