#include "ft_list.h"

void    ft_list_foreach(t_list *begin_list, void (*f)(void *)) {
    for(;begin_list != NULL; begin_list = begin_list->next)
        f(begin_list->data);
}
