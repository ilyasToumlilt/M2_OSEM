#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#include "distributed_words_counter.h"

struct dwc_t {
  int id;
  int* countArray;
};

/*******************************************************************************
 * Private Declarations
 ******************************************************************************/
struct dwc_t* alloc_dwcArray(int nbThreads, int maxWordSize);
void free_dwcArray(struct dwc_t* dwcArray, int nbThreads);
void reset_dwcArray(struct dwc_t* dwcArray, int nbThreads, int maxWordSize);
void print_dwcArray(struct dwc_t* dwcArray, int nbThreads, int maxWordSize);
void countWordsFromArray(struct dwc_t* dwcArray, char** text, int from, int to);
void merge_dwcArray(struct dwc_t* dwcArray, int nbThreads, int maxWordSize);
int countTotalWords(struct dwc_t* dwcArray, int maxWordSize);
void* threads_handler(void* dwc_p);
/*******************************************************************************
 * Local Implementations
 ******************************************************************************/
struct dwc_t* alloc_dwcArray(int nbThreads, int maxWordSize)
{
  struct dwc_t* ret = (struct dwc_t*)malloc(nbThreads*sizeof(struct dwc_t));
  int i;
  for(i=0; i<nbThreads; i++)
    ret[i].countArray = (int*)malloc(maxWordSize*sizeof(int));
  return ret;
}

void free_dwcArray(struct dwc_t* dwcArray, int nbThreads)
{
  int i;
  for(i=0; i<nbThreads; i++)
    free(dwcArray[i].countArray);
  free(dwcArray);
}

void reset_dwcArray(struct dwc_t* dwcArray, int nbThreads, int maxWordSize)
{
  int i;
  for(i=0; i<nbThreads; i++) {
    dwcArray[i].id = i;
    //memset((void*)dwcArray[i].countArray, 0, maxWordSize*sizeof(int));
  }
}

/* fonction plutot debug */
void print_dwcArray(struct dwc_t* dwcArray, int nbThreads, int maxWordSize)
{
  int i, j;
  for(i=0; i<nbThreads; i++) {
    printf("%d:\t", dwcArray[i].id);
    for(j=0; j<maxWordSize; j++)
      printf("%d\t", dwcArray[i].countArray[j]);
    printf("\n");
  }
}

void countWordsFromArray(struct dwc_t* dwcArray, char** text, int from, int to)
{
  int i;
  for(i=from; i<to; i++)
    dwcArray->countArray[strlen(text[i])]++;
}

void merge_dwcArray(struct dwc_t* dwcArray, int nbThreads, int maxWordSize)
{
  int i,j;
  for(i=0; i<maxWordSize; i++){
    for(j=1; j<nbThreads; j++){
      dwcArray[0].countArray[i] += dwcArray[j].countArray[i];
    }
  }
}

int countTotalWords(struct dwc_t* dwcArray, int maxWordSize)
{
  int i, ret = 0;
  for(i=0;i<maxWordSize;i++)
    ret += dwcArray[0].countArray[i];
  return ret;
}

int globalNbThreads;
int globalNbWords;
char** globalText;
void* threads_handler(void* dwc_p)
{
  
  countWordsFromArray(dwc_p, globalText, 
		      ((struct dwc_t*)dwc_p)->id * (globalNbWords/globalNbThreads),
		      ((((struct dwc_t*)dwc_p)->id)+1) * (globalNbWords/globalNbThreads));
  return NULL;
}

struct dwc_bench* super_count(char** text, int nbThreads, 
			      int nbWords, int maxWordSize)
{
  /* on a besoin de compter les temps de traitement pour les benchs */
  struct dwc_bench* ret = (struct dwc_bench*)malloc(sizeof(struct dwc_bench));
  ret->mono_time  = 0;
  ret->multi_time = 0;
  clock_t mono_clock = clock();
  
  /* init */
  pthread_t threadsArray[nbThreads-1];
  struct dwc_t* dwcArray = alloc_dwcArray(nbThreads, maxWordSize);
  reset_dwcArray(dwcArray, nbThreads, maxWordSize);
  
  /* variables globales pour les threads ... */
  globalNbThreads=nbThreads;
  globalNbWords=nbWords;
  globalText=text;

  clock_t mult_clock = clock();

  /* creation des threads */
  int i;
  for(i=0; i<nbThreads-1; i++){
    pthread_create(&(threadsArray[i]), NULL, threads_handler, (void*)(dwcArray+i));
  }

  ret->mono_time += ( clock() - mono_clock );
  
  /* le main travaille aussi ... */
  countWordsFromArray(dwcArray+i, text, i*(nbWords/nbThreads),nbWords-1);

  mono_clock = clock();

  /* attente de terminaison des threads */
  for(i=0; i<nbThreads-1; i++){
    if(pthread_join(threadsArray[i], NULL)){
      perror("Error: pthread_join");
      exit(1);
    }
  }

  ret->multi_time += ( clock() - mult_clock );

  /* merge des résultats */
  merge_dwcArray(dwcArray, nbThreads, maxWordSize);

  /* total */
  int total = countTotalWords(dwcArray, maxWordSize);

  free_dwcArray(dwcArray, nbThreads);

  ret->mono_time += ( clock() - mono_clock );

  return ret;
}

clock_t mono_count(char** text, int nbWords, int maxWordSize)
{
  clock_t time = clock();

  /* init */
  printf("YO\n");
  struct dwc_t* dwcArray = alloc_dwcArray(1, maxWordSize);
  printf("YO\n");
  reset_dwcArray(dwcArray, 1, maxWordSize);
  printf("YO\n");

  /* allez hop on compte */
  countWordsFromArray(dwcArray, text, 0, nbWords-1);
  printf("YO\n");

  int total = countTotalWords(dwcArray, maxWordSize);
  printf("YO\n");

  free_dwcArray(dwcArray, 1);
  printf("YO\n");

  return (clock() - time);
}

