#include "../../inc/libcypher_io.h"
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TOKEN_SIZE 1024
#define TOKEN_NUM 7

typedef struct TOKENS
{
  char **token;
  size_t len;
}TOKENS;

void tokenizing(const CY_BUFF str, TOKENS *tokens)
{
  size_t acc_token_size = 0, gen_token_size = TOKEN_SIZE, acc_token_num = 0, gen_token_num = TOKEN_NUM;

  char **token = malloc(gen_token_num * sizeof (char *));
  token[acc_token_num] = malloc(gen_token_size);

  for (size_t i = 0; i < str.head.cy_len; i++)
  {
    if(str.buffer[i] != ' ' && str.buffer[i] != '\n')
    {
      if((acc_token_size - 1) > gen_token_size)
      {
        gen_token_size += TOKEN_SIZE;
        token[acc_token_num] = realloc(token[acc_token_num], gen_token_size);
      }

      token[acc_token_num][acc_token_size] = str.buffer[i];

      acc_token_size++;
    }
    else
    {
      if(i != str.head.cy_len - 1)
      {
        token[acc_token_num][acc_token_size] = '\0';

        token[acc_token_num] = realloc(token[acc_token_num], acc_token_size + 1);

        if(acc_token_num > gen_token_num)
        {
          gen_token_num += TOKEN_NUM;
          token = realloc(token, gen_token_num * sizeof (char *));
        }

        acc_token_num++;
        acc_token_size = 0;
      }
    }
  }
  acc_token_num++;
  token = realloc(token, acc_token_num * sizeof (char *));
  tokens->token = token;
  tokens->len = acc_token_num;
}

void freeTokens(TOKENS tokens)
{
  for (size_t i = 0; i < tokens.len; i++)
    free(tokens.token[i]);
  free(tokens.token);
}

void cat(const TOKENS tokens)
{
  if(tokens.len != 2)
  {
    printf(tokens.len > 2 ? "unknown arguments: " : "no path specified");
    fflush(stdout);
    for (size_t i = 1; i < tokens.len; i++)
    {
      printf("%s ", tokens.token[i]);
      fflush(stdout);
    }
    printf("\nusage:\n  cat <path>\n");
  }
  else
  {
    int fd = open(tokens.token[1], O_RDONLY);
    if(fd > 0)
    {
      CY_BUFF file;
      cy_read(fd, &file);
      cy_write(STDOUT_FILENO, file);
      printf("\n");
    }
    else
    {
      perror(__func__);
    }
  }
}

void clear(const TOKENS tokens)
{
  if(tokens.len != 1)
  {
    printf("unknown arguments: ");
    fflush(stdout);
    for (size_t i = 1; i < tokens.len; i++)
    {
      printf("%s ", tokens.token[i]);
      fflush(stdout);
    }
    printf("\nusage:\n  cat <path>\n");
  }
  else
  {
    printf("\033[2J\033[3J\033[H"); 
    fflush(stdout);
  }
}

void command(TOKENS tokens)
{
  if(strcmp(tokens.token[0], "cat") == 0)
  {
    cat(tokens);
  }
  else if(strcmp(tokens.token[0], "clear") == 0)
  {
    clear(tokens);
  }
  else if(strcmp(tokens.token[0], "exit") == 0)
  {
    exit(0);
  }
  else
  {
    printf("unknown command %s\n", tokens.token[0]);
  }
  freeTokens(tokens);
}

int main(void)
{
  CY_BUFF cli_txt;
  TOKENS tokens;
  while (1)
  {
    printf("enter your command: ");
    fflush(stdout);
    cy_read_line(STDIN_FILENO, &cli_txt);
    tokenizing(cli_txt, &tokens);
    command(tokens);
  }
  return 0;
}
