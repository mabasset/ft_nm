/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabasset <mabasset@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/25 08:52:02 by mabasset          #+#    #+#             */
/*   Updated: 2022/01/28 23:03:32 by mabasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_dprintf(int fd, const char *str, ...)
{
	va_list		arg;
	t_struct	params;
	size_t		len;

	va_start(arg, str);
	len = 0;
	while (*str != '\0')
	{
		if (*str == '%')
		{
			ft_struct_init(&params);
			str += ft_checkflags(str + 1, &params);
			ft_identifier(*(str + 1), arg, &params);
			str++;
			len += params.len;
		}
		else
		{
			write (fd, str, 1);
			len++;
		}
		str++;
	}
	return (len);
}