#include "ft_list.h"

static t_list  *ft_split(t_list *begin_list) {
    t_list *half;

    half = begin_list->next;
    while (half != NULL) {
        half = half->next;
        if (half != NULL) {
            half = half->next;
            begin_list = begin_list->next;
        }
    }
    half = begin_list->next;
    begin_list->next = NULL;
    return half;
}

static t_list   *ft_merge(t_list *list1, t_list *list2, int (*cmp)(void *, void *)) {
    t_list  *result;

    if (list1 == NULL)
        return list2;
    if (list2 == NULL)
        return list1;
    if (cmp(list1->data, list2->data) <= 0) {
        result = list1;
        result->next = ft_merge(list1->next, list2, cmp);
    } else {
        result = list2;
        result->next = ft_merge(list1, list2->next, cmp);
    }

    return result;
}

void    ft_list_sort(t_list **begin_list, int (*cmp)(void *, void *)) {
    t_list  *half;

    if (*begin_list == NULL || (*begin_list)->next == NULL)
        return ;
    half = ft_split(*begin_list);
    ft_list_sort(begin_list, cmp);
    ft_list_sort(&half, cmp);
    *begin_list = ft_merge(*begin_list, half, cmp);
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
//     printf("\n");
//     ft_list_clear(list, NULL);
// }