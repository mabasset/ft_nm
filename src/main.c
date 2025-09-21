#include "ft_nm.h"

t_flags g_flags = { 0 };

// int get_file_fd(char *file_path) {
//     int fd;

//     fd = open(file_path, O_RDONLY);
//     if (errno == ENOENT)
//         ft_dprintf(STDERR_FILENO, "nm: '%s': No such file\n", file_path);
//     return fd;
// }

// size_t   get_file_size(int fd, char *file_path) {
//     struct stat file_info;

//     if (fstat(fd, &file_info) < 0) {
//         ft_dprintf(STDERR_FILENO, "nm: '%s': %s\n", file_path, strerror(errno));
//         return 0;
//     }
//     if (S_ISDIR(file_info.st_mode)) {
//         ft_dprintf(STDERR_FILENO, "nm: Warning: '%s' is a directory\n", file_path);
//         return 0;
//     }
//     return file_info.st_size;
// }

// char    *get_file_content(int fd, size_t file_size) {
//     void    *map;

//     map = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
//     if (map == NULL || map == MAP_FAILED)
//         return NULL;
//     return (char *) map;
// }

// int check_content(t_string mapped_file) {
//     char    *content = mapped_file.content;
//     size_t  size = mapped_file.size;

//     if (size < sizeof(Elf32_Ehdr) ||
//         content[0] != 0x7f || content[1] != 'E' ||
//         content[2] != 'L' || content[3] != 'F') {
//         return 1;
//     }
//     if(content[EI_CLASS] != ELFCLASS32 && content[EI_CLASS] != ELFCLASS64)
//         return 1;
//     if (content[EI_CLASS] == ELFCLASS64 && size < sizeof(Elf64_Ehdr))
//         return 1;
//     if (content[EI_DATA] != ELFDATA2LSB && content[EI_DATA] != ELFDATA2MSB)
//         return 1;
//     if (content[EI_VERSION] != EV_CURRENT)
//         return 1;
//     return 0;
// }

// int ft_nm(char *file_path) {
//     int         fd __attribute__ ((cleanup(close_fd)));
//     t_string    mapped_file __attribute__ ((cleanup(unmap_file))) = { 0 };
//     t_sym_info  **symbols_info;

//     fd = get_file_fd(file_path);
//     if (fd < 0)
//         return 1;
//     mapped_file.size = get_file_size(fd, file_path);
//     if (mapped_file.size == 0)
//         return 1;
//     mapped_file.content = get_file_content(fd, mapped_file.size);
//     if (check_content(mapped_file)) {
//         ft_dprintf(STDERR_FILENO, "nm: %s: file format not recognized\n", file_path, strerror(errno));
//         return 1;
//     }
//     g_flags.endian_match = define_endianess(mapped_file.content[EI_DATA]);

//     symbols_info = (mapped_file.content[EI_CLASS] == ELFCLASS32) ? 
//        x32_get_symbols_info(mapped_file) : x64_get_symbols_info(mapped_file);
//     if (symbols_info == NULL)
//         return 1;
//     if (symbols_info[0] == NULL) {
//         ft_dprintf(STDERR_FILENO, "nm: %s: no symbols\n", file_path);
//         return 0;
//     }

//     if (!g_flags.no_sort)
//         sort_symbols(symbols_info);
//     display_symbols(symbols_info, file_path, mapped_file.content[EI_CLASS]);
//     free_matrix(&symbols_info);
//     return 0;
// }

char    *handle_long_flag(char *flag) {
    char    *long_flags[] = {"--debug-syms", "--extern-only", "--undefined-only", "--reverse-sort", "--no-sort"};
    char    *short_flags[] = {"-a", "-g", "-u", "-r", "-p"};
    int     size;
    int     i;

    size = sizeof(long_flags) / sizeof(char *);
    for (i = 0; i < size; i++)
        if (ft_strcmp(flag, long_flags[i]) == 0)
            break ;
    size = sizeof(short_flags) / sizeof(char *);
    if (i < size)
        return short_flags[i];
    ft_putstr_err("nm: unrecognized option\n");
    print_usage();

    return NULL;
}

int     set_flags(char *flags) {
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
                print_usage();
                return 1;
        }
    }
    return 0;
}

t_list  *process_arguments(int argc, char *argv[]) {
    t_list  *file_paths;

    file_paths = NULL;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] != '\0') {
            if (set_flags(argv[i])) {
                // print_usage();
                ft_list_clear(file_paths, NULL);
                return NULL;
            }
            continue;
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

    file_paths = process_arguments(argc, argv);
    if (file_paths == NULL)
        return 1;
    exit_status = 0;
    for (;file_paths != NULL; file_paths = file_paths->next)
        ft_printf("%s\n", file_paths->data);
        // exit_status = ft_nm(file_paths) || exit_status;
    ft_list_clear(file_paths, NULL);
    return exit_status;
}
