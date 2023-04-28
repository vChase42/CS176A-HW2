#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//This function checks if a char string consts of ONLY digits
int is_digits(const char buffer[]){
    for(int i = 0; buffer[i] != '\0'; i++){
        if(!isdigit(buffer[i])){
            return 0;
        }
    }
    return 1;
}



int main(int argc, char* argv[]){
    if(argc != 3){  //test for correct number of arguments
        printf("INVALID NUMBER OF ARGUMENTS, 3 EXPECTED\n");
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);

    //Create a socket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        printf("ERROR CREATING INITIAL SOCK\n");
        return 1;
    }

    //create address to send messages to
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);

    //Initialize and get input from user
    char input[128];
    memset(input,0,128);

    printf("Enter string: ");
    scanf("%s", input);

    //send message to server
    sendto(sock, input, strlen(input), 0, (struct sockaddr *)&addr, sizeof(addr));    
    

    char message[128];  
    int num;  //num is used as a test to see if server is done sending messages
    do{
        //receive message
        socklen_t s_addr = sizeof(addr);
        int bytes_received = recvfrom(sock, message, sizeof(message), 0, (struct sockaddr *)&addr, &s_addr);
        if(bytes_received <= 0){
            perror("RECEIVE MESSAGE ERROR\n");
            return 1;
        }else{
            message[bytes_received] = '\0';   //make sure that the message is null terminated
        }
        printf("From server: %s\n",message);
        //check if message is an error message or a number
        if(!is_digits(message)){
            break;
        }

        //convert message to number and loop only if its a single digit
        num = atoi(message);
        //clear message
        memset(message,0,128);
    }while(num >= 10);

    close(sock);

    return 0;
}