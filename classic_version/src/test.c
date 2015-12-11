#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <words_generator.h>
#include <distributed_words_counter.h>

#define NB_WORDS      1024
#define MAX_WORD_SIZE   32

#define NB_THREADS 4

int main(int argc, char** argv)
{
  char** myText = random_text_generator(NB_WORDS, MAX_WORD_SIZE);
  print_text(myText, NB_WORDS);
  
  printf("\n\n\n---------------------------------------------------\n");

  super_count(myText, NB_THREADS, NB_WORDS, MAX_WORD_SIZE);

  free_text(myText, NB_WORDS);
  return EXIT_SUCCESS;
}
