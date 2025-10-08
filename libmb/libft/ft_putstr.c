#include <unistd.h>

void    ft_putstr(const char *s)
{
    while (*s != '\0')
        write(1, s++, 1);
}