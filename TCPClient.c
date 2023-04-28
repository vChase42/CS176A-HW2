#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>

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
    //check for valid number of arguments
    if(argc != 3){
        printf("INVALID NUMBER OF ARGUMENTS, 3 EXPECTED\n");
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);

    //create socket
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1) {
        printf("ERROR CREATING INITIAL SOCK\n");
        return 1;
    }

    //initialize address to send messages to
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);

    //establish a TCP connection with a server
    socklen_t sock_len = sizeof(struct sockaddr_in);
    if(connect(sock, (struct sockaddr*)&addr,sock_len) == -1){
        printf("ERROR ON CONNECTING TO SERVER\n");
        return 1;
    }

    //get input from user
    char input[128];
    memset(input,0,128);

    printf("Enter string: ");
    scanf("%s", input);

    //send user input to server
    send(sock,input,sizeof(input),0);
    
    char message[128];
    int num;

    //loop receiving messages from server
    do{
        //receive message from server
        recv(sock,message,sizeof(message),0);
        printf("From server: %s\n",message);
        
        //if the message isnt ALL digits, break.
        if(!is_digits(message)){
            break;
        }

        //convert message to a number, check if its a single digit, if not: loop
        num = atoi(message);
        memset(message,0,128); //reset message
    }while(num >= 10);

    close(sock);
    return 0;
}