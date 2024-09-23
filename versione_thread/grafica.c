#include "header.h"
#include "global.h"

/* funzione che inizializza le funzioni di "ncurses" */
void inizializza_ncurses(){

    int maxx, maxy ; 
    getmaxyx(stdscr, maxy, maxx) ;
    
    initscr() ;
    noecho() ;
    keypad(stdscr, 1) ;
    curs_set(0) ;
}

void win_screen(){
    int maxx, maxy ;
    getmaxyx(stdscr, maxy, maxx) ; 

    inizializza_ncurses() ; /* funzione che inizializza le funzioni di "ncurses" */
    start_color() ; /* funzione che attiva i colori della libreria */

    erase() ;

    init_pair(8, COLOR_GREEN, COLOR_GREEN) ; /* colore per lo sfondo*/
    init_pair(9, COLOR_BLACK, COLOR_GREEN) ; /* colore della scritta */

    bkgd(COLOR_PAIR(8)) ;

    attrset(COLOR_PAIR(9)) ;
    attron(A_BOLD) ; // imposto la scritta in grassetto

    mvprintw(maxy/2, (maxx/2), "HAI VINTO!") ;
    mvprintw((maxy/2)+1, (maxx/2), "VITE: %d", vite) ;
    mvprintw((maxy/2)+2, (maxx/2), "SCORE: %d", score) ;
    
    refresh() ;

    sleep(5);
}

