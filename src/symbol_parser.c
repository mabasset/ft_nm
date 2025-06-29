#include "ft_nm.h"

extern t_elf_file g_elf_file;

char *x_(get_value)(Elf_Sym symbol) {
    char *str;
    const char *hex_digits = "0123456789abcdef";

    str = malloc(Elf_Addr_len + 1);
    if (!str)
        return NULL;
    for (int i = Elf_Addr_len - 1; i >= 0; i--)
    {
        if (symbol.st_shndx == SHN_UNDEF)
        {
            str[i] = ' ';
            continue;
        }
        str[i] = hex_digits[symbol.st_value & 0xF];
        symbol.st_value >>= 4;
    }
    str[Elf_Addr_len] = '\0';

    return str;
}

char x_(get_type)(Elf_Sym symbol, Elf_Shdr *sh_arr) {
    char c = '?';
    uint64_t flags;
    uint64_t bind = ELF64_ST_BIND(symbol.st_info);
    uint64_t type = ELF64_ST_TYPE(symbol.st_info);
    uint16_t shndx = read_uint16(symbol.st_shndx, file_data);

    uint64_t shnum = read_uint16(elf_header->e_shnum, file_data);

    if (bind == STB_GNU_UNIQUE)
        return 'u';
    if (type == STT_GNU_IFUNC)
        return 'i';
    if (bind == STB_WEAK) {
        if (type == STT_OBJECT)
            return (shndx == SHN_UNDEF) ? 'v' : 'V';
        else
            return (shndx == SHN_UNDEF) ? 'w' : 'W';
    }
    if (symbol.st_shndx == SHN_UNDEF)
        return 'U';
    if (symbol.st_shndx == SHN_ABS)
        return 'a';
    if (symbol.st_shndx == SHN_COMMON)
        return 'C';
    else if (shndx < shnum)
    {
        type = read_uint64(shdr[shndx].sh_type, file_data);
        flags = read_uint64(shdr[shndx].sh_flags, file_data);

        if (type == SHT_NOBITS)
            c = 'B';
        else if (!(flags & SHF_WRITE))
        {
            if (flags & SHF_ALLOC && flags & SHF_EXECINSTR)
                c = 'T';
            else
                c = 'R';
        }
        else if (flags & SHF_EXECINSTR)
            c = 'T';
        else
            c = 'D';
    }
    if (bind == STB_LOCAL && c != '?')
        c += 32;
    return c;
}

char *x_(get_name)(Elf_Sym symbol, char *names) {
    char *name = NULL;

    if (symbol.st_name != 0)
        name = names + symbol.st_name;
    return name;
}

t_sym_info *x_(get_symbols_info)(Elf_Sym *sym_arr, size_t *n_sym, Elf_Shdr *strtab, Elf_Shdr *sh_arr) {
    char        *names;
    t_sym_info  *sym_info_arr;
    size_t      j;

    names = g_elf_file.content + strtab->sh_offset;
    sym_info_arr = malloc(sizeof(t_sym_info) * (*n_sym));
    if (!sym_info_arr)
        return NULL;

    j = 0;
    for (size_t i = 1; i < *n_sym; i++) {
        t_sym_info sym_info;
        sym_info.type = x_(get_type)(sym_arr[i], sh_arr);
        sym_info.name = x_(get_name)(sym_arr[i], names);
        if (!sym_info.name || sym_info.name[0] == '\0')
            continue;
        sym_info.value = x_(get_value)(sym_arr[i]);
        sym_info_arr[j++] = sym_info;
    }
    *n_sym = j;
    return sym_info_arr;
}
