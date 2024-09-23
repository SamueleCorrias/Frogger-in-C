/* file del movimento della rana */

#include "header.h" /* andiamo a richiamare il file header.h */

void funzione_rana(int principale[2], int pipe_rana[2], int pipe_proiettilerana[2], int pipe_troncorana[2], int pipe_segnaleproiettile_rana[2], int pipe_segnalecollisione_proiettile[2]){

    obj rana = {POS_X_RANA, POS_Y_RANA, 'r'} ; /* seto posizione iniziale */
	pid_t pid_proiettile ; /* pid per la creazione del processo proiettile */

	bool rana_partenza = false ; 
	int tasti ;

	/* funzione per rendere il wgetch non bloccante --> lo si rende più veloce*/
	nodelay(stdscr, TRUE);
	timeout(1);

    while(true){
		
		read(pipe_rana[0], &rana_partenza, sizeof(bool)) ;
		read(pipe_troncorana[0], &rana, sizeof(obj)) ;  // sovrascrive la posizione della rana con quella dello spostamento del tronco

		if(rana_partenza == true){
			rana.x = POS_X_RANA ; 
			rana.y = POS_Y_RANA ; 
			rana.id = 'r' ; 	
		}

		rana_partenza = false ;
		tasti = wgetch(stdscr);

        switch(tasti) {
			case KEY_UP: 
				if(rana.y > LIM_SUP_RANA) rana.y -= 3 ;
				break ;
			case KEY_DOWN:
				if(rana.y < LIM_INF_RANA) rana.y += 3 ;
				break ;
			case KEY_LEFT:
				if(rana.x > LIM_SX_RANA) rana.x -= 1 ;
				break ;
			case KEY_RIGHT:
				if(rana.x < LIM_DX_RANA) rana.x += 1 ;
				break ;
			case SPACE_BAR : /* spara proiettile --> creazione di un proiettile se non c'è ne sono altri */
				if(waitpid(-1, NULL, WNOHANG) != 0){ /* WNOHANG rende il waitpid non bloccante */
					
					pid_proiettile = fork() ; 
					write(pipe_proiettilerana[1], &rana, sizeof(obj)) ; 

					if(pid_proiettile == 0){
						proiettile(principale, pipe_proiettilerana, pipe_proiettilerana, pipe_segnalecollisione_proiettile) ; 
					}
					break ; 
				}
		}
		close(principale[0]) ;
        write(principale[1], &rana, sizeof(obj)) ;
    }
}

void proiettile(int principale[2], int pipe_proiettilerana[2], int pipe_segnaleproittile_rana[2], int pipe_segnalecollisione_proiettile[2]){

    obj proiettile ; 
    obj rana ; 
	obj data ; 

    /* legge dalla rana la sua posizione */
    read(pipe_proiettilerana[0], &rana, sizeof(obj)) ;

    /* proiettile posizionato sopra la rana */
    proiettile.x = rana.x + 2; 
    proiettile.y = rana.y ;
    proiettile.id = 'p' ; 

    /* viene scritto inizialmente nella pipe principale per stampare la prima posizione */
    write(principale[1], &proiettile, sizeof(obj)) ;

    /* finché è dentro lo schermo viene fatto muovere */
    while(proiettile.y >= F_TANE){

        /* movimento normale */
        write(principale[1], &proiettile, sizeof(obj)) ;
		read(pipe_segnalecollisione_proiettile[0], &proiettile, sizeof(obj)) ;

        proiettile.y-- ; 
        
        usleep(10000) ; 
    }

    /* quando il proiettile esce dallo schermo, il processo viene killato */
    _exit(0) ; 
}

