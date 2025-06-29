#include "ft_nm.h"

extern t_elf_file g_elf_file;

void    print_str_fd(int fd, char *str) {
    size_t  size;

    size = ft_strlen(str);
    write(fd, str, size);
    write(fd, "\n", 1);
}

void    print_usage() {
    print_str_fd(2, "Usage: nm [option(s)] [file(s)]");
    print_str_fd(2, " List symbols in [file(s)] (a.out by default).");
    print_str_fd(2, " The options are:");
    print_str_fd(2, "  -a, --debug-syms       Display debugger-only symbols");
    print_str_fd(2, "  -A, --print-file-name  Print name of the input file before every symbol");
    print_str_fd(2, "  -B                     Same as --format=bsd");
    print_str_fd(2, "  -C, --demangle[=STYLE] Decode mangled/processed symbol names");
    print_str_fd(2, "                           STYLE can be \"none\", \"auto\", \"gnu-v3\", \"java\",");
    print_str_fd(2, "                           \"gnat\", \"dlang\", \"rust\"");
    print_str_fd(2, "      --no-demangle      Do not demangle low-level symbol names");
    print_str_fd(2, "      --recurse-limit    Enable a demangling recursion limit.  (default)");
    print_str_fd(2, "      --no-recurse-limit Disable a demangling recursion limit.");
    print_str_fd(2, "  -D, --dynamic          Display dynamic symbols instead of normal symbols");
    print_str_fd(2, "  -e                     (ignored)");
    print_str_fd(2, "  -f, --format=FORMAT    Use the output format FORMAT.  FORMAT can be `bsd',");
    print_str_fd(2, "                           `sysv', `posix' or 'just-symbols'.");
    print_str_fd(2, "                           The default is `bsd'");
    print_str_fd(2, "  -g, --extern-only      Display only external symbols");
    print_str_fd(2, "    --ifunc-chars=CHARS  Characters to use when displaying ifunc symbols");
    print_str_fd(2, "  -j, --just-symbols     Same as --format=just-symbols");
    print_str_fd(2, "  -l, --line-numbers     Use debugging information to find a filename and");
    print_str_fd(2, "                           line number for each symbol");
    print_str_fd(2, "  -n, --numeric-sort     Sort symbols numerically by address");
    print_str_fd(2, "  -o                     Same as -A");
    print_str_fd(2, "  -p, --no-sort          Do not sort the symbols");
    print_str_fd(2, "  -P, --portability      Same as --format=posix");
    print_str_fd(2, "  -r, --reverse-sort     Reverse the sense of the sort");
    print_str_fd(2, "      --plugin NAME      Load the specified plugin");
    print_str_fd(2, "  -S, --print-size       Print size of defined symbols");
    print_str_fd(2, "  -s, --print-armap      Include index for symbols from archive members");
    print_str_fd(2, "      --quiet            Suppress \"no symbols\" diagnostic");
    print_str_fd(2, "      --size-sort        Sort symbols by size");
    print_str_fd(2, "      --special-syms     Include special symbols in the output");
    print_str_fd(2, "      --synthetic        Display synthetic symbols as well");
    print_str_fd(2, "  -t, --radix=RADIX      Use RADIX for printing symbol values");
    print_str_fd(2, "      --target=BFDNAME   Specify the target object format as BFDNAME");
    print_str_fd(2, "  -u, --undefined-only   Display only undefined symbols");
    print_str_fd(2, "  -U, --defined-only     Display only defined symbols");
    print_str_fd(2, "      --unicode={default|show|invalid|hex|escape|highlight}");
    print_str_fd(2, "                         Specify how to treat UTF-8 encoded unicode characters");
    print_str_fd(2, "  -W, --no-weak          Ignore weak symbols");
    print_str_fd(2, "      --with-symbol-versions  Display version strings after symbol names");
    print_str_fd(2, "  -X 32_64               (ignored)");
    print_str_fd(2, "  @FILE                  Read options from FILE");
    print_str_fd(2, "  -h, --help             Display this information");
    print_str_fd(2, "  -V, --version          Display this program's version number");
    print_str_fd(2, "nm: supported targets: elf64-x86-64 elf32-i386 elf32-iamcu elf32-x86-64 pei-i386 pe-x86-64 pei-x86-64 elf64-little elf64-big elf32-little elf32-big pe-bigobj-x86-64 pe-i386 pdb srec symbolsrec verilog tekhex binary ihex plugin");
}

void    print_matrix_fd(int fd, char **matrix) {
    size_t  size;

    write(fd, "[", 1);
    while (*matrix != NULL) {
        size = ft_strlen(*matrix);
        write(fd, *matrix, size);
        matrix++;
        if (*matrix != NULL)
            write(fd, ", ", 2);
    }
    write(fd, "]\n", 2);
}

void    print_matrix(char **matrix) {
    print_matrix_fd(1, matrix);
}

int print_error(char *error) {
    write(2, "nm: ", 4);
    if (g_elf_file.path != NULL) {
        write(2, "\'", 1);
        write(2, g_elf_file.path, ft_strlen(g_elf_file.path));
        write(2, "\': ", 3);
    }
    print_str_fd(2, error);
    return (1);
}