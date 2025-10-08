#include "ft_nm.h"

extern t_flags  g_flags;

int print_usage() {

    char *usage =
        "Usage: nm [option(s)] [file(s)]\n"
        " List symbols in [file(s)] (a.out by default).\n"
        " The options are:\n"
        "  -a, --debug-syms       Display debugger-only symbols\n"
        "  -A, --print-file-name  Print name of the input file before every symbol\n"
        "  -B                     Same as --format=bsd\n"
        "  -C, --demangle[=STYLE] Decode mangled/processed symbol names\n"
        "                           STYLE can be \"none\", \"auto\", \"gnu-v3\", \"java\",\n"
        "                           \"gnat\", \"dlang\", \"rust\"\n"
        "      --no-demangle      Do not demangle low-level symbol names\n"
        "      --recurse-limit    Enable a demangling recursion limit.  (default)\n"
        "      --no-recurse-limit Disable a demangling recursion limit.\n"
        "  -D, --dynamic          Display dynamic symbols instead of normal symbols\n"
        "  -e                     (ignored)\n"
        "  -f, --format=FORMAT    Use the output format FORMAT.  FORMAT can be `bsd',\n"
        "                           `sysv', `posix' or 'just-symbols'.\n"
        "                           The default is `bsd'\n"
        "  -g, --extern-only      Display only external symbols\n"
        "    --ifunc-chars=CHARS  Characters to use when displaying ifunc symbols\n"
        "  -j, --just-symbols     Same as --format=just-symbols\n"
        "  -l, --line-numbers     Use debugging information to find a filename and\n"
        "                           line number for each symbol\n"
        "  -n, --numeric-sort     Sort symbols numerically by address\n"
        "  -o                     Same as -A\n"
        "  -p, --no-sort          Do not sort the symbols\n"
        "  -P, --portability      Same as --format=posix\n"
        "  -r, --reverse-sort     Reverse the sense of the sort\n"
        "      --plugin NAME      Load the specified plugin\n"
        "  -S, --print-size       Print size of defined symbols\n"
        "  -s, --print-armap      Include index for symbols from archive members\n"
        "      --quiet            Suppress \"no symbols\" diagnostic\n"
        "      --size-sort        Sort symbols by size\n"
        "      --special-syms     Include special symbols in the output\n"
        "      --synthetic        Display synthetic symbols as well\n"
        "  -t, --radix=RADIX      Use RADIX for printing symbol values\n"
        "      --target=BFDNAME   Specify the target object format as BFDNAME\n"
        "  -u, --undefined-only   Display only undefined symbols\n"
        "  -U, --defined-only     Display only defined symbols\n"
        "      --unicode={default|show|invalid|hex|escape|highlight}\n"
        "                         Specify how to treat UTF-8 encoded unicode characters\n"
        "  -W, --no-weak          Ignore weak symbols\n"
        "      --with-symbol-versions  Display version strings after symbol names\n"
        "  -X 32_64               (ignored)\n"
        "  @FILE                  Read options from FILE\n"
        "  -h, --help             Display this information\n"
        "  -V, --version          Display this program's version number\n"
        "nm: supported targets: elf64-x86-64 elf32-i386 elf32-iamcu elf32-x86-64 pei-i386 pe-x86-64 pei-x86-64 elf64-little elf64-big elf32-little elf32-big pe-bigobj-x86-64 pe-i386 pdb srec symbolsrec verilog tekhex binary ihex plugin\n";

    ft_putstr_err(usage);

    return 1;
}

int print_format_error(char *file_path) {
    ft_putstr_err("nm: ");
    ft_putstr_err(file_path);
    ft_putstr_err(": file format not recognized\n");
    return 1;
}

int print_no_symbols(char *file_path) {
    ft_putstr_err("nm: ");
    ft_putstr_err(file_path);
    ft_putstr_err(": no symbols\n");
    return 0;
}

int print_invalid_option(char option) {
    ft_putstr_err("nm: invalid option -- '");
    ft_putchar_err(option);
    ft_putstr_err("'\n");
    return print_usage();
}

int print_unrecognized_option(char *option) {
    ft_putstr_err("nm: unrecognized option '");
    ft_putstr_err(option);
    ft_putstr_err("'\n");
    return print_usage();
}

static void print_sym_value(char *value, int ei_class) {
    char    filler;
    int     padding;

    filler = ' ';
    padding = (ei_class == ELFCLASS32) ? 8 : 16;
    if (value != NULL) {
        padding -= ft_strlen(value);
        filler = 0;
    }
    for (int i = 0; i < padding; i++)
        ft_putchar(filler);
    if (value != NULL)
        ft_putstr(value);
}

static void print_sym_type(char type) {
    ft_putchar(' ');
    ft_putchar(type);
    ft_putchar(' ');
}

void    print_symbols(t_list *syms, char *file_path, int ei_class) {
    static char *undefined_types = "Uwv";
    static char *external_types = "ABCDGRSTUVvWwi";
    t_sym       *sym;

    if (g_flags.path) {
        ft_putchar('\n');
        ft_putstr(file_path);
        ft_putstr(":\n");
    }
    for (;syms != NULL; syms = syms->next) {
        sym = (t_sym*)syms->data;
        if (g_flags.undefined && !ft_strchr(undefined_types, sym->type))
            continue;
        if (g_flags.external && !ft_strchr(external_types, sym->type))
            continue;
        if (!g_flags.all && (sym->st_type == STT_SECTION || sym->st_type == STT_FILE))
            continue;
        if (sym->type == '?')
            continue;
        
        print_sym_value(sym->value, ei_class);
        print_sym_type(sym->type);
        ft_putstr_nl(sym->name);
    }
}