// Falta implementar os casso de DATA_TYPE e COMMAND
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#define LINE_COMMENT 1
#define BLOCK_COMMENT 2

enum IDENTATION { 
  NO_IDENTATION,
  SPACE,
  ENTER,
  TAB,
  INIT_PARAGRAPH,
  END_PARAGRAPH
};

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
  return c == ' ' || c == '(' || c == ')' || c == '*' || c == '\n' || c == '\t';
}

int isDataType(char srcCode[], int idx);
int isCommand(char srcCode[], int idx);

int isIdentation(char c) {
  switch (c) {
  case ' ':
    identation[] = SPACE;
    return 1;
  }
  case '\n':
    identation[] = ENTER;
    return 1;
  }
  case '\t':
    identation[] = TAB;
    return 1;
  }
  default:
    identation[] = NO_IDENTATION;
    return 0;
  }
}

void parser(char srcCode[], int *idx, enum COLOR *state);

int main(int argc, char *argv[]) {
  char exemplo[] =
      "#include <stdio.h>\n\nint main () {\n\tint exemplo = 0;\n\treturn exemplo;\n}";
  int idx = 0;
  int end = strlen(exemplo);
  enum COLOR color = NO_COLOR;
  enum IDENTATION identation[];

  while (idx != end) {
    parser(exemplo, &idx, &color, identation);
    printf("idx: %d color: %d\n", idx, color);
  }

  return 0;
}

void parser(char srcCode[], int *idx, enum COLOR *state, enum IDENTATION identation) {
  int commentType;
  int beginning = *idx;
  char closeString;
  while (srcCode[*idx] != '\0') {
    char c = srcCode[*idx];
    char next_c = srcCode[*idx + 1];

    switch (*state) {
    case NO_COLOR:

      //Identation
      if(isIdentation(c)){
        (*idx)++;
        break;
      }

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
      if (isDataType(srcCode, *idx)) {
        *state = DATA_TYPE;
      }

      // Command
      if (isCommand(srcCode, *idx)) {
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
      if (commentType == LINE_COMMENT && next_c == '\n') {
        (*idx)++;
        return;
      }
      if (commentType == BLOCK_COMMENT && c == '*' && next_c == '/') {
        *idx += 2;
        return;
      }
      (*idx)++;
      break;

    case PRAGMA:
      if (c == '<') {
        if (*idx != beginning)
          return;
        else {
          *state = LIB;
          break;
        }
      }
      if (*idx == beginning) {
        *state = NO_COLOR;
        continue;
      }
      if (c == '\'' | c == '"' | isdigit(c)) {
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
        return;
      }
      (*idx)++;
      break;

    case DATA_TYPE:
      if (*idx == beginning) {
        *state = NO_COLOR;
        continue;
      }
      // Atribuir variavel ou type cast
      if (c == '=' || c == ')') {
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
      if (!isdigit(c)){
        (*idx)++;
        return;
      }
      (*idx)++;
      break;
    }
  }
  return;
}

int isDataType(char srcCode[], int idx) { return 0; }

int isCommand(char srcCode[], int idx) { return 0; }
