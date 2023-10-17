#include "to_html_file.h"

void to_html_file(char * input_file, char * output_file){
    FILE * input, * output;
    char buf[1000];
    char sep[10] = ",";
    char * col;

    input = fopen(input_file, "r");
    output = fopen(output_file, "w");

    fputs("<!DOCTYPE html>\n", output);
    fputs("<html lang=\"ru\">\n", output);

    fputs("<head>\n", output);
    fputs("<meta charset=\"utf-8\">\n", output);
    fprintf(output, "<title>%s</title>\n", output_file);
    fputs("</head>\n", output);

    fputs("<body>\n", output);
    fputs("<table>\n", output);
    fputs("<tr>", output);
    fgets(buf, sizeof(buf), input); 
    col = strtok(buf, sep); 
    while(col != NULL){
        if(col[strlen(col)-1] == '\n')
            col[strlen(col)-1] = '\0';
        fprintf(output, "<th>%s</th>", col); 
        col = strtok(NULL, sep);
    }
    fputs("</tr>\n", output);

    while(fgets(buf, sizeof(buf), input)){
        fputs("\n<tr>", output);
        col = strtok(buf, sep);
        while(col != NULL){
            if(col[strlen(col)-1] == '\n')
                col[strlen(col)-1] = '\0';
            fprintf(output, "<td>%s</td>", col);
            col = strtok(NULL, sep);
        }
        fputs("</tr>", output);
    }
    fputs("\n</table>\n", output);
    
    fputs("</body>\n", output);
    fputs("</html>\n", output);

    fclose(input);
    fclose(output);
}