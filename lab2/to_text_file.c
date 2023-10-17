#include "to_text_file.h"

void to_text_file(char * input_file, char * output_file){
    FILE * input, * output;
    char buf[1000];
    char sep[10] = ",";
    char * col;

    input = fopen(input_file, "r");
    output = fopen(output_file, "w");

    while(fgets(buf, sizeof(buf), input) != NULL){
        col = strtok(buf, sep);
        while(col != NULL){
            fprintf(output, "%-20s\t", col);
            
            col = strtok(NULL, sep);
        }    
        fprintf(output, "\n");
    }

    fclose(input);
    fclose(output);
}