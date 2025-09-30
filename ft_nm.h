#ifndef     FT_NM_H
# define    FT_NM_H

# include "libmb/libmb.h"

# include <elf.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/mman.h>

# define USAGE_FILEPATH "usage.txt"

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

typedef struct stat t_stat;

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
    char    *content;
    size_t  size;
} t_string;

typedef struct {
    Elf_Shdr    *shdrs;
    Elf_Word    shcount;
    t_string    shstrtab;
    Elf_Sym     *symtabs;
    Elf_Word    symcount;
    t_string    symstrtab;
} t_elf_info;

typedef struct {
    Elf_Word        st_name;
    uint16_t        st_shndx;
    unsigned char   st_type;
    unsigned char   st_bind;
    char            *sh_name;
    char            *value;
    char            *name;
    char            type;
} t_sym;

// elf_info.c
int     x32_set_elf_info(t_elf_info *elf_info, Elf_Ehdr *file_header, t_string file);
int     x64_set_elf_info(t_elf_info *elf_info, Elf_Ehdr *file_header, t_string file);
t_list *x32_get_symbols(t_elf_info elf_info);
t_list *x64_get_symbols(t_elf_info elf_info);


// symbols.c
t_list  *x_(get_symbols)(t_elf_info elf_info);

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
void        close_fd(int *fd);
void        unmap_file(t_string *mapped_file);
int         symcmp(void *sym1, void *sym2);
t_string    extract_content(char *file_path);

// print.c
void    print_usage();
int     print_format_error(char *file_path);
void    print_symbols(t_list *syms, char *file_path, int ei_class);

#endif /* FT_NM_H */