#include "ft_nm.h"

extern t_flags      g_flags;

void    sort_symbols(t_sym_info **symbol_info) {
    t_sym_info *temp;
    size_t      i, j;
    int         diff;

    for (i = 0; symbol_info[i] != NULL && symbol_info[i + 1] != NULL; i++) {
        for (j = 0; symbol_info[j] != NULL && symbol_info[j + 1] != NULL; j++) {
            if (j >= (SIZE_MAX - i - 1))
                break;
            diff = ft_strcmp(symbol_info[j]->name, symbol_info[j + 1]->name);
            if ((diff > 0 && !g_flags.reverse) || (diff < 0 && g_flags.reverse)) {
                temp = symbol_info[j];
                symbol_info[j] = symbol_info[j + 1];
                symbol_info[j + 1] = temp;
            }
        }
    }
}

void    display_symbols(char *file_path, t_sym_info **symbol_info) {
    static char *undefined_types = "Uw";
    static char *external_types = "BCDGRSTUWw";
    t_sym_info  *sym_info;

    if (g_flags.path) {
        write(STDOUT_FILENO, "\n", 1);
        write(STDOUT_FILENO, file_path, ft_strlen(file_path));
        write(STDOUT_FILENO, ":\n", 2);
    }
    for (size_t i = 0; symbol_info[i] != NULL; i++) {
        sym_info = symbol_info[i];
        if (g_flags.undefined && !ft_strchr(undefined_types, sym_info->type))
            continue ;
        if (g_flags.external && !ft_strchr(external_types, sym_info->type))
            continue ;
        write(STDOUT_FILENO, sym_info->value, ft_strlen(sym_info->value));
        write(STDOUT_FILENO, " ", 1);
        write(STDOUT_FILENO, &sym_info->type, 1);
        write(STDOUT_FILENO, " ", 1);
        write(STDOUT_FILENO, sym_info->name, ft_strlen(sym_info->name));
        write(STDOUT_FILENO, "\n", 1);
    }
}

