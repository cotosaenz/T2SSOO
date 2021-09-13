#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

Queue* queue_init(){
  Queue* queue = malloc(sizeof(Queue));
  *queue = (Queue)
  {
    .n1 = 0,
    .n2 = 0,
    .n3 = 0,
    .n4 = 0,
    .f = 0, 
    .start = NULL,
    .end = NULL,
    .en_cpu = NULL
  };
  
  return queue;
};

int vaciar_cola_procesos(Process** cola_procesos){
  for (int i=0; i<8; i++)
  {
    cola_procesos[i] = NULL;
  }
  return 0;
};

void actualizar_f(Queue* queue){
  queue->f = 0;
  if (queue->n1 != 0)
  {
    queue->f += 1;
  }
  if (queue->n2 != 0)
  {
    queue->f += 1;
  }
  if (queue->n3 != 0)
  {
    queue->f += 1;
  }
  if (queue->n4 != 0)
  {
    queue->f += 1;
  }
};

void restar_numero_fabricas(int fabrica, Queue* queue){
  if (fabrica == 1)
  {
    queue->n1 -= 1;
  }
  else if (fabrica == 2)
  {
    queue->n2 -= 1;
  }
  else if (fabrica == 3)
  {
    queue->n3 -= 1;
  }
  else if (fabrica == 4)
  {
    queue->n4 -= 1;
  }
  actualizar_f(queue);
};

void sumar_numero_fabricas(int fabrica, Queue* queue){
  if (fabrica == 1)
  {
    queue->n1 += 1;
  }
  else if (fabrica == 2)
  {
    queue->n2 += 1;
  }
  else if (fabrica == 3)
  {
    queue->n3 += 1;
  }
  else if (fabrica == 4)
  {
    queue->n4 += 1;
  }
  actualizar_f(queue);
};

void desempatar(Process** cola_procesos, Queue* queue, int cont_cola_procesos, int time)
{
  if (cont_cola_procesos == 1)
  {
    queue->end->next = current;
    current->prev = queue->end;
    queue->end = current;
  }
  else
  {
    
  }
};