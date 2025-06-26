#ifndef FT_NM_H
# define FT_NM_H

# include <elf.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <fcntl.h>
# include <stdio.h>
# include <errno.h>
# include <string.h>
# include <sys/mman.h>
# include <unistd.h>
# include <stdlib.h>

typedef struct {
    char    *path;
    char    *content;
    size_t  size;
    int     class;
    int     endian;
} t_elf_info;

typedef struct {
    char   *value;
    char   type;
    char   *name;
} t_sym_info;

int print_error();

// process.c
int process_elf64();

// sections_parser.c - Section header operations
Elf64_Shdr  *get_section_headers(Elf64_Ehdr *header);
Elf64_Shdr  *get_symtab_section(Elf64_Shdr *sections, size_t num_sections);
Elf64_Shdr  *get_strtab_section(Elf64_Shdr *sh_arr, int idx_sym_name);

// symbols_parser.c - Symbol table operations
char        *get_value(Elf64_Sym symbol);
char        get_type(Elf64_Sym symbol, Elf64_Shdr *sh_arr, char *names);
char        *get_name(Elf64_Sym symbol, char *names);
t_sym_info  *get_symbols_info(Elf64_Sym *sym_arr, size_t *n_sym, Elf64_Shdr *strtab, Elf64_Shdr *sh_arr);
void        sort_symbols_info(t_sym_info *sym_info_arr, size_t n_sym);
void        display_symbols_info(t_sym_info *sym_info_arr, size_t n_sym);

#endif /* FT_NM_H */