#include "ft_nm.h"

extern t_elf_info g_elf;

char    *get_value(Elf64_Sym symbol) {
    char        *str;
    const char  *hex_digits = "0123456789abcdef";

    str = malloc(17);
    if (!str)
        return NULL;
    for (int i = 15; i >= 0; i--) {
        if (symbol.st_shndx == SHN_UNDEF) {
            str[i] = ' ';
            continue;
        }
        str[i] = hex_digits[symbol.st_value & 0xF];
        symbol.st_value >>= 4;
    }
    str[16] = '\0';

    return str;
}

char get_type(Elf64_Sym symbol, Elf64_Shdr *sh_arr, char *names) {
    unsigned char bind = ELF64_ST_BIND(symbol.st_info);
    unsigned char type = ELF64_ST_TYPE(symbol.st_info);
    char c = '?';

    if (type == STT_SECTION || type == STT_FILE)
        return '\0';
    
    if (symbol.st_shndx == SHN_UNDEF)
        c = (bind == STB_WEAK) ? 'w' : 'U';
    else if (symbol.st_shndx == SHN_ABS)
        c = 'A';
    else if (symbol.st_shndx == SHN_COMMON)
        c = 'C';
    else if (sh_arr && symbol.st_shndx < SHN_LORESERVE) {
        Elf64_Word sh_flags = sh_arr[symbol.st_shndx].sh_flags;
        Elf64_Word sh_type = sh_arr[symbol.st_shndx].sh_type;

        if (type == STT_FUNC || (type == STT_NOTYPE && sh_flags & SHF_EXECINSTR))
            c = 'T';
        else if (sh_type == SHT_NOBITS)
            c = 'B';
        else if (sh_flags & SHF_WRITE)
            c = 'D';
        else
            c = 'R';
    }

    if (bind == STB_WEAK && symbol.st_shndx != SHN_UNDEF)
        c = (c == 'T') ? 'V' : 'W';
    else if (bind == STB_LOCAL && c != 'U' && c != 'w')
        c += ('a' - 'A');

    return c;
}

char    *get_name(Elf64_Sym symbol, char *names) {
    char    *name = NULL;

    if (symbol.st_name != 0)
        name = names + symbol.st_name;
    return name;
}

t_sym_info  *get_symbols_info(Elf64_Sym *sym_arr, size_t *n_sym, Elf64_Shdr *strtab, Elf64_Shdr *sh_arr) {
    char        *names;
    t_sym_info  *sym_info_arr;
    size_t      j;

    names = g_elf.content + strtab->sh_offset;
    sym_info_arr = malloc(sizeof(t_sym_info) * (*n_sym));
    if (!sym_info_arr)
        return NULL;

    j = 0;
    for (size_t i = 1; i < *n_sym; i++) {
        t_sym_info sym_info;
        sym_info.value = get_value(sym_arr[i]);
        sym_info.type = get_type(sym_arr[i], sh_arr, names);
        sym_info.name = get_name(sym_arr[i], names);
        if (sym_info.type == '\0' || !sym_info.name || sym_info.name[0] == '\0')
            continue;
        sym_info_arr[j++] = sym_info;
    }
    *n_sym = j;
    return sym_info_arr;
}

void    sort_symbols_info(t_sym_info *sym_info_arr, size_t n_sym) {
    t_sym_info  temp;

    for (size_t i = 0; i < n_sym - 1; i++) {
        for (size_t j = 0; j < n_sym - i - 1; j++) {
            if (strcmp(sym_info_arr[j].name, sym_info_arr[j + 1].name) > 0) {
                temp = sym_info_arr[j];
                sym_info_arr[j] = sym_info_arr[j + 1];
                sym_info_arr[j + 1] = temp;
            }
        }
    }
}

void    display_symbols_info(t_sym_info *sym_info_arr, size_t n_sym) {
    t_sym_info  sym_info;

    for (size_t i = 0; i < n_sym; i++) {
        sym_info = sym_info_arr[i];
        write(STDOUT_FILENO, sym_info.value, strlen(sym_info.value));
        write(STDOUT_FILENO, " ", 1);
        write(STDOUT_FILENO, &sym_info.type, 1);
        write(STDOUT_FILENO, " ", 1);
        write(STDOUT_FILENO, sym_info.name, strlen(sym_info.name));
        write(STDOUT_FILENO, "\n", 1);
    }
}