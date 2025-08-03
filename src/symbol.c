#include "ft_nm.h"

extern t_flags  g_flags;

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

char    *x_(get_name)(t_sym_info symbol_info, t_string strtab) {
    if (symbol_info.st_name >= strtab.size)
        return NULL;
    if (symbol_info.st_name != 0)
        return strtab.content + symbol_info.st_name;
    if (symbol_info.st_shndx == SHN_ABS)
        return "";
    if (symbol_info.st_type == STT_SECTION)
        return symbol_info.sh_name;
    return NULL;
}

char    *x_(get_value)(Elf_Sym symbol, uint16_t st_shndx) {
    char        *str;
    uint_t      st_value;
    const char  *hex_digits = "0123456789abcdef";

    str = ft_calloc(Elf_Addr_len + 1, sizeof(char));
    if (str == NULL)
        return NULL;

    if (st_shndx == SHN_UNDEF)
        return str;
    st_value = resolve_endianess(symbol.st_value);
    for (int i = Elf_Addr_len - 1; i >= 0; i--) {
        str[i] = hex_digits[st_value & 0xF];
        st_value >>= 4;
    }
    str[Elf_Addr_len] = '\0';

    return str;
}

void    x_(find_section_type)(char *c, t_sym_info symbol_info, t_sections sections) {
    Elf64_Word  sh_flags, sh_type;

    if (symbol_info.st_shndx >= SHN_LORESERVE || symbol_info.st_shndx >= sections.count)
        return;

    if (ft_strncmp(symbol_info.sh_name, ".debug", 6) == 0) {
        *c = 'N';
        return;
    }

    sh_flags = resolve_endianess(sections.headers[symbol_info.st_shndx].sh_flags);
    sh_type = resolve_endianess(sections.headers[symbol_info.st_shndx].sh_type);

    if (symbol_info.type == STT_FUNC || (sh_flags & SHF_EXECINSTR))
        *c = 'T';
    else if (sh_type == SHT_NOBITS)
        *c = 'B';
    else if (sh_flags & SHF_WRITE)
        *c = 'D';
    else if (sh_flags & SHF_ALLOC)
        *c = 'R';
    else
        *c = 'n';
}

void    x_(find_special_type)(char *c, t_sym_info symbol_info) {
    switch (symbol_info.st_shndx) {
        case SHN_UNDEF:
            *c = 'U';
            return;
        case SHN_COMMON:
            *c = 'C';
            return;
        case SHN_ABS:
            *c = 'A';
            return;
    }
    if (symbol_info.st_bind == STB_GNU_UNIQUE)
        *c = 'u';
    else if (symbol_info.st_type == STT_GNU_IFUNC)
        *c = 'i';
}

void    x_(resolve_visibility)(char *c, t_sym_info symbol_info) {
    if (*c == '?')
        return;

    else if (symbol_info.st_bind == STB_WEAK && symbol_info.st_type != STT_GNU_IFUNC) {
        if (symbol_info.st_type == STT_OBJECT)
            *c = (symbol_info.st_shndx == SHN_UNDEF) ? 'v' : 'V';
        else
            *c = (symbol_info.st_shndx == SHN_UNDEF) ? 'w' : 'W';
    }
    if (symbol_info.st_bind == STB_LOCAL && *c != 'N' && *c != 'n')
        *c += ('a' - 'A');
}

char    x_(get_type)(t_sym_info symbol_info, t_sections sections) {
    char    c = '?';

    x_(find_section_type)(&c, symbol_info, sections);
    x_(find_special_type)(&c, symbol_info);
    x_(resolve_visibility)(&c, symbol_info);

    return c;
}

t_sym_info  *x_(create_symbol_info)(Elf_Sym symbol, t_symbols symbols, t_sections sections) {
    t_sym_info  *sym_info;
    char        *name;

    sym_info = ft_calloc(1, sizeof(t_sym_info));
    if (sym_info == NULL)
        return NULL;
    sym_info->st_bind = ELF_ST_BIND(symbol.st_info);
    sym_info->st_type = ELF_ST_TYPE(symbol.st_info);
    sym_info->st_name = resolve_endianess(symbol.st_name);
    sym_info->st_shndx = resolve_endianess(symbol.st_shndx);
    sym_info->sh_name = x_(get_symbol_section_name)(sym_info->st_shndx, sections);

    name = x_(get_name)(*sym_info, symbols.strtab);
    if (name == NULL) {
        free(sym_info);
        return NULL;
    }
    sym_info->name = name;
    sym_info->value = x_(get_value)(symbol, sym_info->st_shndx);
    sym_info->type = x_(get_type)(*sym_info, sections);

    return sym_info;
}

t_sym_info **x_(init_symbols_info)(t_symbols symbols, t_sections sections) {
    t_sym_info  **symbols_info;
    int         j = 0;

    symbols_info = (t_sym_info **) ft_calloc(symbols.count + 1, sizeof(t_sym_info *));
    if (symbols_info == NULL)
        return NULL;

    for (Elf_Word i = 0; i < symbols.count; i++) {
        symbols_info[j] = x_(create_symbol_info)(symbols.table[i], symbols, sections);
        if (symbols_info[j] != NULL)
            j++;
    }
    symbols_info[j] = NULL;

    return symbols_info;
}
