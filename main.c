#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mylib.h"
#include "tree.h"
#include <getopt.h>
#include <time.h>

static void print_info(int freq, char *word){
    printf("%-4d %s\n", freq, word);
}

int main(int argc, char **argv) {
    const char *outputFilename = "tree-view.dot";
    const char *inputFilename = NULL;
    enum tree_e tree_t = BST;
    const char *optstring = "f:odrc:h";
    char option;
    char word[256];
    int output_dot_file = 0;
    int print_tree_depth = 0;
    int unknownWords = 0;
    FILE *document = NULL;
    tree t;
    /*timing info */
    clock_t fill_start, fill_end, search_start, search_end;
    double fill_time = 0.0;
    double search_time = 0.0;
    /* end timing info */
    
    while ((option = getopt(argc, argv, optstring)) != EOF){
        switch(option){
            case 'f':
                outputFilename = optarg;
                break;
            case 'r':
                tree_t = RBT;
                break;
            case 'c':
                if (NULL == (document = fopen(optarg, "r"))) {
                    fprintf(stderr, "Can't open file '%s' using mode r.\n", optarg);
                    return EXIT_FAILURE;
                }
                else {
                    document = fopen(optarg, "r");
                    inputFilename = optarg;
                }
                break;
            case 'o':
                output_dot_file = 1;
                break;
            case 'd':
                print_tree_depth = 1;
                break;
            case 'h':
            default:
                printf("Usage: %s [OPTION]... <STDIN>\n", argv[0]);
                printf("\n");
                printf("Perform various operations using a binary tree.  ");
                printf("By default, words\nare read from stdin and added");
                printf(" to the tree, before being printed out\n");
                printf("alongside their frequencies to stdout.\n\n");
                printf("-c FILENAME\tCheck spelling of words in FILENAME");
                printf(" using words\n\t\tread from stdin as the dictionary.");
                printf("Print timing\n\t\tinfo & unknown words to stderr ");
                printf("(ignore -d & -o)\n-d\t\tOnly print the tree depth ");
                printf("(ignore -o)\n-f FILENAME\tWrite DOT output to ");
                printf("FILENAME (if -o given)\n");
                printf("-o\t\tOutput the tree in DOT form to file ");
                printf("'tree-view.dot'\n");
                printf("-r\t\tMake the tree an RBT (the default is a BST)");
                printf("\n\n-h\t\tPrint this message\n\n");
                return(EXIT_SUCCESS);
                break;
        }
    }
    t = tree_new(tree_t);
    /* timing info for fill */
    fill_start = clock();

    while (getword(word, sizeof word, stdin) != EOF) {
        t = tree_insert(t, word);
    }
    fill_end = clock();
    fill_time = (fill_end - fill_start)/(double)CLOCKS_PER_SEC;
    /* end timing info for fill */
    /*fclose(document);*/
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
        /* timing info for search */
        search_start = clock();
        while (getword(word, sizeof word, document) != EOF) {
            if (tree_search(t, word) == 0){
                fprintf(stdout, "%s\n", word);
                unknownWords++;
            }
        }
        search_end = clock();
        search_time = (search_end - search_start)/(double)CLOCKS_PER_SEC;
        /*end timing info for search */
            
        /* printing info */
        fprintf(stderr, "Fill time     : %f\n", fill_time);
        fprintf(stderr, "Search time   : %f\n", search_time);
        fprintf(stderr, "Unknown words = %d\n", unknownWords);
        /* end printing info */
    }
    t = tree_free(t);
    return EXIT_SUCCESS;
}
