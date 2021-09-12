#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct process
{
  char* name;            // nombre del proceso
  int start;             // tiempo de inicio
  int fabrica;           // fabrica del proceso
  int n_rafagas;           // total de rafagas del proceso A-B
  int contador_rafagas;   // me dice la rafaga que toca para el siguiente turno
  int* array_rafagas;    // arreglo con todas las rafagas del proceso, A-B
  char* state;           // me dice el estado del proceso, READY, RUNNING, WAITING o FINISHED
  int pendiente;         // variable que guarda el tiempo que faltaba en la rafaga Ai cuando se interrumpe el proceso en la CPU
  struct process* next;  // dice quien es el que le sigue en la cola (lista ligada)
  struct process* prev;  // dice quien es el que va antes que el en la cola 
  int elegido;           // numero de veces que el proceso fue elegido para usar la CPU
  int interrumpido;      // numero de veces que el proceso fue interrumpido pq se acabo su quantum
  int turnaround_time;    // tiempo que esta en el sistema, desde que llega hasta que acaba 
  int response_time;     // tiempo desde que llega al sistema hasta que es atendido
  int waiting_time;      // suma de tiempo entre WAITING y READY  
} Process;

Process* process_init(char* name, int start, int fabrica, int n_rafagas, int* array_rafagas);
void mergeSort(Process** procesos, int l, int r);
void merge(Process** procesos, int l, int m, int r); 