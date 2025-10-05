#include "ft_nm.h"

extern t_flags  g_flags;

int print_usage() {
    t_string    file;

    file = extract_content(USAGE_FILEPATH);
    ft_putstr_err(file.content);
    unmap_file(&file);
    return 1;
}

int print_format_error(char *file_path) {
    ft_putstr_err("nm: ");
    ft_putstr_err(file_path);
    ft_putstr_err(": file format not recognized\n");
    return 1;
}

int print_no_symbols(char *file_path) {
    ft_putstr_err("nm: ");
    ft_putstr_err(file_path);
    ft_putstr_err(": no symbols\n");
    return 0;
}

int print_invalid_option(char option) {
    ft_putstr_err("nm: invalid option -- '");
    ft_putchar_err(option);
    ft_putstr_err("'\n");
    return print_usage();
}

int print_unrecognized_option(char *option) {
    ft_putstr_err("nm: unrecognized option '");
    ft_putstr_err(option);
    ft_putstr_err("'\n");
    return print_usage();
}

static void print_sym_value(char *value, int ei_class) {
    char    filler;
    int     padding;

    filler = ' ';
    padding = (ei_class == ELFCLASS32) ? 8 : 16;
    if (value != NULL) {
        padding -= ft_strlen(value);
        filler = 0;
    }
    for (int i = 0; i < padding; i++)
        ft_putchar(filler);
    if (value != NULL)
        ft_putstr(value);
}

static void print_sym_type(char type) {
    ft_putchar(' ');
    ft_putchar(type);
    ft_putchar(' ');
}

void    print_symbols(t_list *syms, char *file_path, int ei_class) {
    static char *undefined_types = "Uwv";
    static char *external_types = "ABCDGRSTUVvWwi";
    t_sym       *sym;

    if (g_flags.path) {
        ft_putchar('\n');
        ft_putstr(file_path);
        ft_putstr(":\n");
    }
    for (;syms != NULL; syms = syms->next) {
        sym = (t_sym*)syms->data;
        if (g_flags.undefined && !ft_strchr(undefined_types, sym->type))
            continue;
        if (g_flags.external && !ft_strchr(external_types, sym->type))
            continue;
        if (!g_flags.all && (sym->st_type == STT_SECTION || sym->st_type == STT_FILE))
            continue;
        if (sym->type == '?')
            continue;
        
        print_sym_value(sym->value, ei_class);
        print_sym_type(sym->type);
        ft_putstr_nl(sym->name);
    }
}