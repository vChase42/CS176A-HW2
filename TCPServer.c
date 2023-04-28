#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>



int main(){
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("ERROR CREATING SERVER SOCKET\n");
        return 1;
    }

    int port = 8000;

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    socklen_t client_len = sizeof(struct sockaddr);
    
    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        // Handle error
        printf("BIND SOCKET ERROR\n");
        return 1;
    }

    if (listen(sock, 20) == -1) {
        // Handle error
        printf("FAILED TO LISTEN\n");
        return 1;
    }


    
    while(1){
        int client_socket = accept(sock, (struct sockaddr*)&addr, &client_len);
        if (client_socket == -1) {
            printf("Connection error\n");
        }else{
            // printf("Successfully connected!\n");
        }


        char input[128];
        int bytes_received = recv(client_socket,input,sizeof(input),0);
        if(bytes_received <= 0){
            printf("RECEIVE MESSAGE ERROR\n");
            return 1;
        }


        if(strcmp(input,"exit") == 0){     //exit gracefully if 'exit' received from client
            send(client_socket,"exiting",8,0);
            close(client_socket);
            break;
        }

        int sum;
        do{
            sum = 0;
            int i = 0;
            while(input[i] != '\0'){
                int n = input[i] - '0';
                if(n < 0 || n > 9){
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
            sprintf(input,"%d",sum);
            send(client_socket,input,sizeof(input),0);
        }while(sum >= 10);
   
        close(client_socket);
    }


    close(sock);


    // printf("END, SOCKETS CLOSED\n");

    return 0;
}