#include "ft_nm.h"

extern t_flags  g_flags;

void    close_fd(int *fd) {
    if (*fd > 0)
        close(*fd);
}

void    unmap_file(t_string *mapped_file) {
    if (mapped_file->content != NULL)
        munmap(mapped_file->content, mapped_file->size);
}

void free_matrix(t_sym_info ***symbols_info) {
    if (symbols_info == NULL || *symbols_info == NULL)
        return;
    for (int i = 0; (*symbols_info)[i] != NULL; i++) {
        free((*symbols_info)[i]->value);
        free((*symbols_info)[i]);
    }
    free(*symbols_info);
}

void    sort_symbols(t_sym_info **symbols_info) {
    t_sym_info  temp;
    int         diff;

    for (int i = 0; symbols_info[i] != NULL; i++) {
        for (int j = i + 1; symbols_info[j] != NULL; j++) {
            diff = ft_strncmp(symbols_info[i]->name, symbols_info[j]->name, ft_strlen(symbols_info[i]->name));
            if ((diff > 0 && !g_flags.reverse) || (diff < 0 && g_flags.reverse)) {
                temp = *symbols_info[i];
                *symbols_info[i] = *symbols_info[j];
                *symbols_info[j] = temp;
            }
        }
    }
}

void    display_symbols(t_sym_info **symbols_info, char *file_path, int ei_class) {
    static char *undefined_types = "Uw";
    static char *external_types = "BCDGRSTUWw";
    t_sym_info  *symbol_info;
    const char  *fmt;

    fmt = (ei_class == ELFCLASS32) ? "%8s %c %s\n" : "%16s %c %s\n";

    if (g_flags.path)
        ft_printf("\n%s:\n", file_path);
    for (size_t i = 0; symbols_info[i] != NULL; i++) {
        symbol_info = symbols_info[i];
        if (g_flags.undefined && !ft_strchr(undefined_types, symbol_info->type))
            continue;
        if (g_flags.external && !ft_strchr(external_types, symbol_info->type))
            continue;
        if (!g_flags.all && (symbol_info->st_type == STT_SECTION || symbol_info->st_type == STT_FILE))
            continue;
        if (symbol_info->type == '?')
            continue;
        ft_printf(fmt, symbol_info->value, symbol_info->type, symbol_info->name);
    }
}
