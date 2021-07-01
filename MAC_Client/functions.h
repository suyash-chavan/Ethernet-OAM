size_t MAX_COMMAND_SIZE = 1024;

char *command;

void get_input()
{
    getline(&command,&MAX_COMMAND_SIZE,stdin);
}

void get_mac_frames()
{
    get_mac_client_postbox();
    PACKET* mac_frame=(PACKET*)message_get_mac_client.data;
    printf("\nReceived Mac Frame with Data: %s\n",mac_frame->payload.MAC.data);
}

void transmit_frame()
{
    struct _message message;    
    
    PACKET *frame;
    frame = (PACKET *) malloc(sizeof(PACKET));
    
    memset(frame,0,sizeof(frame));

    for (int i = 0; i < 6; i++)
        frame->DA[i] = Slow_Protocols_Multicast[i];

    for (int i = 0; i < 2; i++)
        frame->length[i] = Slow_Protocols_Type[i];

    frame->payload.OAMPDU.subtype = OAM_subtype;

    for (int i = 0; i < 2; i++)
        frame->SA[i] = source_address[i];
    printf("Command is: %s",command);
    memcpy(frame->payload.MAC.data,command,strlen(command));
    printf("Sending this: %s\n",frame->payload.MAC.data);

    memcpy(message.data, (char*)frame,sizeof(*frame));
    
    message.mtype = MCF_MA_DATA_request;
    
    send_message(message);
    
    free(frame);

}

void process_input()
{
    if(!strcmp(command,"SEND"))
    {
        printf("Data: ");
        get_input();
        transmit_frame();
    }
    else if(!strcmp(command,"RECEIVE"))
    {
        get_mac_frames();
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
    printf("------------------ MAC Client ------------------\n");
    
    command=(char*)malloc(MAX_COMMAND_SIZE*sizeof(char));
    
    while(1)
    {
        printf(">");
        get_input();
        command[strlen(command)-1]=0;
        parse_input();
    }
}