void sfondo_gioco(){
    int maxx, maxy ;
    getmaxyx(stdscr, maxy, maxx) ; 

    erase() ;

    inizializza_ncurses() ; /* funzione che inizializza le funzioni di "ncurses" */
    start_color() ; /* funzione che attiva i colori della libreria */

    /* inizializzo i colori */
    init_pair(1, COLOR_WHITE, COLOR_BLACK) ; /* colore bordo superiore */
    init_pair(2, COLOR_YELLOW, COLOR_YELLOW) ; /* colore per le tane */
    init_pair(3, COLOR_BLUE, COLOR_BLUE) ; /* colore per il fiume */
    init_pair(4, COLOR_BLUE, COLOR_YELLOW) ; /* colore per le tane occupate */
    init_pair(5, COLOR_GREEN, COLOR_GREEN) ; /* colore per il prato */
    init_pair(6, COLOR_WHITE, COLOR_WHITE) ; /* colore per il marciapiede */

    bkgd(COLOR_PAIR(1)) ; 
    /* si colorano le tane inizialmente --> vengono colorate linea per linea */
    attron(COLOR_PAIR(2)) ;
    for(size_t y = I_TANE ; y <= F_TANE-3 ; y ++) mvhline(y, 0, CARATTERE_VUOTO, BASE_AG) ; /* mi colora le prime due righe delle tane */
    /* colora le tane */
    for(int y = 5 ; y <= F_TANE; y ++){

        /* --- TANA UNO --- */
        /* coloro la sorta di prato prima delle tane */
        attron(COLOR_PAIR(2)) ;
        mvhline(y, 0, CARATTERE_VUOTO, I_TANA_UNO) ;
        attroff(COLOR_PAIR(2)) ;
        
        if(tane[0] == true){ /* la tana è libera */
            attron(COLOR_PAIR(3)) ; 
            mvhline(y, I_TANA_UNO, CARATTERE_VUOTO, F_TANA_UNO) ; 
            attroff(COLOR_PAIR(3)) ; 
        }
        else{ /* la tana è già stata conquistata */
            attron(COLOR_PAIR(4)) ; 
            mvhline(y, I_TANA_UNO, CARATTERE_TANE, F_TANA_UNO) ; 
            attroff(COLOR_PAIR(4)) ; 
        }
        
        /* --- TANA DUE --- */
        /* coloro la sorta di prato delle tane */
        attron(COLOR_PAIR(2)) ;
        mvhline(y, F_TANA_UNO+1, CARATTERE_VUOTO, I_TANA_DUE-1) ;
        attroff(COLOR_PAIR(2)) ;
        
        if(tane[1] == true){ /* la tana è libera */
            attron(COLOR_PAIR(3)) ; 
            mvhline(y, I_TANA_DUE, CARATTERE_VUOTO, F_TANA_DUE) ; 
            attroff(COLOR_PAIR(3)) ; 
        }
        else{ /* la tana è già stata conquistata */
            attron(COLOR_PAIR(4)) ; 
            mvhline(y, I_TANA_DUE, CARATTERE_TANE, F_TANA_DUE) ; 
            attroff(COLOR_PAIR(4)) ; 
        }

        /* --- TANA TRE --- */
        /* coloro la sorta di prato delle tane */
        attron(COLOR_PAIR(2)) ;
        mvhline(y, F_TANA_DUE+1, CARATTERE_VUOTO, I_TANA_TRE-1) ;
        attroff(COLOR_PAIR(2)) ;
        
        if(tane[2] == true){ /* la tana è libera */
            attron(COLOR_PAIR(3)) ; 
            mvhline(y, I_TANA_TRE, CARATTERE_VUOTO, F_TANA_TRE) ; 
            attroff(COLOR_PAIR(3)) ; 
        }
        else{ /* la tana è già stata conquistata */
            attron(COLOR_PAIR(4)) ; 
            mvhline(y, I_TANA_TRE, CARATTERE_TANE, F_TANA_TRE) ; 
            attroff(COLOR_PAIR(4)) ; 
        }

        /* --- TANA QUATTRO --- */
        /* coloro la sorta di prato delle tane */
        attron(COLOR_PAIR(2)) ;
        mvhline(y, F_TANA_TRE+1, CARATTERE_VUOTO, I_TANA_QUATTRO-1) ;
        attroff(COLOR_PAIR(2)) ;
        
        if(tane[3] == true){ /* la tana è libera */
            attron(COLOR_PAIR(3)) ; 
            mvhline(y, I_TANA_QUATTRO, CARATTERE_VUOTO, F_TANA_QUATTRO) ; 
            attroff(COLOR_PAIR(3)) ; 
        }
        else{ /* la tana è già stata conquistata */
            attron(COLOR_PAIR(4)) ; 
            mvhline(y, I_TANA_QUATTRO, CARATTERE_TANE, F_TANA_QUATTRO) ; 
            attroff(COLOR_PAIR(4)) ; 
        }

        /* --- TANA CINQUE --- */
        /* coloro la sorta di prato delle tane */
        attron(COLOR_PAIR(2)) ;
        mvhline(y, F_TANA_QUATTRO+1, CARATTERE_VUOTO, I_TANA_CINQUE-1) ;
        attroff(COLOR_PAIR(2)) ;
        
        if(tane[4] == true){ /* la tana è libera */
            attron(COLOR_PAIR(3)) ; 
            mvhline(y, I_TANA_CINQUE, CARATTERE_VUOTO, F_TANA_CINQUE) ; 
            attroff(COLOR_PAIR(3)) ; 
        }
        else{ /* la tana è già stata conquistata */
            attron(COLOR_PAIR(4)) ; 
            mvhline(y, I_TANA_CINQUE, CARATTERE_TANE, F_TANA_CINQUE) ; 
            attroff(COLOR_PAIR(4)) ; 
        }

        attron(COLOR_PAIR(2)) ;
        mvhline(y, F_TANA_CINQUE+1, CARATTERE_VUOTO, BASE_AG) ; 
        attroff(COLOR_PAIR(2)) ;
    }

    /* coloro il fiume */
    attron(COLOR_PAIR(3)) ;
    for(size_t y = I_FIUME ; y <= F_FIUME ; y ++) mvhline(y, 0, CARATTERE_VUOTO, BASE_AG) ;
    attroff(COLOR_PAIR(3)) ; 

    /* coloro il prato */
    attron(COLOR_PAIR(5)) ;
    for(size_t y = I_PRATO ; y <= F_PRATO ; y ++) mvhline(y, 0, CARATTERE_VUOTO, BASE_AG) ;
    attroff(COLOR_PAIR(5)) ; 

    /* l'autostrada la si lascia nera perché rende di più */

    /* coloro il marciapiede */
    attron(COLOR_PAIR(6)) ;
    for(size_t y = I_MARCIAPIEDE ; y <= F_MARCIAPIEDE ; y ++) mvhline(y, 0, CARATTERE_VUOTO, BASE_AG) ;
    attroff(COLOR_PAIR(6)) ; 

}

