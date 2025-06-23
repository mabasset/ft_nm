#include "ft_nm.h"

t_elf_info g_elf = {NULL, NULL, 0, ELFCLASSNONE, ELFDATANONE};

int print_error() {
    write(STDERR_FILENO, "nm: '", 5);
    write(STDERR_FILENO, g_elf.path, strlen(g_elf.path));
    write(STDERR_FILENO, "': ", 3);
    perror("");
    return 1;
}

int check_content() {
    if (g_elf.size < sizeof(Elf32_Ehdr) ||
        g_elf.content[0] != 0x7f || g_elf.content[1] != 'E' ||
        g_elf.content[2] != 'L' || g_elf.content[3] != 'F') {
        return 1;
    }
    if(g_elf.content[EI_CLASS] != ELFCLASS32 && g_elf.content[EI_CLASS] != ELFCLASS64)
        return 1;
    if (g_elf.content[EI_CLASS] == ELFCLASS64 && g_elf.size < sizeof(Elf64_Ehdr))
        return 1;
    if (g_elf.content[EI_DATA] != ELFDATA2LSB && g_elf.content[EI_DATA] != ELFDATA2MSB)
        return 1;
    if (g_elf.content[EI_VERSION] != EV_CURRENT)
        return 1;
    return 0;
}

int ft_nm(char *file_path) {
    struct stat file_info;
    int         fd;
    void        *map;

    g_elf.path = file_path;
    fd = open(file_path, O_RDONLY);
    if (fd == -1)
        return print_error();
    if (fstat(fd, &file_info) < 0)
        return print_error();
    if (S_ISDIR(file_info.st_mode))
        return print_error("is a directory");
    g_elf.size = file_info.st_size;
    map = mmap(NULL, file_info.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    if (map == MAP_FAILED)
        return print_error();

    g_elf.content = (char *) map;
    if (check_content()) {
        print_error("file format not recognized");
        munmap(map, file_info.st_size);
        return 1;
    }
    g_elf.class = g_elf.content[EI_CLASS];
    g_elf.endian = g_elf.content[EI_DATA];
    process_elf64();

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