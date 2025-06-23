#include "ft_nm.h"

extern t_elf_info g_elf;

Elf64_Shdr  *get_section_headers(Elf64_Ehdr *header) {
    size_t      sh_arr_size;

    if (header->e_shoff == 0 || header->e_shoff >= g_elf.size)
        return NULL;
    sh_arr_size = header->e_shnum * header->e_shentsize;
    if (header->e_shoff + sh_arr_size > g_elf.size)
        return NULL;

    return (Elf64_Shdr *) (g_elf.content + header->e_shoff);
}

Elf64_Shdr  *get_symtab_section(Elf64_Shdr *sections, size_t num_sections) {
    size_t      i;
    Elf64_Shdr  *symtab;

    for (i = 0; sections[i].sh_type != SHT_SYMTAB; i++) {
        if (i == num_sections) {
            write(STDOUT_FILENO, "nm: ", 4);
            write(STDOUT_FILENO, g_elf.path, strlen(g_elf.path));
            write(STDOUT_FILENO, ": no symbols\n", 13);
            return NULL;
        }
    }
    symtab = &sections[i];
    if (symtab->sh_offset == 0 || symtab->sh_offset >= g_elf.size)
        return NULL;
    return symtab;
}

Elf64_Shdr  *get_strtab_section(Elf64_Shdr *sh_arr, int idx_sym_name) {
    Elf64_Shdr  *strtab;

    strtab = &sh_arr[idx_sym_name];
    if (strtab->sh_offset == 0 || strtab->sh_offset >= g_elf.size)
        return NULL;
    return strtab;
}
