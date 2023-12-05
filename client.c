#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include<pthread.h>

void tempCharPrint(char board[255]);

void * clientThread(void *arg)
{
  int clientSocket = *((int *)arg);
  char sentMessage[255];
    while(1){
      scanf("%s",sentMessage);
      if( send(clientSocket,sentMessage,sizeof(sentMessage),0) < 0)
      {
        printf("Send failed\n");
      }
      bzero(sentMessage,sizeof(sentMessage));
    }
    pthread_exit(NULL);
}

int main(int argc, char ** argv){

  char message[256];
  char buffer[256];
  int clientSocket;
  struct sockaddr_in serverAddr;
  socklen_t addr_size;

  // Create the socket. 
  clientSocket = socket(AF_INET,SOCK_STREAM,0);

  //Configure settings of the server address
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(1100);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  // Jak będzie rozpoczynana rozgrywka?
  // 1. dobór pierwzego lepszego gracza
  // 2. stworzenie rozgrywki
  // 3. dołączenie do rozgrywki




  char user[255];
  printf("Podaj swoja nazwe:\n");
  scanf("%s",user);

  char choice[255];
  printf("\nWybierz rodzaj rozgrywki:\n");
  scanf("%s",choice);

  char user2[255];
  if(choice[0]=='2'){
    printf("\nPodaj nazwe gracza do którego chcesz dołączyć: \n");
    scanf("%s",user2);
  }


  //Connect the socket to the server using the address
  addr_size = sizeof serverAddr;

  connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

  send(clientSocket,user,sizeof(user),0);

  send(clientSocket,choice,sizeof(choice),0);

  if(choice[0]=='2'){
    send(clientSocket,user2,sizeof(user2),0);
  }




  pthread_t thread_id;
  if( pthread_create(&thread_id,NULL,clientThread,&clientSocket) != 0)
    printf("Failed to create thread\n");
  //pthread_detach(thread_id);

    int n;
    char recvMessage[255];
    while(1){
      if((n=recv(clientSocket,recvMessage,sizeof(recvMessage),0))<1){
        printf("receive failed :%d\n",n);
        exit(0);
      }else{
        printf("board: %s\n",recvMessage);
        tempCharPrint(recvMessage);
        bzero(recvMessage,sizeof(recvMessage));
      }
    }

    //Print the received message
    printf("Data received: %s\n", buffer);

    memset(&message, 0, sizeof (message));
    
    close(clientSocket);

  return 0;
}

void tempCharPrint(char board[255]){
  for(int i=0;i<64;i++){
    printf("%c",board[i]);
    if(i%8==7){
      printf("\n");
    }
  }
}