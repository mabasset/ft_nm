#include "ft_btree.h"

void    btree_clear(t_btree *root, void (*free_fct)(void *)) {
    if (root == NULL)
        return ;
    btree_clear(root->left, free_fct);
    btree_clear(root->right, free_fct);
    if (free_fct != NULL)
        free_fct(root->item);
    free(root);
}