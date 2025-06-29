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

# ifdef X32
#  define Elf_Ehdr      Elf32_Ehdr
#  define Elf_Shdr      Elf32_Shdr
#  define Elf_Sym       Elf32_Sym
#  define ELF_ST_BIND   ELF32_ST_BIND
#  define ELF_ST_TYPE   ELF32_ST_TYPE
#  define Elf_Word      uint64_t
#  define x_(func)      x32_##func
# else
#  define Elf_Ehdr      Elf64_Ehdr
#  define Elf_Shdr      Elf64_Shdr
#  define Elf_Sym       Elf64_Sym
#  define ELF_ST_BIND   ELF64_ST_BIND
#  define ELF_ST_TYPE   ELF64_ST_TYPE
#  define Elf_Word      uint32_t
#  define x_(func)      x64_##func
# endif

typedef struct {
    char    *path;
    char    *content;
    size_t  size;
    int     endian;
} t_elf_file;

typedef struct {
    int     all;
    int     external;
    int     undefined;
    int     reverse;
    int     no_sort;
} t_flags;


typedef struct {
    char   *value;
    char   type;
    char   *name;
} t_sym_info;

// process.c
int x32_process_elf();
int x64_process_elf();

// sections_parser.c - Section header operations
Elf_Shdr  *x_(get_section_headers)(Elf_Ehdr *header);
Elf_Shdr  *x_(get_symtab_section)(Elf_Shdr *sections, size_t num_sections);
Elf_Shdr  *x_(get_strtab_section)(Elf_Shdr *sh_arr, int idx_sym_name);

// symbols_parser.c - Symbol table operations
char        *x_(get_value)(Elf_Sym symbol);
char        x_(get_type)(Elf_Sym symbol, Elf_Shdr *sh_arr);
char        *x_(get_name)(Elf_Sym symbol, char *names);
t_sym_info  *x_(get_symbols_info)(Elf_Sym *sym_arr, size_t *n_sym, Elf_Shdr *strtab, Elf_Shdr *sh_arr);

// utils.c
size_t  ft_strlen(char *s);
int     ft_strcmp(char *s1, char *s2);
int     print_error(char *error_msg);
void    sort_symbols(t_sym_info *sym_info_arr, size_t n_sym);
void    display_symbols(t_sym_info *sym_info_arr, size_t n_sym);
void    print_matrix(char **matrix);
void    print_usage();
void    print_str_fd(int fd, char *str);

#endif /* FT_NM_H */