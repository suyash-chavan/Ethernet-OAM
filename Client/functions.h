#define MAX_COMMAND_SIZE 1024

char command[MAX_COMMAND_SIZE];

void get_input()
{
    getline(&command,MAX_COMMAND_SIZE,stdin);
}

void transmit_data(char *data)
{
    write(send_fd, data, strlen(data));          
}

void transmit_oampdu()
{
    FILE *fptr;
    
    fptr=fopen(command,"r");
    
    OAMPDU oampdu;
    
    /*
    
        I need to parse file and create OAMPDU;
    
    */
    
    transmit_data((char *)&oampdu);
}

void process_input()
{
    if(!strcmp(command,"TRANSMIT OAMPDU"))
    {
        printf("Path: ");
        get_input();
        transmit_oampdu();
    }
    else if(!strcmp(command,"LOOPBACK"))
    {
        loopback();
        printf("Loopback Command Sent...\n");
    }
    else if(!strcmp(command,"DISCOVERY"))
    {
        discovery();
        printf("Discovery Command Sent...\n");
    }
    else
    {
        printf("Invalid Command.\n");
    }
}

void parse_input()
{
    int i=0;
    
    while(command[i]!=0)
    {
        if((command[i]>=48 && command[i]<=57) || (command[i]>=65 && command[i]<=90) || command[i]>=97 && command[i]<=122)
        {
            if(command[i]>=97 && command[i]<=122)
                command[i]-=32;
        }
        else
        {
            printf("Invalid Command.\n");
            return ;
        }
        
        i++;
    }
    
    process_input();
}

void interface()
{
    printf("------------------ OAM Administrator ------------------\n");
    
    while(1)
    {
        printf(">");
        get_input();
        parse_input();
    }
}
