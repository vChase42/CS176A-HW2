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

    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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

    // printf("Attempting to establish connection\n");
    socklen_t sock_len = sizeof(struct sockaddr_in);
    if(connect(sock, (struct sockaddr*)&addr,sock_len) == -1){
        printf("ERROR ON CONNECTING TO SERVER\n");
        return 1;
    }


    // printf("Connection established!\n");


    char input[128];
    memset(input,0,128);

    printf("Enter string: ");
    scanf("%s", input);
    // printf("%s\n",input);

    send(sock,input,sizeof(input),0);
    
    
    char req[128];

    int num;
    do{
        recv(sock,req,sizeof(req),0);
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