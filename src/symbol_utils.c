#include "ft_nm.h"

extern t_flags      g_flags;

void    sort_symbols(t_sym_info **symbols_info) {
    t_sym_info  temp;
    int         diff;

    // for (size_t i = 0; i < n_sym - 1; i++) {
    //     for (size_t j = 0; j < n_sym - i - 1; j++) {
    //         diff = ft_strcmp(symbols_info[j].name, symbols_info[j + 1].name);
    //         if ((diff > 0 && !g_flags.reverse) || (diff < 0 && g_flags.reverse)) {
    //             temp = symbols_info[j];
    //             symbols_info[j] = symbols_info[j + 1];
    //             symbols_info[j + 1] = temp;
    //         }
    //     }
    // }
    for (int i = 0; symbols_info[i] != NULL; i++) {
        for (int j = 1; symbols_info[j] != NULL; i++) {
            diff = ft_strcmp((*symbols_info)[j].name, (*symbols_info)[j + 1].name);
        }
    }
}

void    display_symbols(t_sym_info *symbols_info) {
    static char *undefined_types = "Uw";
    static char *external_types = "BCDGRSTUWw";
    t_sym_info  sym_info;

    if (g_flags.path) {
        write(STDOUT_FILENO, "\n", 1);
        write(STDOUT_FILENO, g_elf_file.path, ft_strlen(g_elf_file.path));
        write(STDOUT_FILENO, ":\n", 2);
    }
    for (size_t i = 0; i < n_sym; i++) {
        sym_info = symbols_info[i];
        if (g_flags.undefined && !ft_strchr(undefined_types, sym_info.type))
            continue ;
        if (g_flags.external && !ft_strchr(external_types, sym_info.type))
            continue ;
        write(STDOUT_FILENO, sym_info.value, ft_strlen(sym_info.value));
        write(STDOUT_FILENO, " ", 1);
        write(STDOUT_FILENO, &sym_info.type, 1);
        write(STDOUT_FILENO, " ", 1);
        write(STDOUT_FILENO, sym_info.name, ft_strlen(sym_info.name));
        write(STDOUT_FILENO, "\n", 1);
    }
}

void    free_symbols(t_sym_info *sym_info_arr) {
    for (size_t i = 0; i < n_sym; i++)
        free(sym_info_arr[i].value);
    free(sym_info_arr);
}
