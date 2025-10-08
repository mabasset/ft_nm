#include "ft_list.h"

int ft_list_size(t_list *begin_list) {
    int i = 0;
    for(;begin_list != NULL; begin_list = begin_list->next)
        i++;
    return i;
}