bool game_over_screen(){
    int maxx, maxy ;
    getmaxyx(stdscr, maxy, maxx) ; 

    inizializza_ncurses() ; /* funzione che inizializza le funzioni di "ncurses" */
    start_color() ; /* funzione che attiva i colori della libreria */

    erase() ;

    init_pair(10, COLOR_RED, COLOR_RED) ;
    init_pair(11, COLOR_WHITE, COLOR_RED) ; /* colore della scritta */

    bkgd(COLOR_PAIR(10)) ;

    attrset(COLOR_PAIR(11)) ;
    attron(A_BOLD) ; // imposto la scritta in grassetto

    mvprintw(maxy/2, (maxx/2)-5, "GAME OVER!") ;
    mvprintw((maxy/2)+2, (maxx/2)-5 , "SCORE: %d", score) ;
    

    mvprintw((maxy/2)+5, (maxx/2)-30, "--- SE VUOI CONTINUARE A GIOCARE PREMI LA BARRA SPAZIATRICE ---") ;
    mvprintw((maxy/2)+6, (maxx/2)-15, "--- ALTRIMENTI PREMI ' q ' ---") ;

    refresh();

    int ch;
    while ((ch = getch()) != ' ' && ch != 'q') {
        // aspetta la scelta del giocatore
    }

    // Check which key was pressed
    if (ch == ' ') {
        return true;
    } else if (ch == 'q') {
        return false;
    }

    return 0;
}

void menu(){
    inizializza_ncurses();
    start_color();
    init_pair(MENU, COLOR_BLACK, COLOR_MAGENTA);
    bkgd(COLOR_PAIR(MENU));
    attron(A_BOLD);
    mvwprintw(stdscr, Y_FINE_MARCIAPIEDE_META, 14, "Premi la barra spaziatrice per iniziare a giocare!");
    mvwprintw(stdscr, Y_FINE_MARCIAPIEDE_META+2, 29, "Premi 'q' per uscire");
	scritta_frogger();
    disegno_rana();
	attroff(COLOR_PAIR(MENU));

    refresh();

    int c = getch();
    switch (c)
    {
        case 32:
			clear();
            info();
            break;

        case 113:
			clear();
			endwin();
            _exit(1);
            break;
    }

}

