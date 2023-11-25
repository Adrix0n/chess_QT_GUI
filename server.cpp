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

//gcc program.c -lpthread -Wall -o outfile Remember to link pthread lib

char client_message[256];
//char buffer[256];

/*
  TODO:
    - Napisać obsługę rozłączenia klienta od serwera
    - poprawić odnośniki do odpowiednich gier w wątku, aby nie odnosił się do gry o indeksie gameId, gdyż liczba gier w wektorze może się zmniejszyć
    - Napisać obsługę zakończenia gry (usuwanie, przekazywanie odpowiedniej wiadomości zwrotnej)
*/


std::vector < chess_game* > games;

void * socketThread(void *arg)
{
  int *args = ((int *)arg);
  int newSocket1 = args[0];
  int newSocket2 = args[1];
  int gameId = args[2];

  printf("ns1: %d. ns2: %d, gamid: %d\n",newSocket1,newSocket2,gameId);
  int n;
  char recvMessage[255];

  chess_game *game = games[gameId];
  std::string actualBoard = game->getBoard();

  while(1){
    n=recv(newSocket1,recvMessage,sizeof(recvMessage),0);
    printf("%s\n",recvMessage);
    if(n<1)
      break;
    
    std::string playerMove(recvMessage);

    bool validMove = game->MoveFigureString(playerMove);
    if(validMove){
      printf("\n%s\n",game->getBoard().c_str());
      send(newSocket2,game->getBoard().c_str(),sizeof(recvMessage),0);
      send(newSocket1,game->getBoard().c_str(),sizeof(recvMessage),0);
    }
    memset(&recvMessage, 0, sizeof (recvMessage));
  }
  
  pthread_exit(NULL);
}

int main(){

  // ----- Server initialization -----
  int serverSocket, newSocket1, newSocket2;
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

  while(1)
    {
        //Accept call creates a new socket for the incoming connection
        addr_size = sizeof serverStorage;

        if(notPairedClients==0){
          newSocket1 = accept(serverSocket, (struct sockaddr *) &serverStorage, &addr_size);
          notPairedClients++;
        }
        if(notPairedClients==1){
          newSocket2 = accept(serverSocket, (struct sockaddr *) &serverStorage, &addr_size);
          notPairedClients++;
        }
        if(notPairedClients==2){
          int gameId = games.size();
          games.push_back(new chess_game(gameId,true));
          int socketArray1[3] = {newSocket1,newSocket2,gameId};
          int socketArray2[3] = {newSocket2,newSocket1,gameId};
          if( pthread_create(&thread_id1,NULL,socketThread,&socketArray1) != 0 )
           printf("Failed to create thread\n");

          if( pthread_create(&thread_id2,NULL,socketThread,&socketArray2) != 0 )
           printf("Failed to create thread\n");

          pthread_detach(thread_id1);
          pthread_detach(thread_id2);
          printf("Created game: gameid %d\n",gameId);
          notPairedClients = 0;
        }
        
        //pthread_join(TODO);?
    }
  return 0;
}
