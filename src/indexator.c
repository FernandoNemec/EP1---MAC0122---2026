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
<p>\n\t"

#define END_OF_FILE "\
</p>\
</body>\n\
</html>"

void output_hiperlink (char* text, char* link){

  printf("<a href=%s>%s</a><br>\n\t",text,link);

  return;
}

int main() {
  output_hiperlink("google","https://www.google.com");
  output_hiperlink("youtube","https://www.youtube.com");
  return 0;
}
