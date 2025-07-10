#include "ft_nm.h"

extern t_elf_file   g_elf_file;
extern t_flags      g_flags;

int    x_(process_elf)() {
    Elf_Ehdr    *header;
    Elf_Shdr    *sh_arr;
    Elf_Shdr    *symtab;
    Elf_Shdr    *strtab;
    Elf_Sym     *sym_arr;
    t_sym_info  *sym_info_arr;
    size_t      n_sym;

    header = (Elf_Ehdr *) g_elf_file.content;
    sh_arr = x_(get_section_headers)(header);
    if (!sh_arr)
        return 1;
    symtab = x_(get_symtab_section)(sh_arr, header->e_shnum);
    if (!symtab)
        return 0;
    strtab = x_(get_strtab_section)(sh_arr, symtab->sh_link);
    if (!strtab)
        return 1;

    sym_arr = (Elf_Sym *) (g_elf_file.content + symtab->sh_offset);
    n_sym = symtab->sh_size / sizeof(Elf_Sym);
    sym_info_arr = x_(get_symbols_info)(sym_arr, &n_sym, strtab, sh_arr);
    if (!sym_info_arr)
        return 1;
    if (!g_flags.no_sort)
        sort_symbols(sym_info_arr, n_sym);
    display_symbols(sym_info_arr, n_sym);
    free_symbols(sym_info_arr, n_sym);
    return 0;
}