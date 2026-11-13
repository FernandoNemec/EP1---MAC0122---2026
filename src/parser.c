#include <ctype.h>
#include <stdio.h>
#include <string.h>
#define LINE_COMMENT 1
#define BLOCK_COMMENT 2
#define NUM_DATA_TYPES 10
#define NUM_COMMANDS 35

char KEY_DATAS[][10] = {"bool",  "char",   "int",      "float", "double",
                        "short", "signed", "unsigned", "long",  "void"};

char KEY_COMMANDS[][15] = {
    "alignas",
    "alignof",
    "auto",
    "break",
    "case",
    "const",
    "constexpr",
    "continue",
    "default",
    "do",
    "else",
    "enum",
    "extern",
    "false",
    "for",
    "goto",
    "if",
    "inline",
    "nullptr",
    "register",
    "restrict",
    "return",
    "sizeof",
    "static",
    "static_assert",
    "struct",
    "switch",
    "thread_local",
    "true",
    "typedef",
    "typeof",
    "typeof_unqual",
    "union",
    "volatile",
    "while",
};

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

int isDataType(char srcCode[], int *idx);
int isCommand(char srcCode[], int *idx);
int isIdentation(char c, int *idx_identation, enum IDENTATION identation[]);
void parser(char srcCode[], int *idx, enum COLOR *state,
            enum IDENTATION identation[], int *idx_identation);

int main(int argc, char *argv[]) {

  char exemplo[] = "// Hello World\n#include <stdio.h>\nint main () "
                   "{\n\tprintf(\"Hello World\\n\");"
                   "\n\treturn 0;\n}";

  int idx = 0;
  int idx_identation;

  int end = strlen(exemplo);

  enum COLOR color = NO_COLOR;
  enum IDENTATION identation[100];

  printf("%s\n", exemplo);
  while (idx != end) {
    parser(exemplo, &idx, &color, identation, &idx_identation);
    if (exemplo[idx] == '\n')
      printf("break idx: %d color: %d, letter: \\n\n", idx, color);
    else
      printf("break idx: %d color: %d, letter: %c\n", idx, color, exemplo[idx]);
  }

  return 0;
}

void parser(char srcCode[], int *idx, enum COLOR *state,
            enum IDENTATION identation[], int *idx_identation) {

  int beginning = *idx;
  *idx_identation = 0;

  int commentType;
  char closeString;

  while (srcCode[*idx] != '\0') {

    char c = srcCode[*idx];
    char next_c = srcCode[*idx + 1];

    if (c == '\n')
      printf("idx: %d color: %d, letter: \\n\n", *idx, *state);
    else
      printf("idx: %d color: %d, letter: %c\n", *idx, *state, c);

    switch (*state) {
    case NO_COLOR:

      // Identation
      // while (isIdentation(c, idx_identation, identation)) {
      //   (*idx)++;
      // }
      // beginning = *idx;

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
      if (isDataType(srcCode, idx)) {
        *state = DATA_TYPE;
        return;
      }

      // Command
      if (isCommand(srcCode, idx)) {
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
      if (commentType == LINE_COMMENT && c == '\n') {
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
          (*idx)++;
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
      // Atribuir variavel ou type cast
      if (c == '=' || c == ')') {
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
      if (!isdigit(c)) {
        return;
      }
      (*idx)++;
      break;
    }
  }
  return;
}

int isDataType(char srcCode[], int *idx) {
  if (!is_delimiter(srcCode[*idx]))
    return 0;
  for (int i = 0; i < NUM_DATA_TYPES; i++) {
    char *keyword = KEY_DATAS[i];
    for (int j = 0; j < strlen(keyword); j++) {
      if (srcCode[*idx + 1 + j] != keyword[j])
        break;
      if (j + 1 == strlen(keyword) && is_delimiter(srcCode[*idx + j + 2])) {
        *idx += j + 2;
        return 1;
      }
    }
  }
  return 0;
}

int isCommand(char srcCode[], int *idx) {
  if (!is_delimiter(srcCode[*idx]))
    return 0;
  for (int i = 0; i < NUM_COMMANDS; i++) {
    char *keyword = KEY_COMMANDS[i];
    for (int j = 0; j < strlen(keyword); j++) {
      if (srcCode[*idx + 1 + j] != keyword[j])
        break;
      if (j + 1 == strlen(keyword) && is_delimiter(srcCode[*idx + j + 2])) {
        *idx += j + 2;
        return 1;
      }
    }
  }
  return 0;
}

int isIdentation(char c, int *idx_identation, enum IDENTATION identation[]) {
  switch (c) {
  case ' ':
    identation[(*idx_identation)++] = SPACE;
    return 1;
  case '\n':
    identation[(*idx_identation)++] = ENTER;
    return 1;
  case '\t':
    identation[(*idx_identation)++] = TAB;
    return 1;
  default:
    if (idx_identation == 0)
      identation[*(idx_identation)++] = NO_IDENTATION;
    return 0;
  }
}
