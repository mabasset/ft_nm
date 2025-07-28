#ifndef     FT_NM_H
# define    FT_NM_H

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
    typedef Elf32_Sym   Elf_Sym;
    typedef Elf32_Shdr  Elf_Shdr;
    typedef Elf32_Half  Elf_Half;
    typedef Elf32_Word  Elf_Word;
    typedef Elf32_Xword Elf_Xword;
    typedef Elf32_Off   Elf_Off;
    typedef uint32_t    uint_t;
#   define ELF_ST_BIND  ELF32_ST_BIND
#   define ELF_ST_TYPE  ELF32_ST_TYPE
#   define Elf_Addr_len 8
# else
#   define x_(func)     x64_##func
    typedef Elf64_Ehdr  Elf_Ehdr;
    typedef Elf64_Shdr  Elf_Shdr;
    typedef Elf64_Sym   Elf_Sym;
    typedef Elf64_Shdr  Elf_Shdr;
    typedef Elf64_Half  Elf_Half;
    typedef Elf64_Word  Elf_Word;
    typedef Elf64_Xword Elf_Xword;
    typedef Elf64_Off   Elf_Off;
    typedef uint64_t    uint_t;
#   define ELF_ST_BIND  ELF64_ST_BIND
#   define ELF_ST_TYPE  ELF64_ST_TYPE
#   define Elf_Addr_len 16
# endif

typedef struct {
    int     all;
    int     external;
    int     undefined;
    int     reverse;
    int     no_sort;
    int     path;
    int     endian_match;
} t_flags;

typedef struct {
    uint16_t        shndx;
    unsigned char   type;
    unsigned char   bind;
    char            type;
    char            *name;
    char            *value;
} t_sym_info;

typedef enum {
    MSG_ERROR,
    MSG_WARNING
} t_msg_type;

typedef enum {
    NO_QUOTES,
    SINGLE_QUOTES
} t_quote_style;

typedef struct {
    char    *content;
    size_t  size;
} t_string;

typedef struct {
    Elf_Shdr    *headers;
    Elf_Word    count;
    t_string    strtab;
} t_sections;

typedef struct {
    Elf_Sym     *table;
    Elf_Word    count;
    t_string    strtab;
} t_symbols;

// process_elf.c
t_sym_info  **x32_get_symbols_info(t_string mapped_file);
t_sym_info  **x64_get_symbols_info(t_string mapped_file);

// symbol_parser.c - Symbol table operations
// char        *x_(get_value)(Elf_Sym symbol);
// char         x_(get_type)(Elf_Sym symbol, t_sections sections);
// char        *x_(get_name)(Elf_Sym symbol, t_string strtab);
t_sym_info  **x_(init_symbols_infos)(t_symbols symbols, t_sections sections);

// symbol_utils.c
void    sort_symbols(t_sym_info **symbols_info);
void    display_symbols(t_sym_info **symbols_info);

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
# define resolve_endianess(X) (!g_flags.endian_match ? bswap(X) : (X))

// utils.c
size_t  ft_strlen(char *s);
int     ft_strcmp(char *s1, char *s2);
char    *ft_strchr(char *str, int search_str);
void    **ft_calloc(size_t size);
void    close_fd(int *fd);
void    unmap_file(t_string *mapped_file);
void    free_matrix(t_sym_info ***symbols_infos);

// print.c
void    print_usage();
void    print_str_fd(int fd, char *str);
void    print_matrix(char **matrix);
int     print_error(char *file_path, char *err_msg, t_msg_type type, t_quote_style quotes);
void    print_no_symbols(char *file_path);

#endif /* FT_NM_H */