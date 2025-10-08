/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabasset <mabasset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 18:08:49 by mabasset          #+#    #+#             */
/*   Updated: 2022/01/17 16:55:05 by mabasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static void ft_bzero(void *s, size_t n)
{
    size_t  i;

    i = 0;
    while (i < n)
    {
        *(char *)(s + i) = 0;
        i++;
    }
}

void    *ft_calloc(size_t count, size_t size)
{
    void    *ptr;

    ptr = malloc(count * size);
    if (ptr == NULL)
        return (NULL);
    ft_bzero(ptr, count * size);
    return (ptr);
}
