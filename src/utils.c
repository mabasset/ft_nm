#include "ft_nm.h"

void    close_fd(int *fd) {
    if (*fd > 0)
        close(*fd);
}

void    unmap_file(t_string *file) {
    if (file != NULL)
        munmap(file->content, file->size);
}

int symcmp(void *sym1, void *sym2) {
    return ft_strcmp(((t_sym*)sym1)->name, ((t_sym*)sym2)->name);
}

t_string    extract_content(char *file_path) {
    int         fd __attribute__ ((cleanup(close_fd)));
    t_stat      stat;
    void        *map;
    t_string    file = {0};

    fd = open(file_path, O_RDONLY);
    if (fd < 0 || fstat(fd, &stat) < 0) {
        ft_putstr_err("nm: '");
        ft_putstr_err(file_path);
        ft_putstr_err("': No such file\n");
        return file;
    }
    map = mmap(NULL, stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (map == NULL || map == MAP_FAILED)
        return file;
    file.content = (char*) map;
    file.size = stat.st_size;
    return file;
}
