/**
 * COSC242 Assignment 2020
 * Authors: Maaha Ahmad, Magdeline Huang, Kate Truman
 */

#ifndef TREE_H
#define TREE_H
#include <stddef.h>
#include <stdio.h>

/**
 * Functions that are implemented in tree.c
 */
typedef enum tree_e {BST, RBT} tree_t;
typedef struct tree_node *tree;
extern tree tree_free(tree t);
extern void tree_inorder(tree t, void f(char *str));
extern tree tree_insert(tree t, char *str, int isRoot);
extern tree tree_new();
extern void tree_preorder(tree t, void f(int freq, char *str));
extern int tree_search(tree t, char *str);
extern int tree_depth(tree t);
extern void tree_output_dot(tree t, FILE *out);
extern void tree_colour_print(tree t);
extern tree root_black(tree t);

#endif /* tree_h */