void scritta_frogger(){

    int start_y = Y_FINE_MARCIAPIDE_SOTTO;
    int start_x = X_INIZIO_AG+16;

    init_pair(13, COLOR_YELLOW, COLOR_YELLOW);

    //stampa la F
    attron(COLOR_PAIR(13));
    mvaddch(start_y,start_x,' ');
        mvaddch(start_y,start_x+1,' ');
        mvaddch(start_y,start_x+2,' ');
        mvaddch(start_y,start_x+3,' ');

    mvaddch(start_y+1,start_x,' ');
    mvaddch(start_y+2,start_x,' ');
        mvaddch(start_y+2,start_x+1,' ');
        mvaddch(start_y+2,start_x+2,' ');

    mvaddch(start_y+3,start_x,' ');
    mvaddch(start_y+4,start_x,' ');
    mvaddch(start_y+5,start_x,' ');
    mvaddch(start_y+6,start_x,' ');

    start_x += 6; 

    //stampo la lettera R

    mvaddch(start_y, start_x, ' ');
        mvaddch(start_y, start_x+1, ' ');
        mvaddch(start_y, start_x+2, ' ');
        mvaddch(start_y, start_x+3, ' ');
    
    mvaddch(start_y+1, start_x, ' ');
        mvaddch(start_y+1, start_x+3, ' ');

    mvaddch(start_y+2, start_x, ' ');
        mvaddch(start_y+2, start_x+3, ' ');

    mvaddch(start_y+3, start_x, ' ');
        mvaddch(start_y+3, start_x+1, ' ');
        mvaddch(start_y+3, start_x+2, ' ');
        mvaddch(start_y+3, start_x+3, ' ');
    
    mvaddch(start_y+4, start_x, ' ');
        mvaddch(start_y+4, start_x+2, ' ');

    mvaddch(start_y+5, start_x, ' ');
        mvaddch(start_y+5, start_x+3, ' ');

    mvaddch(start_y+6, start_x, ' ');
        mvaddch(start_y+6, start_x+3, ' ');

    start_x += 6; 

    //stampo la lettera o


    mvaddch(start_y, start_x+1, ' ');
        mvaddch(start_y, start_x+2, ' ');
    
    mvaddch(start_y+1, start_x, ' ');
        mvaddch(start_y+1, start_x+3, ' '); 
    mvaddch(start_y+2, start_x, ' ');
        mvaddch(start_y+2, start_x+3, ' ');
    mvaddch(start_y+3, start_x, ' ');
        mvaddch(start_y+3, start_x+3, ' ');
    mvaddch(start_y+4, start_x, ' ');
        mvaddch(start_y+4, start_x+3, ' ');
    mvaddch(start_y+5, start_x, ' ');
        mvaddch(start_y+5, start_x+3, ' ');

    mvaddch(start_y+6, start_x+1, ' ');
        mvaddch(start_y+6, start_x+2, ' ');

    start_x += 6; 


    //stampo la lettera G

    mvaddch(start_y, start_x+1, ' ');
        mvaddch(start_y, start_x+2, ' ');   
    
    mvaddch(start_y+1, start_x, ' ');
        mvaddch(start_y+1, start_x+3, ' ');

    mvaddch(start_y+2, start_x, ' ');
    mvaddch(start_y+3, start_x, ' ');

    mvaddch(start_y+4, start_x, ' ');
        mvaddch(start_y+4, start_x+2, ' ');
        mvaddch(start_y+4, start_x+3, ' ');

    mvaddch(start_y+5, start_x, ' ');
        mvaddch(start_y+5, start_x+3, ' ');

    mvaddch(start_y+6, start_x+1, ' ');
        mvaddch(start_y+6, start_x+2, ' ');  



    start_x += 6; 


    //stampo la lettera G

    mvaddch(start_y, start_x+1, ' ');
        mvaddch(start_y, start_x+2, ' ');   
    
    mvaddch(start_y+1, start_x, ' ');
        mvaddch(start_y+1, start_x+3, ' ');

    mvaddch(start_y+2, start_x, ' ');
    mvaddch(start_y+3, start_x, ' ');

    mvaddch(start_y+4, start_x, ' ');
        mvaddch(start_y+4, start_x+2, ' ');
        mvaddch(start_y+4, start_x+3, ' ');

    mvaddch(start_y+5, start_x, ' ');
        mvaddch(start_y+5, start_x+3, ' ');

    mvaddch(start_y+6, start_x+1, ' ');
        mvaddch(start_y+6, start_x+2, ' ');  



    start_x += 6; 

    //stampo la lettera E

    mvaddch(start_y, start_x, ' ');
        mvaddch(start_y, start_x+1, ' ');
        mvaddch(start_y, start_x+2, ' ');
        mvaddch(start_y, start_x+3, ' ');
    
    mvaddch(start_y+1, start_x, ' ');
    mvaddch(start_y+2, start_x, ' ');

    mvaddch(start_y+3, start_x, ' ');
        mvaddch(start_y+3, start_x+1, ' ');
        mvaddch(start_y+3, start_x+2, ' ');
    
    mvaddch(start_y+4, start_x, ' ');
    mvaddch(start_y+5, start_x, ' ');

    mvaddch(start_y+6, start_x, ' ');
        mvaddch(start_y+6, start_x+1, ' ');
        mvaddch(start_y+6, start_x+2, ' ');
        mvaddch(start_y+6, start_x+3, ' ');

    start_x += 6;

    //stampo la lettera R

    mvaddch(start_y, start_x, ' ');
        mvaddch(start_y, start_x+1, ' ');
        mvaddch(start_y, start_x+2, ' ');
        mvaddch(start_y, start_x+3, ' ');
    
    mvaddch(start_y+1, start_x, ' ');
        mvaddch(start_y+1, start_x+3, ' ');

    mvaddch(start_y+2, start_x, ' ');
        mvaddch(start_y+2, start_x+3, ' ');

    mvaddch(start_y+3, start_x, ' ');
        mvaddch(start_y+3, start_x+1, ' ');
        mvaddch(start_y+3, start_x+2, ' ');
        mvaddch(start_y+3, start_x+3, ' ');
    
    mvaddch(start_y+4, start_x, ' ');
        mvaddch(start_y+4, start_x+2, ' ');

    mvaddch(start_y+5, start_x, ' ');
        mvaddch(start_y+5, start_x+3, ' ');

    mvaddch(start_y+6, start_x, ' ');
        mvaddch(start_y+6, start_x+3, ' ');


    attroff(COLOR_PAIR(13));
}

