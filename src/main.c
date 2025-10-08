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
    t_string    file __attribute__((cleanup(unmap_file)));
    Elf_Ehdr    *header;
    t_elf_info  elf_info = {0};
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
    if (symbols == NULL)
        return print_no_symbols(file_path);

    if (!g_flags.no_sort) {
        if (g_flags.reverse)
            ft_list_sort(&symbols, symcmp_rev);
        else
            ft_list_sort(&symbols, symcmp);
    }
    print_symbols(symbols, file_path, file.content[EI_CLASS]);
    ft_list_clear(symbols, free_sym);

    return 0;
}

static int  set_flags(char *long_flag, t_btree *flag_map) {
    const char  *short_flags;

    short_flags = long_flag + 1;
    if (*short_flags == '-')
        short_flags = map_get(flag_map, long_flag + 2, map_cmp_str);
    if (short_flags == NULL)
        return print_unrecognized_option(long_flag);

    for (int i = 0; short_flags[i] != '\0'; i++) {
        switch(short_flags[i]) {
            case 'g':
                g_flags.external = 1; break;
            case 'p':
                g_flags.no_sort = 1; break;
            case 'r':
                g_flags.reverse = 1; break;
            case 'u':
                g_flags.undefined = 1; break;
            case 'a':
                g_flags.all = 1; break;
            default:
                return print_invalid_option(short_flags[i]);
        }
    }
    return 0;
}

int process_args(t_list **file_paths, int argc, char *argv[]) {
    int     end_of_options;
    t_btree *flag_map __attribute__((cleanup(clear_map)));
    t_map   entries[] = {
        {"no-sort", "p"},
        {"debug-syms", "a"},
        {"extern-only", "g"},
        {"undefined-only", "u"},
        {"reverse-sort", "r"}
    };

    end_of_options = 0;
    flag_map = map_create_from_entries(entries, sizeof(entries) / sizeof(t_map), map_cmp_entry_str);
    for (int i = 1; i < argc; i++) {
        if (!end_of_options && ft_strcmp(argv[i], "--") == 0)
            end_of_options = 1;
        else if (!end_of_options && (argv[i][0] == '-' && argv[i][1] != '\0')) {
            if (set_flags(argv[i], flag_map))
                return 1;
        }
        else
            ft_list_push_back(file_paths, argv[i]);
    }
    if (*file_paths == NULL)
        ft_list_push_back(file_paths, "a.out");
    if (ft_list_size(*file_paths) > 1)
        g_flags.path = 1;
    return 0;
}

int main(int argc, char *argv[]) {
    int     exit_status;
    t_list  *file_paths __attribute__((cleanup(clear_list)));

    file_paths = NULL;
    if (process_args(&file_paths, argc, argv))
        return 1;
    exit_status = 0;
    for (t_list *curr = file_paths; curr != NULL; curr = curr->next)
        exit_status = ft_nm((char*)curr->data) || exit_status;
    return exit_status;
}
