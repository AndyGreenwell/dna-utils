// Copyright 2013 Calvin Morrison
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kmer_utils.h"

long position = 0;

int main(int argc, char **argv) {

  char *line = NULL;
  long kmer = 6;
  size_t len = 0;
  ssize_t read;
  unsigned long width = 0;

  if(argc != 3) {
    printf("Please supply a filename and a kmer\n");
    exit(EXIT_FAILURE);
  }

  FILE *fh = fopen(argv[1], "r" );
  if(fh == NULL) {
    fprintf(stderr, "Error opening %s - %s\n", argv[1], strerror(errno));
    exit(EXIT_FAILURE);
  }

  width = (int)pow(4, kmer);

  while ((read = getline(&line, &len, fh)) != -1) {
    if(line[0] != '>' && read > kmer) {

      unsigned int i = 0;
      unsigned long long *counts = malloc((width+ 1) * sizeof(unsigned long long));
      if(counts == NULL) 
        exit(EXIT_FAILURE);

      convert_kmer_to_num(line, read);

      for(i = 0; i < read - kmer; i++) {
        counts[num_to_index(&line[i],kmer, width)]++;
      }

      unsigned long total = 0;
      for(i = 0; i < width; i++)
        total += counts[i];

      for(i = 0; i < width - 1; i++)
        printf("%.12f\t", (double)counts[i] / total);
      printf("%.12f\n", (double)counts[width - 1] / total);

      free(counts);
    }
  }

  free(line);


  return EXIT_SUCCESS;
}
