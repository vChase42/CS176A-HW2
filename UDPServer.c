#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


int main(int argc, char* argv[]){
    //check for correct number of arguments
    if(argc != 2){
        printf("INVALID NUMBER OF ARGUMENTS, 2 EXPECTED\n");
        return 1;
    }
    int port = atoi(argv[1]);

    //create socket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        printf("ERROR CREATING SERVER SOCKET\n");
        return 1;
    }

    //create address in order to receive messages from clients
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    
    //bind address to a port
    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        // Handle error
        printf("BIND SOCKET ERROR\n");
        return 1;
    }

    //client addr, we only serve 1 client at a time.
    struct sockaddr_in client_addr = {0};
    socklen_t client_len;

    while(1){

        //receive message from client
        char input[200];
        int bytes_received = recvfrom(sock, input, sizeof(input), 0, (struct sockaddr *)&client_addr, &client_len);
        if(bytes_received <= 0){
            perror("RECEIVE MESSAGE ERROR\n");
            return 1;
        }

        //Exit Redundancy
        if(strcmp(input,"exit") == 0){     //exit gracefully if 'exit' received from client
            sendto(sock, "exiting", 8, 0, (struct sockaddr *)&client_addr, client_len); 
            break;
        }

        //Calculation loop
        int sum;
        do{
            sum = 0;
            int i = 0;
            while(input[i] != '\0'){  //loop until end of string
                int n = input[i] - '0';
                if(n < 0 || n > 9){ //if input[i] is a letter, return error message.
                    sum = 0;
                    char reply[] = "Sorry, cannot compute!";
                    sendto(sock, reply, strlen(reply), 0, (struct sockaddr *)&client_addr, client_len); 
                    break;    
                }
                sum += n;
                i++;
            }
        
            //ANSWER
            memset(input,0,200);
            sprintf(input,"%d",sum);  //convert number to char*
            sendto(sock, input, strlen(input), 0, (struct sockaddr *)&client_addr, client_len); 
        }while(sum >= 10);
   
    }


    close(sock);
    return 0;
}