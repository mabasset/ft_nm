#include "ft_btree.h"

void    btree_insert_data(t_btree **root, void *item, int (*cmpf)(void *, void *)) {
    if (*root == NULL)
        *root = btree_create_node(item);
    else if (cmpf((*root)->item, item) > 0)
        btree_insert_data(&((*root)->left), item, cmpf);
    else
        btree_insert_data(&((*root)->right), item, cmpf);
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
//     btree_apply_infix(btree, ft_print_int);
//     printf("\n");
// }
