#ifndef __PreProcess__preprocess_attributes__
#define __PreProcess__preprocess_attributes__

#include <stdio.h>
#include <stdlib.h>
#include "filename.h"

#define cid_size 3

int preprocess_attributes(int start, int end);

int del_overrange_attributes(int start, int end);
int del_duplicate_attributes();
int del_dontmatch_attributes(int N);

void swap(int *pid_left, char *cid_left, int *pid_right, char *cid_right);
void quicksort(int *pid, char **cid, int left, int right);
void quicksort_attributes(int N);
int binary_search(int array[], int n, int value);

#endif /* defined(__PreProcess__preprocess_attributes__) */
