#include "ft_nm.h"

t_elf_file g_elf_file = { 0 };
t_flags g_flags = { 0 };

// typedef struct {
//     char    *str;
//     int     warn;
//     int     s_quotes;
// } t_error;

char    *get_file_map(char *file_path, void **map) {
    int         fd;
    struct stat file_info;

    fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        if (errno != ENOENT)
            return strerror(errno);
            //return (t_error){ strerror(errno), 0, 0 };
    }
        return (errno == ENOENT) ? "No such file": strerror(errno);
    if (fstat(fd, &file_info) < 0) {
        close(fd);
        return strerror(errno);
    } else if (S_ISDIR(file_info.st_mode)) {
        close(fd);
        return "is a directory";
    }
    *map = mmap(NULL, file_info.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    if (*map == NULL || *map == MAP_FAILED)
        return strerror(errno);

    g_elf_file.size = file_info.st_size;
    return NULL;
}

int check_content(char *content) {
    if (g_elf_file.size < sizeof(Elf32_Ehdr) ||
        content[0] != 0x7f || content[1] != 'E' ||
        content[2] != 'L' || content[3] != 'F') {
        return 1;
    }
    if(content[EI_CLASS] != ELFCLASS32 && content[EI_CLASS] != ELFCLASS64)
        return 1;
    if (content[EI_CLASS] == ELFCLASS64 && g_elf_file.size < sizeof(Elf64_Ehdr))
        return 1;
    if (content[EI_DATA] != ELFDATA2LSB && content[EI_DATA] != ELFDATA2MSB)
        return 1;
    if (content[EI_VERSION] != EV_CURRENT)
        return 1;
    return 0;
}

int ft_nm(char *file_path) {
    int     ret;
    //t_error err;
    void    *map;
    char    *content, *err;

    g_elf_file.path = file_path;
    err = get_file_map(file_path, &map);
    //if (err.str)
    if (err)
        return print_error(err);
        //return print_error(err.str);
    content = (char *) map;
    if (check_content(content)) {
        munmap(map, g_elf_file.size);
        return print_error("file format not recognized");
        //return print_error((t_error){ "file format not recognized", 0, 0 });
    }
    g_elf_file.content = content;
    g_elf_file.endian_match = define_endianess(content[EI_DATA]);
    ret = (content[EI_CLASS] == ELFCLASS32) ? x32_process_elf() : x64_process_elf();

    munmap(map, g_elf_file.size);
    return ret;
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
                print_error(error);
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
        print_error(strerror(errno));
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