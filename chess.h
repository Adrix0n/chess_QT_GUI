#pragma once
#include <iostream>
#include <string.h>
#include "figures.h"
#include <fstream>
#include <stdlib.h>

using namespace std;

    // void PlaceFigures(figure **white, figure **black, figure* b[][8],bool NewGame);
    // void SaveGame(figure **white,figure **black,bool m,string history, int gameID);
    // void LoadGame(figure **white, figure **black, bool *m,figure* b[][8], string* history, int gameID);
    // bool MakeMove(figure **white, figure **black, bool *who, figure* b[][8], char* pos1, char* pos2, figure** fbw[]);
    // string DrawBoard2(figure **white, figure **black,figure* b[][8]);

    // bool CanReach(figure* fig,char* K,figure* b[][8]);
    // bool IsKingSave(figure* fig,char* pos2,bool *who, figure* b[][8], figure** fbw[]);
    // int IsEnd(bool *who, figure* b[][8],figure** fbw[]);
    // bool PawnMoveAdditionalCondition(figure* fig,char* pos2, figure *b[][8]);
    // void PawnPromotion(figure* fig, bool *who, figure** white, figure** black, figure* b[][8]);
class chess_game{
    protected:
        figure **white = new figure*[16];
        figure **black = new figure*[16];
        figure** bw[2] = {black,white};
        figure* board[8][8];
        string MoveHis="";
        bool WhoMove = 1;
        bool moved = 0;
        int gameID;

    public:
    chess_game(int gameID, bool newGame){
        for(int i =0;i<8;i++){
            for(int j = 0;j<8;j++){
                board[i][j] = nullptr;
            }
        }

        for(int i = 0; i<8;i++){
            white[i] = new pawn();
            black[i] = new pawn();
        }

        white[8] = new rook;
        white[9] = new knight;
        white[10] = new bishop;
        white[11] = new queen;
        white[12] = new king;
        white[13] = new bishop;
        white[14] = new knight;
        white[15] = new rook;

        black[8] = new rook;
        black[9] = new knight;
        black[10] = new bishop;
        black[11] = new queen;
        black[12] = new king;
        black[13] = new bishop;
        black[14] = new knight;
        black[15] = new rook;

        for(int i =0;i<16;i++){
            white[i]->setcolor('w');
            black[i]->setcolor('b');
        }

        this->gameID = gameID;
        if(newGame==true){
            PlaceFigures(white,black,board,1);
        }else{
            LoadGame(white,black,&WhoMove,board,&MoveHis,gameID);
            PlaceFigures(white,black,board,false);
        }
    }

    bool getWhoMove(){
        return WhoMove;
    }

    int MoveFigureString(string inString){
        char pos1[2];
        char pos2[2];
        pos1[0] = inString[0];
        pos1[1] = inString[1];
        pos2[0] = inString[2];
        pos2[1] = inString[3];

        moved = MakeMove(white,black,&WhoMove,board,pos1,pos2,bw);

        if(moved!=0){
            MoveHis += inString + " ";
            SaveGame(white,black,WhoMove,MoveHis,gameID);
        }
        return moved;
    }

    string getBoard(){
        return DrawBoard2(white,black,board);
    }

    bool getIsEnd(){
        return IsEnd(&WhoMove,board,bw);
    }

    void PlaceFigures(figure **white, figure **black, figure* b[][8],bool NewGame){
        char pos[2];
        if(NewGame){
            for(int i = 0; i<16;i++){
                pos[0] = 'A'+ i%8;
                pos[1] = '2' - i/8;
                white[i]->place(pos);
                cout<<pos;
                b[pos[0]-'A'][pos[1]-'1'] = white[i];
                pos[0] = 'A' + i%8;
                pos[1] = '7' + i/8;
                black[i]->place(pos);
                cout<<""<<pos<<endl;
                b[pos[0]-'A'][pos[1]-'1'] = black[i];
            }
        }
        else{
            string p;
            for(int i = 0; i < 16; i++){
                if(white[i]->getingame()){
                    p = white[i]->getpos();
                    b[p[0]-'A'][p[1]-'1'] = white[i];
                }
                if(black[i]->getingame()){
                    p = black[i]->getpos();
                    b[p[0]-'A'][p[1]-'1'] = black[i];
                }
            }
        }
    }


