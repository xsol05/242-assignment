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
                printf("\n\n");
                printf("Run the default program using the command './tree'.\n");
                printf("Use command line arguments as follows:\n");
                printf("Use '-c filename' in order to check the spelling of ");
                printf("words in 'filename'. Use words read from stdin as ");
                printf("the dictionary. Unknown words, timing information ");
                printf("and unknown count are printed to stderr.\n");
                printf("Use '-d' to solely print the depth of the tree.\n");
                printf("Use '-f filename' to write the 'dot' output to ");
                printf("filename instead of the default file name if '-o' ");
                printf("is also given.\n");
                printf("Use '-o' to output a representation of the tree in ");
                printf("'dot' form to the file 'tree-view.dot' using the ");
                printf("functions given in output-dot.txt.\n");
                printf("Use '-r' to make the tree an rbt instead of the ");
                printf("default bst..\n");
                printf("Use '-h' to print a help message describing how to ");
                printf("use the program.\n");
                break;
        }
    }
    
    if (optind < argc){
        if (NULL == (document = fopen(argv[optind], "r"))){
            fprintf(stderr, "Couldn't open file so exit program.\n");
            exit(EXIT_FAILURE);
        } else{
            /* timing info for fill */
            fill_start = clock();
            t = tree_new(tree_t);
            while (getword(word, sizeof word, stdin) != EOF) {
                t = tree_insert(t, word);
            }
            fill_end = clock();
            fill_time = (fill_end - fill_start)/(double)CLOCKS_PER_SEC;
            /* end timing info for fill */
        }
        fclose(document);
            
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
    }    
      
    t = tree_free(t);
    return EXIT_SUCCESS;
}
