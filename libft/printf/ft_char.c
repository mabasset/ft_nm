/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_char.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabasset <mabasset@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/25 15:53:41 by mabasset          #+#    #+#             */
/*   Updated: 2022/01/29 00:08:09 by mabasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int ft_toggle(char ch)
{
    if (ch == '-' || ch == '+')
        return (1);
    return (0);
}

void ft_putsep(int fd, char *str, int seplen, t_struct *params)
{
    char ch;

    ch = ' ';
    if (params->zero == 1)
        ch = '0';
    if ((*str == '-' || *str == '+') && ft_isdigit(*(str + 1)))
        seplen -= 1;
    while (seplen > 0)
    {
        params->len += write(fd, &ch, 1);
        seplen--;
    }
}

void ft_printnum(int fd, char *str, t_struct *params)
{
    int seplen;
    int len;
    int i;

    len = ft_strlen(str);
    i = 0;
    len -= ft_toggle(str[i]);
    if (params->precision == 0)
        params->precision = len;
    seplen = params->width - params->precision;
    if (params->minus == 0 && params->zero == 0)
        ft_putsep(fd, str, seplen, params);
    if (str[i] == '-' || str[i] == '+')
    {
        params->len += write(fd, str, 1);
        i = 1;
    }
    if (params->minus == 0 && params->zero == 1)
        ft_putsep(fd, str, seplen, params);
    while (len < params->precision--)
        params->len += write(fd, "0", 1);
    while (str[i] != '\0')
        params->len += write(fd, &str[i++], 1);
    if (params->minus == 1)
        ft_putsep(fd, str, seplen, params);
}

void ft_printchar(int fd, char ch, t_struct *params)
{
    int width;

    width = params->width;
    if (params->minus == 0)
    {
        while (width > 1)
        {
            params->len += write(fd, " ", 1);
            width--;
        }
    }
    params->len += write(1, &ch, 1);
    if (params->minus == 1)
    {
        while (width > 1)
        {
            params->len += write(fd, " ", 1);
            width--;
        }
    }
}

void ft_printstr(int fd, char *str, t_struct *params)
{
    int seplen;
    int len;
    int i;

    if (str == NULL)
    {
        params->len += write(fd, "(null)", 6);
        return;
    }
    i = 0;
    len = ft_strlen(str);
    if (params->precision == 0 && params->point == 1)
        return;
    if (params->precision == 0)
        params->precision = len;
    seplen = params->width - params->precision;
    if (params->minus == 0)
        ft_putsep(fd, str, seplen, params);
    while (str[i] != '\0' && i < params->precision)
        params->len += write(fd, &str[i++], 1);
    if (params->minus == 1)
        ft_putsep(fd, str, seplen, params);
}
