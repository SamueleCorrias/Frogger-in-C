#include "header.h"
#include "global.h"

int main(){
    /* fa si che la pagina si apra a dimensioni del gioco in automatico */
    system("echo -e \"\e[8;41;85t\"") ; 

    menu();
    
    // inizializza le vite, score e tempo
    pthread_mutex_lock(&mutex) ;
    vite = 3;
    score = 0;
    tempo_rimanente = TEMPO;
    pthread_mutex_unlock(&mutex);

    int old_vite;
    int old_score;

    // variabili necessarie alle condizioni di sconfitta/vittoria
    int tane_riempite = 0;
    int tane_riempite_prima = 0;
    int vite_rimosse = 0;
    int count_tane = 0;
    bool manche = false;

    // da il senso alle corsie di strada e fiume
    corsie_setter_fiume(fiumi_array);
    corsie_setter_macchine(strade_array);

    pthread_t tronchi_thread[N_TRONCHI];
    for (size_t i = 0; i < N_TRONCHI; i++){
        pthread_create(&tronchi_thread[i], NULL, &tronchi_function, (void*)&fiumi_array[i]) ;
    }
    
    while(true)
    {   

        macchine_initializer(); // array di macchine viene riempito con posizioni che non collidono

        pthread_mutex_lock(&mutex);
        tempo_rimanente = TEMPO;
        pthread_mutex_unlock(&mutex);

        // proiettili vengono distrutti (messi fuori dallo schermo)
        pthread_mutex_lock(&mutex) ;
        proiettile.x = -1;
        proiettile.y = -1;
        pthread_mutex_unlock(&mutex);

        for (size_t i = 0; i < N_TRONCHI; i++){
            pthread_mutex_lock(&mutex) ;
            proiettili_nemici[i].x = -2;
            proiettili_nemici[i].y = -2;
            pthread_mutex_unlock(&mutex) ;
            
        }
        
        // inizializzazione semafori per macchine e il resto
        sem_init(&count, 0, 0);
        sem_init(&count_macchine, 0, 0);

        // inizializzazione thread e creazione
        pthread_t tempo_thread;
        pthread_t rana_thread ;
        pthread_t proiettile_thread;
        pthread_t macchine_thread[N_MACCHINE];
        pthread_t proiettili_nemici_thread[N_TRONCHI];

        pthread_create(&tempo_thread, NULL, &tempo_function, NULL) ;
        pthread_create(&rana_thread, NULL, &frog_function, NULL) ;
        pthread_create(&proiettile_thread, NULL, &proiettile_function, NULL) ;


        for (int car_index = 0; car_index < N_MACCHINE; car_index++)
        {
            //passiamo il veicolo per prendere l'indice, non è possibile passare un int alla thread function in maniera sicura
            pthread_create(&macchine_thread[car_index], NULL, &macchine_function, (void*)&veicoli[car_index]) ;
        }

        for (size_t i = 0; i < N_TRONCHI; i++){
            pthread_create(&proiettili_nemici_thread[i], NULL, &proiettile_nemico_function, (void*)&fiumi_array[i]) ;
        }

        sem_getvalue(&count, &manche); // qui fa diventare manche true per entrare nel ciclo
        sem_getvalue(&count_macchine, &restart_macchine);

        while(manche == true){
            // controllo aggiuntivo dovuto a desincronizzazione 

            pthread_mutex_lock(&mutex) ;
            if(vite == -2)
            {
                vite = old_vite;
                score = old_score;
            }
            old_vite = vite;
            old_score = score;
            pthread_mutex_unlock(&mutex);
            // se il tempo finisce la rana muore
            if(tempo_rimanente == 0){
                pthread_mutex_lock(&mutex) ;
                vite--;
                rana.x = POS_X_RANA;
                rana.y = POS_Y_RANA;
                tempo_rimanente = TEMPO;
                pthread_mutex_unlock(&mutex);
                manche = false;
            }

            // collisione rana -> fiume
            for (size_t i = 0; i < N_TRONCHI; i++)
            {
                if(rana.y == tronchi[i].y && (rana.x < tronchi[i].x || rana.x > tronchi[i].x + DIM_TRONCO)){
                    vite--;
                    pthread_mutex_lock(&mutex) ;
                    rana.x = POS_X_RANA;
                    rana.y = POS_Y_RANA;
                    tempo_rimanente = TEMPO;
                    pthread_mutex_unlock(&mutex);
                    manche = false;
                }
            }
            
            // collisione proiettile rana --> nemico 
            for(size_t i = 0; i < N_TRONCHI; i++){
                if(proiettile.y == tronchi[i].y && tronchi[i].enemy == true && (proiettile.x >= tronchi[i].x && proiettile.x <= tronchi[i].x +3)){
                    pthread_mutex_lock(&mutex) ;
                    proiettile.x = -1;
                    proiettile.y = -1;
                    tronchi[i].enemy = false ; // uccisione nemico
                    score += 100; 
                    pthread_mutex_unlock(&mutex);
                }
            }
            
            // collisione proiettile nemico --> bordo (uccisione proiettile nemico)
            for(size_t i = 0; i < N_TRONCHI ; i++){
                if(proiettili_nemici[i].y == I_MARCIAPIEDE){
                    pthread_mutex_lock(&mutex) ;
                    proiettili_nemici[i].x = -2;
                    proiettili_nemici[i].y = -2;
                    pthread_mutex_unlock(&mutex);
                }
            }

            // collisione proiettile nemico --> rana (uccisione proiettile nemico e fine manche)
            for(size_t i = 0; i < N_TRONCHI ; i++){
                if((proiettili_nemici[i].x >= rana.x && proiettili_nemici[i].x <= rana.x + DIM_RANA)&& (proiettili_nemici[i].y == rana.y )) {
                    pthread_mutex_lock(&mutex) ;
                    vite--;
                    rana.x = POS_X_RANA;
                    rana.y = POS_Y_RANA;
                    proiettili_nemici[i].x = -2;
                    proiettili_nemici[i].y = -2;
                    tempo_rimanente = TEMPO;
                    pthread_mutex_unlock(&mutex);
                    manche = false;
                }
            }

            // collisione proiettile rana -> proiettile nemico e viceversa
            for (size_t i = 0; i < N_TRONCHI; i++)
            {
                if(proiettili_nemici[i].x == proiettile.x && proiettili_nemici[i].y == proiettile.y )
                {
                    pthread_mutex_lock(&mutex) ;
                    proiettile.x = -1;
                    proiettile.y = -1;                
                    proiettili_nemici[i].x = -2;
                    proiettili_nemici[i].y = -2;
                    pthread_mutex_unlock(&mutex);
                }
            }
            
            // collisione rana -> tronco con nemico
            for (size_t i = 0; i < N_TRONCHI; i++)
            {
                if(rana.y == tronchi[i].y && (rana.x >= tronchi[i].x && rana.x <= tronchi[i].x + DIM_TRONCO) && tronchi[i].enemy == true){
                    pthread_mutex_lock(&mutex) ;
                    vite--;
                    rana.x = POS_X_RANA;
                    rana.y = POS_Y_RANA;
                    tempo_rimanente = TEMPO;
                    pthread_mutex_unlock(&mutex);
                    manche = false;
                }
            }
            
            /* ----- CONTROLLO COLLISIONI TANE ----- */
            /* primo controllo: la rana è all'altezza delle tane */
            if(rana.y == Y_TANE){

                
                /* --- TANA UNO --- */
                if(rana.x >= I_TANA_UNO && rana.x <F_TANA_UNO){ /* controllo che la rana sia entrata nella prima tana */
                    if(tane[0] == true){ /* se la flag di quella tana è true significa che è ancora vuota e quindi la rana la può chiudere non perdendo vite */
                        tane[0] = false ; /* la tana diventa riempita, nel caso la rana ci dovesse tornare perderebbe una vita */
                        tane_riempite ++ ; /* aumento il counter del numero di tane riempite */
                    }
                }

                /* --- TANA DUE --- */
                else if(rana.x >= I_TANA_DUE && rana.x < F_TANA_DUE){ /* controllo che la rana sia entrata nella prima tana */
                    if(tane[1] == true){ /* se la flag di quella tana è true significa che è ancora vuota e quindi la rana la può chiudere non perdendo vite */
                        tane[1] = false ; /* la tana diventa riempita, nel caso la rana ci dovesse tornare perderebbe una vita */
                        tane_riempite ++ ; /* aumento il counter del numero di tane riempite */
                    }
                }

                /* --- TANA TRE --- */
                else if(rana.x >= I_TANA_TRE && rana.x < F_TANA_TRE){ /* controllo che la rana sia entrata nella prima tana */
                    if(tane[2] == true){ /* se la flag di quella tana è true significa che è ancora vuota e quindi la rana la può chiudere non perdendo vite */
                        tane[2] = false ; /* la tana diventa riempita, nel caso la rana ci dovesse tornare perderebbe una vita */
                        tane_riempite ++ ; /* aumento il counter del numero di tane riempite */
                    }
                }

                /* --- TANA QUATTRO --- */
                else if(rana.x == I_TANA_QUATTRO){ /* controllo che la rana sia entrata nella prima tana */
                    if(tane[3] == true){ /* se la flag di quella tana è true significa che è ancora vuota e quindi la rana la può chiudere non perdendo vite */
                        tane[3] = false ; /* la tana diventa riempita, nel caso la rana ci dovesse tornare perderebbe una vita */
                        tane_riempite ++ ; /* aumento il counter del numero di tane riempite */
                    }
                }

                /* --- TANA CINQUE --- */
                else if(rana.x >= I_TANA_CINQUE && rana.x < F_TANA_CINQUE){ /* controllo che la rana sia entrata nella prima tana */
                    if(tane[4] == true){ /* se la flag di quella tana è true significa che è ancora vuota e quindi la rana la può chiudere non perdendo vite */
                        tane[4] = false ; /* la tana diventa riempita, nel caso la rana ci dovesse tornare perderebbe una vita */
                        tane_riempite ++ ; /* aumento il counter del numero di tane riempite */
                    }
                }

                tane_riempite_prima = tane_riempite ;

                if(tane_riempite == tane_riempite_prima){ // vuol dire che la rana è andata in un pezzo dove non doveva, viene tolta una vita
                    pthread_mutex_lock(&mutex) ;
                    vite--;
                    tempo_rimanente = TEMPO;
                    pthread_mutex_unlock(&mutex) ;
                }
                else{
                    pthread_mutex_lock(&mutex) ;
                    score += (tempo_rimanente * 15) ;
                    pthread_mutex_unlock(&mutex) ;
                }
    
                pthread_mutex_lock(&mutex) ;
                rana.x = POS_X_RANA;
                rana.y = POS_Y_RANA;
                tempo_rimanente = TEMPO;
                pthread_mutex_unlock(&mutex);
                manche = false;
            }
        
            // collisioni macchine -> rana
            if(rana.y >= I_AUTOSTRADA && rana.y <= F_AUTOSTRADA){ // se la rana è nella zona delle macchine 

                for(size_t i = 0 ; i < N_MACCHINE ; i++){
                    if(veicoli[i].id == 'm'){ // se va a sbattere con una macchina
                        if((rana.x >= veicoli[i].x && rana.x <= veicoli[i].x + DIM_MACCHINA) && rana.y == veicoli[i].y){
                            pthread_mutex_lock(&mutex) ;
                            vite--;
                            rana.x = POS_X_RANA;
                            rana.y = POS_Y_RANA;
                            tempo_rimanente = TEMPO;
                            pthread_mutex_unlock(&mutex);
                            manche = false;
                            
                        }else if(rana.x + DIM_RANA == veicoli[i].x && rana.y == veicoli[i].y){
                            // la rana torna al centro
                            pthread_mutex_lock(&mutex) ;
                            vite--;
                            rana.x = POS_X_RANA;
                            rana.y = POS_Y_RANA;
                            tempo_rimanente = TEMPO;
                            pthread_mutex_unlock(&mutex);
                            manche = false;
                        }
                    }
                    else if(veicoli[i].id == 'c'){ // se va a sbattere con un camion
                        if((rana.x >= veicoli[i].x && rana.x <= veicoli[i].x + DIM_CAMION) && rana.y == veicoli[i].y){
                            pthread_mutex_lock(&mutex) ;
                            vite--;
                            rana.x = POS_X_RANA;
                            rana.y = POS_Y_RANA;
                            tempo_rimanente = TEMPO;
                            pthread_mutex_unlock(&mutex);
                            manche = false;
                        }else if(rana.x + DIM_RANA == veicoli[i].x && rana.y == veicoli[i].y){
                            pthread_mutex_lock(&mutex) ;
                            vite--;
                            rana.x = POS_X_RANA;
                            rana.y = POS_Y_RANA;
                            tempo_rimanente = TEMPO;
                            pthread_mutex_unlock(&mutex);
                            manche = false;
                        }
                    }
                }
            }
            
            /*
            // se un proiettile nemico tocca una macchina --> il proiettile viene distrutto
            for(size_t i = 0 ; i < N_MACCHINE ; i ++){ // for per i veicoli
                for(size_t j = 0 ; j < N_TRONCHI ; j ++){ // for per i tronchi
                    if(proiettili_nemici[j].x >= veicoli[i].x && ((veicoli[i].id == 'm' && proiettili_nemici[j].x <= veicoli[i].x + DIM_MACCHINA) || (veicoli[i].id == 'c' && proiettili_nemici[j].x <= veicoli[i].x + DIM_CAMION)) && (proiettili_nemici[j].y >= veicoli[i].y && proiettili_nemici[j].y <= veicoli[i].y + 3)){
                        pthread_mutex_lock(&mutex) ;
                        proiettili_nemici[i].x = -2;
                        proiettili_nemici[i].y = -2;
                        pthread_mutex_unlock(&mutex);
                    }
                }
            }*/

            // se un proiettile della rana tocca una macchina --> il proiettile viene distrutto 
            if(proiettile.y >= I_AUTOSTRADA && proiettile.y <= F_AUTOSTRADA){ // se il proiettile è compreso nella zona delle auto
                for(size_t i = 0 ; i < N_MACCHINE ; i ++){ // contatore per le macchine 
                    if(proiettile.x >= veicoli[i].x && ((veicoli[i].id == 'm' && proiettile.x <= veicoli[i].x + DIM_MACCHINA) || (veicoli[i].id == 'c' && proiettile.x <= veicoli[i].x + DIM_CAMION)) && (proiettile.y >= veicoli[i].y && proiettile.y <= veicoli[i].y + 3)){
                        pthread_mutex_lock(&mutex) ;
                        proiettile.x = -1;
                        proiettile.y = -1;
                        pthread_mutex_unlock(&mutex);
                    }
                }
            }

            if(tane_riempite == N_TANE){
                win_screen();
                for (size_t i = 0; i < N_TRONCHI; i++){
                    pthread_cancel(tronchi_thread[i]) ;
                }
                endwin() ;
                exit(1) ;
                return 0 ;
            }
            // fine while manche
            pthread_mutex_lock(&mutex) ;
                if((vite != -2) && (vite != old_vite)){
                    for (size_t id = 0; id < N_TRONCHI; id++)
                    {
                        int minRange = DIM_TRONCO * 2;
                        int maxRange = BASE_AG - DIM_TRONCO * 2;
                        tronchi[id].x = minRange + rand() % (maxRange - minRange + 1);
                        tronchi[id].enemy = false;
                        proiettili_nemici[id].x = -2;
                        proiettili_nemici[id].y = -2;
                    }
                }
                else if(tane_riempite != tane_riempite_prima){
                    for (size_t id = 0; id < N_TRONCHI; id++)
                    {
                        int minRange = DIM_TRONCO * 2;
                        int maxRange = BASE_AG - DIM_TRONCO * 2;
                        tronchi[id].x = minRange + rand() % (maxRange - minRange + 1);
                        tronchi[id].enemy = false;
                        proiettili_nemici[id].x = -2;
                        proiettili_nemici[id].y = -2;
                    }
                }
            pthread_mutex_unlock(&mutex) ;
        }

        for (size_t i = 0; i < N_TRONCHI; i++){
            pthread_cancel(proiettili_nemici_thread[i]) ;
        }

        pthread_cancel(rana_thread) ;
        pthread_cancel(proiettile_thread) ;
        pthread_cancel(tempo_thread);

        
        for (size_t i = 0; i < N_MACCHINE; i++){
            pthread_cancel(macchine_thread[i]) ;
        }
        
        
        sem_destroy(&count);
        sem_destroy(&count_macchine);
        pthread_mutex_destroy(&mutex);
        usleep(300);

        
        bool continua_a_giocare;
        if(vite == 0){
            continua_a_giocare = game_over_screen();
            if(continua_a_giocare)
            {
                // inizializza le vite, score e tempo
                pthread_mutex_lock(&mutex) ;
                vite = 3;
                score = 0;
                tempo_rimanente = TEMPO;
                for (size_t i = 0; i < N_TANE; i++)
                {
                    tane[i] = true;
                }

                tane_riempite = 0;
                
                pthread_mutex_unlock(&mutex);

            }
            else{
                for (size_t i = 0; i < N_TRONCHI; i++){
                pthread_cancel(tronchi_thread[i]) ;
                }
                endwin() ;
                exit(1) ;
                return 0 ;
            }
        }
    }
}

