#include "ft_nm.h"

t_flags g_flags = { 0 };

int get_file_fd(char *file_path) {
    int fd;

    fd = open(file_path, O_RDONLY);
    if (errno == ENOENT)
        print_error(file_path, "No such file", MSG_WARNING, SINGLE_QUOTES);
    return fd;
}

size_t   get_file_size(int fd, char *file_path) {
    struct stat file_info;

    if (fstat(fd, &file_info) < 0) {
        print_error(file_path, strerror(errno), MSG_ERROR, SINGLE_QUOTES);
        return 0;
    }
    if (S_ISDIR(file_info.st_mode)) {
        print_error(file_path, "is a directory", MSG_WARNING, SINGLE_QUOTES);
        return 0;
    }
    return file_info.st_size;
}

char    *get_file_content(int fd, size_t file_size) {
    void    *map;

    map = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (map == NULL || map == MAP_FAILED)
        return NULL;
    return (char *) map;
}

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

int ft_nm(char *file_path) {
    int         fd __attribute__ ((cleanup(close_fd)));
    t_string    mapped_file __attribute__ ((cleanup(unmap_file))) = { 0 };
    t_sym_info  **symbols_info __attribute__ ((cleanup(free_matrix)));

    fd = get_file_fd(file_path);
    if (fd < 0)
        return 1;
    mapped_file.size = get_file_size(fd, file_path);
    if (mapped_file.size == 0)
        return 1;
    mapped_file.content = get_file_content(fd, mapped_file.size);
    if (check_content(mapped_file)) {
        print_error(file_path, "file format not recognized", MSG_ERROR, NO_QUOTES);
        return 1;
    }
    g_flags.endian_match = define_endianess(mapped_file.content[EI_DATA]);

    symbols_info = (mapped_file.content[EI_CLASS] == ELFCLASS32) ? 
       x32_get_symbols_info(mapped_file) : x64_get_symbols_info(mapped_file);
    if (symbols_info == NULL)
        return 1;
    if (symbols_info[0] == NULL) {
        print_no_symbols(file_path);
        return 0;
    }

    // if (!g_flags.no_sort)
    //     sort_symbols(symbols_info);
    // display_symbols(symbols_info);
    return 0;
}

int     set_flags(char *flags) {
    static char error[25] = "invalid option -- \'?\'";

    for (int i = 1; flags[i] != '\0'; i++) {
        switch(flags[i]) {
            case 'a':
                g_flags.all = 1;
                break;
            case 'g':
                g_flags.external = 1;
                break;
            case 'u':
                g_flags.undefined = 1;
                break;
            case 'r':
                g_flags.reverse = 1;
                break;
            case 'p':
                g_flags.no_sort = 1;
                break;
            default:
                error[19] = flags[i];
                print_error(NULL, error, MSG_ERROR, SINGLE_QUOTES);
                return 1;
        }
    }
    return 0;
}

char    **process_arguments(int argc, char *argv[]) {
    char    **file_paths;
    int     k = 0;

    file_paths = malloc(sizeof(char *) * (argc + 1));
    if (file_paths == NULL) {
        print_error(NULL, strerror(errno), MSG_ERROR, SINGLE_QUOTES);
        return NULL;
    }
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] != '\0') {
            if (set_flags(argv[i])) {
                print_usage();
                free(file_paths);
                return NULL;
            }
            continue ;
        }
        file_paths[k] = argv[i];
        k++;
    }
    if (k == 0)
        file_paths[k++] = "a.out";
    file_paths[k] = NULL;
    if (k > 1)
        g_flags.path = 1;
    return file_paths;
}

int main(int argc, char *argv[]) {
    int     exit_status = 0;
    char    **file_paths;

    file_paths = process_arguments(argc, argv);
    if (file_paths == NULL)
        return 1;
    for (int i = 0; file_paths[i] != NULL; i++)
        exit_status = ft_nm(file_paths[i]) || exit_status;
    free(file_paths);
    return exit_status;
}
