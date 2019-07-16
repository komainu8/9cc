#include <stdio.h>
#include <stdlib.h>

typedef enum {
  TK_RESERVED, //Symbol
  TK_NUM, //Numbser
  TK_EOF,
} TokenKind;

typedef struct Token Token;

struct Token
{
  TokenKind kind;
  Token *next;
  int val;
  char *str;
};

Token *token;

int main(int argc, char **argv)
{
  if (argc != 2) {
    fprintf(stderr, "�����θĿ�������������ޤ���\n");
    return 1;
  }

  char *p = argv[1];
  
  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");
  printf("  mov rax, %d\n", strtol(p, &p, 10));

  while (*p) {
    if (*p == '+') {
      p++;
      printf("  add rax, %ld\n", strtol(p, &p, 10));
      continue;
    }
    if (*p == '-') {
      p++;
      printf("  sub rax, %ld\n", strtol(p, &p, 10));
      continue;
    }

    fprintf(stderr, "ͽ�����ʤ�ʸ���Ǥ���: '%c'\n", *p);
    return 1;
  }
  
  printf("  ret\n");

  return 0;
}
