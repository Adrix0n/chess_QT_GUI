#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h> 
#include <unistd.h> 
#include <pthread.h>

#include "chess.h"
#include <cstdio>
#include <vector>
#include <time.h>

#define MESSAGE_SIZE 255

struct ipcMessage{
  long receiver;
  long userID;
}msg;

//gcc program.c -lpthread -Wall -o outfile Remember to link pthread lib

char client_message[255];
//char buffer[256];

struct user{
  long userID;
  char userName[20];
  long gameID;
  int userSocket;
  bool color; //0 - white, 1 - black
}users;

/*
  TODO:
    - Napisać obsługę rozłączenia klienta od serwera
    - poprawić odnośniki do odpowiednich gier w wątku, aby nie odnosił się do gry o indeksie gameId, gdyż liczba gier w wektorze może się zmniejszyć (poprawione, 
      ale należy dodać jeszcze mutexa, bo istnieje sytuacja, gdzie funkcja findgameid się wykonuje, a w tym czasie główny wątek usuwa gry)
    - Napisać obsługę zakończenia gry (usuwanie, przekazywanie odpowiedniej wiadomości zwrotnej)
    - Dopisać destruktor do chess_game :), aby poprawnie wszystko usuwał (chyba jest)
    - Ogarnąć pozostałe komunikaty z serwera do klienta i z klienta do serwera 
      (promocja figury, rozłączenie gracza)
*/

long findGameIndex(long gameID);
long genGameId();
long genUserId();
int findIdxOfActiveUserByID(long userID);
int findIdxOfActiveUserByGameID(long gameID);
int findIdxOfActiveUserByName(char nickname[20]);
std::vector < chess_game* > games;
std::vector < user > activeUsers;
std::vector < user > ownLobbyUsers;

void * socketThread(void *arg)
{
  long *args = ((long *)arg);
  int newSocket1 = args[0];
  int newSocket2 = args[1];
  long gameId = args[2];
  long userId = args[3];

  printf("ns1: %d. ns2: %d, gamid: %ld\n",newSocket1,newSocket2,gameId);
  int n;
  char recvMessage[255];
  char endMessage[255];

  int gameIdx = findGameIndex(gameId);
  chess_game *game = games[gameIdx];
  int useridx = findIdxOfActiveUserByID(userId);
  struct user *player = &activeUsers[useridx]; 

  while(1){
    n=recv(newSocket1,recvMessage,sizeof(recvMessage),0);
    printf("%s\n",recvMessage);
    // Obsługa błędów z recv();

    if(n==0){ // Rozłączono
    // wiadomość do drugiego gracza
    endMessage[0] = 'X'; endMessage[1] = 'D';endMessage[2] = '2'; endMessage[3] = '\0';
    n = send(newSocket2,endMessage,sizeof(endMessage),0);
    if(n==0||n==-1){
        printf("Błąd wysyłania disconnect msg\n");
      }

    // Nie wiem czy potrzebne
    close(newSocket1);
    close(newSocket2);

    //printf("userIdx: %d\n",findIdxOfActiveUserByID(userId));
    activeUsers.erase(activeUsers.begin()+ findIdxOfActiveUserByID(userId));

    if(findIdxOfActiveUserByGameID(gameId)==-1){
        delete game;
        games.erase(games.begin()+findGameIndex(gameId));
    }
      
      // tests
      printf("Błąd! Odebrano 0 bajtów. Pozostali gracze\n");
      for(long unsigned int i=0;i<activeUsers.size();i++){
        printf("%s, userID:%ld, gameID:%ld \n",activeUsers[i].userName,activeUsers[i].userID,activeUsers[i].gameID);
      }
       printf("Pozostale gry\n");
      for(long unsigned int i=0;i<games.size();i++){
        printf("%ld, ",games[i]->getGameID());
      }
      printf("\n");


      break;
    }
    if(n==-1){
      printf("Błąd recv()==-1\n");
    }

    
    std::string playerMove(recvMessage);

    bool validMove = 0;
    bool whoMove = game->getWhoMove();
    if(!whoMove==player->color){
      validMove = game->MoveFigureString(playerMove);
    }
    
    if(validMove){
      int n1,n2;
      printf("\n%s\n",game->getBoard().c_str());
      n1 = send(newSocket2,game->getBoard().c_str(),sizeof(recvMessage),0);
      n2 = send(newSocket1,game->getBoard().c_str(),sizeof(recvMessage),0);
      printf("n1:%d n2:%d\n",n1,n2);
      if(n1==0||n1==-1||n2==0||n2==-1){
        printf("Błąd wysyłania\n");
        break;
      }
    }

    // 3 - Czarne wygrały
    // 2 - Białe wygrały
    // 1 - Remis
    // 0 - nie koniec
    switch(game->getIsEnd()){
      case 0:{
        //nic się nie dzieje
        break;
      }
      case 1:{
        endMessage[0] = 'X'; endMessage[1] = 'E';endMessage[2] = '1'; endMessage[3] = '\0';
        send(newSocket2,endMessage,sizeof(recvMessage),0);
        send(newSocket1,endMessage,sizeof(recvMessage),0);
        // Przesłanie użytkownikom informacji o końcu gry, usunięcie gry i rozłączenie użytkowników (opcja nowej gry)
        break;
      }
      case 2:{
        endMessage[0] = 'X'; endMessage[1] = 'E';endMessage[2] = '2'; endMessage[3] = '\0';
        send(newSocket2,endMessage,sizeof(recvMessage),0);
        send(newSocket1,endMessage,sizeof(recvMessage),0);
        break;
      }
      case 3:{
        endMessage[0] = 'X'; endMessage[1] = 'E';endMessage[2] = '3'; endMessage[3] = '\0';
        send(newSocket2,endMessage,sizeof(recvMessage),0);
        send(newSocket1,endMessage,sizeof(recvMessage),0);
        break;
      }
      default:{
        printf("Nieznany błąd!!!\n");
        break;
      }
    }
    if(game->getIsEnd()!=0){
      // Usuwanie gry i użytkowników

      // Nie wiem czy potrzebne
      close(newSocket1);
      close(newSocket2);

      //printf("userIdx: %d\n",findIdxOfActiveUserByID(userId));

      /// Chyba nie zwalnia pamięci (może jednak zwalnia bo delete player plus to niżej zamyka program)
      activeUsers.erase(activeUsers.begin()+ findIdxOfActiveUserByID(userId));

      if(findIdxOfActiveUserByGameID(gameId)==-1){
        delete game;
        games.erase(games.begin()+findGameIndex(gameId));
      }
      break;
    }
  }


  memset(&recvMessage, 0, sizeof (recvMessage));

  
  pthread_exit(NULL);
}

