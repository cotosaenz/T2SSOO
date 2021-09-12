#include <stdio.h>
#include <stdlib.h>
#include "process.h"

Process* process_init(char* name, int start, int fabrica, int n_rafagas, int* array_rafagas){
  Process* process = malloc(sizeof(Process));
  *process = (Process)
  {
    .name = name,
    .start = start,
    .fabrica = fabrica,
    .n_rafagas = n_rafagas,
    .contador_rafagas = 0,
    .array_rafagas = array_rafagas,
    .state = "READY",
    .pendiente = 0,
    .next = NULL,
    .prev = NULL,
    .elegido = 0,
    .interrumpido = 0,
    .turnaround_time = 0,
    .response_time = 0,
    .waiting_time = 0
  };
  return process;
};

// merge y mergeSort obtenidos de https://www.geeksforgeeks.org/merge-sort/
void merge(Process** procesos, int l, int m, int r)
{
  int i, j, k;
  int n1 = m - l + 1;
  int n2 = r - m;
  Process* L[n1];
  Process* R[n2];
  for (i = 0; i < n1; i++)
  {
    L[i] = procesos[l + i];
  }
  for (j = 0; j < n2; j++)
  {
    R[j] = procesos[m + 1 + j];
  }
  i = 0; 
  j = 0; 
  k = l; 
  while (i < n1 && j < n2) {
    if (L[i]->start <= R[j]->start) {
      procesos[k] = L[i];
      i++;
    }
    else {
      procesos[k] = R[j];
      j++;
    }
    k++;
  }
  while (i < n1) {
    procesos[k] = L[i];
    i++;
    k++;
  }
  while (j < n2) {
    procesos[k] = R[j];
    j++;
    k++;
  }
}

void mergeSort(Process** procesos, int l, int r)
{
  if (l < r) {
    int m = l + (r - l) / 2;
    mergeSort(procesos, l, m);
    mergeSort(procesos, m + 1, r);
    merge(procesos, l, m, r);
  }
}