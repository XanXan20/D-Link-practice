#include "to_c_file.h"

void to_c_file(char * input_file, char * output_file){
    FILE * input, * output;
    char buf[1000];

    input = fopen(input_file, "r");
    output = fopen(output_file, "w");

    fputs("#include <stdio.h>\n", output);
    fputs("int main(){\n", output);
    fputs("\tFILE *output;\n", output);
    fprintf(output, "\toutput = fopen(\"test_%s\", \"w\");\n", input_file);

    while(fgets(buf, sizeof(buf), input) != NULL){
        buf[strlen(buf)-1] = '\0';
        fprintf(output, "\tfprintf(output, \"%s \\n\");\n", buf);
    }

    fputs("\tfclose(output);\n", output);
    fputs("}", output);

    fclose(input);
    fclose(output);
}