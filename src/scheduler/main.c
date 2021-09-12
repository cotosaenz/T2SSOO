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
  while(procesos_ordenada[N-1]->state != "FINISHED" && time<40) // hay que sacar lo ultimo dsps, es solo con el FINISHED del ultimo proceso
  {
    while (cont_procesos < N && procesos_ordenada[cont_procesos]->start == time)
    {
      printf("ME TOCAAAA\n");
      cont_procesos += 1;
    }
    time += 1;
  }



  // liberamos la memoria pedida
  input_file_destroy(file);
  for (int i=0; i<N; i++)
  {
    free(procesos[i]->array_rafagas);
    free(procesos[i]);
  }
  free(procesos);
  free(queue);
}
