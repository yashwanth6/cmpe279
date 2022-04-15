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
int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    int uid_val;
    pid_t pid;
    struct passwd* pw_nobody;

    // Show ASLR
    printf("execve=0x%p\n", execve);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Attaching socket to port 80
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 80
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

    //The split happens here
    
    //To create child use fork() function. 
    //For successful fork(), it returns pid>0 and represents pid of new child process
    //In child, It returns 0 for successful creation and less than 0 for unsuccessful creation of child
    pid=fork(); 
    printf("pid = %ld\n", (long) pid);

    //If child creation is successful
    if(pid==0)
    {
        printf("Child process running\n");
        printf("Current child id : %d \n", getuid());

        pw_nobody = getpwnam("nobody");
        
        //get nobodyid from the OS
        if(pw_nobody == NULL){
            printf("Cannot find id of nobody user\n");
        }
        
        //Set the process id of child to nobody's id. Then they wont have sudo previliges
        uid_val=setuid(pw_nobody->pw_uid); 
        printf("Child id after privilege drop: %d \n", getuid());
        
        if (uid_val==0)
        {
            //Privilege drop successful 
            printf("Child privilege dropped successsfully\n");
            printf("Child id is: %d \n", getuid());
            valread = read( new_socket , buffer, 1024);
            printf("%s\n",buffer );
            send(new_socket , hello , strlen(hello) , 0 );
            printf("Hello message sent\n");
        }

        else{
            printf("Error during privilige drop\n");
            return 0;
        }
    }
    else if(pid>0){
        //Here fork returned pid > 0 at start for successful creation
        printf("pid before wait()= %ld\n", (long) pid);
        //Lets wait until Child Process is completed
        wait(NULL);
        printf("pid after child process is done= %ld\n", (long) pid);
        printf("Parent process is running\n");
    }

    else{
        printf("Error forking\n");
    }

    

    return 0;
}
