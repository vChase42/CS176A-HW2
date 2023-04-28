#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>



int main(int argc, char* argv[]){
    //check for valid number of arguments
    if(argc != 2){
        printf("INVALID NUMBER OF ARGUMENTS, 2 EXPECTED\n");
        return 1;
    }
    int port = atoi(argv[1]);

    //create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("ERROR CREATING SERVER SOCKET\n");
        return 1;
    }

    //initialize address to send messages to
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    //bind server to a port
    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        // Handle error
        printf("BIND SOCKET ERROR\n");
        return 1;
    }

    //listen for a client
    socklen_t client_len = sizeof(struct sockaddr);
    if (listen(sock, 20) == -1) {
        // Handle error
        printf("FAILED TO LISTEN\n");
        return 1;
    }


    //main client-connection-calculation loop
    while(1){
        //accept a TCP connection with a client
        int client_socket = accept(sock, (struct sockaddr*)&addr, &client_len);
        if (client_socket == -1) {
            printf("Connection error\n");
        }

        //receive a message from a client
        char input[128];
        int bytes_received = recv(client_socket,input,sizeof(input),0);
        if(bytes_received <= 0){
            printf("RECEIVE MESSAGE ERROR\n");
            return 1;
        }

        //exit redundancy
        if(strcmp(input,"exit") == 0){     //exit gracefully if 'exit' received from client
            send(client_socket,"exiting",8,0);
            close(client_socket);
            break;
        }

        //main calculation loop
        int sum;
        do{
            sum = 0;
            int i = 0;
            while(input[i] != '\0'){  //loop until end of string
                int n = input[i] - '0';
                if(n < 0 || n > 9){   //return error message if non-digit character encountered
                    sum = 0;
                    char reply[] = "Sorry, cannot compute!";
                    send(client_socket,reply,sizeof(reply),0);
                    break;    
                }
                sum += n;
                i++;
            }
        
            //ANSWER
            memset(input,0,128);
            sprintf(input,"%d",sum); //convert number to charstring
            send(client_socket,input,sizeof(input),0);  //send reply to client
        }while(sum >= 10);
   
        close(client_socket);
    }


    close(sock);


    // printf("END, SOCKETS CLOSED\n");

    return 0;
}