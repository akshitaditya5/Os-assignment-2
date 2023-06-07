/*             GROUP 2    (CLIENT)

    NAME                            ID

ARYAN BHARDWAJ----------------- 2020A3PS0587H
RUSHIL SRIVASTAVA---------------2020AAPS1408H
ARJUN AGARWAL-------------------2020A3PS2135H
AKSHIT ADITYA-------------------2020A1PS2509H
MANAS KUMAR KHANDWAL------------2020A4PS2272H
SHASHWAT TRIPATHI---------------2020A3PS2210H
ROHIT ARYAN --------------------2020AAPS0293H

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdbool.h>

#define SHM_SIZE 1024

struct client_info {
    pid_t pid;
    char name[64];
    int num_clients;
    int num_responses;
    bool flag;
    
};

struct request {
    pid_t pid;
    int operation;
    int arg1;
    int arg2;
};

int main() 
{
    // Get the shared memory key from the user
    key_t key;
    printf("Enter the shared memory key: ");
    scanf("%d", &key);
    fflush(stdin);

    // Asking for the shared memory from the server 
    int shmid = shmget(key, SHM_SIZE, 0666);
    if(shmid == -1) 
    {
        perror("Error fetching shared memory segment on client side....");
        exit(1);
    }

    char shm = (char) shmat(shmid, NULL, 0);
    if(shm == (char*) -1)
     {
        perror("Error attaching shared memory segment on client side...");
        exit(1);
    }
    printf("Client is running...\n");
    int counter=0;
    // Get client info
    struct client_info client = (struct client_info) shm;
    client->flag=false;
    pid_t pid = getpid();
    client->pid = pid;
    while(getchar() != '\n');
    printf("Enter your name: ");
    fgets(client->name, sizeof(client->name), stdin);
    client->name[strcspn(client->name, "\n")] = '\0';
    
    printf("Client '%s' with PID %d registered\n", client->name, pid);
    
    // Request arithmetic operations or check if a number is prime from menu driven approach
    while(1) {
        
        printf("Choose an operation:\n");
        printf("1. Addition\n");
        printf("2. Subtraction\n");
        printf("3. Multiplication\n");
        printf("4. Division\n");
        printf("5. Check if a number is prime\n");
        printf("6. Check if the number is even or odd\n");
        printf("7. Check if the number is negative or not\n");
        printf("8. Quit\n");
        printf("Enter your choice: ");

        int choice=0;
        scanf("%d", &choice);
        if(choice!=0){
            counter++;
        }
        
        client->num_responses+=counter;
        if(choice == 8) {
            break;
        }

        if(choice < 1 || choice > 8) {
            
            printf("Chosen Choice is Invalid....\n");
            continue;
        }

        // Get the operation arguments
        int arg1, arg2;
        printf("Enter first argument: ");
        scanf("%d", &arg1);
        
        if(choice>=1&&choice<=4) {
            printf("Enter second argument: ");
            scanf("%d", &arg2);
        }

        // Send the request to the server
        struct request req = (struct request) shm;
        req->pid = pid;
        req->operation = choice;
        req->arg1 = arg1;
        req->arg2 = arg2;

        // Wait for the response from the server
        while(1) {
            if(req->pid == 0) {
                break;
            }

            sleep(1);
        }

        // Print the result
        printf("Result of the operation: %d\n", req->arg1);
        
    }
    
    // Detach from the shared memory
    if(shmdt(shm) == -1) {
        perror("Error detaching from shared memory....");
        exit(1);
    }
   printf("Detaching from shared memory..Client is terminated...\n");
    return 0;
}