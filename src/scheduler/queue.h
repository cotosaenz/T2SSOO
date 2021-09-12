#include <stdio.h>
#include <stdlib.h>
#include "process.h"

typedef struct queue
{
  int n1;                  // numero de procesos en la cola de la fabrica 1
  int n2;     
  int n3;
  int n4;
  int f;                   // cantidad de fabricas con al menos 1 proceso en ejecucion
  struct process* start;   // primero en la cola
  struct process* end;     // ultimo en la cola
} Queue;

Queue* queue_init();