#include <stdio.h>
#include <stdlib.h>

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
  Process* nuevo_start = NULL;
  Process** cola_procesos = calloc(8, sizeof(Process*));
  int cont_cola_procesos = 0;
  Process* current = NULL;
  while(procesos_ordenada[N-1]->state != "FINISHED" && time<40) // hay que sacar lo ultimo dsps, es solo con el FINISHED del ultimo proceso
  {
    printf("t = %i\n", time);
    // primero revisamos si llegan a la cola procesos nuevos
    while (cont_procesos < N && procesos_ordenada[cont_procesos]->start == time)
    {
      printf("ME TOCAAAA\n");
      cola_procesos[cont_cola_procesos] = procesos_ordenada[cont_procesos]; //Se agrega a la cola procesos el proceso que llega por primera vez
      sumar_numero_fabricas(cola_procesos[cont_cola_procesos]->fabrica, queue);
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
        current->rafaga_next = 1;
        if (cont_cola_procesos > 0)
        {
          cola_procesos[cont_cola_procesos] = current;
        }
        else
        {
          queue->end->next = current;
          current->prev = queue->end;
          queue->end = current;
          //Falta actualizar estado, calcular quantum y correr el proceso
        }
        printf("[t=%i] Proceso %s pasa a WAITING!\n", time, current->name);
        sumar_numero_fabricas(cola_procesos[cont_cola_procesos]->fabrica, queue);
        current = NULL;
        queue->en_cpu = NULL;
      }
      else if ((current->array_rafagas[current->contador_rafagas] == 0) && (current->contador_rafagas+1 == current->n_rafagas))
      {
        current->state = "FINISHED";
        printf("[t=%i] Proceso %s ha terminado!\n", time, current->name);
        // actualizar valores de archivo output
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
          queue->end->next = current;
          current->prev = queue->end;
          queue->end = current;
          //Falta actualizar estado, calcular quantum y correr el proceso
        }
        printf("[t=%i] Proceso %s pasa a READY!\n", time, current->name);
        sumar_numero_fabricas(current->fabrica, queue);
        current = NULL;
        queue->en_cpu = NULL;
      }
      else
      {
        printf("%s sigo corriendo\n", current->name);
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
      if (queue->start->state == "READY")
      {
        nuevo_start = queue->start->next;
        queue->start->quantum = calcular_quantum(queue, Q, queue->start->fabrica);
        queue->start->next->prev = NULL;
        queue->start->next = NULL;
        queue->en_cpu = queue->start;
        queue->start = nuevo_start;
      }
      else if (queue->start->state == "WAITING")
      {
        keep = queue->start->next;
        while ((keep) && (keep->state == "WAITING"))
        {
          keep = keep->next;
        }
        if ((keep) && (keep->state == "READY"))
        {
          keep->prev->next = keep->next;
          keep->next->prev = keep->prev;
          keep->prev = NULL;
          keep->next = NULL;
          queue->en_cpu = keep;
          keep = NULL;
        }
      }
    }

    cont_cola_procesos = vaciar_cola_procesos(cola_procesos);
    time += 1;  
  }


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
