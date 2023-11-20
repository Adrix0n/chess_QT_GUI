#include <iostream>
#include <string.h>
#include "figures.h"
#include <fstream>
#include <stdlib.h>
#include "chess.h"


int main(){
    chess_game game(10,true);
    cout<<game.getBoard()<<endl<<endl;
    //system("clear");
    game.MoveFigureString("A2A3");
    cout<<game.getBoard();








    return 0;
}