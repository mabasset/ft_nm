#include "ft_nm.h"

extern t_elf_info g_elf;

int print_error(char *error) {
    fprintf(stderr, "ft_nm: '%s': %s\n", g_elf.path, error);
    return (1);
}