#include "ft_list.h"

void    ft_sorted_list_insert(t_list **begin_list, void *data, int (*cmp)(void *, void *)) {
    t_list  *node;
    t_list  *curr;

    if (*begin_list == NULL || cmp((*begin_list)->data, data) >= 0) {
        ft_list_push_front(begin_list, data);
        return ;
    }
    for (curr = *begin_list; curr->next != NULL; curr = curr->next)
        if (cmp(curr->next->data, data) >= 0)
            break ;
    node = ft_create_elem(data);
    node->next = curr->next;
    curr->next = node;
}

// #include <stdio.h>
// #include <string.h>

// static void ft_print_str(void *data) {
//     printf("%s ", (char *)data);
// }

// int main() {
//     char    *ar[] = {"a", "a", "d", "c", "d", "b"};
//     t_list  *list = ft_list_push_strs(sizeof(ar) / sizeof(char *), ar);

//     ft_list_foreach(list, ft_print_str);
//     printf("\n---------\n");
//     ft_list_sort(&list, strcmp);
//     ft_list_foreach(list, ft_print_str);
//     printf("\n---------\n");
//     ft_sorted_list_insert(&list, "c", strcmp);
//     ft_list_foreach(list, ft_print_str);
//     printf("\n");
//     ft_list_clear(list, NULL);
// }
