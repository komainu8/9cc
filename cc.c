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

void error(char *format, ...) {
  va_list ap;
  va_start(ap, format);
  vfprint(stderr, format, ap);
  fprintf(stderr, "\n");
  exit(1);
}

bool consume(char operator) {
  if (token->kind != TOKEN_SYMBOL || token->string[0] != operator)
    return false;
  token = token->next;
  return true;
}

void expect(char operator) {
  if (token->kind != TOKEN_SYMBOL || token->str[0] != operator)
    error("This is not a '%c'", operator);
  token = token->next;
}

int expect_number() {
  if (token->kind != TOKEN_NUMBER)
    error("This is not a number");
  int val = token->value;
  token = token->next;
  return val;
}

bool iseol() {
  return token->kind == TOKEN_EOL;
}

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
