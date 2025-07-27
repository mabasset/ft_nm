#include <stdio.h>

asm(".globl global_absolute\n\t"
    ".set global_absolute, 10");
extern const int global_absolute;
asm(".set local_absolute, 10");
extern const int local_absolute;
asm(".set trap.c, 0");
extern const int trap_c asm("trap.c");

int         global_common;
static int  local_uninitialized;

int         global_initialized = 10;
static int  local_initialized = 10;

const int           global_constant = 30;
static const int    local_constant = 20;

extern int undefined;
asm(".globl unique\n\t"
    ".type unique, @gnu_unique_object\n\t"
    ".size unique, 4\n\t"
    "unique:\n\t"
    ".long 42");
extern int unique;

int initialized_weak_variable __attribute__((weak)) = 100;
int uninitialized_weak_variable __attribute__((weak));

void defined_weak_function(void) __attribute__((weak));
void defined_weak_function(void) {
}
void undefined_weak_function(void) __attribute__((weak));

static void indirect_function_impl(void) {
}
static void (*resolve_indirect_function(void))(void) {
    return indirect_function_impl;
}
void indirect_function(void) __attribute__((ifunc("resolve_indirect_function")));

int main() {
    volatile const void *ptr;
    ptr = &global_absolute;
    ptr = &local_absolute;
    ptr = &global_common;
    ptr = &local_uninitialized;
    ptr = &global_initialized;
    ptr = &local_initialized;
    ptr = &global_constant;
    ptr = &local_constant;
    ptr = &undefined;
    ptr = &unique;
    ptr = &initialized_weak_variable;
    ptr = &uninitialized_weak_variable;
    (void)ptr;

    defined_weak_function();
    undefined_weak_function();
    indirect_function();
    return 0;
}