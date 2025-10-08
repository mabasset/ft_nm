#include "ft_list.h"

void    ft_list_reverse_fun(t_list *begin_list) {
    t_list  *node;
    t_list  *tmp_list;

    node = begin_list;
    tmp_list = NULL;
    for (; node != NULL; node = node->next)
        ft_list_push_front(&tmp_list, node->data);
    for(; begin_list != NULL; begin_list = begin_list->next) {
        node = tmp_list->next;
        begin_list->data = tmp_list->data;
        free(tmp_list);
        tmp_list = node;
    }
}

// #include <stdio.h>

// static void ft_print_str(void *data) {
//     printf("%s ", (char *)data);
// }

// int main() {
//     char    *ar[] = {"a", "a", "d", "c", "d", "b"};
//     t_list  *list = ft_list_push_strs(sizeof(ar) / sizeof(char *), ar);

//     ft_list_foreach(list, ft_print_str);
//     printf("\n---------\n");
//     ft_list_reverse_fun(list);
//     ft_list_foreach(list, ft_print_str);
//     printf("\n");
//     ft_list_clear(list, NULL);
// }