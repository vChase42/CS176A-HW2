#include <netinet/in.h>
#include <openssl/ssl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


int main(){

    SSL_library_init();

    SSL_CTX* ctx = SSL_CTX_new(SSLv23_server_method());
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        printf("ERROR CREATING SERVER SOCKET\n");
        return 1;
    }

    int port = 8000;

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    
    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        // Handle error
        printf("BIND SOCKET ERROR\n");
        return 1;
    }

    struct sockaddr_in client_addr = {0};
    socklen_t client_len;

    
    while(1){

        char input[128];
        int bytes_received = recvfrom(sock, input, sizeof(input), 0, (struct sockaddr *)&client_addr, &client_len);
        if(bytes_received <= 0){
            perror("RECEIVE MESSAGE ERROR\n");
            return 1;
        }

        if(strcmp(input,"exit") == 0){     //exit gracefully if 'exit' received from client
            sendto(sock, "exiting", 8, 0, (struct sockaddr *)&client_addr, client_len); 
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
                    sendto(sock, reply, strlen(reply), 0, (struct sockaddr *)&client_addr, client_len); 
                    break;    
                }
                sum += n;
                i++;
            }
        
            //ANSWER
            memset(input,0,128);
            sprintf(input,"%d",sum);
            sendto(sock, input, strlen(input), 0, (struct sockaddr *)&client_addr, client_len); 
        }while(sum >= 10);
   
    }


    close(sock);
    SSL_CTX_free(ctx);

    // printf("END, SOCKETS CLOSED\n");

    return 0;
}