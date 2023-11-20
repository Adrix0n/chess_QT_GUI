#include <iostream>
#include <string.h>
using namespace std;
class figure{
    protected:
        char pos[3]={'A','1','\0'};
        char name;
        char color;
        bool InGame = false;
        bool HasMoved = false;
        bool PreviousHasMovedState = false;
    public:
    figure(){}
    virtual ~figure(){}
    virtual string AllMoves(){
        return "";
     };
    void place(const char *p){
        pos[0] = p[0];
        pos[1] = p[1];
        InGame = true;
     };
    char* getpos(){return pos;}
    char getname(){return name;}
    void remove(){InGame = false;}
    bool getingame(){return InGame;}
    void setingame(bool g){InGame = g;}
    void setcolor(char c){color = c;}
    char getcolor(){return color;}
    void sethasmoved(bool h){HasMoved = h;}
    bool gethasmoved(){return HasMoved;}
    bool getprevioushasmovedstate(){return PreviousHasMovedState;}
    void setprevioushasmovedstate(bool state){PreviousHasMovedState = state;}
};
class king: public figure{
    public:
        king(){name = 'K';}
        string AllMoves(){
            string result = "";
            string toadd = "";
            for(int i = -1; i<2 ; i++){
                for(int j = -1 ; j<2 ; j++){
                    toadd = pos[0]+i;
                    toadd += pos[1]+j;
                    if(toadd[0]<'A'||toadd[0]>'H'||toadd[1]<'1'||toadd[1]>'8'||(toadd[0]==pos[0]&&toadd[1]==pos[1]))
                        continue;
                    result+=toadd;
                }
            }
            if(!HasMoved){
                if(color=='w')
                    result+="B1G1";
                else
                    result+="B8G8";
            }
            return result;
        }
};
class queen: public figure{
    public:
        queen(){name = 'Q';}
        string AllMoves(){
            string result = "";
            string toadd = "";
            for(int i=0;i<8;i++){
                toadd = ('A'+i);
                toadd+= pos[1];
                if(toadd[0]==pos[0]) continue;
                result+=toadd;
            }
            for(int i =0; i<8;i++){
                toadd = pos[0];
                toadd+= + ('1'+i);
                if(toadd[1]==pos[1]) continue;
                result+=toadd;
            }
            for(int i = -7; i<8; i++){
                toadd = (pos[0]+i);
                toadd += pos[1]+i;
                if(toadd[0]<'A'||toadd[0]>'H'||toadd[1]<'1'||toadd[1]>'8'||(toadd[0]==pos[0]&&toadd[1]==pos[1]))
                    continue;
                result+=toadd;
            }
            for(int i = -7;i<8;i++){
                toadd = (pos[0]-i);
                toadd += pos[1]+i;
                if(toadd[0]<'A'||toadd[0]>'H'||toadd[1]<'1'||toadd[1]>'8'||(toadd[0]==pos[0]&&toadd[1]==pos[1]))
                    continue;
                result+=toadd;
            }

            return result;
        }
};
class rook: public figure{
    public:
        rook(){name = 'R';}
        string AllMoves(){
            string result = "";
            string toadd = "";
            for(int i=0;i<8;i++){
                toadd = ('A'+i);
                toadd+= pos[1];
                if(toadd[0]==pos[0]) continue;
                result+=toadd;
            }
            for(int i =0; i<8;i++){
                toadd = pos[0];
                toadd+= + ('1'+i);
                if(toadd[1]==pos[1]) continue;
                result+=toadd;
            }
            return result;
        }
};
class bishop: public figure{
    public:
        bishop(){name = 'B';}
        string AllMoves(){
            string result = "";
            string toadd = "";
            for(int i = -7; i<8; i++){
                toadd = (pos[0]+i);
                toadd += pos[1]+i;
                if(toadd[0]<'A'||toadd[0]>'H'||toadd[1]<'1'||toadd[1]>'8'||(toadd[0]==pos[0]&&toadd[1]==pos[1]))
                    continue;
                result+=toadd;
            }
            for(int i = -7;i<8;i++){
                toadd = (pos[0]-i);
                toadd += pos[1]+i;
                if(toadd[0]<'A'||toadd[0]>'H'||toadd[1]<'1'||toadd[1]>'8'||(toadd[0]==pos[0]&&toadd[1]==pos[1]))
                    continue;
                result+=toadd;
            }
            return result;
        }
};
class knight: public figure{
    public:
        knight(){name = 'H';}
    string AllMoves(){
        string result = "";
        string toadd = "";
        int x[4] = {-2,-1,1,2};
        for(int i = 0;i<4;i++){
            for(int j = 0; j<4 ;j++){
                if(i==j) continue;
                if(x[i]==-x[j]) continue;
                toadd = pos[0] + x[i];
                toadd += pos[1] + x[j];
                if(!(toadd[0]<'A'||toadd[0]>'H'||toadd[1]<'1'||toadd[1]>'8'))
                result+=toadd;
            }
        }
        
        return result;
    }
};
class pawn: public figure{
    public:
        pawn(){name = 'P';}
        string AllMoves(){
            string result = "";
            string toadd = "";

            int j;
            j = color=='b'? -1 : 1;
            j *= HasMoved? 1 : 2;
            for(int i = -1; i<2;i++){
                for(int k = j;k!=0;k/=2){
                    toadd = pos[0] + i;
                    toadd += pos[1] + k;
                    if(toadd[0]<'A'||toadd[0]>'H'||toadd[1]<'1'||toadd[1]>'8')
                        continue;
                    result+=toadd;
                }
            }
            return result;
        }
};