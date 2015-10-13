#include <stdio.h>
#include <stdlib.h>

#include <words_generator.h>

char random_char_gen()
{
  return ('a' + rand()%26);
}

char* random_word_gen(int maxWordSize)
{
  int rand_word_size = rand()%(maxWordSize+1);
  char* word = (char*)malloc(rand_word_size*sizeof(char));
  int i;
  for(i=0; i<rand_word_size-1; i++){
    word[i]=random_char_gen();
  }
  word[rand_word_size] = '\0';
  return word;
}

char** random_text_generator(int nbWords, int maxWordSize) 
{
  char** wordsArr = (char**)malloc(nbWords*sizeof(char*));
  int i;
  for(i=0; i<nbWords; i++)
    wordsArr[i] = random_word_gen(maxWordSize);
  return wordsArr;
}

void free_text(char** text, int nbWords)
{
  int i;
  for(i=0; i<nbWords; i++)
    free(text[i]);
  free(text);
}

void print_text(char** text, int nbWords)
{
  int i;
  for(i=0; i<nbWords; i++)
    printf("%s ", text[i]);
  printf("\n");
}
