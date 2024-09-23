/* file dove vengono creati tutti i processi e funzione area di gioco */

#include "header.h" /* andiamo a richiamare il file header.h */

int main(){

    inizializza_ncurses() ; /* funzione che inizializza le funzioni di "ncurses" */
    start_color() ; /* funzione che attiva i colori della libreria */

    /* fa si che la pagina si apra a dimensioni del gioco in automatico */
    system("echo -e \"\e[8;41;85t\"") ; 

    menu();

    int vite = 3 ;
    int vite_rimosse = 0;
    int count_tane = 0; // contatore per le tane, per aggiudicarsi la vittoria
    bool tane[N_TANE] = {true, true, true, true, true} ;
    int score[1] = {0} ; 
    
    int pipe_veicoli[2];
    int pipe_collissioni_macchine[2] ; // pipe per le macchine
    int pipe_coda[2] ; // pipe per la coda delle macchine
    pipe(pipe_veicoli) ; 
    pipe(pipe_collissioni_macchine) ;
    pipe(pipe_coda);

    fcntl(pipe_collissioni_macchine[0], F_SETFL, fcntl(pipe_collissioni_macchine[0], F_GETFL) | O_NONBLOCK) ;
    fcntl(pipe_coda[0], F_SETFL, fcntl(pipe_coda[0], F_GETFL) | O_NONBLOCK) ;
    fcntl(pipe_veicoli[0], F_SETFL, fcntl(pipe_veicoli[0], F_GETFL) | O_NONBLOCK) ;

    // inizializzazione senso e velocità delle corsie per il macchine 
    corsia strade_array[N_CORSIE];
    corsie_setter_macchine(strade_array);
    pid_t pid_veicoli[N_MACCHINE] ; 
    for(size_t i = 0 ; i < N_MACCHINE; i ++){
        pid_veicoli[i] = fork(); 
        if(pid_veicoli[i] == 0) inizializzazione_veicoli(pipe_veicoli, pipe_collissioni_macchine, pipe_coda, i, strade_array) ; 
    }

    while(vite >= 0){

        // inizializzazione senso e velocità delle corsie per il fiume 
        corsia fiumi_array[N_TRONCHI];
        corsie_setter_fiume(fiumi_array);

        // inizializzazione senso e velocità delle corsie per il macchine 
        //corsia strade_array[N_CORSIE];
        //corsie_setter_macchine(strade_array);

        /* dichiarazione delle pipe */
        int principale[2] ;  /* creo pipe principale */
        int pipe_rana[2] ; /* pipe per la rana che muore */
        int pipe_proiettilerana[2] ; // pipe che comunica la posizione della rana nel proiettile rana 
        int pipe_troncorana[2] ; // pipe per la rana che va sul tronco 
        int pipe_tronchi[2] ; 
        //int pipe_veicoli[2];
        int pipe_nemici[N_TRONCHI][2] ; 
        int pipe_segnaleproiettilenemici[N_TRONCHI][2] ;
        //int pipe_collissioni_macchine[2] ; // pipe per le macchine
        //int pipe_coda[2] ; // pipe per la coda delle macchine
        int pipe_segnaleproittile_rana[2] ; // pipe per segnale le collissioni del proiettile della rana
        int pipe_nemico_sul_tronco[N_TRONCHI][2] ; 
        int pipe_segnalecollisione_proiettile[2] ;
        int pipe_time[2] ;

        /* creazione delle pipe */
        pipe(principale) ;
        pipe(pipe_rana) ;
        pipe(pipe_proiettilerana) ; 
        pipe(pipe_troncorana) ;
        pipe(pipe_tronchi) ; 
        //pipe(pipe_collissioni_macchine) ; 
        //pipe(pipe_coda) ; 
        //pipe(pipe_veicoli) ; 
        pipe(pipe_segnaleproittile_rana) ; 
        pipe(pipe_segnalecollisione_proiettile) ; 
        pipe(pipe_time) ; 

        for(size_t i = 0 ; i < N_TRONCHI ; i++){
            pipe(pipe_nemici[i]) ; // creazione pipe per i nemici
            pipe(pipe_segnaleproiettilenemici[i]) ; // creazione pipe per i proiettili nemici
            pipe(pipe_nemico_sul_tronco[i]) ; 
            fcntl(pipe_nemici[i][0], F_SETFL, fcntl(pipe_nemici[i][0], F_GETFL) | O_NONBLOCK) ;
            fcntl(pipe_segnaleproiettilenemici[i][0], F_SETFL, fcntl(pipe_segnaleproiettilenemici[i][0], F_GETFL) | O_NONBLOCK) ;
            fcntl(pipe_nemico_sul_tronco[i][0], F_SETFL, fcntl(pipe_nemico_sul_tronco[i][0], F_GETFL) | O_NONBLOCK) ;
        }

        /* alcune pipe vengono rese non bloccanti */
        fcntl(pipe_rana[0], F_SETFL, fcntl(pipe_rana[0], F_GETFL) | O_NONBLOCK) ;
        fcntl(pipe_proiettilerana[0], F_SETFL, fcntl(pipe_proiettilerana[0], F_GETFL) | O_NONBLOCK) ;
        fcntl(pipe_troncorana[0], F_SETFL, fcntl(pipe_troncorana[0], F_GETFL) | O_NONBLOCK) ;
        fcntl(pipe_tronchi[0], F_SETFL, fcntl(pipe_tronchi[0], F_GETFL) | O_NONBLOCK) ;
        //fcntl(pipe_collissioni_macchine[0], F_SETFL, fcntl(pipe_collissioni_macchine[0], F_GETFL) | O_NONBLOCK) ;
        //fcntl(pipe_coda[0], F_SETFL, fcntl(pipe_coda[0], F_GETFL) | O_NONBLOCK) ;
        //fcntl(pipe_veicoli[0], F_SETFL, fcntl(pipe_veicoli[0], F_GETFL) | O_NONBLOCK) ;
        fcntl(pipe_segnaleproittile_rana[0], F_SETFL, fcntl(pipe_segnaleproittile_rana[0], F_GETFL) | O_NONBLOCK) ;
        fcntl(pipe_segnalecollisione_proiettile[0], F_SETFL, fcntl(pipe_segnalecollisione_proiettile[0], F_GETFL) | O_NONBLOCK) ;
        fcntl(pipe_time[0], F_SETFL, fcntl(pipe_time[0], F_GETFL) | O_NONBLOCK) ;

        /* dichiarazione dei pid */
        pid_t pid_rana, pid_tronco1, pid_tronco2, pid_tronco3, pid_tronco4 ; 
        //pid_t pid_veicoli[N_MACCHINE] ; 
        pid_t pid_timer ; 

        pid_rana = fork();
        if(pid_rana == 0) funzione_rana(principale, pipe_rana, pipe_proiettilerana, pipe_troncorana, pipe_segnaleproittile_rana, pipe_segnalecollisione_proiettile) ; /* la funzione rana scrive le posizioni della rana nella pipe */
        
        pid_tronco1 = fork();
        if(pid_tronco1 == 0) gestione_tronco(fiumi_array[0], principale, pipe_tronchi, pipe_nemici[0], pipe_segnaleproiettilenemici[0], pipe_nemico_sul_tronco[0]) ;

        pid_tronco2 = fork();
        if(pid_tronco2 == 0) gestione_tronco(fiumi_array[1], principale, pipe_tronchi, pipe_nemici[1], pipe_segnaleproiettilenemici[1], pipe_nemico_sul_tronco[1]) ;

        pid_tronco3 = fork();
        if(pid_tronco3 == 0) gestione_tronco(fiumi_array[2], principale, pipe_tronchi, pipe_nemici[2], pipe_segnaleproiettilenemici[2], pipe_nemico_sul_tronco[2]) ;

        pid_tronco4 = fork();
        if(pid_tronco4 == 0) gestione_tronco(fiumi_array[3], principale, pipe_tronchi, pipe_nemici[3], pipe_segnaleproiettilenemici[3], pipe_nemico_sul_tronco[3]) ;
        
        

        pid_timer = fork();
        if(pid_timer == 0) timer(pipe_time, TEMPO);

        vite_rimosse = area_gioco(vite, tane, score, principale, pipe_rana, pipe_proiettilerana, pipe_troncorana, pipe_tronchi, pipe_nemici, pipe_segnaleproiettilenemici, pipe_collissioni_macchine, pipe_coda, pipe_veicoli, pipe_segnaleproittile_rana, pipe_nemico_sul_tronco, pipe_segnalecollisione_proiettile, pipe_time);
        
        vite = vite - vite_rimosse; 
        
        
        bool choice;
        if(vite == 0){ // in caso di game over
            choice = game_over_screen() ;
            
            if(choice == true){ // il gioco esce
                vite = -1 ;
            }
            else if(choice == false){ // il gioco continua 
                vite = 3 ;
                score[0] = 0 ; 
                for (size_t i = 0; i < N_TANE; i++)
                {
                    tane[i] = true;
                }
                
            }
        }

        wait_manche_screen() ;

        // uccisione di tutti i pid
        kill(pid_rana, SIGKILL) ;
        kill(pid_tronco1, SIGKILL) ;
        kill(pid_tronco2, SIGKILL) ;
        kill(pid_tronco3, SIGKILL) ;
        kill(pid_tronco4, SIGKILL) ;
        kill(pid_timer, SIGKILL) ;

    }

    /* blocco di uscita */
    for (int i = 0; i < N_MACCHINE; i++) {
        kill(pid_veicoli[i], SIGKILL) ;
    }
    endwin() ;
    exit(1) ;
    return 0 ;
}