    void SaveGame(figure **white,figure **black,bool m,string history, int gameID){
        ofstream fout;
        string fileName = "game" + to_string(gameID) + ".txt";
        fout.open(fileName);
        for(int i=0;i<16;i++)
            fout<<white[i]->getpos()<<white[i]->getingame()<<white[i]->gethasmoved()<<endl;
        for(int i=0;i<16;i++)
            fout<<black[i]->getpos()<<black[i]->getingame()<<black[i]->gethasmoved()<<endl;
        fout<<m<<endl;
        fout<<history;
        fout.close();
    }

    void LoadGame(figure **white, figure **black, bool *m,figure* b[][8], string* history, int gameID){
        string line;
        ifstream fin;
        string fileName = "game" + to_string(gameID) + ".txt";
        fin.open(fileName);
        for(int i = 0;i<16;i++){
            getline(fin,line);
            white[i]->place(line.c_str());
            if(!(line[2]-'0')) white[i]->remove();
            white[i]->sethasmoved(line[3]-'0');
            cout<<line[3];
        }
        for(int i = 0;i<16;i++){
            getline(fin,line);
            black[i]->place(line.c_str());
            if(!(line[2]-'0')) black[i]->remove();
            black[i]->sethasmoved(line[3]-'0');       
        }
        getline(fin,line);
        if(line[0]=='1') *m = 1;
        else *m = 0;
        getline(fin,line);
        *history = line;
        fin.close();
    }

