#include <ncurses.h>

int main(void)
{	
    initscr();              // Initialise la structure WINDOW et autres paramC(tres 
    printw("Hello World");  // C	crit Hello World C  l'endroit oC9 le curseur logique est positionnC)
    refresh();              // RafraC.chit la fenC*tre courante afin de voir le message apparaC.tre
    getch();                // On attend que l'utilisateur appui sur une touche pour quitter
    endwin();               // Restaure les paramC(tres par dC)faut du terminal
    
    return 0;
}
