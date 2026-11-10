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

//Define new identations here
enum IDENTATION { NO_IDENTATION,
                  ENTER,
                  TAB,
                  INIT_PARAGRAPH,
                  END_PARAGRAPH
};

//Define new colors here
enum COLOR { NO_COLOR,
             RED,
             PINK,
             GREEN,
             BLUE
};

void output_line_html (int color, char* text, int* identation, int number_of_identations){

  //Identation output in html
  //The \n and \t outputed are meant to let the html clean and identaded for humam reading
  //New identations can be added
  for(int i = 0; i < number_of_identations; i++){
    switch(identation[i]){
      case NO_IDENTATION:
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
      /*
      case NEW_IDENTATION:
        printf("identation implementation in html");
        break;
      */
    }
  }

  //Colored text output in html
  //default falls into non colored texts
  //New colors can be added
  switch(color){
    case NO_COLOR:
      printf("%s",text);
      break;
    case RED:
      printf("<span style=\"color: red;\">%s</span>",text);
      break;
    case PINK:
      printf("<span style=\"color: pink;\">%s</span>",text);
      break;
    case GREEN:
      printf("<span style=\"color: green;\">%s</span>",text);
      break;
    case BLUE:
      printf("<span style=\"color: blue;\">%s</span>",text);
      break;
    /*
    case NEW_COLOR:
      printf("<span style=\"color: color wanted;\">%s</span>",text);
      break;
    */
    default:
      printf("%s",text);
      break;
  }

  return;
}

int exemplo1 [1] = {INIT_PARAGRAPH};
int exemplo2 [1] = {ENTER};
int exemplo3 [1] = {TAB};
int exemplo4 [2] = {ENTER,TAB};
int exemplo5 [1] = {NO_IDENTATION};
int exemplo6 [3] = {ENTER,ENTER,ENTER};
int exemplo7 [1] = {END_PARAGRAPH};

int main(){
  output_line_html(RED,"Test1",exemplo1,1);
  output_line_html(RED,"Test2",exemplo2,1);
  output_line_html(PINK,"Test3",exemplo3,1);
  output_line_html(GREEN,"Test4",exemplo4,2);
  output_line_html(BLUE,"Test5",exemplo5,1);
  output_line_html(NO_COLOR,"Test6",exemplo6,3);
  output_line_html(BLUE,"Test7",exemplo7,1);
  return 0;
}
