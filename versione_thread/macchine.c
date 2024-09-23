#include "header.h"
#include "global.h"

void macchine_initializer() {
    int corsia_scelta;
    
    for (size_t i = 0; i < N_MACCHINE; i++) {
        switch(rand() % 2) {
            case 0:
                veicoli[i].id = 'c';
                break;
            case 1:
                veicoli[i].id = 'm';
                break;
        }

        // cerca quale corsia ha zero traffico, in caso scegli quella invece che quella random
        int corsia_zero_traffico = -1 ;
        for (size_t i = 0; i < N_CORSIE; i++)
        {  
            //trova corsia senza traffico
            if(strade_array[i].traffico == 0){
                corsia_zero_traffico = i;
            }
        }

        // se è stata trovata una corsia con zero traffico, allora aggiungi la macchina in quella
        if(corsia_zero_traffico != -1)
        {
            corsia_scelta = corsia_zero_traffico;
        }
        else{ // altrimenti scegli quella random
            // sceglie la nuova corsia
            corsia_scelta = rand() % N_CORSIE;
        }

        corsia_scelta = rand() % N_CORSIE;
        veicoli[i].y = strade_array[corsia_scelta].y;
        veicoli[i].x = 10 + rand() % (BASE_AG - 20 + 1);


        veicoli[i].id_num = i; // serve nella funzione che gestisce il movimento per identificarla


        bool collision;
        do {
            collision = false;

            for(size_t j = 0; j < i; j++) {
                if(veicoli[j].y == veicoli[i].y) {
                    if (!(veicoli[j].x > veicoli[i].x + DIM_CAMION || veicoli[j].x + DIM_CAMION < veicoli[i].x)) {
                        collision = true;
                        break;
                    }
                }
            }

            if (collision) {
                veicoli[i].x = 10 + rand() % (BASE_AG - 20 + 1);

            }

            
        } while (collision);
        //incrementa il traffico nella corsia scelta
        pthread_mutex_lock(&mutex) ;
        strade_array[corsia_scelta].traffico += 1;
        pthread_mutex_unlock(&mutex) ;
    }
}

void* macchine_function(void * veicolo){
    obj macchina = *((obj*)veicolo);

    int car_idx = macchina.id_num ;
    pthread_mutex_lock(&mutex) ;
    sem_post(&count) ;
    pthread_mutex_unlock(&mutex) ;

    int id;
    int corsia_scelta;

    while(true){
    // trova l'id giusto per scegliere la corsia da array_fiumi
        switch(veicoli[car_idx].y){
            case 23:
                id = 0;
                break;
            case 26:
                id = 1;
                break;
            case 29:
                id = 2;
                break;
            case 32:
                id = 3;
                break;
            case 35:
                id = 4;
                break;
        }

        if(veicoli[car_idx].x >= 1 && veicoli[car_idx].x <= BASE_AG - DIM_CAMION - 1)
            veicoli[car_idx].x += strade_array[id].senso;
        else{
            // esce dalla corsia e quindi decrementa il traffico
            pthread_mutex_lock(&mutex) ;
            strade_array[id].traffico -= 1;
            pthread_mutex_unlock(&mutex) ;

            // esegui il cambio corsia sotto mutex così aspetti che anche le altre macchine lo abbiano fatto
            pthread_mutex_lock(&mutex) ;
            
            // cerca quale corsia ha zero traffico, in caso scegli quella invece che quella random
            int corsia_zero_traffico = -1 ;
            for (size_t i = 0; i < N_CORSIE; i++)
            {  
                //trova corsia senza traffico
                if(strade_array[i].traffico == 0){
                    corsia_zero_traffico = i;
                }
            }

            // se è stata trovata una corsia con zero traffico, allora aggiungi la macchina in quella
            if(corsia_zero_traffico != -1)
            {
                corsia_scelta = corsia_zero_traffico;
            }
            else{ // altrimenti scegli quella random
                // sceglie la nuova corsia
                corsia_scelta = rand() % N_CORSIE;
            }
            veicoli[car_idx].y = strade_array[corsia_scelta].y;
            

            // a seconda del verso cambia la x
            switch(strade_array[corsia_scelta].senso)
            {
                case -1:
                    veicoli[car_idx].x = BASE_AG - DIM_CAMION - 1;
                    break;
                case 1:
                    veicoli[car_idx].x = 1;
                    break;
            }
            
            pthread_mutex_unlock(&mutex) ;

            // cambiare di corsia se trova collisione nella corsia 
            bool collision;
            do {
                collision = false;

                for(size_t j = 0; j < N_MACCHINE; j++) {
                    if(veicoli[j].y == veicoli[car_idx].y && car_idx != j) {
                        if (!(veicoli[j].x > veicoli[car_idx].x + DIM_CAMION || veicoli[j].x + DIM_CAMION < veicoli[car_idx].x)) {
                            collision = true;
                            break;
                        }
                    }
                }

                if (collision) {
                    corsia_scelta = rand() % N_CORSIE;
                    veicoli[car_idx].y = strade_array[corsia_scelta].y;
                }
            } while (collision);

            pthread_mutex_lock(&mutex) ;
            strade_array[corsia_scelta].traffico += 1; //incrementa il traffico della corsia scelta
            pthread_mutex_unlock(&mutex) ;
        }
        usleep(60000);
    }
}


void vehicle_printer(obj veicolo){

    char* macchina_sprite[3] = {"|--|", "oooo", "|__|"} ;
    char* camion_sprite[3] = {"|-----|", "+++++++", "|_____|"} ; 

    if(veicolo.x >= 0 && veicolo.x <= BASE_AG-DIM_CAMION){
        if(veicolo.id == 'm'){ // se il veicolo è una brum brum
            for(size_t i = 0 ; i < 3 ; i++){
      	        mvaddstr((veicolo.y) + i, veicolo.x, macchina_sprite[i]);
            }
        }

        if(veicolo.id == 'c'){ // se il veicolo è una brum brum +
            for(size_t i = 0 ; i < 3 ; i++){
      	        mvaddstr((veicolo.y) + i, veicolo.x, camion_sprite[i]);
            }
        }
    }
}