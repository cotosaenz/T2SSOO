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
  int first;               // me dice si ya llego alguien al sistema 1 o 0 
  struct process* start;   // primero en la cola
  struct process* end;     // ultimo en la cola
  struct process* en_cpu;
} Queue;

Queue* queue_init();
int vaciar_cola_procesos(Process** cola_cprocesos);
void actualizar_f(Queue* queue);
void restar_numero_fabricas(int fabrica, Queue* queue);
void sumar_numero_fabricas(int fabrica, Queue* queue);
void desempatar(Process** cola_procesos, Queue* queue, int cont_cola_procesos, int time);
int calcular_quantum(Queue* queue, int Q, int fabrica);