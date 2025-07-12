#include <stdio.h>

asm(".globl ABSOLUTE\n\t"
    ".set ABSOLUTE, 10");
asm(".set absolute, 2");
extern const int ABSOLUTE;
extern const int absolute;

char    common;
int     COMMON;

static int uninitialized;

int INITIALIZED = 10;
static int initialized = 10;

int main() {
    return 0;
}