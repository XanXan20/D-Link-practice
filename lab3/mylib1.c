#include "mylib1.h"

void print_text(char * str){
    for(int i = 0, is_text = 1; str[i] != '\0'; i++){
        if(str[i] == '<')
            is_text = 0;
        else if(str[i] == '>')
            is_text = 1;
        else if(is_text)
            putchar(str[i]);
    }
}

void remove_tags(FILE *file){
    char buf[400];
    while(fgets(buf, sizeof(buf), file) != NULL){
        print_text(buf);
    }
}

void find_headers(FILE *file){
    char buf[400];
    while(fgets(buf, sizeof(buf), file) != NULL){
        if(strstr(buf, "<h1>") || strstr(buf, "<h2>") || strstr(buf, "<h3>") || strstr(buf, "<h4>") || strstr(buf, "<h5>") || strstr(buf, "<h6>"))
            print_text(buf);
    }
}

void find_references(FILE * file){
    char buf[400];
    while(fgets(buf, sizeof(buf), file) != NULL){
        if(strstr(buf, "<a>") != NULL)
            print_text(buf);
    }
}