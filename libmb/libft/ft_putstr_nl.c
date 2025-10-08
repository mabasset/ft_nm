#include <unistd.h>

void    ft_putstr_nl(char *s)
{
    while (*s != '\0')
        write(1, s++, 1);
    write(1, "\n", 1);
}