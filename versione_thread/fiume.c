#include "header.h"
#include "global.h"

void* tronchi_function(void* fiumi_array){

    corsia fiume = *((corsia*)fiumi_array);
    int id;
    int timer_spawn_nemici = 120 + rand()%SPAWN_NEMICI;
    int timer_sparo = SPARO_NEMICO;

    switch(fiume.y){
        case 8:
            id = 0;
            break;
        case 11:
            id = 1;
            break;
        case 14:
            id = 2;
            break;
        case 17:
            id = 3;
            break;
    }

    int minRange = DIM_TRONCO * 2;
    int maxRange = BASE_AG - DIM_TRONCO * 2;

    tronchi[id].x = minRange + rand() % (maxRange - minRange + 1);
    tronchi[id].y = fiume.y;

    pthread_mutex_lock(&mutex) ;
    sem_post(&count) ;
    pthread_mutex_unlock(&mutex) ;

    while(true){
        
        // se il tronco tocca il bordo, allora cambia direzione
        if(tronchi[id].x == LIM_SX_FIUME || tronchi[id].x + DIM_TRONCO == LIM_DX_FIUME - 1){

            fiume.senso *= -1;
        }
        // muovi il tronco in direzione del senso
        tronchi[id].x += fiume.senso;
        
        // muovi la rana in direzione del tronco se sta sopra il tronco        
        if(rana.y == tronchi[id].y && (rana.x >= tronchi[id].x && rana.x <= tronchi[id].x + DIM_TRONCO))
        {
            rana.x += fiume.senso;
        }
        
        // decrementa i timer per sparo e spawn nemici
        if(tronchi[id].enemy == false) timer_spawn_nemici--;

        if(tronchi[id].enemy == true) timer_sparo--;
        
        // spawna nemico se rana non è presente nel tronco e il timer è a zero 
        if(timer_spawn_nemici <= 0 && rana.y != tronchi[id].y){
            tronchi[id].enemy = true;
            timer_spawn_nemici = 120 + rand()%SPAWN_NEMICI;
        }
        
        // se il timer sparo finisce allora fai sparare il nemico, resetta il timer
        if(timer_sparo <= 0 && tronchi[id].enemy == true){
            pthread_mutex_lock(&mutex) ;
            proiettili_nemici[id].x = tronchi[id].x;
            proiettili_nemici[id].y = tronchi[id].y + 1 ;
            pthread_mutex_unlock(&mutex);
            timer_sparo = SPARO_NEMICO;
        }

        usleep(fiume.speed);
    }
}

void* proiettile_nemico_function(void* fiumi_array){

    corsia fiume = *((corsia*)fiumi_array);

    int id;

    switch(fiume.y){
        case 8:
            id = 0;
            break;
        case 11:
            id = 1;
            break;
        case 14:
            id = 2;
            break;
        case 17:
            id = 3;
            break;
    }

    pthread_mutex_lock(&mutex) ;
    sem_post(&count) ;
    pthread_mutex_unlock(&mutex) ;

    while(true){
        // se un proiettile nemico tocca una macchina --> il proiettile viene distrutto
        for(size_t i = 0 ; i < N_MACCHINE ; i ++){ // for per i veicoli

            if(proiettili_nemici[id].x >= veicoli[i].x && ((veicoli[i].id == 'm' && proiettili_nemici[id].x <= veicoli[i].x + DIM_MACCHINA) || (veicoli[i].id == 'c' && proiettili_nemici[id].x <= veicoli[i].x + DIM_CAMION)) && (proiettili_nemici[id].y >= veicoli[i].y && proiettili_nemici[id].y <= veicoli[i].y + 3)){
                pthread_mutex_lock(&mutex) ;
                proiettili_nemici[id].x = -2;
                proiettili_nemici[id].y = -2;
                pthread_mutex_unlock(&mutex);
            
            }
        }
        if(proiettili_nemici[id].y <= BASE_AG-1){
            pthread_mutex_lock(&mutex) ;
            proiettili_nemici[id].y ++;
            pthread_mutex_unlock(&mutex) ;
            usleep(30000);
        }
    }
}

void log_printer(obj tronco){ 

    char* tronco_sprite[3] = {" ------------ ", "|  .  -:- -  |", " ------------ "};

    for(int i = 0; i < 3; i++){
          mvaddstr((tronco.y) + i, tronco.x, tronco_sprite[i]);
    }
}

void enemy_printer(obj nemico){

    start_color();
    init_pair(7, COLOR_WHITE, COLOR_RED);

    char* nemico_sprite[3] = {"###", "#O#", "###"};

    for(int i = 0; i < 3; i++)
    {
        attron(COLOR_PAIR(7));
          mvaddstr((nemico.y) + i, nemico.x, nemico_sprite[i]);
        attroff(COLOR_PAIR(7));
    }
}