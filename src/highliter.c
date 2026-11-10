#include <stdio.h>
#include <stdlib.h>

#define HEADER "\
<!DOCTYPE html>\n\
<html lang=\"pt-BR\">\n\
<head>\n\
    <meta charset=\"UTF-8\">\n\
    <title>Higlighting of code in C</title>\n\
    <style>\n\
    </style>\n\
</head>\n\
<body>\n"

#define END_OF_FILE "\
</body>\n\
</html>"

enum IDENTATION { NO_IDENTATION,
                  SPACE,
                  ENTER,
                  TAB,
                  INIT_PARAGRAPH,
                  END_PARAGRAPH
};

enum COLOR { NO_COLOR,
             COMMENT,
             PRAGMA,
             LIB,
             DATA_TYPE,
             COMMAND,
             FUNCTION,
             STRING,
             NUMBER
};

void output_line_html (int color, char* text, int* identation, int number_of_identations){

  //Identation output in html
  //The \n and \t outputed are meant to let the html clean
  for(int i = 0; i < number_of_identations; i++){
    switch(identation[i]){
      case NO_IDENTATION:
        printf("");
        break;
      case SPACE:
        printf(" ");
        break;
      case ENTER:
        printf("<br>\n\t");
        break;
      case TAB:
        printf("&nbsp;&nbsp;&nbsp;&nbsp;");
        break;
      case INIT_PARAGRAPH:
        printf("%s",HEADER);
        printf("<p>\n\t");
        return;
      case END_PARAGRAPH:
        printf("</p>");
        printf("%s",END_OF_FILE);
        return;
    }
  }

  //Colored text output in html
  //Default falls into non colored texts
  switch(color){
    case NO_COLOR:
      printf("%s",text);
      break;
    case COMMENT:
      printf("<span style=\"color: #3D7B7B;\">%s</span>",text);
      break;
    case PRAGMA: printf("<span style=\"color: #9C6500;\">%s</span>",text);
      break;
    case LIB:
      printf("<span style=\"color: #3D7B7B;\">%s</span>",text);
      break;
    case DATA_TYPE:
      printf("<span style=\"color: #B00040;\">%s</span>",text);
      break;
    case COMMAND:
      printf("<span style=\"color: #008000;\">%s</span>",text);
      break;
    case FUNCTION:
      printf("<span style=\"color: #00F;\">%s</span>",text);
      break;
    case STRING:
      printf("<span style=\"color: #BA2121;\">%s</span>",text);
      break;
    case NUMBER:
      printf("<span style=\"color: #666;\">%s</span>",text);
      break;
    default:
      printf("%s",text);
      break;
  }

  return;
}

int exemplo1 [1] = {INIT_PARAGRAPH};
int exemplo2 [1] = {SPACE};
int exemplo3 [1] = {ENTER};
int exemplo4 [1] = {SPACE};
int exemplo5 [2] = {ENTER,ENTER};
int exemplo6 [1] = {SPACE};
int exemplo7 [1] = {NO_IDENTATION};
int exemplo8 [1] = {NO_IDENTATION};
int exemplo9 [1] = {SPACE};
int exemplo10 [2] = {ENTER,TAB};
int exemplo11 [1] = {NO_IDENTATION};
int exemplo12 [1] = {NO_IDENTATION};
int exemplo13 [1] = {NO_IDENTATION};
int exemplo14 [1] = {NO_IDENTATION};
int exemplo15 [2] = {ENTER,TAB};
int exemplo16 [1] = {SPACE};
int exemplo17 [1] = {NO_IDENTATION};
int exemplo18 [1] = {ENTER};
int exemplo19 [1] = {END_PARAGRAPH};

int main(){
  output_line_html(NO_COLOR,"",exemplo1,1);
  output_line_html(COMMENT,"// Hello World",exemplo2,1);
  output_line_html(PRAGMA,"#include",exemplo3,1);
  output_line_html(LIB,"<stdio.h>",exemplo4,1);
  output_line_html(DATA_TYPE,"int",exemplo5,2);
  output_line_html(FUNCTION,"main",exemplo6,3);
  output_line_html(NO_COLOR,"(",exemplo7,1);
  output_line_html(NO_COLOR,")",exemplo8,1);
  output_line_html(NO_COLOR,"{",exemplo9,1);
  output_line_html(FUNCTION,"printf",exemplo10,2);
  output_line_html(NO_COLOR,"(",exemplo11,1);
  output_line_html(STRING,"\"Hello World\\n\"",exemplo12,1);
  output_line_html(NO_COLOR,")",exemplo13,1);
  output_line_html(NO_COLOR,";",exemplo14,1);
  output_line_html(COMMAND,"return",exemplo15,1);
  output_line_html(NUMBER,"0",exemplo16,1);
  output_line_html(NO_COLOR,";",exemplo17,1);
  output_line_html(NO_COLOR,"}",exemplo18,1);
  output_line_html(NO_COLOR,"",exemplo19,1);
  return 0;
}
