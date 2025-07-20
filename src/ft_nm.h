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
#   define x_(func)     x32_##func
    typedef Elf32_Ehdr  Elf_Ehdr;
    typedef Elf32_Shdr  Elf_Shdr;
    typedef Elf32_Sym   Elf_Sym ;
    typedef Elf32_Shdr  Elf_Shdr;
    typedef uint32_t    Elf_Off;
    typedef uint32_t    Elf_Addr;
#   define ELF_ST_BIND  ELF32_ST_BIND
#   define ELF_ST_TYPE  ELF32_ST_TYPE
#   define Elf_Addr_len 8
# else
#   define x_(func)     x64_##func
    typedef Elf64_Ehdr  Elf_Ehdr;
    typedef Elf64_Shdr  Elf_Shdr;
    typedef Elf64_Sym   Elf_Sym ;
    typedef Elf64_Shdr  Elf_Shdr;
    typedef uint64_t    Elf_Off;
    typedef uint64_t    Elf_Addr;
#   define ELF_ST_BIND  ELF64_ST_BIND
#   define ELF_ST_TYPE  ELF64_ST_TYPE
#   define Elf_Addr_len 16
# endif

typedef uint16_t    Elf_Half;

typedef struct {
    char    *path;
    char    *content;
    size_t  size;
    int     endian_match;
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

typedef enum {
    MSG_ERROR,
    MSG_WARNING
} t_msg_type;

typedef enum {
    NO_QUOTES,
    SINGLE_QUOTES
} t_quote_style;

// process_elf.c
int x32_process_elf();
int x64_process_elf();

// section_parser.c - Section header operations
Elf_Shdr  *x_(get_section_headers)(Elf_Ehdr *header);
Elf_Shdr  *x_(get_symtab_section)(Elf_Shdr *sections, size_t num_sections);
Elf_Shdr  *x_(get_strtab_section)(Elf_Shdr *sh_arr, int idx_sym_name);

// symbol_parser.c - Symbol table operations
char         x_(get_type)(Elf_Sym symbol, Elf_Shdr *sh_arr);
char        *x_(get_name)(Elf_Sym symbol, char *names);
char        *x_(get_value)(Elf_Sym symbol);
t_sym_info  *x_(get_symbols_info)(Elf_Sym *sym_arr, size_t *n_sym, Elf_Shdr *strtab, Elf_Shdr *sh_arr);

// symbol_utils.c
void    sort_symbols(t_sym_info *sym_info_arr, size_t n_sym);
void    display_symbols(t_sym_info *sym_info_arr, size_t n_sym);
void    free_symbols(t_sym_info *sym_info_arr, size_t n_sym);

// endian.c - Endian swapping operation
int         define_endianess(int file_endianess);
uint16_t    bswap_16(uint16_t val);
uint32_t    bswap_32(uint32_t val);
uint64_t    bswap_64(uint64_t val);

# define bswap(X) _Generic((X), \
    uint16_t: bswap_16(X), \
    uint32_t: bswap_32(X), \
    uint64_t: bswap_64(X) \
)
# define resolve_endianess(X) (!g_elf_file.endian_match ? bswap(X) : (X))

// utils.c
size_t  ft_strlen(char *s);
int     ft_strcmp(char *s1, char *s2);
char    *ft_strchr(char *str, int search_str);

// print.c
void    print_str_fd(int fd, char *str);
void    print_usage();
void    print_matrix(char **matrix);
int     print_error(char *err_msg, t_msg_type type, t_quote_style quotes);

#endif /* FT_NM_H */