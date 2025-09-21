#include "ft_nm.h"

void    print_usage() {
    int     fd;
    t_stat  stat;
    void    *map;

    fd = open(USAGE_FILEPATH, O_RDONLY);
    if (fd < 0 || fstat(fd, &stat) < 0) {
        ft_putstr_err("Usage: unavailable");
        return ;
    }
    map = mmap(NULL, stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (map == NULL || map == MAP_FAILED) {
        ft_putstr_err("Usage: unavailable");
        return ;
    }
    ft_putstr_err((char*) map);
    munmap(map, stat.st_size);
}
