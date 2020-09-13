/*
 * COSC242 Assignment 2020
 * tree.c implementation file
 * Authors: Maaha Ahmad, Magdeline Huang, Kate Truman
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "mylib.h"

#define IS_BLACK(x) ((NULL==(x)) || (BLACK == (x)->colour))
#define IS_RED(x) ((NULL != (x)) && (RED == (x)->colour))

typedef enum { RED, BLACK} tree_colour;
enum tree_e tree_type;

struct tree_node {
    char *key;
    tree_colour colour;
    tree left;
    tree right;
    int frequency;
    tree parent;
};

/*
 * Sets a new tree instance to its starting state.
 * 
 * Uses the value of the tree_t parameter to set the tree's tree_type
 * to either a BST or RBT.
 *
 * @param t Tree type of the new tree
 * @return A new NULL tree
 */
tree tree_new(tree_t t) {
    if (t == RBT) {
        tree_type = RBT;
    } else {
        tree_type = BST;
    }
    return NULL;
}

/*
 * Right rotates a given tree.
 *
 * The tree is right rotated by setting the root's left child to be the root
 * of the tree and setting the left child of the original root to be
 * the right child of the new root. We then set the right child of the new
 * root to the original root of the tree.
 *
 * @param t The tree to be right rotated
 * @return The updated tree
 */
static tree right_rotate(tree t) {
    tree temp = t;
    t = t->left;
    t->parent = temp->parent;
    temp->left = t->right;
    if (temp->right != NULL) {
        temp->left->parent = temp;
    }
    t->right = temp;
    temp->parent = t;
    return t;
}

/*
 * Left rotates a given tree.
 *
 * The tree is left rotated by setting the root's right child to be the root
 * of the tree and setting the right child of the original root to be
 * the left child of the new root. We then set the right child of the new
 * root to the original root of the tree.
 *
 * @param t The tree to be right rotated
 * @return The new updated tree
 */
static tree left_rotate(tree t) {
    tree temp = t;
    t = t->right;
    t->parent = temp->parent;
    temp->right = t->left;
    if (temp->right != NULL) {
        temp->right->parent = temp;
    }
    t->left = temp;
    temp->parent = t;
    return t;
}

/*
 * Fixes the colour of nodes in a given tree, so that the tree
 * fulfills the properties of a red-black tree (except for the node
 * being black).
 *
 * This function checks the colour of child nodes and uses the left_rotate
 * and right_rotate functions to restore properties of a red-black tree
 * to the specified tree.
 *
 * @param t The tree that needs to be fixed
 * @return The new updated tree
 */
static tree tree_fix(tree t) {
    if (IS_RED(t->left) && IS_RED(t->left->left)) {
        if (IS_RED(t->right)) {
            t->left->colour = BLACK;
            t->right->colour = BLACK;
            if (t->parent == NULL || t->parent->key == NULL) {
                t->colour = BLACK;
            } else {
                t->colour = RED;
            }
        } else {
            t = right_rotate(t);
            t->colour = BLACK;
            t->right->colour = RED;
        }
    } else if (IS_RED(t->left) && IS_RED(t->left->right)) {
        if (IS_RED(t->right)) {
            t->left->colour = BLACK;
            t->right->colour = BLACK;
            if (t->parent == NULL || t->parent->key == NULL) {
                t->colour = BLACK;
            } else {
                t->colour = RED;
            }
        } else {
            t->left = left_rotate(t->left);
            t = right_rotate(t);
            t->colour = BLACK;
            t->right->colour = RED;
        }
    } else if (IS_RED(t->right) && IS_RED(t->right->left)) {
        if (IS_RED(t->left)) {
            t->left->colour = BLACK;
            t->right->colour = BLACK;
            if (t->parent == NULL || t->parent->key == NULL) {
                t->colour = BLACK;
            } else {
                t->colour = RED;
            }
        } else {
            t->right = right_rotate(t->right);
            t = left_rotate(t);
            t->colour = BLACK;
            t->left->colour = RED;
        }
    } else if (IS_RED(t->right) && IS_RED(t->right->right)) {
        if (IS_RED(t->left)) {
            t->left->colour = BLACK;
            t->right->colour = BLACK;
            if (t->parent == NULL || t->parent->key == NULL) {
                t->colour = BLACK;
            } else {
                t->colour = RED;
            }
        } else {
            t = left_rotate(t);
            t->colour = BLACK;
            t->left->colour = RED;
        }
        return t;
    }
    return t;
}

/*
 * Inserts a specified string into a specified tree.
 *
 * By comparing the keys of nodes of the tree to the string,
 * the method correctly navigates down the tree to find
 * the correct position for the string. If the string can be already
 * found in the tree, the frequency of the node at which the string is
 * found is incremented by one.
 *
 * @param t The tree in which to insert the string
 * @param str The string to be inserted
 * @return The new updated tree
 */
tree tree_insert(tree t, char *str, int isRoot) {
    while (t != NULL) {
        if (strcmp(t->key, str) < 0) {
            t->right = tree_insert(t->right, str, 0);
            t->right->parent = t;
            if (tree_type == RBT) {
                t = tree_fix(t);
            }
            return t;
        } else if(strcmp(t->key, str) > 0) {
            t->left = tree_insert(t->left, str, 0);
            t->left->parent = t;
            if (tree_type == RBT) {
                t= tree_fix(t);
            }
            return t;
        } else {
            t->frequency = t->frequency + 1;
            return t;
        }
    }
    t = emalloc(sizeof *t);
    t->key = emalloc((strlen(str) + 1) * sizeof str[0]);
    t->parent = emalloc(sizeof *t->parent);
    t->parent->key = NULL;
    strcpy(t->key, str);
    t->frequency = 1;
    t->left = NULL;
    t->right = NULL;
    if (tree_type == RBT) {
        t->colour = RED;
        t = tree_fix(t);
        if (isRoot == 1) {
            t->colour = BLACK;
        }
    }
    return t;
}

