struct dwc_bench {
  clock_t mono_time;  /* temps passé dans la partie séquentielle */
  clock_t multi_time; /* temps passé dans la partie parallèle */
};

struct dwc_bench* super_count(char** text, int nbThreads, 
			      int nbWords, int maxWordSize);
clock_t mono_count(char** text, int nbWords, int maxWordSize);