void stampa_tutto(){
    erase();
    sfondo_gioco();

    for (size_t i = 0; i < N_TRONCHI; i++){

        log_printer(tronchi[i]) ; // stampa dei tronchi

        if(tronchi[i].enemy == true)
            enemy_printer(tronchi[i]) ; // stampa dei nemici sui tronchi
        
        if(proiettili_nemici[i].y < I_MARCIAPIEDE) 
            mvaddch(proiettili_nemici[i].y, proiettili_nemici[i].x, 'o') ; // stampa i proiettili nemici
    }

    for (size_t i = 0; i < N_MACCHINE; i++)
    {
        vehicle_printer(veicoli[i]);
    }
    

    if(proiettile.y > I_FIUME)
        mvaddch(proiettile.y, proiettile.x, 'o');

    print_rana(rana);

    // stampo il tempo
    mvprintw(STAMPA_STATISTICHE_Y, STAMPA_TEMPO_X, "TEMPO: %d", tempo_rimanente) ;

    // stampo il punteggio
    mvprintw(STAMPA_STATISTICHE_Y, STAMPA_SCORE_X, "SCORE: %d", score) ;

    // stampo vite rimanenti
    mvprintw(STAMPA_STATISTICHE_Y, 1, "VITE RIMANENTI: %d", vite) ;

    refresh();
}

