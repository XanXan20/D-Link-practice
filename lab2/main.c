#include <string.h>
#include <stdio.h>
#include "to_c_file.h"
#include "to_text_file.h"
#include "to_html_file.h"

int main(int argc, char const *argv[])
{
    //проверки на введенные аргументы 

    if(argc != 5){
        puts("Неверное количество аргументов");
        return 1;
    }
    

    char * input_file = argv[1];
    char output_file[sizeof(argv[3]) + sizeof(argv[4])];

    strcpy(input_file, argv[1]);
    strcpy(output_file, argv[3]);
    strcat(output_file, argv[4]);

    if(!strcmp(argv[4], ".c")){
        to_c_file(input_file, output_file);
    }
    else if(!strcmp(argv[4], ".txt")){
        to_text_file(input_file, output_file);
    }
    else if(!strcmp(argv[4], ".html")){
        to_html_file(input_file, output_file);
    }
    else{
        puts("Неизвестный формат выходного файла");
        return 1;
    }

    //to_c_file(input_file, output_file);
    //to_text_file(input_file, output_file);
    //to_html_file(input_file, output_file);

    return 0;
}
