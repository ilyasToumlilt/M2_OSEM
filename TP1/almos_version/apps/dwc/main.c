#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "words_generator.h"
#include "distributed_words_counter.h"

#define MAX_WORD_SIZE   32

int main(int argc, char** argv)
{
  /* nbWords en param */
  if( argc != 2 ) {
    perror("Usage: dwc <nbWords>\n");
    return 1;
  }
  int nbThreads = sysconf(_SC_NPROCESSORS_ONLN);
  int nbWords = atoi(argv[1]);
  struct dwc_bench* mult_bench;
  clock_t mono_time;

  printf("Starting dwc with:\n\t%d\tThreads\n\t%d\tWords\n\t%d\tMaxWordSize\n",
	 nbThreads, nbWords, MAX_WORD_SIZE);
  
  char** myText = random_text_generator(nbWords, MAX_WORD_SIZE);

  mono_time  = mono_count(myText, nbWords, MAX_WORD_SIZE);
  mult_bench = super_count(myText, nbThreads, nbWords, MAX_WORD_SIZE);

  printf("Mono Bench :\n\tmono_time=%f\n\tmulti_time=%f\n\ttotal=%f\n",
	 (float)mono_time, (float)0, (float)mono_time);
  printf("Multi Bench :\n\tmono_time=%f\n\tmulti_time=%f\n\ttotal=%f\n",
	 (float)mult_bench->mono_time, (float)mult_bench->multi_time,
	 (float)(mult_bench->mono_time + mult_bench->multi_time));
  printf("Speedup ==>%f\n",((float)mono_time/(float)(mult_bench->mono_time 
                                                    + mult_bench->multi_time)));

  free_text(myText, nbWords);
  free(mult_bench);
  printf("[MainDwc] Finished");
  
  return EXIT_SUCCESS;
}

/* TODO:
 * pages 4ko
 * compter le temps d'init
 */
