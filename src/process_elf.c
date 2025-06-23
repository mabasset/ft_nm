#include "ft_nm.h"

extern t_elf_info g_elf;

int    process_elf64() {
    Elf64_Ehdr  *header;
    Elf64_Shdr  *sh_arr;
    Elf64_Shdr  *symtab;
    Elf64_Shdr  *strtab;
    Elf64_Sym   *sym_arr;
    t_sym_info  *sym_info_arr;
    size_t      n_sym;

    header = (Elf64_Ehdr *) g_elf.content;
    sh_arr = get_section_headers(header);
    if (!sh_arr)
        return 1;
    symtab = get_symtab_section(sh_arr, header->e_shnum);
    if (!symtab)
        return 0;
    strtab = get_strtab_section(sh_arr, symtab->sh_link);
    if (!strtab)
        return 1;

    sym_arr = (Elf64_Sym *) (g_elf.content + symtab->sh_offset);
    n_sym = symtab->sh_size / sizeof(Elf64_Sym);
    sym_info_arr = get_symbols_info(sym_arr, &n_sym, strtab, sh_arr);
    if (!sym_info_arr)
        return 1;
    sort_symbols_info(sym_info_arr, n_sym);
    display_symbols_info(sym_info_arr, n_sym);
    free(sym_info_arr);
    return 0;
}