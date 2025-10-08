#include "ft_list.h"

static void ft_remove(t_list *node, void (*free_fct)(void *)) {
    if (free_fct != NULL)
        free_fct(node->data);
    free(node);
}

void    ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)(void *, void *), void (*free_fct)(void *)) {
    t_list  *prev;
    t_list  *curr;

    if (*begin_list == NULL)
        return ;
    prev = *begin_list;
    curr = prev->next;
    while (curr != NULL) {
        if(cmp(curr->data, data_ref) == 0) {
            prev->next = curr->next;
            ft_remove(curr, free_fct);
        } else
            prev = prev->next;
        curr = prev->next;
    }
    if(cmp((*begin_list)->data, data_ref) != 0)
        return ;
    curr = (*begin_list)->next;
    ft_remove(*begin_list, free_fct);
    *begin_list = curr;
}

// #include <stdio.h>
// #include <string.h>

// static void ft_print_str(void *data) {
//     printf("%s ", (char *)data);
// }

// int main() {
//     char    *ar[3];
//     t_list  *list;

//     for(int i = 0; i < 3; i++)
//         ar[i] = malloc(sizeof(char) * 2);
//     strcpy(ar[0], "c");
//     strcpy(ar[1], "b");
//     strcpy(ar[2], "a");
//     list = ft_list_push_strs(3, ar);
//     ft_list_foreach(list, ft_print_str);
//     printf("\n---------\n");
//     ft_list_remove_if(&list, "c", strcmp, free);
//     ft_list_foreach(list, ft_print_str);
//     printf("\n");
//     ft_list_clear(list, free);
// }