/*
 * Traverses a given tree in inorder and applies a given
 * function to each node in the tree.
 *
 * @param t Tree to be traversed
 * @param f Function to apply to each node in the tree
 * @return The new updated tree
 */
void tree_inorder(tree t, void f(char *str)) {
    if (t!= NULL) {
        if (t->left != NULL) {
            tree_inorder(t->left, f);
        }
        f(t->key);
        if (t->right != NULL) {
            tree_inorder(t->right, f);
        }
    }
    return;
}

/*
 * Traverses a given tree in preorder and applies a given
 * function to each node in the tree.
 *
 * @param t Tree to be traversed
 * @param f Function to apply to each node in the tree
 */
void tree_preorder(tree t, void f(int freq, char *str)) {
    if (t == NULL || t->key == NULL) {
        return;
    }
    f(t->frequency, t->key);
    tree_preorder(t->left, f);
    tree_preorder(t->right, f);
}

/*
 * Finds the tree depth of a given tree, where the tree depth is
 * the length of the longest path between the root node and the
 * furthest leaf node.
 * 
 * This function operates recursively, by setting the depth of the
 * current node to be one greater than the depth of its deepest child
 * node. Leaves in the tree have a depth of zero.
 *
 * @param t The tree for which the depth should be found
 * @return The depth found
 */
int tree_depth(tree t) {
    int left_depth;
    int right_depth;
    if (t->left == NULL && t->right == NULL) {
        return 0;
    }
    if (t->left == NULL) {
        return 1 + tree_depth(t->right);
    }
    if (t->right == NULL) {
        return 1 + tree_depth(t->left);
    }
    left_depth = tree_depth(t->left);
    right_depth = tree_depth(t->right);
    if (left_depth > right_depth) {
        return 1 + left_depth;
    } else {
        return 1 + right_depth;
    }
}

/*
 * Prints the colour of each node of the tree using preorder traversal.
 *
 * @param t The tree to be traversed
 */
void tree_colour_print(tree t) {
    if (IS_BLACK(t)) {
        printf("black: %s\n", t->key);
    } else {
        printf("red: %s\n", t->key);
    }
    if (t->left != NULL) {
        tree_colour_print(t->left);
    }
    if (t->right != NULL) {
        tree_colour_print(t->right);
    }
}

/*
 * Searches the given tree for a given string.
 * This function calls itself recursively in order to check nodes
 * where the string could possibly be located.
 * Returns 0 if the string is not found, and 1 if it is found.
 *
 * @param t The tree to be searched
 * @param str The string to search for
 *
 * @return Integer specifying whether the string was found (1)
 * or not (0)
 */

int tree_search(tree t, char *str) {
    if (t == NULL || t->key == NULL) {
        return 0;
    } else if (strcmp(t->key, str) > 0) {
        return tree_search(t->left, str);
    } else if(strcmp(t->key, str) < 0) {
        return tree_search(t->right, str);
    }
    return 1;
}

/*
 * Traverses the tree writing a DOT description about connections, and
 * possibly colours, to the given output stream.
 *
 * @param t The tree to output a DOT description of.
 * @param out The stream to write the DOT output to.
 */

static void tree_output_dot_aux(tree t, FILE *out) {
    if (t->key != NULL) {
        fprintf(out, "\"%s\"[label=\"{<f0>%s:%d|{<f1>|<f2>}}\"color=%s];\n",
            t->key, t->key, t->frequency,
            (RBT == tree_type && RED == t->colour) ? "red":"black");
    }
    if (t->left != NULL) {
        tree_output_dot_aux(t->left, out);
        fprintf(out, "\"%s\":f1 -> \"%s\":f0;\n", t->key, t->left->key);
    }
    if (t->right != NULL) {
        tree_output_dot_aux(t->right, out);
        fprintf(out, "\"%s\":f2 -> \"%s\":f0;\n", t->key, t->right->key);
    }
}

/*
 * Output a DOT description of this tree to the given output stream.
 * DOT is a plain text graph description language (see www.graphviz.org).
 * You can create a viewable graph with the command
 *
 *    dot -Tpdf < graphfile.dot > graphfile.pdf
 *
 * You can also use png, ps, jpg, svg... instead of pdf
 *
 * @param t The tree to output the DOT description of.
 * @param out The stream to write the DOT description to.
 */
void tree_output_dot(tree t, FILE *out) {
    fprintf(out, "digraph tree {\nnode [shape = Mrecord, penwidth = 2];\n");
    tree_output_dot_aux(t, out);
    fprintf(out, "}\n");
}

/*
 * Frees memory allocated for a given tree.
 *
 * The tree is right rotated by setting the root's left child to be the root
 * of the tree and setting the left child of the original root to be
 * the right child of the new root. We then set the right child of the new
 * root to the original root of the tree.
 *
 * @param t The tree to be right rotated
 * @return NULL
 */
tree tree_free(tree t) {
    if (t!= NULL) {
        t->left = tree_free(t->left);
        t->right = tree_free(t->right);
        free(t->key);
        free(t);
    }
    return NULL;
}
