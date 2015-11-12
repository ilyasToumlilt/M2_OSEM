#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "words_generator.h"
#include "distributed_words_counter.h"

#define MAX_WORD_SIZE   32

int main(int argc, char** argv)
{
  /* nbThreads en param */
  if( argc != 3 ) {
    perror("Usage: dwc <nbThreads> <nbWords>\n");
    return 1;
  }
  int nbThreads = atoi(argv[1]);
  int nbWords = atoi(argv[2]);

  printf("Starting dwc with:\n\t%d\tThreads\n\t%d\tWords\n\t%d\tMaxWordSize\n",
	 nbThreads, nbWords, MAX_WORD_SIZE);
  
  char** myText = random_text_generator(nbWords, MAX_WORD_SIZE);
  //print_text(myText, NB_WORDS);
  
  //printf("\n\n\n---------------------------------------------------\n");

  super_count(myText, nbThreads, nbWords, MAX_WORD_SIZE);

  free_text(myText, NB_WORDS);
  return EXIT_SUCCESS;
}

/* TODO:
 * pages 4ko
 * compter le temps d'init
 */
