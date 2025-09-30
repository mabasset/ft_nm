#include "ft_nm.h"

t_flags g_flags = { 0 };

int check_content(t_string mapped_file) {
    char    *content = mapped_file.content;
    size_t  size = mapped_file.size;

    if (size < sizeof(Elf32_Ehdr) ||
        content[0] != 0x7f || content[1] != 'E' ||
        content[2] != 'L' || content[3] != 'F') {
        return 1;
    }
    if(content[EI_CLASS] != ELFCLASS32 && content[EI_CLASS] != ELFCLASS64)
        return 1;
    if (content[EI_CLASS] == ELFCLASS64 && size < sizeof(Elf64_Ehdr))
        return 1;
    if (content[EI_DATA] != ELFDATA2LSB && content[EI_DATA] != ELFDATA2MSB)
        return 1;
    if (content[EI_VERSION] != EV_CURRENT)
        return 1;
    return 0;
}

static int ft_nm(char *file_path) {
    t_string    file __attribute__ ((cleanup(unmap_file)));
    Elf_Ehdr    *header;
    t_elf_info  elf_info;
    int         elf_error;
    t_list      *symbols;

    file = extract_content(file_path);
    if (file.content == NULL)
        return 1;
    if (check_content(file))
        return print_format_error(file_path);
    g_flags.endian_match = define_endianess(file.content[EI_DATA]);
    header = (Elf_Ehdr*) file.content;

    elf_error = (file.content[EI_CLASS] == ELFCLASS32) ?
        x32_set_elf_info(&elf_info, header, file) : x64_set_elf_info(&elf_info, header, file);
    if (elf_error)
        return print_format_error(file_path);
    symbols = (file.content[EI_CLASS] == ELFCLASS32) ? 
       x32_get_symbols(elf_info) : x64_get_symbols(elf_info);
    if (symbols == NULL) {
        ft_putstr_err("nm: ");
        ft_putstr_err(file_path);
        ft_putstr_err(": no symbols\n");
        return 0;
    }
    if (!g_flags.no_sort)
        ft_list_sort(&symbols, symcmp);
    print_symbols(symbols, file_path, file.content[EI_CLASS]);
    ft_list_clear(symbols, free);
    return 0;
}

static char *handle_long_flag(char *flag) {
    char    *long_flags[] = {"--debug-syms", "--extern-only", "--undefined-only", "--reverse-sort", "--no-sort"};
    char    *short_flags[] = {"-a", "-g", "-u", "-r", "-p"};
    int     length;
    int     size;
    int     i;

    length = ft_strlen(flag);
    size = sizeof(long_flags) / sizeof(char *);
    for (i = 0; i < size; i++)
        if (ft_strncmp(flag, long_flags[i], length) == 0)
            break ;
    size = sizeof(short_flags) / sizeof(char *);
    if (i < size)
        return short_flags[i];
    ft_putstr_err("nm: unrecognized option -- '");
    ft_putstr_err(flag);
    ft_putstr_err("'\n");
    print_usage();

    return NULL;
}

static int  set_flags(char *flags) {
    char    *short_flag = NULL;

    for (int i = 1; flags[i] != '\0'; i++) {
        switch(flags[i]) {
            case '-':
                short_flag = handle_long_flag(flags);
                if (short_flag == NULL)
                    return 1;
                flags = short_flag;
                i = 0;
                break;
            case 'a':
                g_flags.all = 1; break;
            case 'g':
                g_flags.external = 1; break;
            case 'u':
                g_flags.undefined = 1; break;
            case 'r':
                g_flags.reverse = 1; break;
            case 'p':
                g_flags.no_sort = 1; break;
            default:
                ft_putstr_err("nm: invalid option\n");
                ft_putchar_err(flags[i]);
                ft_putstr_err("'\n");
                print_usage();
                return 1;
        }
    }
    return 0;
}

static t_list   *process_args(int argc, char *argv[]) {
    t_list  *file_paths;

    file_paths = NULL;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] != '\0') {
            if (set_flags(argv[i]) == 0)
                continue ;
            ft_list_clear(file_paths, NULL);
            return NULL;
        }
        ft_list_push_back(&file_paths, argv[i]);
    }
    if (file_paths == NULL)
        ft_list_push_back(&file_paths, "a.out");
    if (ft_list_size(file_paths) > 1)
        g_flags.path = 1;
    return file_paths;
}

int main(int argc, char *argv[]) {
    int     exit_status;
    t_list  *file_paths;

    file_paths = process_args(argc, argv);
    if (file_paths == NULL)
        return 1;
    exit_status = 0;
    for (;file_paths != NULL; file_paths = file_paths->next)
        exit_status = ft_nm((char*)file_paths->data) || exit_status;
    ft_list_clear(file_paths, NULL);
    return exit_status;
}
