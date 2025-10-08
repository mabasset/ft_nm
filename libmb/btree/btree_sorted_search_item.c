#include "ft_btree.h"

void    *btree_sorted_search_item(t_btree *root, void *data_ref, int (*cmpf)(void *, void *)) {
    int diff;

    if (root == NULL)
        return NULL;
    diff = cmpf(root->item, data_ref);
    if (diff == 0)
        return root->item;
    if (diff > 0)
        return btree_sorted_search_item(root->left, data_ref, cmpf);

    return btree_sorted_search_item(root->right, data_ref, cmpf);
}

// #include <stdio.h>
// #include <math.h>

// int ft_intcmp(void *num1, void *num2) {
//     return *((int*)num1) - *((int*)num2);
// }

// void    ft_print_int(void *data) {
//     printf("%d ", *((int*)data));
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
//     x = 0;
//     printf("search for: %d\n", x);
//     item = btree_sorted_search_item(btree, &x, ft_intcmp);
//     if (item == NULL)
//         printf("not found\n");
//     else
//         printf("found: %d\n", *((int*)item));
// }