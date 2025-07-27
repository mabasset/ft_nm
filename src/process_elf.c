#include "ft_nm.h"

extern t_flags  g_flags;

int x_(init_sections)(t_sections *sections, Elf_Ehdr *file_header, t_string mapped_file) {
    Elf_Off     offset;
    Elf_Word    count;
    Elf_Word    size;

    offset = resolve_endianess(file_header->e_shoff);
    count = resolve_endianess(file_header->e_shnum);
    size = resolve_endianess(file_header->e_shentsize);
    if (offset + count * size > mapped_file.size)
        return 1;
    sections->headers = (Elf_Shdr *) (mapped_file.content + offset);
    sections->count = count;

    return 0;
}

Elf_Shdr  *x_(get_shdr)(t_sections sections, Elf_Word shndx) {
    Elf_Word    index;

    index = resolve_endianess(shndx);
    if (index >= sections.count)
        return NULL;

    return sections.headers + index;
}

char  *x_(get_sh)(Elf_Shdr *shdr, t_string mapped_file) {
    Elf_Off sh_offset;
    uint_t  sh_size;

    sh_offset = resolve_endianess(shdr->sh_offset);
    sh_size = resolve_endianess(shdr->sh_size);
    if (sh_offset + sh_size > mapped_file.size)
        return NULL;

    return mapped_file.content + sh_offset;
}

int x_(init_strtab)(t_string *shstr, t_sections sections, Elf_Word shstrndx, t_string mapped_file) {
    Elf_Shdr    *shstrhdr;
    char        *strtab;

    shstrhdr = x_(get_shdr)(sections, shstrndx);
    if (shstrhdr == NULL)
        return 1;
    strtab = x_(get_sh)(shstrhdr, mapped_file);
    if (strtab == NULL)
        return 1; 
    shstr->content = strtab;
    shstr->size = resolve_endianess(shstrhdr->sh_size);

    return 0;
}

Elf_Shdr  *x_(get_symhdr)(t_sections sections) {
    for (size_t i = 0; i < sections.count; i++) {
        if (sections.headers[i].sh_type != SHT_SYMTAB)
            continue ;
        return sections.headers + i;
    }

    return NULL;
}

int x_(init_symbols)(t_symbols *symbols, t_sections sections, t_string mapped_file) {
    Elf_Shdr    *symhdr;
    Elf_Sym     *symtab;

    symhdr = x_(get_symhdr)(sections);
    if (symhdr == NULL)
        return 1;
    symtab = (Elf_Sym *) x_(get_sh)(symhdr, mapped_file);
    if (symtab == NULL)
        return 1;

    if (x_(init_strtab)(&symbols->strtab, sections, symhdr->sh_link, mapped_file))
        return 1;

    if (symhdr->sh_entsize == 0)
        return 1;
    symbols->count = resolve_endianess(symhdr->sh_size / symhdr->sh_entsize);
    symbols->table = symtab;

    return 0;
}

t_sym_info  **x_(get_symbols_info)(t_string mapped_file) {
    Elf_Ehdr    *file_header;
    t_sections  sections;
    t_symbols   symbols;

    file_header = (Elf_Ehdr *) mapped_file.content;
    if (x_(init_sections)(&sections, file_header, mapped_file))
        return NULL;
    if (x_(init_strtab)(&sections.strtab, sections, file_header->e_shstrndx, mapped_file))
        return NULL;
    if (x_(init_symbols)(&symbols, sections, mapped_file))
        return (t_sym_info **) ft_calloc(1);

    return x_(init_symbols_infos)(symbols, sections);
}