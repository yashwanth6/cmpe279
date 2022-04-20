// Server side C/C++ program to demonstrate Socket programming

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>

#define PORT 8081

int set_sock();
char *hello = "Hello from server";

int main(int argc, char const *argv[])
{
    if(argc==1)
    {
        int new_socket;
        new_socket=set_sock();
        pid_t current;
        char soc_id[12];
        current=fork();

        if(current==0)
        {
            printf("Child process is running\n");

            //re exec
            sprintf(soc_id, "%d", new_socket);
            printf("Socket string id: %s", soc_id);
            char *args[] = {"./server", soc_id, NULL}; 
            execvp(args[0],args);

            }
        else if(current>0){
            wait(NULL);
            printf("Parent is running\n");
        }

        else{
            printf("Error while forking\n");
        }
        }

    else{
        
        //re execution of the program
        printf("Current child id is: %d \n", getuid());
        //drop privilege
        struct passwd* pw_nobody;
        
        pw_nobody = getpwnam("nobody");
        if(pw_nobody == NULL){
            printf("Cannot find id of nobody user\n");
        }

        int uid_val;
        uid_val=setuid(pw_nobody->pw_uid); 
        printf("Child id after privilege drop is: %d \n", getuid());
            
        if (uid_val==0)
        {
            printf("Child privilege dropped\n");
            printf("Child id is: %d \n", getuid());
        
            //sending message 
            int valread;
            char buffer[1024] = {0};
            valread = read(atoi(argv[1]), buffer, 1024);
            printf("%s\n",buffer );
            send(atoi(argv[1]) , hello , strlen(hello) , 0 );
            printf("Hello message sent\n");
        }

        else{
            printf("Error during privilige drop\n");
    }
}

}


int set_sock(){
    printf("execve=0x%p\n", execve);
    int server_fd, new_socket;
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Attaching socket to port 8081
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 

    // Forcefully attaching socket to the port 8081
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    
    }
    return new_socket;

}
