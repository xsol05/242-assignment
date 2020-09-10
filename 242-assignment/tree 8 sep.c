#include "tree.h"
#include "mylib.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
typedef enum { RED, BLACK} tree_colour;
enum tree_t tree_type;
struct tree_node {
    char *key;
    tree_colour colour;
    tree left;
    tree right;
    int frequency;
};
#define IS_BLACK(x) ((NULL==(x)) || (BLACK == (x)->colour))
#define IS_RED(x) ((NULL != (x)) && (RED == (x)->colour))
tree tree_free(tree t){
    if (t!= NULL){
        t->left = tree_free(t->left);
        t->right = tree_free(t->right);
        free(t->key);
        free(t);
    }
    return NULL;
}
tree tree_delete(tree t, char *str){
    if (t== NULL){
        return t;
    }
    if (strcmp(t->key, str) < 0){
        t->right = tree_delete(t->right, str);
        return t;
    }
    else if (strcmp(t->key, str) > 0){
        t->left = tree_delete(t->left, str);
        return t;
    }
    else {
        if (t->left == NULL && t->right == NULL){
            tree_free(t);
            return NULL;
        }
        else if(t->left != NULL && t->right != NULL){
            tree c = t->right;
            tree temp = NULL;
            while (c->left != NULL){
                c = c->left;
            }
            temp = t;
            strcpy(t->key, c->key);
            strcpy(c->key, temp->key);
            t->right = tree_delete(t->right, c->key);
            return t;
        }
        else {
            if (t->left == NULL){
                return t->right;
            }
            else {
                return t->left;
            }
        }
    }
}
void tree_inorder(tree t, void f(char *str)){
    if (t!= NULL){
        if (t->left != NULL){
            tree_inorder(t->left, f);
        }
        f(t->key);
        if (t->right != NULL){
            tree_inorder(t->right, f);
        }
    }
    return;
 
}

static tree right_rotate(tree t){
    tree temp = t;
    t = t->left;
    temp->left = t->right;
    t->right = temp;
    return t;
}


static tree left_rotate(tree t){
    tree temp = t;
    t = t->right;
    temp->right = t->left;
    t->left = temp;
    return t;
}

static tree tree_fix(tree t){
    if (IS_RED(t->left) && IS_RED(t->left->left)){
        if (IS_RED(t->right)){
            t->colour = RED;
            t->left->colour = BLACK;
            t->right->colour = BLACK;
        }
        else {
            t = right_rotate(t);
            t->colour = BLACK;
            t->right->colour = RED;
        }
        return t;
    }
    if (IS_RED(t->left) && IS_RED(t->left->right)){
        if(IS_RED(t->right)){
            t->colour = RED;
            t->left->colour = BLACK;
            t->right->colour = BLACK;
        }
        else {
            t->left = left_rotate(t->left);
            t = right_rotate(t);
            t->colour = BLACK;
            t->right->colour = RED;
        }
        return t;
    }
    if (IS_RED(t->right) && IS_RED(t->right->left)){
        if(IS_RED(t->left)){
            t->colour = RED;
            t->left->colour = BLACK;
            t->right->colour = BLACK;
        }
        else {
            t->right = right_rotate(t->right);
            t = left_rotate(t);
            t->colour = BLACK;
            t->left->colour = RED;
        }
        return t;
    }
    if (IS_RED(t->right) && IS_RED(t->right->right)){
        if(IS_RED(t->left)){
            t->colour = RED;
            t->left->colour = BLACK;
            t->right->colour = BLACK;
        }
        else {
            t = left_rotate(t);
            t->colour = BLACK;
            t->left->colour = RED;
        }
        return t;
    }
    return t;
}

/*int tree_depth(tree t){
    return 0;
} */

tree tree_insert(tree t, char *str){
    while (t != NULL){
        printf("not null");
        if(strcmp(t->key, str) < 0){
            t->right = tree_insert(t->right, str);
            t = tree_fix(t);
            return t;
        }
        else if(strcmp(t->key, str) > 0){
            t->left = tree_insert(t->left, str);
            t = tree_fix(t);
            return t;
        }
        else {
            return NULL;
        }
    }
    t = emalloc(sizeof *t);
    t->key = emalloc((strlen(str) + 1) * sizeof str[0]);
    printf("emalloced key\n");
    strcpy(t->key, str);
    printf("%s\n",t->key);
    t->left = NULL;
    t->right = NULL;
    /*if(tree_type == RBT){
        printf("BST\n"); */
        t->colour = RED;
        t = tree_fix(t);
    /*}*/
    return t;
}

tree tree_new(/*tree_t tt*/){
    /*if(tt == RBT){
        tree_type = RBT;
    }
    else {
        tree_type = BST;
    }*/
    return NULL;
}


void tree_preorder(tree t, void f(char *str)){
    if (t->key == NULL){
        return;
    }
    f(t->key);
    tree_preorder(t->left, f);
    tree_preorder(t->right, f);
}


void tree_colour_print(tree t){
        if (IS_BLACK(t)){
            printf("black: %s\n", t->key);
        }
        else {
            printf("red: %s\n", t->key);
        }
        if(t->left != NULL){
            tree_colour_print(t->left);
        }
        if(t->right != NULL){
            tree_colour_print(t->right);
        }
       
}


int tree_search(tree t, char *str){
    if (t->key == NULL){
        return 0;
    }
    else if(strcmp(t->key, str) > 0){
        tree_search(t->left, str);
    }
    else if(strcmp(t->key, str) < 0){
        tree_search(t->right, str);
    }
    return 1;
}

/**
 * Traverses the tree writing a DOT description about connections, and
 * possibly colours, to the given output stream.
 *
 * @param t the tree to output a DOT description of.
 * @param out the stream to write the DOT output to.
 */
/*
static void tree_output_dot_aux(tree t, FILE *out) {
   if(t->key != NULL) {
      fprintf(out, "\"%s\"[label=\"{<f0>%s:%d|{<f1>|<f2>}}\"color=%s];\n", t->key, t->key, t->frequency, (tree == tree_type && RED == t->colour) ? "red":"black");
   }
   if(t->left != NULL) {
      tree_output_dot_aux(t->left, out);
      fprintf(out, "\"%s\":f1 -> \"%s\":f0;\n", t->key, t->left->key);
   }
   if(t->right != NULL) {
      tree_output_dot_aux(t->right, out);
      fprintf(out, "\"%s\":f2 -> \"%s\":f0;\n", t->key, t->right->key);

}
*/

/**
 * Output a DOT description of this tree to the given output stream.
 * DOT is a plain text graph description language (see www.graphviz.org).
 * You can create a viewable graph with the command
 *
 *    dot -Tpdf < graphfile.dot > graphfile.pdf
 *
 * You can also use png, ps, jpg, svg... instead of pdf
 *
 * @param t the tree to output the DOT description of.
 * @param out the stream to write the DOT description to.
 */
/*
void tree_output_dot(tree t, FILE *out) {
   fprintf(out, "digraph tree {\nnode [shape = Mrecord, penwidth = 2];\n");
   tree_output_dot_aux(t, out);
   fprintf(out, "}\n");
}
*/