    bool MakeMove(figure **white, figure **black, bool *who, figure* b[][8], char* pos1, char* pos2, figure** fbw[]){
        int x1 = pos1[0]-'A', x2 = pos2[0]-'A', y1 = pos1[1]-'1', y2 = pos2[1]-'1';
        
        
        // Testing, if the given coordinates are within boundaries
        if(x1<0||x1>7||x2<0||x2>7||y1<0||y1>7||y2<0||y2>7||(x1==x2&&y1==y2)){
            cout<<"Incorrect move - incorrect coordinates\n";
            return 0;
        }

        // Testing, if the player is trying to move an empty field
        figure* fig = b[x1][y1];
        if(fig==nullptr){
            cout<<"Incorrect move - you are trying to move empty field\n";
            return 0;
        }

        // Testing, if the player is trying to move an opponent's figure
        if((b[x1][y1]->getcolor()=='w'&&(*who)==0)||(b[x1][y1]->getcolor()=='b'&&(*who)==1)){
            cout<<"Incorrect move - it is not your piece";
            return 0;
        }

        // Testing, if move is found in figure's AllMoves() result
        string allmoves = fig->AllMoves();
        cout<<allmoves<<endl;
        bool LegalMove = false;
        for(int i=0;i<(int)allmoves.length();i+=2){
            if(allmoves[0+i]==pos2[0]&&allmoves[1+i]==pos2[1]){
                LegalMove = true;
                break;
            }
        }
        if(!LegalMove){
            cout<<"Incorrect move - this piece cannot move there\n";
            return 0;
        }

        // Testing, if the way is clear (exluding the destination field)
        int a1 = x1-x2==0? 0 : 1;
        if(x1>x2) a1*=-1;
        int a2 = y1-y2==0? 0 : 1;
        if(y1>y2) a2*=-1;
        for(int i=1;i<max(abs(x1-x2),abs(y1-y2));i++){
            if(fig->getname()=='H') break;
            if(b[x1+i*a1][y1+i*a2]!=nullptr){
                LegalMove = false;
                break;
            }
        }

        // Additional move condition for pawn
        if(fig->getname()=='P'&&!PawnMoveAdditionalCondition(fig,pos2,b)){
            cout<<"Incorrect move - you cannot move your pawn there\n";
            return 0;
        }
                            
        if(!LegalMove){
            cout<<"Incorrect move - other figure is blocking the move\n";
            return 0;
        }

        // Testing, if the player is trying to take an own figure 
        if((b[x2][y2]!=nullptr)&&(b[x2][y2]->getcolor()==fig->getcolor())){
            cout<<"Incorrect move - you are trying to take your own piece\n";
            return 0;
        }

        // Castle test
        if((pos2[0]=='B'||pos2[0]=='G')&&(pos2[1]=='1'||pos2[1]=='8')&&fig->getname()=='K'&&(b[x2][y2]==nullptr)){
            int row = fbw[*who][12]->getpos()[1]-'1';
            int dist = pos2[0]=='G' ? 2 : 3;
            int direction = pos2[0]=='G' ? 1 : -1;
            int oldRockCol = pos2[0] == 'G' ? 7 : 0;
            char rookCol = pos2[0]=='G' ? 'F' : 'C';
            if(b[oldRockCol][row]==nullptr){
                cout<<"Incorrect move - piece already moved\n";
                return 0;
            }
            if(fbw[*who][12]->gethasmoved()==true||b[oldRockCol][row]->gethasmoved()==true){
                cout<<"Incorrect move - piece already moved\n";
                return 0;
            }

            char *tpos = fbw[*who][12]->getpos();
            for(int j = 0; j<dist;j++){    
                for(int i = 0; i<16; i++){
                    if(CanReach(fbw[!(*who)][i],fbw[*who][12]->getpos(),b)){
                        tpos[0]='E';
                        fbw[*who][12]->place(tpos);
                        fbw[*who][12]->sethasmoved(false);
                        LegalMove = false;
                        break;
                    }
                if(!LegalMove)
                    break;
                }
                tpos[0] += (1 * direction);
                fbw[*who][12]->place(tpos);
            }
            fig->place(tpos);
            char rookNewPos[3] = {rookCol, (char)(row + '1'), '\0'};
            pos2[0] == 'G' ? fbw[*who][15]->place(rookNewPos) : fbw[*who][8]->place(rookNewPos);
            b[pos2[0]-'A'][row]=fig;
            b[rookNewPos[0]-'A'][row]=fbw[*who][8];
            b[oldRockCol][row]=nullptr;
            b[4][row]=nullptr;
            *who = !(*who);
            b[pos2[0]-'A'][row]->setprevioushasmovedstate(b[pos2[0]-'A'][row]->gethasmoved());
            b[rookNewPos[0]-'A'][row]->setprevioushasmovedstate(b[rookNewPos[0]-'A'][row]->gethasmoved());
            b[pos2[0]-'A'][row]->sethasmoved(true);
            b[rookNewPos[0]-'A'][row]->sethasmoved(true);
            return 1;

        }

        // Testing, if the king is save after the move
        LegalMove = IsKingSave(fig,pos2,who,b,fbw);
    
        if(!LegalMove){
            cout<<"Incorrect move - you reveal your king\n";
            return 0;
        }

        // When everything went positive and we can proceed the move
        fig->place(pos2);
        if(b[x2][y2]!=nullptr)
            b[x2][y2]->setingame(false);
        b[x2][y2] = fig;
        b[x1][y1] = nullptr;
        *who=!(*who);

        fig->setprevioushasmovedstate(fig->gethasmoved());
        fig->sethasmoved(1);

        // Pawn promotion
        if(fig->getname()=='P'&&(pos2[1]=='1'||pos2[1]=='8'))
            PawnPromotion(fig, who,white,black,b);

        // En passant
        char color = fig->getcolor()=='w' ? 'b' : 'w';
        if(fig->getname()=='P'&&b[x2][y1]!=nullptr&&b[x2][y1]->getname()=='P'&&b[x2][y1]->getcolor()==color&&b[x2][y1]->getprevioushasmovedstate()==false){
            b[x2][y1]->setingame(false);
            b[x2][y1]=nullptr;
        }


        return 1;
    }

