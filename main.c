/*
 * COSC242 Assignment 2020
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
 * @param freq: The number of times the word appears
 * @param word: The word of interest
 */
static void print_info(int freq, char *word) {
    printf("%-4d %s\n", freq, word);
}

/*
 * Processes two groups of words and effectively performs a basic spell check.
 * The first group of words will be read from stdin and will be inserted into the tree.
 * The second group of words will be read from a file specified on the command line.
 * If any word read from the file is not contained in the tree then it should get printed to stdout.
 * This program is implemented as a combination tree ADT which can be either an
 * ordinary binary search tree or a balanced red-black tree.
 * This function will deallocate all memory before finishing. 
 *
 * @param argc: The number of command line arguments
 * @param argv: An array of strings that holds the command line arguments
 *
 * @return: The exit status of the program, either EXIT_SUCCESS or EXIT_FALIURE
 */
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
    clock_t fill_start, fill_end, search_start, search_end;
    double fill_time = 0.0;
    double search_time = 0.0;
    
    while ((option = getopt(argc, argv, optstring)) != EOF) {
        switch(option) {
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
                fprintf(stderr, "Usage: ./%s [OPTION]... <STDIN>\n", argv[0]);
                fprintf(stderr, "\n");
                fprintf(stderr, "Perform various operations using a binary tree.  By default, words\n");
                fprintf(stderr, "are read from stdin and added to the tree, before being printed out\n");
                fprintf(stderr, "alongside their frequencies to stdout.\n");
                fprintf(stderr, "\n");
                fprintf(stderr, "-c FILENAME  Check spelling of words in FILENAME using words\n");
                fprintf(stderr, "             read from stdin as the dictionary.  Print timing\n");
                fprintf(stderr, "             info & unknown words to stderr (ignore -d & -o)\n");
                fprintf(stderr, "-d           Only print the tree depth (ignore -o)\n");
                fprintf(stderr, "-f FILENAME  Write DOT output to FILENAME (if -o given)\n");
                fprintf(stderr, "-o           Output the tree in DOT form to file 'tree-view.dot'\n");
                fprintf(stderr, "-r           Make the tree an RBT (the default is a BST)\n");
                fprintf(stderr, "\n");
                fprintf(stderr, "-h           Print this message\n");
                fprintf(stderr, "\n");
                return(EXIT_SUCCESS);
                break;
        }
    }
    
    t = tree_new(tree_t);
    
    fill_start = clock();
    while (getword(word, sizeof word, stdin) != EOF) {
        t = tree_insert(t, word);
    }
    fill_end = clock();
    fill_time = (fill_end - fill_start)/(double)CLOCKS_PER_SEC;
    
    if (inputFilename == NULL) {
        if (print_tree_depth == 1) {
            printf("%d\n", tree_depth(t));
        }
        else if (output_dot_file == 1){
            printf("Creating dot file '%s'\n", outputFilename);
            tree_output_dot(t, fopen(outputFilename, "w"));
        }
        else {
            tree_preorder(t, print_info);
        }
    } else {
        search_start = clock();
        while (getword(word, sizeof word, document) != EOF) {
            if (tree_search(t, word) == 0){
                fprintf(stdout, "%s\n", word);
                unknownWords++;
            }
        }
        fclose(document);
        search_end = clock();
        search_time = (search_end - search_start)/(double)CLOCKS_PER_SEC;
            
        fprintf(stderr, "Fill time     : %f\n", fill_time);
        fprintf(stderr, "Search time   : %f\n", search_time);
        fprintf(stderr, "Unknown words = %d\n", unknownWords);
    }
    
    t = tree_free(t);
    return EXIT_SUCCESS;
}
