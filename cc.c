#include <stdio.h>
#include <stdlib.h>

//Kind of token
typedef enum {
  TOKEN_SYMBOL,
  TOKEN_NUMBER,
  TOKEN_EOL,    // End of line
} TokenKind;

typedef struct Token Token;

struct Token {
  TokenKind kind;
  Token *next;
  int value;
  char *string;
} Token;

Token *token;

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Invalid number of arguments.\n");
    return 1;
  }

  char *p = argv[1];

  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");
  printf("  mov rax, %ld\n", strtol(p, &p, 10));

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

    fprintf(stderr, "Invalid charactor: '%c'\n", *p);
    return 1;
  }

  printf("  ret\n");
  return 0;
}
