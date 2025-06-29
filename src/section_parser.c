#include "ft_nm.h"

extern t_elf_file g_elf_file;

Elf_Shdr  *x_(get_section_headers)(Elf_Ehdr *header) {
    size_t      sh_arr_size;
    Elf_Off     shoff;
    Elf_Half    shnum;
    Elf_Half    shentsize;

    shoff = swap_endian_if_needed(header->e_shoff);
    shnum = swap_endian_if_needed(header->e_shnum);
    shentsize = swap_endian_if_needed(header->e_shentsize);

    if (shoff == 0 || shoff >= g_elf_file.size)
        return NULL;
    sh_arr_size = shnum * shentsize;
    if (shoff + sh_arr_size > g_elf_file.size)
        return NULL;

    return (Elf_Shdr *) (g_elf_file.content + shoff);
}

Elf_Shdr  *x_(get_symtab_section)(Elf_Shdr *sections, size_t num_sections) {
    size_t      i;
    Elf_Shdr    *symtab;

    for (i = 0; sections[i].sh_type != SHT_SYMTAB; i++) {
        if (i == num_sections) {
            write(STDOUT_FILENO, "ft_nm: ", 4);
            write(STDOUT_FILENO, g_elf_file.path, ft_strlen(g_elf_file.path));
            write(STDOUT_FILENO, ": no symbols\n", 13);
            return NULL;
        }
    }
    symtab = &sections[i];
    if (symtab->sh_offset == 0 || symtab->sh_offset >= g_elf_file.size)
        return NULL;
    return symtab;
}

Elf_Shdr  *x_(get_strtab_section)(Elf_Shdr *sh_arr, int idx_sym_name) {
    Elf_Shdr  *strtab;

    strtab = &sh_arr[idx_sym_name];
    if (strtab->sh_offset == 0 || strtab->sh_offset >= g_elf_file.size)
        return NULL;
    return strtab;
}
