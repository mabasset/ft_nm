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

void    x_(find_section_name_type)(char *c, uint16_t st_shndx, t_sections sections) {
    char    *sh_names[] = { ".bss", ".data", ".text", ".rodata", NULL };
    char    sym_types[] = { 'B', 'D', 'T', 'R', '\0' };
    char    *sh_name;

    sh_name = x_(get_symbol_section_name)(st_shndx, sections);
    if (sh_name == NULL)
        return;
    for (int i = 0; sh_names[i] != NULL; i++)
        if (ft_strcmp(sh_name, sh_names[i]) == 0)
            *c = sym_types[i];
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

void    x_(find_symbol_relations)(char *c, uint16_t st_shndx, unsigned char bind, unsigned char type) {
    if (bind == STB_GNU_UNIQUE)
        *c = 'u';
    if (bind == STB_WEAK) {
        if (type == STT_OBJECT)
            *c = (st_shndx == SHN_UNDEF) ? 'v' : 'V';
        *c = (st_shndx == SHN_UNDEF) ? 'w' : 'W';
    }
}

char    x_(resolve_visibility)(char c, unsigned char bind) {
    if (bind == STB_LOCAL && c != 'U')
        return c += ('a' - 'A');
    return c;
}

char    x_(get_type)(Elf_Sym symbol, t_sections sections) {
    unsigned char   bind, type;
    uint16_t        st_shndx;
    char            c = '\0';

    bind = ELF_ST_BIND(symbol.st_info);
    type = ELF_ST_TYPE(symbol.st_info);
    st_shndx = resolve_endianess(symbol.st_shndx);

    x_(find_special_type)(&c, st_shndx);
    x_(find_section_name_type)(&c, st_shndx, sections);
    x_(find_symbol_relations)(&c, st_shndx, bind, type);
    return x_(resolve_visibility)(c, bind);
}

char    *x_(get_name)(Elf_Sym symbol, t_string strtab) {
    uint32_t    st_name;

    st_name = resolve_endianess(symbol.st_name);
    if (st_name == 0 || st_name >= strtab.size)
        return NULL;
    return strtab.content + st_name;
}

t_sym_info  **x_(init_symbols_infos)(t_symbols symbols, t_sections sections) {
    t_sym_info      **symbols_infos;
    Elf_Sym         symbol;
    uint16_t        st_shndx;
    unsigned char   st_type;
    char            type, *name;
    int             j = -1;

    symbols_infos = (t_sym_info **) malloc(sizeof(t_sym_info *) * (symbols.count + 1));
    if (symbols_infos == NULL)
        return NULL;

    for (Elf_Word i = 0; i < symbols.count; i++) {
        symbol = symbols.table[i];
        st_shndx = resolve_endianess(symbol.st_shndx);

        type = x_(get_type)(symbol, sections);
        name = x_(get_name)(symbol, symbols.strtab);
        if (name == NULL || !name[0] == NULL)
            continue;
        j++;
        symbols_infos[j] = malloc(sizeof(t_sym_info));
        symbols_infos[j]->value = x_(get_value)(symbol);
        symbols_infos[j]->type = type;
        symbols_infos[j]->name = name;
        printf("%s %c %s\n", symbols_infos[j]->value, symbols_infos[j]->type, symbols_infos[j]->name);
    }
    symbols_infos[++j] = NULL;

    return symbols_infos;
}
