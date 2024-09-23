// file per le funzioni della rana 
#include "header.h"
#include "global.h"

void* frog_function(){
    int tasti;

    // posizioniamo la rana in posizione di partenza 
    pthread_mutex_lock(&mutex) ;
    rana.y = POS_Y_RANA ;
    rana.x = POS_X_RANA ; 
    rana.id = 'r' ;
    pthread_mutex_unlock(&mutex) ;

    pthread_mutex_lock(&mutex) ;
    sem_post(&count) ;
    pthread_mutex_unlock(&mutex) ;

    while(true){
        
        pthread_mutex_lock(&mutex) ;
        stampa_tutto();
        pthread_mutex_unlock(&mutex);

        timeout(1) ;
        tasti = getch();
        
        switch(tasti) {
			case KEY_UP: 
				if(rana.y > LIM_SUP_RANA){
                    pthread_mutex_lock(&mutex) ;
                    rana.y -= 3 ;
                    pthread_mutex_unlock(&mutex) ;
                }
				break ;
			case KEY_DOWN:
				if(rana.y < LIM_INF_RANA){
                    pthread_mutex_lock(&mutex) ;
                    rana.y += 3 ;
                    pthread_mutex_unlock(&mutex) ;
                }
				break ;
			case KEY_LEFT:
				if(rana.x > LIM_SX_RANA){
                    pthread_mutex_lock(&mutex) ;
                    rana.x -= 1;
                    pthread_mutex_unlock(&mutex) ;
                }
				break ;
			case KEY_RIGHT:
				if(rana.x < LIM_DX_RANA){
                    pthread_mutex_lock(&mutex) ;
                    rana.x += 1 ;
                    pthread_mutex_unlock(&mutex) ;
                }

				break ;
			case SPACE_BAR :
                if(proiettile.y < 0)
                {
                    pthread_mutex_lock(&mutex) ;
                    proiettile.x = rana.x + 2 ;
                    proiettile.y = rana.y;
                    pthread_mutex_unlock(&mutex);
                }
                break ; 
		}


    }
}

void* proiettile_function(){

    pthread_mutex_lock(&mutex) ;
    sem_post(&count) ;
    pthread_mutex_unlock(&mutex) ;

    while(true){
        if(proiettile.y >= 0){
            pthread_mutex_lock(&mutex) ;
            proiettile.y --;
            pthread_mutex_unlock(&mutex) ;
            usleep(30000);
        }
    }
}

void print_rana(obj rana){
    
    mvprintw(rana.y, rana.x, " _ _ ") ;
    mvprintw(rana.y+1, rana.x, " 0 0 ") ;
    mvprintw(rana.y+2, rana.x, "_( )_") ;
}