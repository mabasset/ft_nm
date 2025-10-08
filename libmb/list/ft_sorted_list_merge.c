#include "ft_list.h"

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

void    ft_sorted_list_merge(t_list **begin_list1, t_list *begin_list2, int (*cmp)(void *, void *)) {
    *begin_list1 = ft_merge(*begin_list1, begin_list2, cmp);
}

// #include <stdio.h>
// #include <string.h>

// static void ft_print_str(void *data) {
//     printf("%s ", (char *)data);
// }

// int main() {
//     char    *ar1[] = {"z", "a", "f", "c", "d", "b"};
//     char    *ar2[] = {"o", "m", "d", "x", "d", "p"};
//     t_list  *list1 = ft_list_push_strs(sizeof(ar1) / sizeof(char *), ar1);
//     t_list  *list2 = ft_list_push_strs(sizeof(ar2) / sizeof(char *), ar2);

//     ft_list_sort(&list1, strcmp);
//     ft_list_sort(&list2, strcmp);
//     ft_list_foreach(list1, ft_print_str);
//     printf("\n");
//     ft_list_foreach(list2, ft_print_str);
//     printf("\n---------\n");
//     ft_sorted_list_merge(&list1, list2, strcmp);
//     ft_list_foreach(list1, ft_print_str);
//     printf("\n");
//     ft_list_clear(list1, NULL);
// }