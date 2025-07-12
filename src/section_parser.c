#include "ft_nm.h"

extern t_elf_file g_elf_file;

Elf_Shdr  *x_(get_section_headers)(Elf_Ehdr *header) {
    size_t      sh_arr_size;
    Elf_Off     shoff;
    Elf_Half    shnum;
    Elf_Half    shentsize;

    shoff = resolve_endianess(header->e_shoff);
    shnum = resolve_endianess(header->e_shnum);
    shentsize = resolve_endianess(header->e_shentsize);

    if (shoff == 0 || shoff >= g_elf_file.size)
        return NULL;
    sh_arr_size = shnum * shentsize;
    if (shoff + sh_arr_size > g_elf_file.size)
        return NULL;

    return (Elf_Shdr *) (g_elf_file.content + shoff);
}

Elf_Shdr  *x_(get_symtab_section)(Elf_Shdr *sections, size_t num_sections) {
    uint32_t    sh_type;
    Elf_Shdr    *symtab;
    Elf_Off     sh_offset;

    for (size_t i = 0; i < num_sections; i++) {
        sh_type = resolve_endianess(sections[i].sh_type);
        if (sh_type == SHT_SYMTAB) {
            symtab = &sections[i];
            sh_offset = resolve_endianess(symtab->sh_offset);
            if (sh_offset > 0 || sh_offset < g_elf_file.size)
                return symtab;
            break;
        }
    }
    write(STDOUT_FILENO, "ft_nm: ", 4);
    write(STDOUT_FILENO, g_elf_file.path, ft_strlen(g_elf_file.path));
    write(STDOUT_FILENO, ": no symbols\n", 13);
    return NULL;
}

Elf_Shdr  *x_(get_strtab_section)(Elf_Shdr *sh_arr, int idx_sym_name) {
    Elf_Shdr    *strtab;
    Elf32_Off   sh_offset;

    strtab = &sh_arr[idx_sym_name];
    sh_offset = resolve_endianess(strtab->sh_offset);
    if (sh_offset == 0 || sh_offset >= g_elf_file.size)
        return NULL;
    return strtab;
}
