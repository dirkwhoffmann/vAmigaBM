#include <stdio.h>
#include "Amiga.h"

int main(int argc, char **argv)
{
    printf("Hello, vAmiga\n");

    Amiga amiga;
    amiga.dump();
}
