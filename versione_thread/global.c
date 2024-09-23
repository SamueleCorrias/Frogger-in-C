#include "header.h"

obj rana ;
obj proiettile ; 
obj tronchi[N_TRONCHI];
obj veicoli[N_MACCHINE];
obj proiettili_nemici[N_TRONCHI];


int vite;
int score;
int tempo_rimanente;

corsia fiumi_array[N_TRONCHI];
corsia strade_array[N_CORSIE];
bool tane[N_TANE] = {true, true, true, true, true};
bool restart_manche; 
int restart_macchine;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER ;
sem_t count;
sem_t count_macchine;