#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../file_manager/manager.h"
#include "queue.h"

int main(int argc, char **argv)
{
  // creamos el parametro Q, en caso de recibirlo se asigna ese, si no lo entregan, queda por defecto en 100
  int Q;
  if (argc == 3)
  {
    Q = 100;
  }
  else
  {
    Q = atoi(argv[3]);
  }

  InputFile *file = read_file("input.txt");
  char* output_file = argv[2];
  // creamos la cola y un arreglo con todos los procesos
  printf("Reading file of length %i:\n", file->len);
  int N = file->len;
  Queue* queue = queue_init();
  printf("%i\n", queue->n1);
  Process** procesos = calloc(N, sizeof(Process*));
  Process** procesos_ordenada = calloc(N, sizeof(Process*));
  int cont_procesos = 0;

  // instanciamos a todos los procesos y los guardamos en un calloc

  for (int i = 0; i < N; i++)
  {
    char **line = file->lines[i];
    char* name = line[0];
    int start = atoi(line[1]);
    int fab = atoi(line[2]);
    int n_rafagas = atoi(line[3]);
    int final = (n_rafagas*2)-1;
    int* rafagas = calloc(final, sizeof(int));
    for (int j=0; j<final; j++)
    {
      rafagas[j] = atoi(line[4+j]);
    }
    printf(
        "\tProcess %s from factory %i has init time of %i and %i bursts.\n",
        name, fab, start, n_rafagas);
    Process* p = process_init(name, start, fab, n_rafagas, rafagas);         // creamos el proceso y lo guardamos en en el arreglo procesos
    procesos[i] = p;
    procesos_ordenada[i] = p;
  }

  //ordenamos el arreglo procesos_ordenada segun su otiempo de inicio en el sistema
  mergeSort(procesos_ordenada, 0, N-1);

  // ahora empieza a funcionar el scheduler
  printf("si %s\n", procesos_ordenada[N-1]->name);
  int time = 0;
  Process* keep = NULL;
  Process* otro = NULL;
  Process* nuevo_start = NULL;
  Process** cola_procesos = calloc(8, sizeof(Process*));
  int cont_cola_procesos = 0;
  Process* current = NULL;
  while((strcmp(procesos_ordenada[N-1]->state, "FINISHED") != 0)) // hay que sacar lo ultimo dsps, es solo con el FINISHED del ultimo proceso
  {
    // primero revisamos si llegan a la cola procesos nuevos
    while (cont_procesos < N && procesos_ordenada[cont_procesos]->start == time)
    {
      cola_procesos[cont_cola_procesos] = procesos_ordenada[cont_procesos]; //Se agrega a la cola procesos el proceso que llega por primera vez
      printf("[t = %i] El proceso %s ha sido creado.\n", time, cola_procesos[cont_cola_procesos]->name);
      sumar_numero_fabricas(cola_procesos[cont_cola_procesos]->fabrica, queue);
      cola_procesos[cont_cola_procesos]->turnaround_time = time;
      cola_procesos[cont_cola_procesos]->response_time = time;
      cont_cola_procesos += 1;
      cont_procesos += 1;
    }


    // aca revisamos que pasa con el proceso que estaba RUNNING en la CPU, si se acaba o vuelve a la cola, y en que estado
    if (queue->en_cpu)
    {
      current = queue->en_cpu;
      if ((current->array_rafagas[current->contador_rafagas] == 0) && (current->rafaga_next == 0) && (current->contador_rafagas+1 < current->n_rafagas))
      {
        current->state = "WAITING";
        current->contador_rafagas += 1;
        current->array_rafagas[current->contador_rafagas] += 1;
        current->rafaga_next = 1;
        if (cont_cola_procesos > 0)
        {
          cola_procesos[cont_cola_procesos] = current;
        }
        else
        {
          if (!queue->first)
          {
            queue->start = current;
            queue->end = current;
            queue->first = 1;
          }
          else
          {
            queue->end->next = current;
            current->prev = queue->end;
            queue->end = current;
          }
          //Falta actualizar estado, calcular quantum y correr el proceso
        }
        printf("[t = %i] El proceso %s ha pasado a estado WAITING.\n", time, current->name);
        sumar_numero_fabricas(current->fabrica, queue);
        current = NULL;
        queue->en_cpu = NULL;
      }
      else if ((current->array_rafagas[current->contador_rafagas] == 0) && (current->contador_rafagas+1 == current->n_rafagas))
      {
        current->state = "FINISHED";
        printf("[t = %i] El proceso %s ha pasado a estado FINISHED.\n", time, current->name);
        // actualizar valores de archivo output
        current->turnaround_time = time - current->turnaround_time;
        current = NULL;
        queue->en_cpu = NULL;
      }
      else if (current->quantum == 0)
      {
        current->state = "READY";
        if (cont_cola_procesos > 0)
        {
          cola_procesos[cont_cola_procesos] = current;
        }
        else
        {
          if (!queue->first)
          {
            queue->start = current;
            queue->end = current;
            queue->first = 1;
          }
          else
          {
            queue->end->next = current;
            current->prev = queue->end;
            queue->end = current;
          }
          //Falta actualizar estado, calcular quantum y correr el proceso
        }
        printf("[t = %i] El proceso %s ha pasado a estado READY!\n", time, current->name);
        current->interrumpido += 1;
        current->waiting_time -= 1;
        sumar_numero_fabricas(current->fabrica, queue);
        current = NULL; 
        queue->en_cpu = NULL;
      }
      else
      {
        current->quantum -= 1;
        current->array_rafagas[current->contador_rafagas] -= 1;
      }
    }
    if (!queue->first)
    {
      if (cont_cola_procesos == 1) 
      {
        //Llegar y meter
        queue->start = cola_procesos[0];
        queue->end = cola_procesos[0];
        cola_procesos[0]->state = "READY";
        cola_procesos[0]->estoy = 1;
        queue->first = 1;
      }
    }
    // desempatan acorde a sus prioridades todos los procesos que llegan a la cola en ese instante
    if (cont_cola_procesos > 0)
    {
      desempatar(cola_procesos, queue, cont_cola_procesos, time);
    }

    // agregar a la cola y seguir corriendo
    if ((!queue->en_cpu) && queue->start)
    {
      if (strcmp(queue->start->state, "READY") == 0)
      {
        nuevo_start = queue->start->next;
        if (!nuevo_start)
        {
          if (!queue->start->first_time)
          {
            queue->start->first_time = 1;
            queue->start->response_time = time - queue->start->response_time;
          }
          queue->first = 0;
          queue->start->quantum = calcular_quantum(queue, Q, queue->start->fabrica);
          queue->start->prev = NULL;
          queue->en_cpu = queue->start;
          queue->en_cpu->state = "RUNNING";
          queue->en_cpu->elegido += 1;
          restar_numero_fabricas(queue->start->fabrica, queue);
          queue->start = nuevo_start;
        }
        else
        {
          if (!queue->start->first_time)
          {
            queue->start->first_time = 1;
            queue->start->response_time = time - queue->start->response_time;
          }
          queue->start->quantum = calcular_quantum(queue, Q, queue->start->fabrica);
          queue->start->next->prev = NULL;
          queue->start->next = NULL;
          queue->start->prev = NULL;
          queue->en_cpu = queue->start;
          restar_numero_fabricas(queue->start->fabrica, queue);
          queue->en_cpu->elegido += 1;
          queue->en_cpu->state = "RUNNING";\
          printf("[t = %i] El proceso %s ha pasado a estado RUNNING!\n", time, queue->en_cpu->name);
          queue->start = nuevo_start;
        }
      }
      else if (strcmp(queue->start->state, "WAITING") == 0)
      {
        keep = queue->start->next;
        while ((keep) && (strcmp(keep->state, "WAITING") == 0))
        {
          keep = keep->next;
        }
        if ((keep) && (strcmp(keep->state, "READY") == 0))
        {
          otro = keep->next;
          if (!otro)
          {
            if (!keep->first_time)
            {
              keep->first_time = 1;
              keep->response_time = time - queue->start->response_time;
            }
            queue->end = keep->prev;
            keep->prev = NULL;
            keep->next = NULL;
            queue->en_cpu = keep;
            keep->quantum = calcular_quantum(queue, Q, keep->fabrica);
            queue->en_cpu->state = "RUNNING";
            queue->en_cpu->elegido += 1;
            printf("[t = %i] El proceso %s ha pasado a estado RUNNING!\n", time, keep->name);
            keep = NULL;
          }
          else
          {
            if (!keep->first_time)
            {
              keep->first_time = 1;
              keep->response_time = time - queue->start->response_time;
            }
            keep->prev->next = keep->next;
            keep->next->prev = keep->prev;
            keep->prev = NULL;
            keep->next = NULL;
            queue->en_cpu = keep;
            keep->quantum = calcular_quantum(queue, Q, keep->fabrica);
            queue->en_cpu->state = "RUNNING";
            queue->en_cpu->elegido += 1;
            printf("[t = %i] El proceso %s ha pasado a estado RUNNING!\n", time, keep->name);
            keep = NULL;
          }
        }
      }
    }

    cont_cola_procesos = vaciar_cola_procesos(cola_procesos);
    actualizar_tiempos(queue, time);
    if (!queue->en_cpu)
    {
      printf("[t = %i] No hay ningun proceso ejecutando en la CPU\n", time);
    }
    time += 1;  
  }

  escribir_output(procesos, output_file, cont_procesos);
  // liberamos la memoria pedida
  input_file_destroy(file);
  for (int i=0; i<N; i++)
  {
    free(procesos[i]->array_rafagas);
    free(procesos[i]);
  }
  free(cola_procesos);
  free(procesos_ordenada);
  free(procesos);
  free(queue);
}
