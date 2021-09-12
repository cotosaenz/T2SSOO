#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

Queue* queue_init(){
  Queue* queue = malloc(sizeof(Queue));
  *queue = (Queue)
  {
    .n1 = 0,
    .n2 = 1,
    .n3 = 0,
    .n4 = 0,
    .f = 0, 
    .start = NULL,
    .end = NULL,
  };
  
  return queue;
};