int main(){

  // ----- Server initialization -----
  int serverSocket;
  int newSockets[2];
  struct sockaddr_in serverAddr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;

  //Create the socket. 
  serverSocket = socket(AF_INET,SOCK_STREAM,0);
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(1100);
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  int valuevalue = 1;
  if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &valuevalue, sizeof(int)) < 0)
    perror("setsockopt(SO_REUSEADDR) failed");

  //Bind the address struct to the socket 
  bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  if(listen(serverSocket,10)==0){
    printf("Listening\n");
  }else{
    printf("Error\n");
  }
  
  pthread_t thread_id1, thread_id2;

  int notPairedClients = 0;

  long userIds[2];
  char userNames[2][20];
  while(1)
    {
        //Accept call creates a new socket for the incoming connection
        addr_size = sizeof serverStorage;
        bzero(client_message,sizeof(client_message));
        newSockets[notPairedClients] = accept(serverSocket, (struct sockaddr *) &serverStorage, &addr_size);
        if(recv(newSockets[notPairedClients],client_message,sizeof(client_message),0)<1){
          printf("bład przypisania nazwy: %s\n",client_message);
          break;
        }
        for(int i=0;i<20;i++){
          userNames[notPairedClients][i]=client_message[i];
        }

        userIds[notPairedClients] = genUserId();

        bzero(client_message,sizeof(client_message));
        if(recv(newSockets[notPairedClients],client_message,sizeof(client_message),0)<1){
          printf("bład przypisania wyboru: %s\n",client_message);
          break;
        }

        if(client_message[0]=='0'){
          // Zwykły wybor

          notPairedClients++;
          if(notPairedClients==2){
            long gameId = genGameId();
            games.push_back(new chess_game(gameId,true));
            long socketArray1[4] = {newSockets[0],newSockets[1],gameId,userIds[0]};
            long socketArray2[4] = {newSockets[1],newSockets[0],gameId,userIds[1]};
            user user1, user2;

            user1.userID=userIds[0];
            strcpy(user1.userName,userNames[0]);
            user1.gameID = gameId;
            user1.color = 0;

            user2.userID=userIds[1];
            strcpy(user2.userName,userNames[1]);
            user2.gameID = gameId;
            user2.color = 1;

            activeUsers.push_back(user1);
            activeUsers.push_back(user2);

            if( pthread_create(&thread_id1,NULL,socketThread,&socketArray1) != 0 )
            printf("Failed to create thread\n");

            if( pthread_create(&thread_id2,NULL,socketThread,&socketArray2) != 0 )
            printf("Failed to create thread\n");

            pthread_detach(thread_id1);
            pthread_detach(thread_id2);
            printf("Created game: gameid %ld\n",gameId);
            notPairedClients = 0;
          }
        }
        if(client_message[0]=='1'){
          // Tworzenie rozgrywki
          long gameId = genGameId();
          games.push_back(new chess_game(gameId,true));


          user user1;
          user1.userID=userIds[notPairedClients];
          strcpy(user1.userName,userNames[notPairedClients]);
          user1.gameID = gameId;
          user1.color = 0;
          user1.userSocket = newSockets[notPairedClients];

          ownLobbyUsers.push_back(user1);
        }
        if(client_message[0]=='2'){
          // Dołączanie do rozgrywki
          bzero(client_message,sizeof(client_message));
          if(recv(newSockets[notPairedClients],client_message,sizeof(client_message),0)<1){
            printf("bład przypisania nazwy2: %s\n",client_message);
            break;
          }

          char nickname[20];
          for(int i=0;i<20;i++){
            nickname[i]=client_message[i];
          }

          int useridx = findIdxOfActiveUserByName(nickname);
          if(useridx ==-1){
            printf("Nie znaleziono takiego gracza\n");
          }else{
            user user1 = ownLobbyUsers[useridx]; 

            user user2;
            user2.userID=userIds[notPairedClients];
            strcpy(user2.userName,userNames[notPairedClients]);
            user2.gameID = user1.gameID;
            user2.color = 1;
            user2.userSocket = newSockets[notPairedClients];


            long socketArray1[4] = {user2.userSocket,user1.userSocket,user2.gameID,user2.userID};
            long socketArray2[4] = {user1.userSocket,user2.userSocket,user2.gameID,user1.userID};

            if( pthread_create(&thread_id1,NULL,socketThread,&socketArray1) != 0 )
             printf("Failed to create thread\n");

            if( pthread_create(&thread_id2,NULL,socketThread,&socketArray2) != 0 )
             printf("Failed to create thread\n");

            pthread_detach(thread_id1);
            pthread_detach(thread_id2);
            printf("Created game: gameid %ld\n",user2.gameID);



            activeUsers.push_back(user1);
            activeUsers.push_back(user2);
            ownLobbyUsers.erase(ownLobbyUsers.begin()+findIdxOfActiveUserByName(user1.userName));

          }

        }

    }
  return 0;
}


