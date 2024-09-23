#include "header.h"

extern obj rana ;
extern obj proiettile ; 
extern obj tronchi[N_TRONCHI];
extern obj veicoli[N_MACCHINE];
extern obj proiettili_nemici[N_TRONCHI];

extern corsia fiumi_array[N_TRONCHI];
extern corsia strade_array[N_CORSIE];
extern bool tane[N_TANE];

extern int vite;
extern int score;
extern int tempo_rimanente;


extern bool restart_manche; 
extern int restart_macchine;


extern pthread_mutex_t mutex;
extern sem_t count; 
extern sem_t count_macchine;