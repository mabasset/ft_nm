#include "ft_nm.h"

extern t_elf_info g_elf;

Elf64_Shdr  *get_symtab(Elf64_Shdr *sections, size_t num_sections) {
    size_t i;

    for (i = 0; sections[i].sh_type != SHT_SYMTAB; i++)
        if (i == num_sections)
            return NULL;
    return &sections[i];
}

char get_type(Elf64_Sym symbol) {
    unsigned char bind = ELF64_ST_BIND(symbol.st_info);
    unsigned char type = ELF64_ST_TYPE(symbol.st_info);
    char c;

    switch (type) {
        case STT_NOTYPE:
            c = (symbol.st_shndx == SHN_UNDEF) ? 'U' : 'N';
            break;
        case STT_OBJECT:
            c = (symbol.st_shndx == SHN_COMMON) ? 'C' : 'D';
            break;
        case STT_FUNC:
            c = 'T';
            break;
        case STT_SECTION:
            c = 'S';
            break;
        case STT_FILE:
            c = 'F';
            break;
        default:
            c = '?';
    }
    if (bind == STB_LOCAL && c != '?') {
        c = c + ('a' - 'A');
    }

    return c;
}

char    *get_name(size_t idx_name, char *names, size_t names_size) {
    if (idx_name == 0 || idx_name >= names_size)
        return NULL;
    return names + idx_name;
}

t_symbol    *translate_symbol(Elf64_Shdr strtab, Elf64_Shdr *symtab, char *content) {

}

char    *get_names(unsigned long int offset) {
    if (offset == 0 || offset >= g_elf.size)
        return NULL;
    return g_elf.content + offset;
}

t_symbol    *translate_symbols(Elf64_Shdr strtab, Elf64_Shdr *symtab) {
    char        *names;
    Elf64_Sym   *elf_symbols;
    size_t      num_symbols;
    t_symbol    *symbols;


    if (strtab.sh_offset == 0 || strtab.sh_offset >= g_elf.size)
        return NULL;
    names = g_elf.content + strtab.sh_offset;

    if (symtab->sh_offset == 0 || symtab->sh_offset >= g_elf.size)
        return NULL;
    elf_symbols = (Elf64_Sym *) (g_elf.content + symtab->sh_offset);

    num_symbols = symtab->sh_size / sizeof(Elf64_Sym);

    symbols = malloc(sizeof(t_symbol) * num_symbols);
    if (!symbols)
        return NULL;

    for (size_t i = 0, k = 0; i < num_symbols; i++) {
        t_symbol symbol = create_symbol(elf_symbols[i], names, strtab.sh_size);
        if (!symbol)
            continue ;
        symbols[k] = *symbol;
        printf("%c %s\n", name);
    }
    return symbols;
}

int    process_elf64() {
    Elf64_Ehdr  *header;
    Elf64_Shdr  *sections;
    Elf64_Shdr  *symtab;
    Elf64_Shdr  strtab;
    size_t      sections_size;

    header = (Elf64_Ehdr *) g_elf.content;
    if (header->e_shoff == 0 || header->e_shoff >= g_elf.size)
        return 1;
    sections_size = header->e_shnum * header->e_shentsize;
    if (header->e_shoff + sections_size > g_elf.size)
        return 1;

    sections = (Elf64_Shdr *) (g_elf.content + header->e_shoff);
    symtab = get_symtab(sections, header->e_shnum);
    if (!symtab) {
        printf("nm: %s: no symbols\n", g_elf.path);
        return 0;
    }
    strtab = sections[symtab->sh_link];

    return display_symbols(strtab, symtab, g_elf.content);
}