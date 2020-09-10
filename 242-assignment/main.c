#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mylib.h"
#include "tree.h"


static void print_info(int freq, char *word){
    printf("%-4d %s\n", freq, word);
}

int main(void) {
    enum tree_t tt = BST;
    tree t = tree_new(tt);
    char word[256];
    while (getword(word, sizeof word, stdin) != EOF) {
        t = tree_insert(t, word);
    }
    printf("Depth: %d\n", tree_depth(t));
    tree_preorder(t, print_info);
    tree_colour_print(t);
    tree_output_dot(t, fopen("tree-view.dot", "w"));
    /* printf("--------\n");
       b = rbt_delete(b, "sentence"); */
    /*    rbt_inorder(b,print_key); */
    t = tree_free(t);
    return EXIT_SUCCESS;
}






