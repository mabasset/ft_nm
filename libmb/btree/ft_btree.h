#ifndef FT_BTREE_H
# define FT_BTREE_H

# include <stdlib.h>

#include "../list/ft_list.h"

typedef struct s_btree
{
    struct s_btree  *left;
    struct s_btree  *right;
    void            *item;
} t_btree;

t_btree *btree_create_node(void *item);
void    btree_apply_preorder(t_btree *root, void (*applyf)(void *));
void    btree_apply_inorder(t_btree *root, void (*applyf)(void *));
void    btree_apply_postorder(t_btree *root, void (*applyf)(void *));
void    btree_apply_by_level(t_btree *root, void (*applyf)(void *item, int current_level, int is_first_elem));
void    btree_insert_data(t_btree **root, void *item, int (*cmpf)(void *, void *));
void    *btree_search_item(t_btree *root, void *data_ref, int (*cmpf)(void *, void *));
void    *btree_sorted_search_item(t_btree *root, void *data_ref, int (*cmpf)(void *, void *));
int     btree_level_count(t_btree *root);
void    btree_insert_list(t_btree **root, t_list *list, int (*cmpf)(void *, void *));
t_btree *btree_create_from_list(t_list *list, int (*cmpf)(void *, void *));
void    btree_clear(t_btree *root, void (*free_fct)(void *));

#endif