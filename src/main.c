#include <stdio.h>

#include "main.h"

int main(int argc, char* argv[]) {
    if (argc >1 ) {
        printf("%s", argv[1]);
    } else {
        printf("Hello World");
    }
    return 0;
}