    string DrawBoard2(figure **white, figure **black,figure* b[][8]){
        string result = "";
        char tile[2] = {' ','#'};
        for(int i = 7;i>-1;i--){
            bool TileColor = i%2;
            for(int j = 0;j<8; j++){
                if(b[j][i]!=nullptr){
                    char f = b[j][i]->getname();
                    if(b[j][i]->getcolor() == 'b')
                        f += 32;
                    result += f;
                }else{
                    result += tile[TileColor];
                }
                TileColor = !TileColor;
            }
        }
        return result;
    }

    bool CanReach(figure* fig, char* pos2,figure* b[][8]){
        if(!(fig->getingame()))
            return false;

        // Checking with AllMoves()
        char* pos1 = fig->getpos();
        string allmoves = fig->AllMoves();

        bool LegalMove = false;
        for(int i=0;i<(int)allmoves.length();i+=2){
            if(allmoves[0+i]==pos2[0]&&allmoves[1+i]==pos2[1]){
                LegalMove = true;
                break;
            }
        }
        if(!LegalMove)
            return LegalMove;
        
        int x1 = pos1[0]-'A', x2 = pos2[0]-'A', y1 = pos1[1]-'1', y2 = pos2[1]-'1';
        int a1 = x1-x2==0 ? 0 : 1;
        int a2 = y1-y2==0 ? 0 : 1;
            for(int i=1;i<max(abs(x1-x2),abs(y1-y2));i++){
                if(fig->getname()=='H') break;
                if(b[min(x1,x2)+i*a1][min(y1,y2)+i*a2]!=nullptr){
                    LegalMove = false;
                    break;
                }
            }
        return LegalMove;
    }

    bool IsKingSave(figure* fig, char* pos2,bool *who, figure* b[][8], figure** fbw[]){
        bool result = true;
        char* tpos1 = fig->getpos();
        string pos1 = {tpos1[0],tpos1[1]};
        int x1 = pos1[0]-'A', x2 = pos2[0]-'A', y1 = pos1[1]-'1', y2 = pos2[1]-'1';

        // Move simulation
        if(b[x2][y2]!=nullptr)
            b[x2][y2]->setingame(false);
        fig->place(pos2);
        figure* tmp = b[x2][y2];
        b[x2][y2] = fig;
        b[x1][y1] = nullptr;

        // Checking if the king is reachable after move
            for(int i =0;i<16;i++){
                result = CanReach(fbw[!(*who)][i],fbw[*who][12]->getpos(),b);
                if(fbw[!(*who)][i]->getname()=='P'&&(fbw[!(*who)][i]->getpos())[0]==pos2[0]){
                    continue;
                }
                if(fbw[!(*who)][i]->getname()=='P'&&(fbw[!(*who)][i]->getpos())[0]==(fbw[*who][12]->getpos())[0]){
                    continue;
                }
                if(result)
                    break;
            }

        // Backing to previous position
        fig->place(pos1.c_str());
        b[x1][y1] = fig;
        b[x2][y2] = tmp;
        if(b[x2][y2]!=nullptr)
            b[x2][y2]->setingame(true);
        return !result;
    }

