/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabasset <mabasset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 17:48:07 by mabasset          #+#    #+#             */
/*   Updated: 2022/01/18 18:11:36 by mabasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static char *ft_strdup(const char *s1)
{
    char    *ptr;
    int     i;

    i = 0;
    while (s1[i] != '\0')
        i++;
    ptr = (char *) malloc (sizeof(char) * i + 1);
    i = 0;
    while (s1[i] != 0)
    {
        ptr[i] = s1[i];
        i++;
    }
    ptr[i] = '\0';
    return (ptr);
}

char    *ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
    char            *str;
    unsigned int    i;

    if (s == NULL || f == NULL)
        return (NULL);
    str = ft_strdup(s);
    if (str == NULL)
        return (NULL);
    i = 0;
    while (str[i] != '\0')
    {
        str[i] = (*f)(i, str[i]);
        i++;
    }
    return (str);
}
