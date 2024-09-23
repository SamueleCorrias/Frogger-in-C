/* file del fiume: tronchi, nemici */

#include "header.h" /* andiamo a richiamare il file header.h */

void gestione_tronco(corsia fiume, int principale[2], int pipe_tronchi[2], int pipe_nemici[2], int pipe_segnaleproiettilenemici[2], int pipe_nemico_sul_tronco[2]){
    // inizializzazione random con il pid del processo come seed
    srand(getpid());

    obj tronco ; 

    bool sparo = false ; 
    bool collisione_proiettile_veicolo = false; 

    int max_spawn = LIM_DX_FIUME - DIM_TRONCO ;
    int timer_sparo_nemico = SPARO_NEMICO ; // timer per far sparare un nemico 

    int minRange = DIM_TRONCO * 2;
    int maxRange = BASE_AG - DIM_TRONCO * 2;

    tronco.x = minRange + rand() % (maxRange - minRange + 1); // la coordinata x del tronco è random
    tronco.y = fiume.y;
    tronco.flag = true;
    tronco.id = 't';

    if(fiume.y == 8) tronco.id_num = 0 ; 
    if(fiume.y == 11) tronco.id_num = 1 ; 
    if(fiume.y == 14) tronco.id_num = 2 ; 
    if(fiume.y == 17) tronco.id_num = 3 ; 
    write(pipe_tronchi[1], &tronco, sizeof(obj));


    while(true){ 

        // --- GESTIONE DEL MOVIMENTO DEL TRONCO ---
        tronco.x += fiume.senso; // movimento del tronco, il senso è stabilito all'inizio del programma

        //cambio di senso quando viene toccato uno dei bordi
        if(tronco.x == LIM_DX_FIUME - DIM_TRONCO-1 || tronco.x == LIM_SX_FIUME) fiume.senso *= -1;
        write(pipe_tronchi[1], &tronco, sizeof(obj));
        usleep(fiume.speed);


        read(pipe_nemico_sul_tronco[0], &tronco.enemy, sizeof(bool)) ; // leggo se è presente un nemico nel tronco
        
        if(tronco.enemy == true){ // se è presente il nemico

            timer_sparo_nemico -- ;
            if(timer_sparo_nemico == 0){

                timer_sparo_nemico = SPARO_NEMICO ;
                pid_t pid_proiettile_nemico ; // creazione del pid del proiettile
                obj proiettile_nemico ; 

                if(waitpid(-1, NULL, WNOHANG) != 0){ /* WNOHANG rende il waitpid non bloccante */

                    pid_proiettile_nemico = fork() ; 
                    write(pipe_nemici[1], &proiettile_nemico, sizeof(obj)) ;

                    if(pid_proiettile_nemico == 0){
                        
                        // posiziono il proiettile inizialmente sotto il nemico 
                        proiettile_nemico.x = tronco.x ; 
                        proiettile_nemico.y = tronco.y ; 
                        proiettile_nemico.id = 'n' ; 
                        proiettile_nemico.id_num = tronco.id_num ;

                        obj segnale_sparo ;
                        segnale_sparo.id = 'v' ; 
                        
                        write(pipe_nemici[1], &proiettile_nemico, sizeof(obj)) ; // viene scritto alla pipe per stampare la prima posizione 
                        
                        while(proiettile_nemico.y < I_MARCIAPIEDE-1){ // finché è dentro lo schermo viene fatto muovere
                        
                            read(pipe_segnaleproiettilenemici[0], &segnale_sparo, sizeof(obj)) ; // leggo dalla pipe la posizione del proiettile 
                            if(segnale_sparo.id == 'n' && segnale_sparo.id_num == proiettile_nemico.id_num){

                                segnale_sparo.id = 'v' ; 
                                proiettile_nemico.y = ALTEZZA_AG-1 ;
                            }

                            proiettile_nemico.y ++ ; // movimento del proiettile nemico 
                            write(pipe_nemici[1], &proiettile_nemico, sizeof(obj)) ; // comunicazione della posizione del proiettile nemico 
                            usleep(100000) ; 
                        }
                        _exit(0) ; // quando il proiettile esce dalla sua zona, viene killato
                    }
                }
            }
        }
    }
}

void log_printer(obj tronco){

    char* tronco_sprite[3] = {" ------------ ", "|  .  -:- -  |", " ------------ "};

    for(int i = 0; i < 3; i++)
    {
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