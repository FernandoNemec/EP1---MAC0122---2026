#include <ctype.h>
#include <stdio.h>
#include <string.h>
#define LINE_COMMENT 1
#define BLOCK_COMMENT 2
#define NUM_DATA_TYPES 10
#define NUM_COMMANDS 35
#define NUM_DELIMITERS 12

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

char DELIMITERS[] = {' ', '\n', '\t', '(', ')', '[',
                     ']', '{',  '}',  '*', ';', ','};

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

int isDelimiter(char c);
int isDataType(char srcCode[], int *idx);
int isCommand(char srcCode[], int *idx);
void parse_syntax(char srcCode[], int *idx, enum COLOR *state);
void parse_special_chars(char srcCode[], char f_code[], int *text_size,
                         int start);
void printCode(FILE *html, char f_code[], int text_size, enum COLOR state);
void parseSrcCode(FILE *html, char srcCode[]);

void parse_syntax(char srcCode[], int *idx, enum COLOR *state) {

  int beginning = *idx;

  int commentType;
  char closeString;

  while (srcCode[*idx] != '\0') {

    char c = srcCode[*idx];
    char next_c = srcCode[*idx + 1];

    switch (*state) {
    case NO_COLOR:

      if (isDelimiter(c)) {
        (*idx)++;
        return;
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
      if (c == '=' || c == ')' || c == '[' || c == ';') {
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
      if ((srcCode[*idx - 1] == '\\' && c == '\\' && next_c == closeString) ||
          c != '\\' && next_c == closeString) {
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

void parse_special_chars(char srcCode[], char f_code[], int *text_size,
                         int start) {
  int f_idx = 0;
  for (int i = 0; i < *text_size; i++) {
    char c = srcCode[start + i];
    switch (c) {
    case '<':
      f_code[f_idx++] = '&';
      f_code[f_idx++] = 'l';
      f_code[f_idx++] = 't';
      break;
    case '>':
      f_code[f_idx++] = '&';
      f_code[f_idx++] = 'g';
      f_code[f_idx++] = 't';
      break;
    case '&':
      f_code[f_idx++] = '&';
      f_code[f_idx++] = 'a';
      f_code[f_idx++] = 'm';
      f_code[f_idx++] = 'p';
      break;
    case '\n':
      f_code[f_idx++] = '<';
      f_code[f_idx++] = 'b';
      f_code[f_idx++] = 'r';
      f_code[f_idx++] = '>';
      f_code[f_idx++] = '\n';
      f_code[f_idx++] = '\t';
      break;
    case '\t':
      f_code[f_idx++] = '&';
      f_code[f_idx++] = 'e';
      f_code[f_idx++] = 'm';
      f_code[f_idx++] = 's';
      f_code[f_idx++] = 'p';
      f_code[f_idx++] = ';';
      break;
    case ' ':
      f_code[f_idx++] = '&';
      f_code[f_idx++] = 'n';
      f_code[f_idx++] = 'b';
      f_code[f_idx++] = 's';
      f_code[f_idx++] = 'p';
      f_code[f_idx++] = ';';
      break;
    default:
      f_code[f_idx++] = c;
      break;
    }
  }
  *text_size = f_idx;
}

void printCode(FILE *html, char f_code[], int text_size, enum COLOR state) {
  switch (state) {
  case NO_COLOR:
    fprintf(html, "%.*s", text_size, f_code);
    break;
  case COMMENT:
    fprintf(html, "<span style=\"color: #3D7B7B;\">%.*s</span>", text_size,
            f_code);
    break;
  case PRAGMA:
    fprintf(html, "<span style=\"color: #9C6500;\">%.*s</span>", text_size,
            f_code);
    break;
  case LIB:
    fprintf(html, "<span style=\"color: #3D7B7B;\">%.*s</span>", text_size,
            f_code);
    break;
  case DATA_TYPE:
    fprintf(html, "<span style=\"color: #B00040;\">%.*s</span>", text_size,
            f_code);
    break;
  case COMMAND:
    fprintf(html, "<span style=\"color: #008000;\">%.*s</span>", text_size,
            f_code);
    break;
  case FUNCTION:
    fprintf(html, "<span style=\"color: #00F;\">%.*s</span>", text_size,
            f_code);
    break;
  case STRING:
    fprintf(html, "<span style=\"color: #BA2121;\">%.*s</span>", text_size,
            f_code);
    break;
  case NUMBER:
    fprintf(html, "<span style=\"color: #666;\">%.*s</span>", text_size,
            f_code);
    break;
  }
}

int isDataType(char srcCode[], int *idx) {
  if (*idx > 0 && !isDelimiter(srcCode[*idx - 1]))
    return 0;
  for (int i = 0; i < NUM_DATA_TYPES; i++) {
    char *keyword = KEY_DATAS[i];
    for (int j = 0; j < strlen(keyword); j++) {
      if (srcCode[*idx + j] != keyword[j])
        break;
      if (j == strlen(keyword) - 1 && isDelimiter(srcCode[*idx + j + 1])) {
        *idx += j + 1;
        return 1;
      }
    }
  }
  return 0;
}

int isCommand(char srcCode[], int *idx) {
  if (*idx > 0 && !isDelimiter(srcCode[*idx - 1]))
    return 0;
  for (int i = 0; i < NUM_COMMANDS; i++) {
    char *keyword = KEY_COMMANDS[i];
    for (int j = 0; j < strlen(keyword); j++) {
      if (srcCode[*idx + j] != keyword[j])
        break;
      if (j == strlen(keyword) - 1 && isDelimiter(srcCode[*idx + j + 1])) {
        *idx += j + 1;
        return 1;
      }
    }
  }
  return 0;
}

int isDelimiter(char c) {
  for (int i = 0; i < NUM_DELIMITERS; i++) {
    if (c == DELIMITERS[i])
      return 1;
  }
  return 0;
}

void parseSrcCode(FILE *html, char srcCode[]) {
  int idx = 0;
  enum COLOR color = NO_COLOR;

  while (idx < strlen(srcCode)) {
    int start = idx;
    enum COLOR previousColor = color;
    parse_syntax(srcCode, &idx, &color);
    int srcCode_size = idx - start;
    char f_srcCode[1000000];
    parse_special_chars(srcCode, f_srcCode, &srcCode_size, start);
    if (previousColor != color) {
      // fprintf(html, "<br>");
    }
    printCode(html, f_srcCode, srcCode_size, color);
  }
}
