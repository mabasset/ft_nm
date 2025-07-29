#include "ft_nm.h"

extern t_flags  g_flags;

char    *x_(get_value)(t_sym_info symbol_info) {
    char        *str;
    const char  *hex_digits = "0123456789abcdef";

    str = malloc(Elf_Addr_len + 1);
    if (!str)
        return NULL;

    for (int i = Elf_Addr_len - 1; i >= 0; i--) {
        if (symbol_info.st_shndx == SHN_UNDEF) {
            str[i] = ' ';
            continue;
        }
        str[i] = hex_digits[symbol_info.st_value & 0xF];
        symbol_info.st_value >>= 4;
    }
    str[Elf_Addr_len] = '\0';
    return str;
}


char    *x_(get_symbol_section_name)(uint16_t st_shndx, t_sections sections) {
    Elf_Shdr    shdr;
    Elf_Word    sh_name;

    if (st_shndx >= sections.count)
        return NULL;
    shdr = sections.headers[st_shndx];
    sh_name = resolve_endianess(shdr.sh_name);
    if (sh_name >= sections.strtab.size)
        return NULL;

    return sections.strtab.content + sh_name; 
}

void    x_(find_section_name_type)(char *c, uint16_t st_shndx, t_sections sections) {
    char    *sh_names[] = { ".bss", ".data", ".text", ".rodata", NULL };
    char    sym_types[] = { 'B', 'D', 'T', 'R', '\0' };
    char    *sh_name;

    sh_name = x_(get_symbol_section_name)(st_shndx, sections);
    if (sh_name == NULL)
        return;
    for (int i = 0; sh_names[i] != NULL; i++) {
        if (ft_strcmp(sh_name, sh_names[i]) != 0)
            continue;
        *c = sym_types[i];
    }
}

void    x_(find_special_type)(char *c, uint16_t st_shndx) {
    switch (st_shndx) {
        case SHN_UNDEF:
            *c = 'U';
            break;
        case SHN_COMMON:
            *c = 'C';
            break;
        case SHN_ABS:
            *c = 'A';
            break;
    }
}

void    x_(find_symbol_relations)(char *c, t_sym_info symbol_info) {
    if (symbol_info.st_bind == STB_GNU_UNIQUE)
        *c = 'u';
    else if (symbol_info.st_type == STT_GNU_IFUNC)
        *c = 'i';
    else if (symbol_info.st_bind == STB_WEAK) {
        if (symbol_info.st_type == STT_OBJECT)
            *c = (symbol_info.st_shndx == SHN_UNDEF) ? 'v' : 'V';
        else
            *c = (symbol_info.st_shndx == SHN_UNDEF) ? 'w' : 'W';
    }
}

void    x_(resolve_visibility)(char *c, unsigned char bind) {
    if (*c == '\0')
        return;
    if (bind == STB_LOCAL && *c != 'U')
        *c += ('a' - 'A');
}

char    x_(get_type)(t_sym_info symbol_info, t_sections sections) {
    char    c = '\0';

    x_(find_special_type)(&c, symbol_info.st_shndx);
    x_(find_section_name_type)(&c, symbol_info.st_shndx, sections);
    x_(find_symbol_relations)(&c, symbol_info);
    x_(resolve_visibility)(&c, symbol_info.st_bind);
    return c;
}

char    *x_(get_name)(t_sym_info symbol_info, t_string strtab, t_sections sections) {
    if (!g_flags.all && (symbol_info.st_type == STT_SECTION || symbol_info.st_type == STT_FILE))
        return '\0';
    if (symbol_info.st_type == STT_SECTION)
        return x_(get_symbol_section_name)(symbol_info.st_shndx, sections);

    if (symbol_info.st_name == 0 || symbol_info.st_name >= strtab.size)
        return NULL;

    return strtab.content + symbol_info.st_name;
}

t_sym_info  **x_(init_symbols_infos)(t_symbols symbols, t_sections sections) {
    t_sym_info  **symbols_infos;
    t_sym_info  tmp_symbol_info;
    Elf_Sym     symbol;
    int         j = 0;

    symbols_infos = (t_sym_info **) malloc(sizeof(t_sym_info *) * (symbols.count + 1));
    if (symbols_infos == NULL)
        return NULL;

    for (Elf_Word i = 0; i < symbols.count; i++) {
        symbol = symbols.table[i];
        tmp_symbol_info.st_shndx = resolve_endianess(symbol.st_shndx);
        tmp_symbol_info.st_value = resolve_endianess(symbol.st_value);
        tmp_symbol_info.st_name  = resolve_endianess(symbol.st_name);
        tmp_symbol_info.st_bind  = ELF_ST_BIND(symbol.st_info);
        tmp_symbol_info.st_type  = ELF_ST_TYPE(symbol.st_info);

        tmp_symbol_info.name = x_(get_name)(tmp_symbol_info, symbols.strtab, sections);
        if (!tmp_symbol_info.name || tmp_symbol_info.name[0] == '\0')
            continue;
        tmp_symbol_info.type = x_(get_type)(tmp_symbol_info, sections);
        if (tmp_symbol_info.type == '\0')
            continue;
        tmp_symbol_info.value = x_(get_value)(tmp_symbol_info);
        if (tmp_symbol_info.value == NULL)
            continue;

        printf("%s %c %s\n", tmp_symbol_info.value, tmp_symbol_info.type, tmp_symbol_info.name);
        symbols_infos[j] = malloc(sizeof(t_sym_info));
        *symbols_infos[j] = tmp_symbol_info;
        j++;
    }
    symbols_infos[j] = NULL;

    return symbols_infos;
}