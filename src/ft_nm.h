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
# include <stdint.h>

# ifdef X32
#  define x_(func)      x32_##func
#  define Elf_Ehdr      Elf32_Ehdr
#  define Elf_Shdr      Elf32_Shdr
#  define Elf_Sym       Elf32_Sym
#  define ELF_ST_BIND   ELF32_ST_BIND
#  define ELF_ST_TYPE   ELF32_ST_TYPE
#  define Elf_Off       uint32_t
#  define Elf_Addr_len  8
# else
#  define x_(func)      x64_##func
#  define Elf_Ehdr      Elf64_Ehdr
#  define Elf_Shdr      Elf64_Shdr
#  define Elf_Sym       Elf64_Sym
#  define ELF_ST_BIND   ELF64_ST_BIND
#  define ELF_ST_TYPE   ELF64_ST_TYPE
#  define Elf_Off       uint64_t
#  define Elf_Addr_len  16
# endif

# define Elf_Half   uint16_t

# define bswap(X) _Generic((X), \
    uint16_t: bswap_16(X), \
    uint32_t: bswap_32(X), \
    uint64_t: bswap_64(X) \
)
# define swap_endian_if_needed(X) (g_elf_file.endian == ELFDATA2MSB ? bswap(X) : (X))

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
    int     path;
} t_flags;

typedef struct {
    char   type;
    char   *name;
    char   *value;
} t_sym_info;

// process.c
int         x32_process_elf();
int         x64_process_elf();

// sections_parser.c - Section header operations
Elf_Shdr  *x_(get_section_headers)(Elf_Ehdr *header);
Elf_Shdr  *x_(get_symtab_section)(Elf_Shdr *sections, size_t num_sections);
Elf_Shdr  *x_(get_strtab_section)(Elf_Shdr *sh_arr, int idx_sym_name);

// symbols_parser.c - Symbol table operations
char        x_(get_type)(Elf_Sym symbol, Elf_Shdr *sh_arr);
char        *x_(get_name)(Elf_Sym symbol, char *names);
char        *x_(get_value)(Elf_Sym symbol);
t_sym_info  *x_(get_symbols_info)(Elf_Sym *sym_arr, size_t *n_sym, Elf_Shdr *strtab, Elf_Shdr *sh_arr);

// utils.c
void        print_str_fd(int fd, char *str);
void        print_usage();
void        print_matrix(char **matrix);
int         print_error(char *error_msg);
uint16_t    bswap_16(uint16_t val);
uint32_t    bswap_32(uint32_t val);
uint64_t    bswap_64(uint64_t val);

// print.c
size_t  ft_strlen(char *s);
int     ft_strcmp(char *s1, char *s2);
void    sort_symbols(t_sym_info *sym_info_arr, size_t n_sym);
void    display_symbols(t_sym_info *sym_info_arr, size_t n_sym);
void    free_symbols(t_sym_info *sym_info_arr, size_t n_sym);

#endif /* FT_NM_H */