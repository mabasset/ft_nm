#include "ft_nm.h"

extern t_flags  g_flags;

char    *x_(get_value)(Elf_Sym symbol) {
    char        *str;
    const char  *hex_digits = "0123456789abcdef";
    uint_t      st_value;

    str = malloc(Elf_Addr_len + 1);
    if (!str)
        return NULL;
    st_value = resolve_endianess(symbol.st_value);

    for (int i = Elf_Addr_len - 1; i >= 0; i--) {
        if (resolve_endianess(symbol.st_shndx) == SHN_UNDEF) {
            str[i] = ' ';
            continue;
        }
        str[i] = hex_digits[st_value & 0xF];
        st_value >>= 4;
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

char    x_(find_section_name_type)(uint16_t st_shndx, t_sections sections) {
    char    *sh_names[] = { ".bss", ".data", ".text", ".rodata", NULL };
    char    sym_types[] = { 'B', 'D', 'T', 'R', '\0' };
    char    *sh_name;

    sh_name = x_(get_symbol_section_name)(st_shndx, sections);
    if (sh_name == NULL)
        return '?';
    for (int i = 0; sh_names[i] != NULL; i++)
        if (ft_strcmp(sh_name, sh_names[i]) == 0)
            return sym_types[i];

    return '?';
}

char    x_(find_special_type)(uint16_t st_shndx) {
    switch (st_shndx) {
        case SHN_UNDEF:
            return 'U';
        case SHN_COMMON:
            return 'C';
        case SHN_ABS:
            return 'A';
        default:
            return '?';
    }
}

char    x_(get_type)(Elf_Sym symbol, t_sections sections) {
    unsigned char   bind, type;
    uint16_t        st_shndx;
    char            c = '?';

    bind = ELF_ST_BIND(symbol.st_info);
    type = ELF_ST_TYPE(symbol.st_info);

    st_shndx = resolve_endianess(symbol.st_shndx);
    c = x_(find_special_type)(st_shndx);
    if ()
    c = x_(find_section_name_type)(st_shndx, sections);
    if (c == '?') 

    return c;
}

char    *x_(get_name)(Elf_Sym symbol, t_string strtab) {
    uint32_t    st_name;

    st_name = resolve_endianess(symbol.st_name);
    if (st_name == 0 || st_name >= strtab.size)
        return NULL;
    return strtab.content + st_name;
}

t_sym_info  **x_(init_symbols_infos)(t_symbols symbols, t_sections sections) {
    t_sym_info  **symbols_infos;
    Elf_Sym     sym;
    char        *name;
    int         j = -1;

    printf("%d\n", symbols.count);
    symbols_infos = (t_sym_info **) malloc(sizeof(t_sym_info *) * (symbols.count + 1));
    if (symbols_infos == NULL)
        return NULL;

    for (Elf_Word i = 0; i < symbols.count; i++) {
        sym = symbols.table[i];
        name = x_(get_name)(sym, symbols.strtab);
        if (name == NULL || name[0] == '\0')
            continue ;
        j++;
        symbols_infos[j] = malloc(sizeof(t_sym_info));
        symbols_infos[j]->value = x_(get_value)(sym);
        symbols_infos[j]->type = x_(get_type)(sym, sections);
        symbols_infos[j]->name = name;
        // Elf_Shdr *shdr = &sections.headers[sym.st_shndx];
        // printf("%s %s %s\n", sym_info->value, sym_info->name, sections.strtab.content + shdr->sh_name);
        // if (sym.st_shndx < symbols.count) {
        //     Elf_Shdr shdr = sections.headers[sym.st_shndx];
            printf("%s %c %s\n", symbols_infos[j]->value, symbols_infos[j]->type, symbols_infos[j]->name);
        // }
    }
    symbols_infos[++j] = NULL;
    printf("ciao\n");
    return symbols_infos;
}
