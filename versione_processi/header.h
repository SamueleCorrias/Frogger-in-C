/* importazione di tutte le librerie per il gioco */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ncurses.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>

/* define per il main */

/* define per le varie */ 
#define TEMPO 60
#define STAMPA_TEMPO_X 40
#define STAMPA_SCORE_X 73
#define STAMPA_STATISTICHE_Y 1 
#define SPACE_BAR 32
#define CARATTERE_VUOTO ' '
#define CARATTERE_TANE '#'
#define SPARO_NEMICO 100
#define SPAWN_NEMICI 15000

/* define per la grafica */
/* AG = area gioco     I = inizio/iniziale      F = fine     SX = sinistra       DX = destra        LIM = limite */

#define BASE_AG 85 /* corrisponde all'asse X */
#define ALTEZZA_AG 41 /* corrisponde all'asse Y */

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
#define N_MACCHINE 5

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
}corsia;

/*funzioni del file grafica.c */
void inizializza_ncurses() ; 
void sfondo_gioco(bool flags[N_TANE]) ;
void win_screen(int vite, int punteggio[1]) ;
bool game_over_screen() ;
void wait_manche_screen() ;

/* funzione del file main.c */
int area_gioco(int vite, bool tane[N_TANE], int score[1], int principale[2], int pipe_rana[2], int pipe_proiettilerana[2], int pipe_troncorana[2], int pipe_tronchi[2], int pipe_nemici[N_TRONCHI][2], int pipe_segnaleproiettilenemici[N_TRONCHI][2], int pipe_macchine[2], int pipe_coda[2], int pipe_veicoli[2], int pipe_segnaleproittile_rana[2], int pipe_nemico_sul_tronco[N_TRONCHI][2], int pipe_segnalecollisione_proiettile[2], int pipe_time[2]) ;
void corsie_setter_fiume(corsia*);
void corsie_setter_macchine(corsia*) ;
void timer(int pipe_tempo[2], int tempo) ;

/* funzione del file rana.c */
void funzione_rana(int principale[2], int pipe_rana[2], int pipe_proiettilerana[2], int pipe_troncorana[2], int pipe_segnaleproiettile_rana[2], int pipe_segnalecollisione_proiettile[2]) ; 
void proiettile(int principale[2], int pipe_proiettilerana[2], int pipe_segnaleproittile_rana[2], int pipe_segnalecollisione_proiettile[2]) ;

/* funzioni del file fiume.c */
void gestione_tronco(corsia id, int principale[2], int pipe_tronchi[2], int pipe_nemici[2], int pipe_segnaleproiettilenemici[2], int pipe_nemico_sul_tronco[2]) ;
void log_printer(obj);
void enemy_printer(obj);

void menu();
void scritta_frogger();
void info();
void disegno_rana();

/* funzione del file auto.c */
void genera_veicoli(int pipe_veicoli[2], int pipe_macchine[2], int pipe_coda[2], corsia*) ;
void inizializzazione_veicoli(int pipe_veicoli[2], int pipe_macchine[2], int pipe_coda[2], int, corsia*) ;
void controllo_generazione_macchine(int pipe_veicolo[2], int pipe_macchine[2]) ;
void vehicle_printer(obj) ; 