int area_gioco(int vite, bool tane[N_TANE], int score[1], int principale[2], int pipe_rana[2], int pipe_proiettilerana[2], int pipe_troncorana[2], int pipe_tronchi[2], int pipe_nemici[N_TRONCHI][2], int pipe_segnaleproiettilenemici[N_TRONCHI][2], int pipe_collissioni_macchine[2], int pipe_coda[2], int pipe_veicoli[2],  int pipe_segnaleproittile_rana[2], int pipe_nemico_sul_tronco[N_TRONCHI][2], int pipe_segnalecollisione_proiettile[2], int pipe_time[2]){

    obj rana ;
    obj data ; /* data corrisponde a una struttura che mi serve per leggere il dato */
    obj proiettile ; 
    obj tronco[N_TRONCHI] ; 
    obj nemico[N_TRONCHI] ; 
    obj old_tronco[N_TRONCHI] ; 
    obj old_rana ; 
    obj log ; // struttura che ci serve solo per la lettura dalla pipe qualcosa --> poi scegliamo con id e id_num
    obj proiettile_nemico[N_TRONCHI] ;
    obj veicoli[N_MACCHINE] ; 
    obj vehicle ; 

    proiettile.x = -1 ;
    proiettile.y = -1 ; 
    proiettile.id = 'p' ; 

    bool manche = false ; 
    int vite_tolte = 0 ;

    bool collisione_proiettile_veicolo[N_TRONCHI] = {false}; 
    bool old_collisione[N_TRONCHI];

    int tane_riempite = 0 ; /* contatore delle tane riempite */
    int tane_riempite_prima = 0 ;

    int enemy_timer = 120+rand()%SPAWN_NEMICI ; // timer per la frequenza di spawn del nemico 

    int tempo_rimanente = TEMPO; 

    for(size_t i = 0 ; i < N_TRONCHI ; i ++) tronco[i].enemy = false ; // inizializzazione della variabile nemici di ogni tronco a false 

    controllo_generazione_macchine(pipe_veicoli, pipe_collissioni_macchine) ;
    
    while(manche == false){

        erase() ; /* che sarebbe come una clear() */
        
        read(principale[0], &data, sizeof(obj)) ; /* legge dalla pipe principale */
        read(pipe_proiettilerana[0], &proiettile, sizeof(obj)) ; /* legge dalla pipe del proiettile della rana */
        read(pipe_tronchi[0], &log, sizeof(obj)) ; 
        read(pipe_veicoli[0], &vehicle, sizeof(obj)) ; // legge dalla pipe per posizione delle macchine
        for(size_t i = 0 ; i < N_TRONCHI ; i ++){ // legge dalla pipe il proiettile nemico, nemico per nemico
            read(pipe_nemici[i][0], &proiettile_nemico[i], sizeof(obj)) ; 
        }
        read(pipe_time[0], &tempo_rimanente, sizeof(int)) ;

        bool end_game = false;
        bool sparo_nemico = false ; 

        sfondo_gioco(tane) ; // quando inizia il gioco, si chiama la funzione che lo colora 
        
        if(data.id == 'r') rana = data ;

        if(data.id == 'p') proiettile = data ;

        for(size_t i = 0 ; i < N_TRONCHI ; i ++){ // lettura dei tronchi
            if(log.id ==  't' && log.id_num == i){ 
                tronco[i].x = log.x ; // inizializzazione della posizione x dei tronchi
                tronco[i].y = log.y ; // inizializzazione della posizione y dei tronchi
            }
        }

        for(size_t i = 0 ; i < N_MACCHINE ; i ++){ // lettura delle macchine 
            if((vehicle.id == 'm' || vehicle.id == 'c') && vehicle.id_num == i){ 
                veicoli[i].x = vehicle.x ;
                veicoli[i].y = vehicle.y ; 
                veicoli[i].id = vehicle.id ; 
                veicoli[i].id_num = vehicle.id_num ;
            }
        }

        for(size_t i = 0 ; i < N_TRONCHI ; i ++){ // stampo i tronchi
            log_printer(tronco[i]); 
        }

        for(size_t i = 0 ; i < N_MACCHINE ; i ++){ // stampo le macchine 
            vehicle_printer(veicoli[i]) ;
        }

        // stampo la rana
        mvprintw(rana.y, rana.x, " _ _ ") ;
        mvprintw(rana.y+1, rana.x, " 0 0 ") ;
        mvprintw(rana.y+2, rana.x, "_( )_") ;

        // stampo il proiettile 
        if(proiettile.y > F_TANE) mvprintw(proiettile.y, proiettile.x, "o") ;

        // stampo i proiettili del nemici
        for(size_t i = 0 ; i < N_TRONCHI ; i++){
            if(!(proiettile_nemico[i].y >= 0 && proiettile_nemico[i].y <= 3) && (tronco[i].enemy == true)){
                mvprintw(proiettile_nemico[i].y, proiettile_nemico[i].x, "o") ;
            }
        }

        // stampo il tempo
        mvprintw(STAMPA_STATISTICHE_Y, STAMPA_TEMPO_X, "TEMPO: %d", tempo_rimanente) ;

        // stampo il punteggio
        mvprintw(STAMPA_STATISTICHE_Y, STAMPA_SCORE_X, "SCORE: %d", score[0]) ;

        // stampo vite rimanenti
        mvprintw(STAMPA_STATISTICHE_Y, 1, "VITE RIMANENTI: %d", vite) ;

        /* ----- CONTROLLO COLLISIONI TANE ----- */
        /* primo controllo: la rana è all'altezza delle tane */
        if(rana.y == Y_TANE){
            
            /* --- TANA UNO --- */
            if(rana.x >= I_TANA_UNO && rana.x <F_TANA_UNO){ /* controllo che la rana sia entrata nella prima tana */
                if(tane[0] == true){ /* se la flag di quella tana è true significa che è ancora vuota e quindi la rana la può chiudere non perdendo vite */
                    tane[0] = false ; /* la tana diventa riempita, nel caso la rana ci dovesse tornare perderebbe una vita */
                    tane_riempite ++ ; /* aumento il counter del numero di tane riempite */
                    vite_tolte = 0 ;
                }
            }

            /* --- TANA DUE --- */
            else if(rana.x >= I_TANA_DUE && rana.x < F_TANA_DUE){ /* controllo che la rana sia entrata nella prima tana */
                if(tane[1] == true){ /* se la flag di quella tana è true significa che è ancora vuota e quindi la rana la può chiudere non perdendo vite */
                    tane[1] = false ; /* la tana diventa riempita, nel caso la rana ci dovesse tornare perderebbe una vita */
                    tane_riempite ++ ; /* aumento il counter del numero di tane riempite */
                    vite_tolte = 0 ;
                }
            }

            /* --- TANA TRE --- */
            else if(rana.x >= I_TANA_TRE && rana.x < F_TANA_TRE){ /* controllo che la rana sia entrata nella prima tana */
                if(tane[2] == true){ /* se la flag di quella tana è true significa che è ancora vuota e quindi la rana la può chiudere non perdendo vite */
                    tane[2] = false ; /* la tana diventa riempita, nel caso la rana ci dovesse tornare perderebbe una vita */
                    tane_riempite ++ ; /* aumento il counter del numero di tane riempite */
                    vite_tolte = 0 ;
                }
            }

            /* --- TANA QUATTRO --- */
            else if(rana.x == I_TANA_QUATTRO){ /* controllo che la rana sia entrata nella prima tana */
                if(tane[3] == true){ /* se la flag di quella tana è true significa che è ancora vuota e quindi la rana la può chiudere non perdendo vite */
                    tane[3] = false ; /* la tana diventa riempita, nel caso la rana ci dovesse tornare perderebbe una vita */
                    tane_riempite ++ ; /* aumento il counter del numero di tane riempite */
                    vite_tolte = 0 ;
                }
            }

            /* --- TANA CINQUE --- */
            else if(rana.x >= I_TANA_CINQUE && rana.x < F_TANA_CINQUE){ /* controllo che la rana sia entrata nella prima tana */
                if(tane[4] == true){ /* se la flag di quella tana è true significa che è ancora vuota e quindi la rana la può chiudere non perdendo vite */
                    tane[4] = false ; /* la tana diventa riempita, nel caso la rana ci dovesse tornare perderebbe una vita */
                    tane_riempite ++ ; /* aumento il counter del numero di tane riempite */
                    vite_tolte = 0 ;
                }
            }

            if(tane_riempite == tane_riempite_prima){ // vuol dire che la rana è andata in un pezzo dove non doveva, viene tolta una vita
                vite_tolte = 1 ; 
            }
            tane_riempite_prima = tane_riempite ;
            manche = true ; // la rana torna al centro
        }
        
        /* --- CONTROLLO COLLISIONI CON IL FIUME --- */
        // se la rana scende dal fiume
        if(old_rana.y == tronco[3].y && rana.y == I_PRATO){
            rana.flag = true ; 
            write(pipe_troncorana[1], &rana, sizeof(obj)) ; 
            rana.flag = false ; 
        }

        if(rana.y >= I_FIUME && rana.y <= F_FIUME){ // se la rana è nella zona del fiume

            rana.flag = true ; // il flag è settato a tr ue perché la rana deve cambiare posizione con il tronco 

            for(size_t i = 0; i < N_TRONCHI ; i ++){
                if(tronco[i].x != old_tronco[i].x && rana.y == tronco[i].y){ //controllo che il tronco abbia cambiato posizione e che la rana sia su un certo tronco
                    rana.x = rana.x + tronco[i].x - old_tronco[i].x ; // spostamento della rana sul tronco
                    // controllo rana sui tronchi
                    if(rana.y == tronco[i].y && (rana.x < tronco[i].x || rana.x >= tronco[i].x + DIM_TRONCO)){ //rana scende dal tronco e cade sul fiume 

                        manche = true ; // la rana torna al centro
                        vite_tolte = 1 ;
                    }
                    tronco[i].flag = false; // serve a far capire al tronco che è occupato dalla rana
                    write(pipe_tronchi[1], &tronco[i], sizeof(obj)) ; 
                    write(pipe_troncorana[1], &rana, sizeof(obj)) ; 
                }
            }
        }
        // aggiorno le posizioni di tronco e rana 
        for(size_t i = 0;  i < N_TRONCHI ; i ++){
            old_tronco[i] = tronco[i] ; 
        }
        old_rana = rana ; 

        // --- CONTROLLO COLLISIONI TRONCO, NEMICI E COMPANY --- 
        int spawn_in_log = rand()%N_TRONCHI ; // su quale tronco deve spawnare 
        enemy_timer -- ; // decremento per lo spawn casuale 
        
        if(rana.y != tronco[spawn_in_log].y && enemy_timer <= 0){ // condizione per far spawnare un nemico su un tronco 
            tronco[spawn_in_log].enemy = true ; // ci dice che il nemico è stato creato in un determinato tronco 
            write(pipe_nemico_sul_tronco[spawn_in_log][1], &tronco[spawn_in_log].enemy, sizeof(bool)) ; 
            enemy_timer = 120+rand()%SPAWN_NEMICI ; // resetta il timer per lo spawn del nemico
        } 

        for(size_t i = 0;  i < N_TRONCHI ; i ++){ // condizione per stampare i nemici sui tronchi
            if(tronco[i].enemy == true){ 
                enemy_printer(tronco[i]) ; // stampo i nemici sui tronchi se ci sono
            }
        }

        for(size_t i = 0;  i < N_TRONCHI ; i ++){ // condizione per vedere se la rana tocca un tronco con dei nemici
            if(tronco[i].enemy == true && rana.y == tronco[i].y){
                // la rana torna al centro
                manche = true ; // la rana torna al centro
                vite_tolte = 1 ;
            }
        }

        for(size_t i = 0;  i < N_TRONCHI ; i ++){ // condizione per uccidere un nemico
            if(proiettile.y == tronco[i].y && (proiettile.x >= tronco[i].x && proiettile.x < tronco[i].x + 3)){ 
                score[0] += 100 ;
                tronco[i].enemy = false ; // condizione che uccide il nemico
                write(pipe_nemico_sul_tronco[i][1], &tronco[i].enemy, sizeof(bool)) ; 

                proiettile.y = F_TANE + 1; // condizione che killa il proiettile
                write(pipe_segnalecollisione_proiettile[1], &proiettile, sizeof(obj)) ;
            }
        } 

        for(size_t i = 0 ; i < N_TRONCHI ; i ++){ // killare il proiettile nemico se arriva alla fine dell'autostrada senza prendere macchine
            if(proiettile_nemico[i].y == F_AUTOSTRADA - 1){
                write(pipe_segnaleproiettilenemici[i][1], &proiettile_nemico[i], sizeof(obj)) ;
                proiettile_nemico[i].y = ALTEZZA_AG ;
            }
        }

        for(size_t i = 0 ; i < N_TRONCHI ;  i++){ // controllo collisione proiettile nemico --> rana
            if((proiettile_nemico[i].y == rana.y) && (proiettile_nemico[i].x >= rana.x-1 && proiettile_nemico[i].x <= rana.x+5)){
                // la rana torna al centro
                vite_tolte = 1 ;
                manche = true ; // la rana torna al centro
            }
        }

        // --- CONTROLLO COLLISSIONI AUTO, RANA E PROIETTILI ---
        // se la rana tocca una macchina --> perde una vita
        if(rana.y >= I_AUTOSTRADA && rana.y <= F_AUTOSTRADA){ // se la rana è nella zona delle macchine 

            for(size_t i = 0 ; i < N_MACCHINE ; i++){
                if(veicoli[i].id == 'm'){ // se va a sbattere con una macchina
                    if((rana.x >= veicoli[i].x && rana.x <= veicoli[i].x + DIM_MACCHINA) && rana.y == veicoli[i].y){
                        // la rana torna al centro
                        vite_tolte = 1 ;
                        manche = true ; // la rana torna al centro
                    }else if(rana.x + DIM_RANA == veicoli[i].x && rana.y == veicoli[i].y){
                        // la rana torna al centro
                        vite_tolte = 1 ;
                        manche = true ; // la rana torna al centro
                    }
                }
                else if(veicoli[i].id == 'c'){ // se va a sbattere con un camion
                    if((rana.x >= veicoli[i].x && rana.x <= veicoli[i].x + DIM_CAMION) && rana.y == veicoli[i].y){
                        // la rana torna al centro
                        vite_tolte = 1 ;
                        manche = true ; // la rana torna al centro
                    }else if(rana.x + DIM_RANA == veicoli[i].x && rana.y == veicoli[i].y){
                        // la rana torna al centro
                        vite_tolte = 1 ;
                        manche = true ; // la rana torna al centro
                    }
                }
            }
        }
        
        // se un proiettile nemico tocca una macchina --> il proiettile viene distrutto
        for(size_t i = 0 ; i < N_MACCHINE ; i ++){ // for per i veicoli
            for(size_t j = 0 ; j < N_TRONCHI ; j ++){ // for per i tronchi
                if(proiettile_nemico[j].x >= veicoli[i].x && ((veicoli[i].id == 'm' && proiettile_nemico[j].x <= veicoli[i].x + DIM_MACCHINA) || (veicoli[i].id == 'c' && proiettile_nemico[j].x <= veicoli[i].x + DIM_CAMION)) && (proiettile_nemico[j].y >= veicoli[i].y && proiettile_nemico[j].y <= veicoli[i].y + 3)){
                    write(pipe_segnaleproiettilenemici[j][1], &proiettile_nemico[j], sizeof(obj)) ;
                    proiettile_nemico[j].y = ALTEZZA_AG ;
                }
            }
        }

        // se un proiettile della rana tocca una macchina --> il proiettile viene distrutto 
        if(proiettile.y >= I_AUTOSTRADA && proiettile.y <= F_AUTOSTRADA){ // se il proiettile è compreso nella zona delle auto
            for(size_t i = 0 ; i < N_MACCHINE ; i ++){ // contatore per le macchine 
                if(proiettile.x >= veicoli[i].x && ((veicoli[i].id == 'm' && proiettile.x <= veicoli[i].x + DIM_MACCHINA) || (veicoli[i].id == 'c' && proiettile.x <= veicoli[i].x + DIM_CAMION)) && (proiettile.y >= veicoli[i].y && proiettile.y <= veicoli[i].y + 3)){
                    proiettile.y = F_TANE + 1;
                    write(pipe_segnalecollisione_proiettile[1], &proiettile, sizeof(obj)) ; 
                }
            }
        }
            
        // se il proiettile della rana collide con uno dei proiettili nemici
        for(size_t i = 0 ; i < N_TRONCHI ; i++){ // contatore per i proiettili nemici
            if(proiettile.y == proiettile_nemico[i].y && proiettile.x == proiettile_nemico[i].x){
                score[0] += 200 ;
                // condizione che killa il proiettile della rana
                proiettile.y = F_TANE + 1;
                write(pipe_segnalecollisione_proiettile[1], &proiettile, sizeof(obj)) ; 
                // condizione che killa i proiettili nemici
                proiettile_nemico[i].y = ALTEZZA_AG ;
                write(pipe_segnaleproiettilenemici[i][1], &proiettile_nemico[i], sizeof(obj)) ;
            }
        }

        // --- CONTROLLO DEL TEMPO ---
        if(tempo_rimanente == 0){
            vite_tolte = 1 ;
            manche = true ; // la rana torna al centro
        }


        if(tane_riempite == N_TANE){ // in caso di vittoria 
            win_screen(vite, score) ;
        }

        refresh() ;
    }
    if(vite_tolte == 0) score[0] += (tempo_rimanente * 15) ; //somma punteggio se la rana ha toccato una tana
    return vite_tolte;
}

/* inizializza tutte le corsie */
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

void timer(int pipe_time[2], int tempo){
    int tempo_rimanente;
    tempo_rimanente = tempo;
    close(pipe_time[0]);

    for (size_t i = 0 ; i <= tempo ; i++){

        write(pipe_time[1], &tempo_rimanente, sizeof(int));
        tempo_rimanente --;
        sleep(1);
    }
}
