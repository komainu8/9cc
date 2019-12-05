#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ctype.h>

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
};

Token *token;

void error(char *format, ...) {
  va_list ap;
  va_start(ap, format);
  vfprintf(stderr, format, ap);
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
  if (token->kind != TOKEN_SYMBOL || token->string[0] != operator)
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

Token *create_new_token(TokenKind kind, Token *current, char *string) {
  Token *token = calloc(1, sizeof(Token));
  token->kind = kind;
  token->string = string;
  current->next = token;
  return token;
}

Token *tokenize(char *p) {
  Token head;
  head.next = NULL;
  Token *current = &head;

  while(*p) {
    if (isspace(*p)) {
      p++;
      continue;
    }

    if (*p == '+' || *p == '-') {
      current = create_new_token(TOKEN_SYMBOL, current, p++);
      continue;
    }

    if (isdigit(*p)) {
      current = create_new_token(TOKEN_NUMBER, current, p);
      current->value = strtol(p, &p, 10);
      continue;
    }

    error("'%c' can not tokenize.", p);
  }

  create_new_token(TOKEN_EOL, current, p);
  return head.next;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Invalid number of arguments.\n");
    return 1;
  }

  token = tokenize(argv[1]);

  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");
  printf("  mov rax, %d\n", expect_number());

  while (!iseol()) {
    if (consume('+')) {
      printf("  add rax, %d\n", expect_number());
      continue;
    }

    expect('-');
    printf("  sub rax, %d\n", expect_number());
  }

  printf("  ret\n");
  return 0;
}