void info(){
    inizializza_ncurses();
    start_color();
    init_pair(INFO, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(INFO_OK, COLOR_WHITE, COLOR_MAGENTA);

    bkgd(COLOR_PAIR(INFO));
    attron(A_BOLD);

    mvwprintw(stdscr, Y_FINE_MARCIAPIEDE_META, 14, "Frogger: Attraversa strade e fiumi evitando ostacoli.");
    mvwprintw(stdscr, Y_FINE_MARCIAPIEDE_META + 1, 14, "Ogni attraversamento con successo guadagna punti.");
    mvwprintw(stdscr, Y_FINE_MARCIAPIEDE_META + 2, 14, "Tre vite disponibili, evita auto, camion, e cadute nell'acqua.");
    mvwprintw(stdscr, Y_FINE_MARCIAPIEDE_META + 6, 14, "Premi qualsiasi tasto per iniziare a giocare!");
    attroff(COLOR_PAIR(INFO));
    
    
    scritta_frogger();
    disegno_rana();
    

    refresh();

    int c = getch();
    switch (c)
    {
        case 32:
            clear();
            break;
    }

}

void disegno_rana(){

    int start_y = 4;
    int start_x = 39;

    init_pair(BORDI_PAIR, COLOR_BLACK, COLOR_BLACK);
    init_pair(CORONA, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(FACCIA_RANA, COLOR_GREEN, COLOR_GREEN);
    init_pair(OCCHI_RANA, COLOR_WHITE, COLOR_WHITE);

    /*stampo bordi per la corona*/
    attron(COLOR_PAIR(BORDI_PAIR));
    mvaddch(start_y, start_x, ' ');
    mvaddch(start_y+1, start_x-1, ' ');
    mvaddch(start_y+1, start_x-4, ' ');
    mvaddch(start_y+1, start_x-5, ' ');
    mvaddch(start_y+1, start_x+1, ' ');
    mvaddch(start_y+1, start_x+3, ' ');
    mvaddch(start_y+1, start_x+4, ' ');
    mvaddch(start_y+2, start_x-2, ' ');
    mvaddch(start_y+2, start_x-3, ' ');
    mvaddch(start_y+2, start_x-6, ' ');
    mvaddch(start_y+2, start_x+2, ' ');
    mvaddch(start_y+2, start_x+5, ' ');
    mvaddch(start_y+3, start_x-5, ' ');
    mvaddch(start_y+3, start_x+4, ' ');
    mvaddch(start_y+4, start_x-5, ' ');
    mvaddch(start_y+4, start_x+4, ' ');
    mvaddch(start_y+5, start_x-4, ' ');
    mvaddch(start_y+5, start_x+3, ' ');
    mvaddch(start_y+6, start_x-4, ' ');
    mvaddch(start_y+6, start_x+3, ' ');
    mvaddch(start_y+6, start_x-3, ' ');
    mvaddch(start_y+6, start_x-2, ' ');
    mvaddch(start_y+6, start_x-1, ' ');
    mvaddch(start_y+6, start_x, ' ');
    mvaddch(start_y+6, start_x+1, ' ');
    mvaddch(start_y+6, start_x+2, ' ');
    /*stampo colore per la corona*/
    attron(COLOR_PAIR(CORONA));
    mvaddch(start_y+1, start_x, ' ');
    mvaddch(start_y+2, start_x-5, ' ');
    mvaddch(start_y+2, start_x-4, ' ');
    mvaddch(start_y+2, start_x-1, ' ');
    mvaddch(start_y+2, start_x, ' ');
    mvaddch(start_y+2, start_x+1, ' ');
    mvaddch(start_y+2, start_x+3, ' ');
    mvaddch(start_y+2, start_x+4, ' ');
    mvaddch(start_y+3, start_x-4, ' ');
    mvaddch(start_y+3, start_x-3, ' ');
    mvaddch(start_y+3, start_x-2, ' ');
    mvaddch(start_y+3, start_x-1, ' ');
    mvaddch(start_y+3, start_x, ' ');
    mvaddch(start_y+3, start_x+1, ' ');
    mvaddch(start_y+3, start_x+2, ' ');
    mvaddch(start_y+3, start_x+3, ' ');
    mvaddch(start_y+4, start_x-4, ' ');
    mvaddch(start_y+4, start_x-3, ' ');
    mvaddch(start_y+4, start_x-2, ' ');
    mvaddch(start_y+4, start_x-1, ' ');
    mvaddch(start_y+4, start_x, ' ');
    mvaddch(start_y+4, start_x+1, ' ');
    mvaddch(start_y+4, start_x+2, ' ');
    mvaddch(start_y+4, start_x+3, ' ');
    mvaddch(start_y+5, start_x-3, ' ');
    mvaddch(start_y+5, start_x-2, ' ');
    mvaddch(start_y+5, start_x-1, ' ');
    mvaddch(start_y+5, start_x, ' ');
    mvaddch(start_y+5, start_x+1, ' ');
    mvaddch(start_y+5, start_x+2, ' ');

    /*stampo bordi per la rana*/
    attron(COLOR_PAIR(BORDI_PAIR));
    mvaddch(start_y+6, start_x-5, ' ');
    mvaddch(start_y+6, start_x+4, ' ');
    mvaddch(start_y+6, start_x-11, ' ');
    mvaddch(start_y+6, start_x+10, ' ');
    mvaddch(start_y+5, start_x-6, ' ');
    mvaddch(start_y+5, start_x+5, ' ');
    mvaddch(start_y+5, start_x-10, ' ');
    mvaddch(start_y+5, start_x+9, ' ');
    mvaddch(start_y+4, start_x-7, ' ');
    mvaddch(start_y+4, start_x-8, ' ');
    mvaddch(start_y+4, start_x-9, ' ');
    mvaddch(start_y+4, start_x+6, ' ');
    mvaddch(start_y+4, start_x+7, ' ');
    mvaddch(start_y+4, start_x+8, ' ');

    mvaddch(start_y+7, start_x-11, ' ');
    mvaddch(start_y+7, start_x+10, ' ');
    mvaddch(start_y+8, start_x-11, ' ');
    mvaddch(start_y+8, start_x+10, ' ');
    mvaddch(start_y+9, start_x-11, ' ');
    mvaddch(start_y+9, start_x+10, ' ');
    mvaddch(start_y+10, start_x-11, ' ');
    mvaddch(start_y+10, start_x+10, ' ');
    mvaddch(start_y+11, start_x-11, ' ');
    mvaddch(start_y+11, start_x+10, ' ');
    mvaddch(start_y+12, start_x-11, ' ');
    mvaddch(start_y+12, start_x+10, ' ');

    mvaddch(start_y+13, start_x-10, ' ');
    mvaddch(start_y+13, start_x+9, ' ');
    mvaddch(start_y+14, start_x-9, ' ');
    mvaddch(start_y+14, start_x+8, ' ');
    mvaddch(start_y+14, start_x-8, ' ');
    mvaddch(start_y+14, start_x+7, ' ');
    mvaddch(start_y+15, start_x-7, ' ');
    mvaddch(start_y+15, start_x-6, ' ');
    mvaddch(start_y+15, start_x-5, ' ');
    mvaddch(start_y+15, start_x-4, ' ');
    mvaddch(start_y+15, start_x-3, ' ');
    mvaddch(start_y+15, start_x-2, ' ');
    mvaddch(start_y+15, start_x-1, ' ');
    mvaddch(start_y+15, start_x, ' ');
    mvaddch(start_y+15, start_x+1, ' ');
    mvaddch(start_y+15, start_x+2, ' ');
    mvaddch(start_y+15, start_x+3, ' ');
    mvaddch(start_y+15, start_x+4, ' ');
    mvaddch(start_y+15, start_x+5, ' ');
    mvaddch(start_y+15, start_x+6, ' ');
    
    /*disegno le labbra*/
    mvaddch(start_y+10, start_x-3, ' ');
    mvaddch(start_y+10, start_x+3, ' ');
    mvaddch(start_y+11, start_x-2, ' ');
    mvaddch(start_y+11, start_x-1, ' ');
    mvaddch(start_y+11, start_x, ' ');
    mvaddch(start_y+11, start_x+1, ' ');
    mvaddch(start_y+11, start_x+2, ' ');
    
    /*disegno gli occhi*/
    mvaddch(start_y+7, start_x-8, ' ');
    mvaddch(start_y+7, start_x+7, ' ');
    attron(COLOR_PAIR(OCCHI_RANA));
    mvaddch(start_y+6, start_x-9, ' ');
    mvaddch(start_y+6, start_x-8, ' ');
    mvaddch(start_y+6, start_x-7, ' ');
    mvaddch(start_y+6, start_x+6, ' ');
    mvaddch(start_y+6, start_x+7, ' ');
    mvaddch(start_y+6, start_x+8, ' ');
    mvaddch(start_y+7, start_x-9, ' ');
    mvaddch(start_y+7, start_x-7, ' ');
    mvaddch(start_y+7, start_x+6, ' ');
    mvaddch(start_y+7, start_x+8, ' ');
    mvaddch(start_y+8, start_x-9, ' ');
    mvaddch(start_y+8, start_x-8, ' ');
    mvaddch(start_y+8, start_x-7, ' ');
    mvaddch(start_y+8, start_x+6, ' ');
    mvaddch(start_y+8, start_x+7, ' ');
    mvaddch(start_y+8, start_x+8, ' ');

    /*coloro la rana*/
    attron(COLOR_PAIR(FACCIA_RANA));
    mvaddch(start_y+5, start_x-9, ' ');
    mvaddch(start_y+5, start_x-8, ' ');
    mvaddch(start_y+5, start_x-7, ' ');
    mvaddch(start_y+5, start_x+6, ' ');
    mvaddch(start_y+5, start_x+7, ' ');
    mvaddch(start_y+5, start_x+8, ' ');

    mvaddch(start_y+6, start_x-10, ' ');
    mvaddch(start_y+6, start_x-6, ' ');
    mvaddch(start_y+6, start_x+5, ' ');
    mvaddch(start_y+6, start_x+9, ' ');

    mvaddch(start_y+7, start_x-10, ' ');
    mvaddch(start_y+7, start_x-6, ' ');
    mvaddch(start_y+7, start_x+5, ' ');
    mvaddch(start_y+7, start_x+9, ' ');
    mvaddch(start_y+7, start_x-5, ' ');
    mvaddch(start_y+7, start_x-4, ' ');
    mvaddch(start_y+7, start_x-3, ' ');
    mvaddch(start_y+7, start_x-2, ' ');
    mvaddch(start_y+7, start_x-1, ' ');
    mvaddch(start_y+7, start_x, ' ');
    mvaddch(start_y+7, start_x+1, ' ');
    mvaddch(start_y+7, start_x+2, ' ');
    mvaddch(start_y+7, start_x+3, ' ');
    mvaddch(start_y+7, start_x+4, ' ');

    mvaddch(start_y+8, start_x-10, ' ');
    mvaddch(start_y+8, start_x-6, ' ');
    mvaddch(start_y+8, start_x+5, ' ');
    mvaddch(start_y+8, start_x+9, ' ');
    mvaddch(start_y+8, start_x-5, ' ');
    mvaddch(start_y+8, start_x-4, ' ');
    mvaddch(start_y+8, start_x-3, ' ');
    mvaddch(start_y+8, start_x-2, ' ');
    mvaddch(start_y+8, start_x-1, ' ');
    mvaddch(start_y+8, start_x, ' ');
    mvaddch(start_y+8, start_x+1, ' ');
    mvaddch(start_y+8, start_x+2, ' ');
    mvaddch(start_y+8, start_x+3, ' ');
    mvaddch(start_y+8, start_x+4, ' ');

    mvaddch(start_y+9, start_x-10, ' ');
    mvaddch(start_y+9, start_x-9, ' ');
    mvaddch(start_y+9, start_x-8, ' ');
    mvaddch(start_y+9, start_x-7, ' ');
    mvaddch(start_y+9, start_x-6, ' ');
    mvaddch(start_y+9, start_x-5, ' ');
    mvaddch(start_y+9, start_x-4, ' ');
    mvaddch(start_y+9, start_x-3, ' ');
    mvaddch(start_y+9, start_x-2, ' ');
    mvaddch(start_y+9, start_x-1, ' ');
    mvaddch(start_y+9, start_x, ' ');
    mvaddch(start_y+9, start_x+1, ' ');
    mvaddch(start_y+9, start_x+2, ' ');
    mvaddch(start_y+9, start_x+3, ' ');
    mvaddch(start_y+9, start_x+4, ' ');
    mvaddch(start_y+9, start_x+5, ' ');
    mvaddch(start_y+9, start_x+6, ' ');
    mvaddch(start_y+9, start_x+7, ' ');
    mvaddch(start_y+9, start_x+8, ' ');
    mvaddch(start_y+9, start_x+9, ' ');
    
    mvaddch(start_y+10, start_x-10, ' ');
    mvaddch(start_y+10, start_x-9, ' ');
    mvaddch(start_y+10, start_x-8, ' ');
    mvaddch(start_y+10, start_x-7, ' ');
    mvaddch(start_y+10, start_x-6, ' ');
    mvaddch(start_y+10, start_x-5, ' ');
    mvaddch(start_y+10, start_x-4, ' ');
    mvaddch(start_y+10, start_x-2, ' ');
    mvaddch(start_y+10, start_x-1, ' ');
    mvaddch(start_y+10, start_x, ' ');
    mvaddch(start_y+10, start_x+1, ' ');
    mvaddch(start_y+10, start_x+2, ' ');
    mvaddch(start_y+10, start_x+4, ' ');
    mvaddch(start_y+10, start_x+5, ' ');
    mvaddch(start_y+10, start_x+6, ' ');
    mvaddch(start_y+10, start_x+7, ' ');
    mvaddch(start_y+10, start_x+8, ' ');
    mvaddch(start_y+10, start_x+9, ' ');

    mvaddch(start_y+11, start_x-10, ' ');
    mvaddch(start_y+11, start_x-9, ' ');
    mvaddch(start_y+11, start_x-8, ' ');
    mvaddch(start_y+11, start_x-7, ' ');
    mvaddch(start_y+11, start_x-6, ' ');
    mvaddch(start_y+11, start_x-5, ' ');
    mvaddch(start_y+11, start_x-4, ' ');
    mvaddch(start_y+11, start_x-3, ' ');
    mvaddch(start_y+11, start_x+3, ' ');
    mvaddch(start_y+11, start_x+4, ' ');
    mvaddch(start_y+11, start_x+5, ' ');
    mvaddch(start_y+11, start_x+6, ' ');
    mvaddch(start_y+11, start_x+7, ' ');
    mvaddch(start_y+11, start_x+8, ' ');
    mvaddch(start_y+11, start_x+9, ' ');

    mvaddch(start_y+12, start_x-10, ' ');
    mvaddch(start_y+12, start_x-9, ' ');
    mvaddch(start_y+12, start_x-8, ' ');
    mvaddch(start_y+12, start_x-7, ' ');
    mvaddch(start_y+12, start_x-6, ' ');
    mvaddch(start_y+12, start_x-5, ' ');
    mvaddch(start_y+12, start_x-4, ' ');
    mvaddch(start_y+12, start_x-3, ' ');
    mvaddch(start_y+12, start_x-2, ' ');
    mvaddch(start_y+12, start_x-1, ' ');
    mvaddch(start_y+12, start_x, ' ');
    mvaddch(start_y+12, start_x+1, ' ');
    mvaddch(start_y+12, start_x+2, ' ');
    mvaddch(start_y+12, start_x+3, ' ');
    mvaddch(start_y+12, start_x+4, ' ');
    mvaddch(start_y+12, start_x+5, ' ');
    mvaddch(start_y+12, start_x+6, ' ');
    mvaddch(start_y+12, start_x+7, ' ');
    mvaddch(start_y+12, start_x+8, ' ');
    mvaddch(start_y+12, start_x+9, ' ');
    
    mvaddch(start_y+13, start_x-9, ' ');
    mvaddch(start_y+13, start_x-8, ' ');
    mvaddch(start_y+13, start_x-7, ' ');
    mvaddch(start_y+13, start_x-6, ' ');
    mvaddch(start_y+13, start_x-5, ' ');
    mvaddch(start_y+13, start_x-4, ' ');
    mvaddch(start_y+13, start_x-3, ' ');
    mvaddch(start_y+13, start_x-2, ' ');
    mvaddch(start_y+13, start_x-1, ' ');
    mvaddch(start_y+13, start_x, ' ');
    mvaddch(start_y+13, start_x+1, ' ');
    mvaddch(start_y+13, start_x+2, ' ');
    mvaddch(start_y+13, start_x+3, ' ');
    mvaddch(start_y+13, start_x+4, ' ');
    mvaddch(start_y+13, start_x+5, ' ');
    mvaddch(start_y+13, start_x+6, ' ');
    mvaddch(start_y+13, start_x+7, ' ');
    mvaddch(start_y+13, start_x+8, ' ');

    mvaddch(start_y+14, start_x-7, ' ');
    mvaddch(start_y+14, start_x-6, ' ');
    mvaddch(start_y+14, start_x-5, ' ');
    mvaddch(start_y+14, start_x-4, ' ');
    mvaddch(start_y+14, start_x-3, ' ');
    mvaddch(start_y+14, start_x-2, ' ');
    mvaddch(start_y+14, start_x-1, ' ');
    mvaddch(start_y+14, start_x, ' ');
    mvaddch(start_y+14, start_x+1, ' ');
    mvaddch(start_y+14, start_x+2, ' ');
    mvaddch(start_y+14, start_x+3, ' ');
    mvaddch(start_y+14, start_x+4, ' ');
    mvaddch(start_y+14, start_x+5, ' ');
    mvaddch(start_y+14, start_x+6, ' ');

    refresh();
}