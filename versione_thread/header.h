#ifndef HEADER_H
#define HEADER_H

// inclusione delle librerie
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ncurses.h>
#include <time.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

/* define per il main */

/* struct generale */
typedef struct{
    int x ;
    int y ;
    char id ;
    int id_num ; // serve ad identificare i diversi tronchi e le diverse macchine, siccome hanno lo stesso id
    bool flag ;
    bool enemy ; // serve per capire se il tronco ha un nemico
}obj ;

typedef struct{
    int y ;
    int senso ;
    int speed ;
    int traffico ; 
}corsia ;


/* define per le varie */ 
#define TEMPO 60
#define STAMPA_TEMPO_X 40
#define STAMPA_SCORE_X 73
#define STAMPA_STATISTICHE_Y 1 
#define SPACE_BAR 32
#define CARATTERE_VUOTO ' '
#define CARATTERE_TANE '#'
#define SPARO_NEMICO 100
#define SPAWN_NEMICI 300

/* define per la grafica */
/* AG = area gioco     I = inizio/iniziale      F = fine     SX = sinistra       DX = destra        LIM = limite */

#define BASE_AG 85 /* corrisponde all'asse X */
#define ALTEZZA_AG 41 /* corrisponde all'asse Y */

#define MENU 99
#define Y_FINE_MARCIAPIEDE_META 23
#define Y_FINE_MARCIAPIDE_SOTTO 44
#define Y_INIZIO_MARCIAPIEDE_SOTTO 41
#define X_INIZIO_AG
#define INFO 98
#define INFO_OK 97
#define BORDI_PAIR 60
#define CORONA 96
#define FACCIA_RANA 95
#define OCCHI_RANA 94

/* define coordinate Y della grafica */
#define I_BORDO_SUP 0
#define F_BORDO_SUP  2
#define I_TANE 3
#define F_TANE 7
#define I_FIUME 8
#define F_FIUME 19
#define I_PRATO 20
#define F_PRATO 22
#define I_AUTOSTRADA 23
#define F_AUTOSTRADA 37
#define I_MARCIAPIEDE 38
#define F_MARCIAPIEDE 41

/* define coordinate X della grafica */ 
#define DIM_TANE 5
#define DIM_PRATO_TANE 10

/* define per le auto */
#define N_CORSIE 5
#define N_MACCHINE 6

#define DIM_MACCHINA 4 
#define DIM_CAMION 7

#define ATTESA_CAMBIO_CORSIA 90000

/* define per il fiume */
#define LIM_SX_FIUME 0
#define LIM_DX_FIUME 80
#define DIM_TRONCO 9
#define N_TRONCHI 4 

//define per le corsie dei tronchi
#define CORSIA_ZERO 17 ;
#define CORSIA_UNO 14 ;
#define CORSIA_DUE 11 ;
#define CORSIA_TRE 8 ;

/* define per la rana */
#define POS_X_RANA 40 /* posizione iniziale asse X della rana */ 
#define POS_Y_RANA 38 /* posizione iniziale asse Y della rana */

#define DIM_RANA 5
#define LIM_SX_RANA 0
#define LIM_DX_RANA 80
#define LIM_SUP_RANA 5
#define LIM_INF_RANA 38

#define N_TANE 5

/* define coordinate X delle tane */
#define I_TANA_UNO 10
#define F_TANA_UNO 14
#define I_TANA_DUE 25
#define F_TANA_DUE 29
#define I_TANA_TRE 40
#define F_TANA_TRE 44 
#define I_TANA_QUATTRO 55
#define F_TANA_QUATTRO 59
#define I_TANA_CINQUE 70
#define F_TANA_CINQUE 74

/* define coordinata Y delle tane */
#define Y_TANE 5 

extern pthread_mutex_t mutex ;

/* funcioni per la grafica */
void inizializza_ncurses();
void sfondo_gioco() ;
void stampa_tutto() ;
void corsie_setter_fiume(corsia*);
void corsie_setter_macchine(corsia*);
bool game_over_screen();
void win_screen();
void menu();
void scritta_frogger();
void info();
void disegno_rana();

// funzione per tempo
void* tempo_function() ; 

// rana e proiettile
void* frog_function();
void* proiettile_function();

// printer delle entità
void print_rana(obj);
void log_printer(obj);
void enemy_printer(obj);

// funzioni del fiume 
void* tronchi_function(void*);
void* proiettile_nemico_function(void*);

// funzioni dell'autostrada
void* macchine_function(void*);
void macchine_initializer();
void vehicle_printer(obj);

#endif