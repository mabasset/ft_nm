#include "ft_btree.h"

void    btree_apply_preorder(t_btree *root, void (*applyf)(void *)) {
    if (root == NULL)
        return ;
    applyf(root->item);
    btree_apply_preorder(root->left, applyf);
    btree_apply_preorder(root->right, applyf);
}
