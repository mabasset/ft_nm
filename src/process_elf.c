#include "ft_nm.h"

extern t_elf_file   g_elf_file;
extern t_flags      g_flags;

Elf_Shdr  *x_(get_sections_headers)(Elf_Ehdr *file_header) {
    Elf_Off     offset;
    Elf_Half    count;
    Elf_Half    size;

    offset = resolve_endianess(file_header->e_shoff);
    count = resolve_endianess(file_header->e_shnum);
    size = resolve_endianess(file_header->e_shentsize);
    if (offset == 0 || offset + count * size > g_elf_file.size)
        return NULL;

    return (Elf_Shdr *) (g_elf_file.content + offset);
}

Elf_Shdr  *x_(get_symbol_table_header)(Elf_Shdr *sections_headers, Elf_Half headers_count) {

    for (size_t i = 0; i < headers_count; i++) {
        if (sections_headers[i].sh_type != SHT_SYMTAB)
            continue ;
        return sections_headers + i;
    }
    print_no_symbols();

    return NULL;
}

char  *x_(get_string_table)(Elf_Shdr *sections_headers, Elf_Word string_table_header_index) {
    Elf_Shdr    *string_table_header;
    Elf32_Off   sh_offset;

    strtab = &sh_arr[idx_sym_name];
    sh_offset = resolve_endianess(strtab->sh_offset);
    if (sh_offset == 0 || sh_offset >= g_elf_file.size)
        return NULL;
    return strtab;
}

int    x_(process_elf)() {
    Elf_Ehdr    *file_header;
    Elf_Shdr    *sections_headers;
    Elf_Shdr    *symbol_table_header;
    char        *symbols_string_table;
    char        *sections_string_table;
    Elf_Sym     *simbols;
    int         simbols_count;

    t_sym_info  *sym_info_arr;

    file_header = (Elf_Ehdr *) g_elf_file.content;
    sections_headers = x_(get_sections_headers)(file_header);
    if (sections_headers == NULL)
        return 1;
    symbol_table_header = x_(get_symbol_table_header)(sections_headers, file_header->e_shnum);
    if (symbol_table_header == NULL)
        return 0;
    symbols_string_table = x_(get_string_table)(sections_headers, resolve_endianess(symbol_table_header->sh_link));
    if (string_table == NULL)
        return 1;
    simbols = (Elf_Sym *) (g_elf_file.content + resolve_endianess(symbol_table_header->sh_offset));
    simbols_count = resolve_endianess(symbol_table_header->sh_size) / sizeof(Elf_Sym);

    sym_info_arr = x_(get_symbols_info)(sym_arr, &n_sym, strtab, sh_arr);
    if (!sym_info_arr)
        return 1;
    if (!g_flags.no_sort)
        sort_symbols(sym_info_arr, n_sym);
    display_symbols(sym_info_arr, n_sym);
    free_symbols(sym_info_arr, n_sym);
    return 0;
}