void corsie_setter_fiume(corsia* fiumi_array){

    srand(getpid());
    // assegnazione corsie fiume
    fiumi_array[0].y = 8;
    fiumi_array[1].y = 11;
    fiumi_array[2].y = 14;
    fiumi_array[3].y = 17;

    // assegnazione velocità per corsia
    fiumi_array[0].speed = 50000;
    fiumi_array[1].speed = 56000;
    fiumi_array[2].speed = 45000;
    fiumi_array[3].speed = 40000;

    // assegnazione del senso random
    switch(rand()%2){
        case 0:
            fiumi_array[0].senso = 1;
            fiumi_array[1].senso = -1;
            fiumi_array[2].senso = 1;
            fiumi_array[3].senso = -1;
            break;
        case 1:
            fiumi_array[0].senso = -1;
            fiumi_array[1].senso = 1;
            fiumi_array[2].senso = -1;
            fiumi_array[3].senso = 1;
            break;
    }
}

void corsie_setter_macchine(corsia* strade_array){

    srand(getpid()) ; 
    // assegnazione corsie macchine
    strade_array[0].y = 23;
    strade_array[1].y = 26;
    strade_array[2].y = 29;
    strade_array[3].y = 32;
    strade_array[4].y = 35;
    
    // assegnazione del traffico a zero
    for(size_t i = 0; i < N_CORSIE ; i++){
        strade_array[i].traffico = 0 ; 
    }
    // assegnazione velocità 
    for(size_t i = 0; i < N_CORSIE ; i++){
        strade_array[i].speed = 50000 ; 
    }

    // assegnazione random del seme
    switch (rand()%2){
        case 0:
            strade_array[0].senso = 1 ;
            strade_array[1].senso = -1 ;
            strade_array[2].senso = 1 ;
            strade_array[3].senso = -1 ;
            strade_array[4].senso = 1 ;
            break;
        case 1:
            strade_array[0].senso = -1 ;
            strade_array[1].senso = 1 ;
            strade_array[2].senso = -1 ;
            strade_array[3].senso = 1 ;
            strade_array[4].senso = -1 ;
            break;
    }
}

void* tempo_function(){

    pthread_mutex_lock(&mutex) ;
    sem_post(&count) ;
    pthread_mutex_unlock(&mutex) ;

    //ogni secondo con la sleep viene decrementata la variabile tempo
    while(true){
        tempo_rimanente-- ;
        sleep(1) ;
    }
}