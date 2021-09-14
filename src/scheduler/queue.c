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
  //Crear una cola para retornarla
  Process** cola_ordenada = calloc(cont_cola_procesos, sizeof(Process*)); 
  int cont_cola_ordenada = 0;
  int m = 0;
  
  mergeSort2(cola_procesos, 0, cont_cola_procesos-1);

  if (cont_cola_procesos == 1) 
  {
    //Llegar y meter
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
         int result = strcmp(cola_procesos[m]->name, cola_procesos[m+1]->name);
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
    }
    //Ahora, con la colaordenada lista, hay que agregar los elementos de colaordenana a la queue original.

     //1) Iterar sobre los procesos de cola procesos

    //2) Si proceso salió recién de la CPU (su start es menor que el time), se mete primero (indpte si estaba waiting o ready)
    //Y no se mete a revisar ese proceso en lo que viene abajo

    //4) si proceso->start == time, tiene prioridad 3 
    //5) si tienen el mismo time, se debe comparar el num de fabrica y elegir el con menor numero
    //6) si además de la misma fábrica, se debe comparar el largo del string con strcmp()
    

    

    //queue->end->next = current;
    //current->prev = queue->end;
    //queue->end = current;
  }

  free(cola_ordenada);
};