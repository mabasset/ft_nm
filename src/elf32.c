#include "ft_nm.h"

extern t_elf_info g_elf;

int    process_elf32(char* content) {
    (void)content;
    // Elf32_Ehdr  *header = (Elf32_Ehdr *) content;

    // if (header->e_shoff == 0 || header->e_shoff >= g_elf.size)
    //     return 1;
    // size_t sections_size = header->e_shnum * header->e_shentsize;
    // if (header->e_shoff + sections_size > g_elf.size)
    //     return 1;

    // Elf32_Shdr  *sections = content + header->e_shoff;
    // for (int i = 0)
    return 0;
}