    // 0 - the game lasts
    // 1 - the end
    // 2 - the end, stalemate
    int IsEnd(bool *who,figure* b[][8], figure** fbw[]){
        bool IsKingUnderAttack = false;
        bool IsDraw = true;
        // Checking, if king is under attack
        for(int i =0;i<16;i++){
            IsKingUnderAttack = CanReach(fbw[!(*who)][i],fbw[*who][12]->getpos(),b);
            if(IsKingUnderAttack){
                break;
            }
        }

        // Checking, if there is a move that can save the king
        string AllMoves;
        char tmppos[2];
        for(int i=0;i<16;i++){
            AllMoves = fbw[*who][i]->AllMoves();
            char colorChar = (*who) ? 'b' : 'w';
            for(int j=0;j<(int)AllMoves.length()/2;j+=2){
                tmppos[0] = AllMoves[0+j];
                tmppos[1] = AllMoves[1+j];
                if(CanReach(fbw[*who][i],tmppos,b)&&IsKingSave(fbw[*who][i],tmppos,who,b,fbw)&&(b[tmppos[0]-'A'][tmppos[1]-'1']==nullptr||b[tmppos[0]-'A'][tmppos[1]-'1']->getcolor()==colorChar)){
                    if(fbw[*who][i]->getname()=='P'&&!PawnMoveAdditionalCondition(fbw[*who][i],tmppos,b))
                        continue;
                    IsKingUnderAttack = false;
                    IsDraw = false;
                    break;
                }
            }
        }
        // Checking, if there are only kings left on the board
        bool OnlyKings = true;
        for(int i=0;i<16;i++){
            if(i==12) continue;
            OnlyKings = fbw[*who][i]->getingame()? 0 : 1;
            if(!OnlyKings)
            break;
            OnlyKings = fbw[!(*who)][i]->getingame()? 0 : 1;
            if(!OnlyKings)
            break;
        }
        
        if(OnlyKings) return 2;
        if(IsKingUnderAttack) return 1;
        if(IsDraw) return 2;
        return 0;
    }

    bool PawnMoveAdditionalCondition(figure* fig,char* pos2, figure *b[][8]){
            bool LegalMove = true;
            char* pos1 = fig->getpos();
            int x1 = pos1[0]-'A', x2 = pos2[0]-'A', y1 = pos1[1]-'1', y2 = pos2[1]-'1';
            int a3 = fig->getcolor()=='w'? 1 : -1;
            if(fig->gethasmoved()==true){
                if(x1==x2&&y1+a3==y2&&b[x1][y1+a3]!=nullptr){
                    LegalMove = false;
                }else{
                    if(x1!=x2&&b[x2][y2]==nullptr){
                        if(b[x2][y1]!=nullptr&&b[x2][y1]->getname()=='P'&&b[x2][y1]->getprevioushasmovedstate()==false){
                            LegalMove = true;
                        }
                        else{
                            LegalMove = false;
                        }
                    }
                }
            }else{
                if(x1!=x2||(CanReach(fig,pos2,b)&&b[x2][y2]!=nullptr))
                    LegalMove = false;
                if(x1!=x2&&b[x2][y2]!=nullptr)
                    LegalMove = true;
            }
            return LegalMove;
        }

    void PawnPromotion(figure* fig,bool *who, figure** white, figure** black, figure* b[][8]){  
        cout<<"Promotion. Choose figure ('R','H','B','Q'): ";
        string choose;
        int i;
        for(i=0;i<16;i++){
            if((white[i]->getpos()==fig->getpos())||(black[i]->getpos()==fig->getpos()))
                break;
        }
        
        char* tpos = fig->getpos();
        do{
            cin>>choose;
        }while(choose[0]!='R'&&choose[0]!='H'&&choose[0]!='B'&&choose[0]!='Q');
        switch(choose[0]){
            case 'R': fig = new rook; break;
            case 'H': fig = new knight; break;
            case 'B': fig = new bishop; break;
            case 'Q': fig = new queen; break;
            default: cout<<"Impossible case"<<endl;
        }
        fig->setingame(true);
        fig->place(tpos);
        fig->sethasmoved(true);
        if(!(*who)){
            fig->setcolor('w');
            white[i] = fig;
        }else{
            fig->setcolor('b');
            black[i] = fig; 
        }
        b[tpos[0]-'A'][tpos[1]-'1'] = fig;
    }

};