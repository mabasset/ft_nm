#include "ft_nm.h"

int symcmp(void *sym1, void *sym2) {
    return ft_strcmp(((t_sym*)sym1)->name, ((t_sym*)sym2)->name);
}

int symcmp_rev(void *sym1, void *sym2) {
    return ft_strcmp(((t_sym*)sym2)->name, ((t_sym*)sym1)->name);
}