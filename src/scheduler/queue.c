#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include <string.h>

Queue* queue_init(){
  Queue* queue = malloc(sizeof(Queue));
  *queue = (Queue)
  {
    .n1 = 0,
    .n2 = 0,
    .n3 = 0,
    .n4 = 0,
    .f = 0, 
    .first = 0,
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

void desempatar(Process** cola_procesos, Queue* queue, int cont_cola_procesos, int time){
  //Crear una cola que guarde los procesos ordenados segun su porioridad
  Process** cola_ordenada = calloc(cont_cola_procesos, sizeof(Process*)); 
  int cont_cola_ordenada = 0;
  int m = 0;
  int result;
  Process* current = NULL;
  
  mergeSort2(cola_procesos, 0, cont_cola_procesos-1);

  if (cont_cola_procesos == 1) 
  {
    //Llegar y meter
    if (!queue->first)
    {
      queue->first = 1;
      queue->start = cola_procesos[0];
      queue->end = cola_procesos[0];
      cola_procesos[0]->state = "READY";
    }
    else
    {
      if (!cola_procesos[0]->estoy)
      {
        queue->end->next = cola_procesos[0];
        cola_procesos[0]->prev = queue->end;
        queue->end = cola_procesos[0];
        cola_procesos[0]->state = "READY";
      }
    }
  }

  else
  {
    for (int i = 0; i < cont_cola_procesos; i++)
    {
      if (cola_procesos[i]->start < time) //Revisamos si el proceso salió de la CPU
      {
        cola_ordenada[cont_cola_ordenada] = cola_procesos[i];
        cont_cola_ordenada += 1;
      }
    }

    while (m < cont_cola_procesos)
    {
      if ((cola_procesos[m]->start == time) && (m+1 < cont_cola_procesos)) //Revisamos si es un proceso nuevo
      {
        if (cola_procesos[m]->fabrica == cola_procesos[m+1]->fabrica)
        {
          result = strcmp(cola_procesos[m]->name, cola_procesos[m+1]->name);
          if (result < 0)
          {
            cola_ordenada[cont_cola_ordenada] = cola_procesos[m];
            cola_ordenada[cont_cola_ordenada] = cola_procesos[m+1];
            cont_cola_ordenada += 2;
            m += 2;
          }
          else 
          {
            cola_ordenada[cont_cola_ordenada] = cola_procesos[m+1];
            cola_ordenada[cont_cola_ordenada] = cola_procesos[m];
            cont_cola_ordenada += 2;
            m += 2;
          }
        }
        else
        {
          cola_ordenada[cont_cola_ordenada] = cola_procesos[m];
          cont_cola_ordenada += 1;
          m += 1;
        } 
      }
      else if ((cola_procesos[m]->start == time) && (m+1 == cont_cola_procesos))
      {
        cola_ordenada[cont_cola_ordenada] = cola_procesos[m];
        cont_cola_ordenada += 1;
        m += 1;
      }
      else
      {
        m += 1; 
      }
    }
    //Ahora, con cola_ordenada lista, hay que agregar los elementos de cola_ordenana a la queue original.
    cont_cola_ordenada = 0;
    if (!queue->first)
    {
      //Llegar y meter
      queue->start = cola_ordenada[0];
      queue->end = cola_ordenada[0];
      queue->first = 1;
      cont_cola_ordenada += 1;
      while (cont_cola_ordenada < cont_cola_procesos)
      {
        current = cola_ordenada[cont_cola_ordenada];
        queue->end->next = current;
        current->prev = queue->end;
        queue->end = current;
        cont_cola_ordenada += 1;
      }
    }
    else
    {
      while (cont_cola_ordenada < cont_cola_procesos)
      {
        current = cola_ordenada[cont_cola_ordenada];
        queue->end->next = current;
        current->prev = queue->end;
        queue->end = current;
        cont_cola_ordenada += 1;
      }
    }
  }

  free(cola_ordenada);
};

int calcular_quantum(Queue* queue, int Q, int fabrica)
{
  int quantum;
  int denom;
  int ni;
  if (fabrica == 1)
  {
    ni = queue->n1;
  }
  else if (fabrica == 2)
  {
    ni = queue->n2;
  }
  else if (fabrica == 3)
  {
    ni = queue->n3;
  }
  else if (fabrica == 4)
  {
    ni = queue->n4;
  }
  denom = (ni*queue->f);
  quantum = Q/denom;
  return quantum;
};

void actualizar_tiempos(Queue* queue, int time){
  Process* current = queue->start;
  while (current)
  {
    if (strcmp(current->state, "WAITING") == 0)
    {
      current->array_rafagas[current->contador_rafagas] -= 1;
      if (current->array_rafagas[current->contador_rafagas] == 0)
      {
        current->contador_rafagas += 1;
        current->rafaga_next = 0;
        current->state = "READY";
        printf("[t = %i] El proceso %s ha pasado a estado READY!\n", time, current->name);
      }
    }
    else if (strcmp(current->state, "READY") == 0)
    {
      current->waiting_time += 1;
    }
    current = current->next;
  }
};