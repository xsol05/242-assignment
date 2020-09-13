/*
 * COSC242 Assignment 2020
 * Main function
 * Authors: Maaha Ahmad, Magdeline Huang, Kate Truman
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include "mylib.h"
#include "tree.h"

/* 
 * Prints the frequencies of the words
 *
 * @param freq The number of times the word appears
 * @param word The word of interest
 */
static void print_info(int freq, char *word) {
    printf("%-4d %s\n", freq, word);
}

/*
 * Deals with input based on the different option flags given which are 
 * -f, -o, -d, -r, -c, -h. Details are in the help message.
 *
 * In particular for the -c option flag, this program effectively performs
 * a basic spell check.
 * 
 * This program is implemented as a combination tree ADT which can be either an
 * ordinary binary search tree or a balanced red-black tree.
 *
 * This function will deallocate all memory before finishing. 
 *
 * @param argc The number of command line arguments
 * @param argv An array of strings that holds the command line arguments
 *
 * @return The exit status of the program, either EXIT_SUCCESS or EXIT_FALIURE
 */
int main(int argc, char **argv) {
    const char *output_file_name = "tree-view.dot";
    const char *input_file_name = NULL;
    enum tree_e tree_t = BST;
    const char *optstring = "f:odrc:h";
    char option;
    char word[256];
    int output_dot_file = 0;
    int print_tree_depth = 0;
    int empty_tree = 1;
    int unknown_words = 0;
    int file_missing = 0;
    FILE *document = NULL;
    tree t;
    clock_t fill_start, fill_end, search_start, search_end;
    double fill_time = 0.0;
    double search_time = 0.0;
    
    while ((option = getopt(argc, argv, optstring)) != EOF) {
        switch(option) {
            case 'f':
                output_file_name = optarg;
                break;
            case 'r':
                tree_t = RBT;
                break;
            case 'c':
                if (NULL == (document = fopen(optarg, "r"))) {
                    file_missing = 1;
                }
                input_file_name = optarg;
                break;
            case 'o':
                output_dot_file = 1;
                break;
            case 'd':
                print_tree_depth = 1;
                break;
            case 'h':
            default:
                fprintf(stderr, "Usage: %s [OPTION]... <STDIN>\n\n", argv[0]);
                fprintf(stderr, "Perform various operations using a binary ");
                fprintf(stderr, "tree. By default, words\nare read from stdin");
                fprintf(stderr, " and added to the tree, before being printed");
                fprintf(stderr, " out\nalongside their frequencies to stdout.");
                fprintf(stderr, "\n\n");
                fprintf(stderr, "-c FILENAME  Check spelling of words in ");
                fprintf(stderr, "FILENAME using words\n             ");
                fprintf(stderr, "read from stdin as the ");
                fprintf(stderr, "dictionary.  Print timing\n             ");
                fprintf(stderr, "info & unknown words to stderr ");
                fprintf(stderr, "(ignore -d & -o)\n");
                fprintf(stderr, "-d\t     Only print the tree depth ");
                fprintf(stderr, "(ignore -o)\n");
                fprintf(stderr, "-f FILENAME  Write DOT output to FILENAME ");
                fprintf(stderr, "(if -o given)\n");
                fprintf(stderr, "-o\t     Output the tree in DOT form to ");
                fprintf(stderr, "file 'tree-view.dot'\n");
                fprintf(stderr, "-r\t     Make the tree an RBT (the default");
                fprintf(stderr, " is a BST)\n\n");
                fprintf(stderr, "-h\t     Print this message\n\n");
                return(EXIT_SUCCESS);
                break;
        }
    }
    
    t = tree_new(tree_t);
    fill_start = clock();
    while (getword(word, sizeof word, stdin) != EOF) {
        t = tree_insert(t, word, empty_tree);
        empty_tree = 0;
    }
    fill_end = clock();
    fill_time = (fill_end - fill_start)/(double)CLOCKS_PER_SEC;
    if (empty_tree == 1) {
        return(EXIT_FAILURE);
    }
    if (file_missing == 1) {
        fprintf(stderr, "Can't open file '%s' using mode r.\n",
                input_file_name);
        input_file_name = NULL;
        return (EXIT_FAILURE);
    }
    if (input_file_name == NULL) {
        if (print_tree_depth == 1) {
            printf("%d\n", tree_depth(t));
        } else if (output_dot_file == 1) {
            printf("Creating dot file '%s'\n", output_file_name);
            tree_output_dot(t, fopen(output_file_name, "w"));
        } else {
            tree_preorder(t, print_info);
        }
    } else {
        search_start = clock();
        while (getword(word, sizeof word, document) != EOF) {
            if (tree_search(t, word) == 0) {
                fprintf(stdout, "%s\n", word);
                unknown_words++;
            }
        }
        fclose(document);
        search_end = clock();
        search_time = (search_end - search_start)/(double)CLOCKS_PER_SEC;
            
        fprintf(stderr, "Fill time     : %f\n", fill_time);
        fprintf(stderr, "Search time   : %f\n", search_time);
        fprintf(stderr, "Unknown words = %d\n", unknown_words);
    }
    t = tree_free(t);
    return EXIT_SUCCESS;
}
