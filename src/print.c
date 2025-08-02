#include "ft_nm.h"

void    print_usage() {
    ft_printf("Usage: nm [option(s)] [file(s)]\n");
    ft_printf(" List symbols in [file(s)] (a.out by default).\n");
    ft_printf(" The options are:\n");
    ft_printf("  -a, --debug-syms       Display debugger-only symbols\n");
    ft_printf("  -A, --print-file-name  Print name of the input file before every symbol\n");
    ft_printf("  -B                     Same as --format=bsd\n");
    ft_printf("  -C, --demangle[=STYLE] Decode mangled/processed symbol names\n");
    ft_printf("                           STYLE can be \"none\", \"auto\", \"gnu-v3\", \"java\",\n");
    ft_printf("                           \"gnat\", \"dlang\", \"rust\"\n");
    ft_printf("      --no-demangle      Do not demangle low-level symbol names\n");
    ft_printf("      --recurse-limit    Enable a demangling recursion limit.  (default)\n");
    ft_printf("      --no-recurse-limit Disable a demangling recursion limit.\n");
    ft_printf("  -D, --dynamic          Display dynamic symbols instead of normal symbols\n");
    ft_printf("  -e                     (ignored)\n");
    ft_printf("  -f, --format=FORMAT    Use the output format FORMAT.  FORMAT can be `bsd',\n");
    ft_printf("                           `sysv', `posix' or 'just-symbols'.\n");
    ft_printf("                           The default is `bsd'\n");
    ft_printf("  -g, --extern-only      Display only external symbols\n");
    ft_printf("    --ifunc-chars=CHARS  Characters to use when displaying ifunc symbols\n");
    ft_printf("  -j, --just-symbols     Same as --format=just-symbols\n");
    ft_printf("  -l, --line-numbers     Use debugging information to find a filename and\n");
    ft_printf("                           line number for each symbol\n");
    ft_printf("  -n, --numeric-sort     Sort symbols numerically by address\n");
    ft_printf("  -o                     Same as -A\n");
    ft_printf("  -p, --no-sort          Do not sort the symbols\n");
    ft_printf("  -P, --portability      Same as --format=posix\n");
    ft_printf("  -r, --reverse-sort     Reverse the sense of the sort\n");
    ft_printf("      --plugin NAME      Load the specified plugin\n");
    ft_printf("  -S, --print-size       Print size of defined symbols\n");
    ft_printf("  -s, --print-armap      Include index for symbols from archive members\n");
    ft_printf("      --quiet            Suppress \"no symbols\" diagnostic\n");
    ft_printf("      --size-sort        Sort symbols by size\n");
    ft_printf("      --special-syms     Include special symbols in the output\n");
    ft_printf("      --synthetic        Display synthetic symbols as well\n");
    ft_printf("  -t, --radix=RADIX      Use RADIX for printing symbol values\n");
    ft_printf("      --target=BFDNAME   Specify the target object format as BFDNAME\n");
    ft_printf("  -u, --undefined-only   Display only undefined symbols\n");
    ft_printf("  -U, --defined-only     Display only defined symbols\n");
    ft_printf("      --unicode={default|show|invalid|hex|escape|highlight}\n");
    ft_printf("                         Specify how to treat UTF-8 encoded unicode characters\n");
    ft_printf("  -W, --no-weak          Ignore weak symbols\n");
    ft_printf("      --with-symbol-versions  Display version strings after symbol names\n");
    ft_printf("  -X 32_64               (ignored)\n");
    ft_printf("  @FILE                  Read options from FILE\n");
    ft_printf("  -h, --help             Display this information\n");
    ft_printf("  -V, --version          Display this program's version number\n");
    ft_printf("nm: supported targets: elf64-x86-64 elf32-i386 elf32-iamcu elf32-x86-64 pei-i386 pe-x86-64 pei-x86-64 elf64-little elf64-big elf32-little elf32-big pe-bigobj-x86-64 pe-i386 pdb srec symbolsrec verilog tekhex binary ihex plugin\n");
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

int print_error(char *file_path, char *err_msg, t_msg_type type, t_quote_style quotes) {
    const char  *prefix;
    const char  *quote_char;

    prefix = (type == MSG_WARNING) ? "nm: Warning: " : "nm: ";
    quote_char = (quotes == SINGLE_QUOTES) ? "'" : "";
    if (file_path != NULL)
        ft_printf("%s%s%s%s: %s\n", prefix, quote_char, file_path, quote_char, err_msg);
    else
        ft_printf("%s%s\n", prefix, err_msg);

    return (1);
}
