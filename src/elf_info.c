#include "ft_nm.h"

extern t_flags  g_flags;

Elf_Shdr  *x_(get_shdr)(Elf_Shdr *headers, Elf_Word count, Elf_Word index) {
    if (index >= count)
        return NULL;

    return headers + index;
}

char  *x_(get_shcontent)(Elf_Shdr *header, t_string file) {
    Elf_Off sh_offset;
    uint_t  sh_size;

    sh_offset = resolve_endianess(header->sh_offset);
    sh_size = resolve_endianess(header->sh_size);
    if (sh_offset + sh_size > file.size)
        return NULL;

    return file.content + sh_offset;
}

int x_(set_strtab)(t_string *strtab, Elf_Shdr *headers, Elf_Word count, Elf_Word index, t_string file) {
    Elf_Shdr    *strtabhdr;
    char        *content;

    strtabhdr = x_(get_shdr)(headers, count, index);
    if (strtabhdr == NULL)
        return 1;
    content = x_(get_shcontent)(strtabhdr, file);
    if (content == NULL)
        return 1;
    strtab->content = content;
    strtab->size = resolve_endianess(strtabhdr->sh_size);

    return 0;
}

Elf_Shdr  *x_(find_symtabhdr)(Elf_Shdr *headers, Elf_Word count) {
    Elf_Word    sh_type;

    for (size_t i = 0; i < count; i++) {
        sh_type = resolve_endianess(headers[i].sh_type);
        if (sh_type != SHT_SYMTAB)
            continue;
        return headers + i;
    }

    return NULL;
}

Elf_Shdr *x_(get_shdrs)(Elf_Ehdr *ehdr, t_string file) {
    Elf_Off     offset;
    Elf_Word    count;
    Elf_Word    size;

    offset = resolve_endianess(ehdr->e_shoff);
    count = resolve_endianess(ehdr->e_shnum);
    size = resolve_endianess(ehdr->e_shentsize);
    if (offset + count * size > file.size)
        return NULL;
    return (Elf_Shdr *) (file.content + offset);
}

int x_(set_elf_info)(t_elf_info *elf_info, Elf_Ehdr *ehdr, t_string file) {
    Elf_Shdr    *symtabhdr;

    elf_info->shdrs = x_(get_shdrs)(ehdr, file);
    elf_info->shcount = resolve_endianess(ehdr->e_shnum);
    if (x_(set_strtab)(&elf_info->shstrtab, elf_info->shdrs, elf_info->shcount, resolve_endianess(ehdr->e_shstrndx), file))
        return 1;

    symtabhdr = x_(find_symtabhdr)(elf_info->shdrs, elf_info->shcount);
    if (symtabhdr == NULL || symtabhdr->sh_entsize == 0)
        return 0;
    elf_info->symtabs = (Elf_Sym *) x_(get_shcontent)(symtabhdr, file);
    if (elf_info->symtabs == NULL)
        return 1;
    elf_info->symcount = resolve_endianess(symtabhdr->sh_size) / resolve_endianess(symtabhdr->sh_entsize);
    if (x_(set_strtab)(&elf_info->symstrtab, elf_info->shdrs, elf_info->shcount, resolve_endianess(symtabhdr->sh_link), file))
        return 1;

    return 0;
}
