#include "ft_list.h"

t_list  *ft_list_at(t_list *begin_list, unsigned int nbr) {
    for (unsigned int i = 0; begin_list != NULL; i++, begin_list = begin_list->next)
        if (i == nbr)
            break ;

    return begin_list;
}
