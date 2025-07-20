#include <stdio.h>

asm(".globl ABSOLUTE\n\t"
    ".set ABSOLUTE, 10");
extern const int ABSOLUTE;

int         COMMON;
static int  uninitialized;

int         INITIALIZED = 10;
static int  initialized = 10;

const int           READ_ONLY = 30;
static const int    read_only = 20;

extern int UNDEFINED;
asm(".globl unique\n\t"
    ".type unique, @gnu_unique_object\n\t"
    ".size unique, 4\n\t"
    "unique:\n\t"
    ".long 42");
extern int unique;

// 'V' symbol - a global weak object.
// It can be overridden by a strong symbol of the same name from another file.
int weak_global __attribute__((weak)) = 100;

// Note: A local weak object ('v') cannot be created in C, as the 'weak'
// attribute requires external linkage, which 'static' prevents.

static void indirect_function_impl(void) {
}
static void (*resolve_indirect_function(void))(void) {
    return indirect_function_impl;
}
void indirect_function(void) __attribute__((ifunc("resolve_indirect_function")));

int main() {
    volatile void *ptr = &UNDEFINED;
    volatile void *ptr2 = &unique;
    volatile void *ptr3 = &weak_global;
    (void)ptr2;
    (void)ptr3;
    indirect_function();
    return 0;
}