#include <stdio.h>
#include <stdlib.h>

#define HEADER "\
<!DOCTYPE html>\n\
<html lang=\"pt-BR\">\n\
<head>\n\
    <meta charset=\"UTF-8\">\n\
    <title>Higlighting of code in C</title>\n\
    <style>\n\
      body {\n\
      font-family: Arial, Helvetica, sans-serif;\n\
      }\n\
    </style>\n\
</head>\n\
<body>\n\
<p>\n"

#define END_OF_FILE "\
</p>\n\
</body>\n\
</html>"

void output_hiperlink (char* text, char* link){

  printf("\t<a href=%s>%s</a><br>\n",text,link);

  return;
}

int main() {
  printf("%s",HEADER);
  output_hiperlink("google","https://www.google.com");
  output_hiperlink("youtube","https://www.youtube.com");
  printf("%s",END_OF_FILE);
  return 0;
}
