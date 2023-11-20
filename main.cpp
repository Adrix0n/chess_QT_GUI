#include <iostream>
#include <string.h>
#include "figures.h"
#include <fstream>
#include <stdlib.h>
#include "chess.h"


int main(){
    chess_game game(10,false);
    string f;
    while(1){
        cout<<game.getBoard()<<endl;
        
        cin>>f;
        game.MoveFigureString(f);
        if(game.getIsEnd()){
            break;
        }
    }

    return 0;
}