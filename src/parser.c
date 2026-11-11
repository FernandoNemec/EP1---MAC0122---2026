// Falta implementar os casso de DATA_TYPE e COMMAND
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#define LINE_COMMENT 1
#define BLOCK_COMMENT 2

char closeString;
enum COLOR {
  NO_COLOR,
  COMMENT,
  PRAGMA,
  LIB,
  DATA_TYPE,
  COMMAND,
  FUNCTION,
  STRING,
  NUMBER
};

int is_delimiter(char c) {
  return c == ' ' || c == '(' || c == ')' || c == '*' || c == '\n';
}

int isDataType(char srcCode[], int idx);
int isCommand(char srcCode[], int idx);

void parser(char srcCode[], int *idx, enum COLOR *state);

int main(int argc, char *argv[]) {
  char exemplo[] =
      "#include <stdio.h>\nint main () {\nint exemplo = 0;\nreturn exemplo;\n}";
  int idx = 0;
  int end = strlen(exemplo);
  enum COLOR color = NO_COLOR;

  while (idx != end) {
    parser(exemplo, &idx, &color);
    printf("idx: %d color: %d\n", idx, color);
  }

  return 0;
}

void parser(char srcCode[], int *idx, enum COLOR *state) {
  int commentType;
  int beginning = *idx;
  while (srcCode[*idx] != '\0') {
    char c = srcCode[*idx];
    char next_c = srcCode[*idx + 1];

    switch (*state) {
    case NO_COLOR:
      // Comments
      if (c == '/' && (next_c == '/' || next_c == '*')) {
        *state = COMMENT;
        if (next_c == '/')
          commentType = LINE_COMMENT;
        else
          commentType = BLOCK_COMMENT;
      }

      // Pragma
      if (c == '#')
        *state = PRAGMA;

      // Data type
      if (is_delimiter(c) && isDataType(srcCode, *idx)) {
        *state = DATA_TYPE;
        return;
      }

      // Command
      if (is_delimiter(c) && isCommand(srcCode, *idx)) {
        *state = COMMAND;
        return;
      }

      // String
      if (c == '\'' || c == '"') {
        *state = STRING;
        closeString = c;
      }

      // Number
      if (isdigit(c))
        *state = NUMBER;

      if (*state != NO_COLOR && *idx != beginning) {
        *state = NO_COLOR;
        return;
      }
      (*idx)++;
      break;

    case COMMENT:
      if (*idx == beginning) {
        *state = NO_COLOR;
        continue;
      }
      (*idx)++;
      if (commentType == LINE_COMMENT && c == '\n') {
        (*idx)++;
        return;
      }
      if (commentType == BLOCK_COMMENT && c == '*' && next_c == '/') {
        *idx += 2;
        return;
      }
      break;

    case PRAGMA:
      if (*idx == beginning) {
        *state = NO_COLOR;
        continue;
      }
      if (c == '<') {
        *state = LIB;
        return;
      }
      if (c == '\n') {
        (*idx)++;
        *state = NO_COLOR;
        return;
      }
      (*idx)++;
      break;

    case LIB:
      if (*idx == beginning) {
        *state = NO_COLOR;
        continue;
      }
      if (c == '>') {
        (*idx)++;
        *state = NO_COLOR;
        return;
      }
      (*idx)++;
      break;

    case DATA_TYPE:
      // Atribuir variavel ou type cast
      if (c == '=' || c == '(') {
        *state = NO_COLOR;
        return;
      }
      // Declarar funcao
      if (c == '(') {
        *state = FUNCTION;
        return;
      }
      (*idx)++;
      break;

    case COMMAND:
      *state = NO_COLOR;
      break;

    case FUNCTION:
      *state = NO_COLOR;
      break;

    case STRING:
      if (*idx == beginning) {
        *state = NO_COLOR;
        continue;
      }
      if (c != '\\' && next_c == closeString) {
        *idx += 2;
        return;
      }
      (*idx)++;
      break;

    case NUMBER:
      if (*idx == beginning) {
        *state = NO_COLOR;
        continue;
      }
      if (!isdigit(c))
        return;
      (*idx)++;
      break;
    }
  }
}

int isDataType(char srcCode[], int idx) { return 0; }

int isCommand(char srcCode[], int idx) { return 0; }
