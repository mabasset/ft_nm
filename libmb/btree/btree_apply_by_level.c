#include "ft_btree.h"

void    btree_apply_in_level(t_list *curr, int level, void (*applyf)()) {
    t_list  *next;
    int     first;
    t_btree *data;

    next = NULL;
    first = 1;
    while(curr != NULL) {
        data = (t_btree*)curr->data;
        applyf(data->item, level, first);
        first = 0;
        if (data->left != NULL)
            ft_list_push_back(&next, data->left);
        if (data->right != NULL)
            ft_list_push_back(&next, data->right);
        curr = curr->next;
    }
    if (next == NULL)
        return ;
    btree_apply_in_level(next, level + 1, applyf);
    ft_list_clear(next, NULL);
}


void    btree_apply_by_level(t_btree *root, void (*applyf)(void *item, int current_level, int is_first_elem)) {
    t_list  *first_level;
    
    if (root == NULL)
        return ;
    first_level = NULL;
    ft_list_push_back(&first_level, root);
    btree_apply_in_level(first_level, 0, applyf);
}

// #include <stdio.h>
// #include <math.h>

// int ft_intcmp(void *num1, void *num2) {
//     return *((int*)num1) - *((int*)num2);
// }

// void    ft_print_int(void *data) {
//     printf("%d ", *((int*)data));
// }

// void    ft_print_int_level(void *item, int current_level, int is_first_elem) {
//     printf("[%d, %d, %d]\n", *((int*)item), current_level, is_first_elem);
// }

// int main() {
//     t_btree *btree;
//     int     ar[10];
//     int     x;
//     void    *item;

//     for(int i = 0; i < 10; i++)
//         ar[i] = rand() % 10;
//     btree = NULL;
//     for (int i = 0; i < 10; i++)
//         btree_insert_data(&btree, &ar[i], ft_intcmp);
//     btree_apply_inorder(btree, ft_print_int);
//     printf("\n------------------\n");
//     btree_apply_by_level(btree, ft_print_int_level);
// }