#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mylib.h"
#include "tree.h"
#include <getopt.h>

static void print_info(int freq, char *word){
    printf("%-4d %s\n", freq, word);
}

int main(int argc, char **argv) {
    const char *outputFilename = "tree-view.dot";
    const char *inputFilename = NULL;
    enum tree_t tt = BST;
    const char *optstring = "f:odrc:h";
    char option;
    tree t = tree_new(tt);
    char word[256];
    int output_dot_file = 0;
    int print_tree_depth = 0;
    int unknownWords = 0;
    FILE *document = NULL;
    while ((option = getopt(argc, argv, optstring)) != EOF){
        switch(option){
            case 'f':
                outputFilename = optarg;
                break;
            case 'r':
                tt = RBT;
                break;
            case 'c':
                document = fopen(optarg, "r");
                inputFilename = optarg;
                break;
            case 'o':
                output_dot_file = 1;
                break;
            case 'd':
                print_tree_depth = 1;
                break;
            case 'h':
            default:
                printf("-------------\nRun the default program using the command './tree'.\nUse command line arguments as follows:\n");
                printf("Use '-c filename' in order to check the spelling of words in 'filename' using words read from stdin as the dictionary. Unknown words, timing information and unknown count are printed to stderr.\n");
                printf("Use '-d' to solely print the depth of the tree.\n");
                printf("Use '-f filename' to write the 'dot' output to filename instead of the default file name if '-o' is also given.\n");
                printf("Use '-o' to output a representation of the tree in 'dot' form to the file 'tree-view.dot' using the functions given in output-dot.txt.\n");
                printf("Use '-r' to make the tree an rbt instead of the default bst..\n");
                printf("Use '-h' to print a help message describing how to use the program.\n");
                break;

        }
    }
    while (getword(word, sizeof word, stdin) != EOF) {
        t = tree_insert(t, word);
    }
    if (inputFilename == NULL){
        if (print_tree_depth == 1){
            printf("Depth: %d\n", tree_depth(t));
        }
        else if (output_dot_file == 1){
            tree_output_dot(t, fopen(outputFilename, "w"));
        }
        else {
            tree_preorder(t, print_info);
        }
    }
    else {
        while (getword(word, sizeof word, document) != EOF) {
            if (tree_search(t,word) == 0){
                printf("%s\n", word);
                unknownWords++;
            }
        }
        printf("Unknown words: %d\n", unknownWords);
    }
    t = tree_free(t);
    return EXIT_SUCCESS;
}






