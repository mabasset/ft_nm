#include "ft_btree.h"

int btree_level_count(t_btree *root) {
    int left;
    int right;

    if (root == NULL)
        return 0;
    left = btree_level_count(root->left);
    right = btree_level_count(root->right);
    if (right > left)
        return right + 1;

    return left + 1;
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

//     for(int i = 0; i < 10; i++) {
//         ar[i] = rand() % 10;
//         printf("%d ", ar[i]);
//     }
//     printf("\n------------------\n");
//     btree = NULL;
//     for (int i = 0; i < 10; i++)
//         btree_insert_data(&btree, &ar[i], ft_intcmp);
//     btree_apply_inorder(btree, ft_print_int);
//     printf("\n------------------\n");
//     printf("level count: %d\n", btree_level_count(btree));
// }
