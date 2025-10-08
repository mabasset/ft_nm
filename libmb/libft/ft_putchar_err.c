#include <unistd.h>

void    ft_putchar_err(char c)
{
    write(2, &c, 1);
}