#include "ft_list.h"

t_list  *ft_list_push_strs(int size, char **strs) {
    t_list  *begin_list;

    begin_list = NULL;
    for (int i = 0; i < size; i++)
        ft_list_push_front(&begin_list, strs[i]);

    return begin_list;
}
