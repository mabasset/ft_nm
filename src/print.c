#include "ft_nm.h"

void    print_usage() {
    ft_dprintf(STDERR_FILENO, "Usage: nm [option(s)] [file(s)]\n");
    ft_dprintf(STDERR_FILENO," List symbols in [file(s)] (a.out by default).\n");
    ft_dprintf(STDERR_FILENO," The options are:\n");
    ft_dprintf(STDERR_FILENO,"  -a, --debug-syms       Display debugger-only symbols\n");
    ft_dprintf(STDERR_FILENO,"  -A, --print-file-name  Print name of the input file before every symbol\n");
    ft_dprintf(STDERR_FILENO,"  -B                     Same as --format=bsd\n");
    ft_dprintf(STDERR_FILENO,"  -C, --demangle[=STYLE] Decode mangled/processed symbol names\n");
    ft_dprintf(STDERR_FILENO,"                           STYLE can be \"none\", \"auto\", \"gnu-v3\", \"java\",\n");
    ft_dprintf(STDERR_FILENO,"                           \"gnat\", \"dlang\", \"rust\"\n");
    ft_dprintf(STDERR_FILENO,"      --no-demangle      Do not demangle low-level symbol names\n");
    ft_dprintf(STDERR_FILENO,"      --recurse-limit    Enable a demangling recursion limit.  (default)\n");
    ft_dprintf(STDERR_FILENO,"      --no-recurse-limit Disable a demangling recursion limit.\n");
    ft_dprintf(STDERR_FILENO,"  -D, --dynamic          Display dynamic symbols instead of normal symbols\n");
    ft_dprintf(STDERR_FILENO,"  -e                     (ignored)\n");
    ft_dprintf(STDERR_FILENO,"  -f, --format=FORMAT    Use the output format FORMAT.  FORMAT can be `bsd',\n");
    ft_dprintf(STDERR_FILENO,"                           `sysv', `posix' or 'just-symbols'.\n");
    ft_dprintf(STDERR_FILENO,"                           The default is `bsd'\n");
    ft_dprintf(STDERR_FILENO,"  -g, --extern-only      Display only external symbols\n");
    ft_dprintf(STDERR_FILENO,"    --ifunc-chars=CHARS  Characters to use when displaying ifunc symbols\n");
    ft_dprintf(STDERR_FILENO,"  -j, --just-symbols     Same as --format=just-symbols\n");
    ft_dprintf(STDERR_FILENO,"  -l, --line-numbers     Use debugging information to find a filename and\n");
    ft_dprintf(STDERR_FILENO,"                           line number for each symbol\n");
    ft_dprintf(STDERR_FILENO,"  -n, --numeric-sort     Sort symbols numerically by address\n");
    ft_dprintf(STDERR_FILENO,"  -o                     Same as -A\n");
    ft_dprintf(STDERR_FILENO,"  -p, --no-sort          Do not sort the symbols\n");
    ft_dprintf(STDERR_FILENO,"  -P, --portability      Same as --format=posix\n");
    ft_dprintf(STDERR_FILENO,"  -r, --reverse-sort     Reverse the sense of the sort\n");
    ft_dprintf(STDERR_FILENO,"      --plugin NAME      Load the specified plugin\n");
    ft_dprintf(STDERR_FILENO,"  -S, --print-size       Print size of defined symbols\n");
    ft_dprintf(STDERR_FILENO,"  -s, --print-armap      Include index for symbols from archive members\n");
    ft_dprintf(STDERR_FILENO,"      --quiet            Suppress \"no symbols\" diagnostic\n");
    ft_dprintf(STDERR_FILENO,"      --size-sort        Sort symbols by size\n");
    ft_dprintf(STDERR_FILENO,"      --special-syms     Include special symbols in the output\n");
    ft_dprintf(STDERR_FILENO,"      --synthetic        Display synthetic symbols as well\n");
    ft_dprintf(STDERR_FILENO,"  -t, --radix=RADIX      Use RADIX for printing symbol values\n");
    ft_dprintf(STDERR_FILENO,"      --target=BFDNAME   Specify the target object format as BFDNAME\n");
    ft_dprintf(STDERR_FILENO,"  -u, --undefined-only   Display only undefined symbols\n");
    ft_dprintf(STDERR_FILENO,"  -U, --defined-only     Display only defined symbols\n");
    ft_dprintf(STDERR_FILENO,"      --unicode={default|show|invalid|hex|escape|highlight}\n");
    ft_dprintf(STDERR_FILENO,"                         Specify how to treat UTF-8 encoded unicode characters\n");
    ft_dprintf(STDERR_FILENO,"  -W, --no-weak          Ignore weak symbols\n");
    ft_dprintf(STDERR_FILENO,"      --with-symbol-versions  Display version strings after symbol names\n");
    ft_dprintf(STDERR_FILENO,"  -X 32_64               (ignored)\n");
    ft_dprintf(STDERR_FILENO,"  @FILE                  Read options from FILE\n");
    ft_dprintf(STDERR_FILENO,"  -h, --help             Display this information\n");
    ft_dprintf(STDERR_FILENO,"  -V, --version          Display this program's version number\n");
    ft_dprintf(STDERR_FILENO,"nm: supported targets: elf64-x86-64 elf32-i386 elf32-iamcu elf32-x86-64 pei-i386 pe-x86-64 pei-x86-64 elf64-little elf64-big elf32-little elf32-big pe-bigobj-x86-64 pe-i386 pdb srec symbolsrec verilog tekhex binary ihex plugin\n");
}

void    print_matrix(char **matrix) {
    ft_printf("[");
    while (*matrix != NULL) {
        ft_printf("%s", *matrix);
        matrix++;
        if (*matrix != NULL)
            ft_printf(", ");
    }
    ft_printf("]\n");
}