long findGameIndex(long gameID){
  for(long unsigned int i=0;i<games.size();i++){
    if(games[i]->getGameID()==gameID){
      return i;
    }
  }
  return -1;
}

long genGameId(){
  //bool exists = true;
  long randId = random();
  // while(exists){
  //   exists = false;
  //   randId = random();
  //   for(int i=0; i<games.size();i++){
  //     if(games[i]->getGameID()==randId)
  //       exists = true;
  //   }
  // }
  return randId;
}

long genUserId(){
  //bool exists = true;
  long randId = random();
  // while(exists){
  //   exists = false;
  //   randId = random();
  //   for(int i=0; i<activeUsers.size();i++){
  //     if(activeUsers[i].userID==randId)
  //       exists = true;
  //   }
  // }

  return randId;
}

int findIdxOfActiveUserByID(long userID){
  for(long unsigned int i=0;i<activeUsers.size();i++){
    if(activeUsers[i].userID==userID)
      return i;
  }
  return -1;
}

int findIdxOfActiveUserByGameID(long gameID){
  for(long unsigned int i=0;i<activeUsers.size();i++){
    if(activeUsers[i].gameID==gameID)
      return i;
  }
  return -1;
}

int findIdxOfActiveUserByName(char nickname[20]){
  for(long unsigned int i=0;i<ownLobbyUsers.size();i++){
    if(strcmp(ownLobbyUsers[i].userName,nickname)==0)
      return i;
   }
  return -1;
}