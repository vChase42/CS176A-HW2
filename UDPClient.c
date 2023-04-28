#include <netinet/in.h>
#include <openssl/ssl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>


int is_digits(const char buffer[]){
    for(int i = 0; buffer[i] != '\0'; i++){
        if(!isdigit(buffer[i])){
            return 0;
        }
    }
    return 1;
}



int main(){
    SSL_library_init();
    SSL_CTX* ctx = SSL_CTX_new(SSLv23_server_method());

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        printf("ERROR CREATING INITIAL SOCK\n");
        return 1;
        // Handle error
    }


    int port = 8000;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(port);






    char input[128];
    memset(input,0,128);

    printf("Enter string: ");
    scanf("%s", input);
    // printf("%s\n",input);

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
    SSL_CTX_free(ctx);

    return 0;
}