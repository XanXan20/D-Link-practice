#include <stdio.h>
#include <string.h>
#include <dlfcn.h>

int main(int argc, char const *argv[])
{
    if(argc != 3)
        return 1;

    FILE * input = fopen(argv[1], "r");
    void *library = dlopen("./libmy1.so", RTLD_LAZY);

    void (*plugin)(FILE * file) = dlsym(library, argv[2]);
    (*plugin)(input);
    
    fclose(input);
    dlclose(library);

    return 0;
}
