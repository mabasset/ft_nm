#include "ft_nm.h"

t_elf_info g_elf = {NULL, 0, ELFCLASSNONE, ELFDATANONE};

static int (*elf_processors[])() = {
    NULL,
    process_elf32,
    process_elf64
};

int check_content(char *content) {
    if (g_elf.size < sizeof(Elf32_Ehdr) ||
        content[0] != 0x7f || content[1] != 'E' ||
        content[2] != 'L' || content[3] != 'F') {
        return 1;
    }
    if(content[EI_CLASS] != ELFCLASS32 && content[EI_CLASS] != ELFCLASS64)
        return 1;
    if (content[EI_CLASS] == ELFCLASS64 && g_elf.size < sizeof(Elf64_Ehdr))
        return 1;
    if (content[EI_DATA] != ELFDATA2LSB && content[EI_DATA] != ELFDATA2MSB)
        return 1;
    if (content[EI_VERSION] != EV_CURRENT)
        return 1;
    return 0;
}

int ft_nm(char *file_path) {
    struct stat file_info;
    int         fd;
    void        *map;
    char        *content;

    g_elf.path = file_path;
    fd = open(file_path, O_RDONLY);
    if (fd == -1)
        return print_error(strerror(errno));
    if (stat(file_path, &file_info) < 0)
        return print_error(strerror(errno));
    if (S_ISDIR(file_info.st_mode))
        return print_error("is a directory");
    g_elf.size = file_info.st_size;
    map = mmap(NULL, file_info.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    if (map == MAP_FAILED)
        return print_error(strerror(errno));
    content = (char *) map;
    if (check_content(content)) {
        print_error("file format not recognized");
        munmap(map, file_info.st_size);
        return 1;
    }

    g_elf.content = content;
    g_elf.class = g_elf.content[EI_CLASS];
    g_elf.endian = g_elf.content[EI_DATA];
    elf_processors[g_elf.class]();

    munmap(map, file_info.st_size);
    return 0;
}

int main(int argc, char *argv[]) {
    int exit_status = 0;

    if (argc < 2)
        return ft_nm("a.out");
    for (int i = 1; i < argc; i++)
        exit_status = ft_nm(argv[i]) || exit_status;

    return exit_status;
}