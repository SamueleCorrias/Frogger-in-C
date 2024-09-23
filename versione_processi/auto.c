/* file delle auto, movimento auto, ecc */

#include "header.h" /* andiamo a richiamare il file header.h */

void inizializzazione_veicoli(int pipe_veicoli[2], int pipe_collissioni_macchine[2], int pipe_coda[2], int numero, corsia* strade){

    srand(getpid()) ; 
    obj veicolo ; 
    int corsia ; 
    int posizione_x ;

    /* file per il debug */

    switch(rand()%2){ // scelta se camion o macchina
        case 0:
            veicolo.id = 'c' ; 
            break;
        case 1:
            veicolo.id = 'm' ; 
            break;
    }

    corsia = rand() % N_CORSIE ; // assegnazione corsia iniziale 
    veicolo.y = strade[corsia].y ;
    posizione_x = 10 + rand() % BASE_AG - 10; // assegnazione x iniziale 
    veicolo.x = posizione_x ;
    veicolo.id_num = numero ; 

    write(pipe_veicoli[1], &veicolo, sizeof(obj)) ; 


    int flag = 1 ; 
    while(flag == 1){
        read(pipe_collissioni_macchine[0], &flag, sizeof(int)) ; 
        usleep(100000) ; 

        if(flag == 1){ // se è presente la collisione, rigenera le posizioni

            corsia = rand() % N_CORSIE ; // assegnazione corsia iniziale 
            veicolo.y = strade[corsia].y ;
            posizione_x = 3 + rand() % BASE_AG - 3; // assegnazione x iniziale 
            veicolo.x = posizione_x ;
        }
    }

    write(pipe_veicoli[1], &veicolo, sizeof(obj)) ; 
    usleep(1000000) ;

    // INIZIAMO A GIOCARE
    int direzione; 
    bool cambio_corsia = true; 
    bool n = false ; 
    while(true){
        cambio_corsia = true ;
        if(veicolo.x < 0 || veicolo.x > BASE_AG || veicolo.y < 0 || veicolo.y >= ALTEZZA_AG){ // controllo collisione con la x --> se esce fuori dalla corsia
            while(cambio_corsia){
                n = false ; 
                read(pipe_coda[0], &n, sizeof(bool)) ; // leggo dalla pipe_coda per vedere se una macchina ha chiesto di cambiare corsia

                if(n == true){ // se sta già avenendo uno scambio tra macchine, allora attendo
                    usleep(ATTESA_CAMBIO_CORSIA*2) ; 
                }
                else{ // se la coda è libera, posso fare il cambio corsia 
                    cambio_corsia = false ; // occupo il cambio corsia 
                    n = true ; 

                    for(size_t i = 0 ; i < N_MACCHINE ; i ++){
                        write(pipe_coda[1], &n, sizeof(bool)) ; // dico alle altre macchine che sto facendo il cambio corsia 
                    }

                    // cambia senso della direzione e corsia della macchina
                    corsia = rand() % N_CORSIE ;
                    veicolo.y = strade[corsia].y ;
                    if(strade[corsia].senso == 1){
                        veicolo.x = 0 ;
                    }
                    else if(strade[corsia].senso == -1){
                        veicolo.x = BASE_AG - DIM_CAMION ; 
                    }
                    usleep(ATTESA_CAMBIO_CORSIA/2) ; // effettuato il cambio corsia, faccio aspettare la macchina prima di muoversi
                }
            }
        }
        else{ // se la macchina non si trova nei bordi, si muove in base alla corsia
            
            direzione = strade[corsia].senso ; 
            veicolo.x += direzione ; 
        }
        write(pipe_veicoli[1], &veicolo, sizeof(obj)) ; 
        usleep(80000) ; 
    }
    _exit(0) ; // killare la macchina quando cambiamo la roba 
}

void controllo_generazione_macchine(int pipe_veicolo[2], int pipe_collissioni_macchine[2]){

    int count = 0 ; 
    obj veicolo[N_MACCHINE] ; 
    obj data ; 

    int matrice_pos_iniziali[N_MACCHINE][2] = {{0}} ;
    int controllo_lettura[N_MACCHINE] = {0} ;
    bool band = true ; 
    
    const int max_plan_b = 200;
    int plan_b = 0;
    while (band){
        plan_b++;
        if(plan_b == max_plan_b){
            band = false ; 
            break;
        }
        read(pipe_veicolo[0], &data, sizeof(obj)) ;
        if(count < N_MACCHINE){
            for(size_t i = 0 ; i < N_MACCHINE ; i ++){
                if(data.id_num == i && controllo_lettura[i] == 0){
                    veicolo[i].x = data.x ; 
                    veicolo[i].y = data.y ; 
                    veicolo[i].id = data.id ;
                    veicolo[i].id_num = data.id_num ; 
                    count ++ ; 

                    matrice_pos_iniziali[i][0] = veicolo[i].x ; 
                    matrice_pos_iniziali[i][1] = veicolo[i].y ; 
                    controllo_lettura[i] = 1 ; 

                }
            }
        }
        
        if(count == N_MACCHINE){
            int collisione = 0 ; 
            
            for(size_t i = 0 ; i < N_MACCHINE ; i ++){ // macchina che viene confrontata con tutte le altre
                for(size_t j = 0 ; j < N_MACCHINE ; j ++){
                    if(i != j){
                        if(matrice_pos_iniziali[i][1] == matrice_pos_iniziali[j][1]){ // controlla prima la y
                            if(!((matrice_pos_iniziali[j][0] > matrice_pos_iniziali[i][0] + DIM_CAMION+2) || (matrice_pos_iniziali[j][0] + DIM_CAMION+2 < matrice_pos_iniziali[i][0]))){ // controlla la x
                                count = 0 ; 
                                collisione = 1 ; 
                                
                                i = N_MACCHINE ; 
                                j = N_MACCHINE ; 
                                // scrivo nella pipe per ogni macchina 
                                for(size_t i = 0 ; i < N_MACCHINE ; i ++){
                                    write(pipe_collissioni_macchine[1], &collisione, sizeof(int)) ; 
                                }
                                // azzeramento array che controllava la lettura di ogni singola macchina 
                                for(size_t i = 0 ; i < N_MACCHINE ; i ++){
                                    controllo_lettura[i] = 0 ; 
                                }
                            }
                        } 
                    }
                }
            }
            
            if(collisione == 0){
                for(size_t i = 0 ; i < N_MACCHINE ; i ++){
                    write(pipe_collissioni_macchine[1], &collisione, sizeof(int)) ; 
                }
                band = false ; 
            }
        }
    }

    if(plan_b == max_plan_b){
        int fake = 0;
        for(size_t i = 0; i < N_MACCHINE; i++){
            write(pipe_collissioni_macchine[1], &fake, sizeof(int));
        }
    }
}

void vehicle_printer(obj veicolo){

    char* macchina_sprite[3] = {"|--|", "oooo", "|__|"} ;
    char* camion_sprite[3] = {"|-----|", "+++++++", "|_____|"} ; 

    if(veicolo.x > 0 && veicolo.x < BASE_AG-DIM_CAMION){
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