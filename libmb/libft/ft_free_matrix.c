/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_matrix.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabasset <mabasset@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 18:28:34 by mabasset          #+#    #+#             */
/*   Updated: 2022/06/06 18:30:23 by mabasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void    ft_free_matrix(void **matrix)
{
    int i;

    i = 0;
    while (matrix[i] != NULL)
    {
        free(matrix[i]);
        i++;
    }
    free(matrix);
}