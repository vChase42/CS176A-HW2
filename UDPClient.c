#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


int is_digits(const char buffer[]){
    for(int i = 0; buffer[i] != '\0'; i++){
        if(!isdigit(buffer[i])){
            return 0;
        }
    }
    return 1;
}



int main(int argc, char* argv[]){
    if(argc != 3){
        printf("INVALID NUMBER OF ARGUMENTS, 3 EXPECTED\n");
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);



    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        printf("ERROR CREATING INITIAL SOCK\n");
        return 1;
        // Handle error
    }



    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);






    char input[128];
    memset(input,0,128);

    printf("Enter string: ");
    scanf("%s", input);
    // printf("%s\n",input);
    sleep(1);
    sendto(sock, input, strlen(input), 0, (struct sockaddr *)&addr, sizeof(addr));    
    
    char req[128];

    int num;
    do{
        socklen_t s_addr = sizeof(addr);
        int bytes_received = recvfrom(sock, req, sizeof(req), 0, (struct sockaddr *)&addr, &s_addr);
        if(bytes_received <= 0){
            perror("RECEIVE MESSAGE ERROR\n");
            return 1;
        }else{
            req[bytes_received] = '\0';
        }
        printf("From server: %s\n",req);
        //ANSWER
        
        if(!is_digits(req)){
            break;
        }

    
        num = atoi(req);
        memset(req,0,128);
    }while(num >= 10);

    close(sock);


    return 0;
}