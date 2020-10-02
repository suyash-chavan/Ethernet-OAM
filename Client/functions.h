size_t MAX_COMMAND_SIZE = 1024;

#define bitset(byte,nbit)   ((byte) |=  (1<<(nbit)))
#define bitclear(byte,nbit) ((byte) &= ~(1<<(nbit)))
#define bitflip(byte,nbit)  ((byte) ^=  (1<<(nbit)))
#define bitcheck(byte,nbit) ((byte) &   (1<<(nbit)))

char *command;

unsigned short get_oampdu_config()
{
    unsigned short oampdu_config = 0;
    int track = 1;

    /*
        Discussion Needed...
    */

}

unsigned char get_oam_config()
{
    unsigned char oam_config = 0;

    if (local_oam_mode == ACTIVE)
        bitset(oam_config, 0);



    if (local_unidirectional == TRUE)
        bitset(oam_config, 1);



    if (remote_loopback_support == TRUE)
        bitset(oam_config, 2);



    if (link_event_support == TRUE)
        bitset(oam_config, 3);



    if (variable_retrieval_support == TRUE)
        bitset(oam_config, 4);


    return oam_config;
}

unsigned char get_state()
{
    unsigned char state = 0;

    switch (local_par_action)
    {
    case par_FWD:
        break;

    case par_LB:
        bitset(state, 0);
        break;

    case par_DISCARD:
        bitset(state, 1);
        break;
    }

    switch (local_mux_action)
    {
    case mux_FWD:
        break;

    case mux_DISCARD:
        bitset(state, 2);
        break;
    }

    return state;
}

unsigned int generate_flags()
{
    unsigned int flags = 0;

    if (local_link_status == FAIL)
        bitset(flags, 0);

    if (local_dying_gasp == TRUE)
        bitset(flags, 1);

    if (local_critical_event == TRUE)
        bitset(flags, 2);


    if (local_satisfied == FALSE)
        ;
    else if (local_satisfied == TRUE && local_pdu != ANY)
        bitset(flags, 3);
    else if (local_satisfied == TRUE && remote_stable == TRUE && local_pdu == ANY)
    {
        bitset(flags, 3);
        bitset(flags, 4);
    }

/*
    if (remote_state_valid == TRUE)
    {
        if (peer.peer_local_evaluating == 1)
            bitset(flags, 5);

        if (peer.peer_local_stable == 1)
            bitset(flags, 6);
    }
*/
    return flags;
}

void get_input()
{
    getline(&command,&MAX_COMMAND_SIZE,stdin);
}

/*
void transmit_data(char *data)
{
    write(send_fd, data, strlen(data));
}

void transmit_oampdu()
{
    FILE *fptr;
    
    fptr=fopen(command,"r");
    
    OAMPDU oampdu;
    
    transmit_data((char *)&oampdu);
}
*/

void loopback()
{
    struct _message message;    
    
    PACKET *oampdu;
    oampdu = (PACKET *) malloc(sizeof(PACKET));
    
    memset(oampdu,0,sizeof(oampdu));

    for (int i = 0; i < 6; i++)
        oampdu->DA[i] = Slow_Protocols_Multicast[i];

    for (int i = 0; i < 2; i++)
        oampdu->length[i] = Slow_Protocols_Type[i];

    oampdu->payload.OAMPDU.subtype = OAM_subtype;

    for (int i = 0; i < 2; i++)
        oampdu->SA[i] = source_address[i];

    oampdu->payload.OAMPDU.flags = generate_flags();
    oampdu->payload.OAMPDU.code = Loopback_Control_OAMPDU;
    oampdu->payload.OAMPDU.data.information_tlv.local_info.info_type = 0x01;
    oampdu->payload.OAMPDU.data.information_tlv.local_info.info_len = 0x10;
    oampdu->payload.OAMPDU.data.information_tlv.local_info.oam_version = local_oam_version; // Should be stored in Memory
    oampdu->payload.OAMPDU.data.information_tlv.local_info.revision = local_tlv_revision;  // Should be stored in Memory
    oampdu->payload.OAMPDU.data.information_tlv.local_info.state = get_state();
    oampdu->payload.OAMPDU.data.information_tlv.local_info.oam_config = get_oam_config();
    oampdu->payload.OAMPDU.data.information_tlv.local_info.oampdu_config = get_oampdu_config();
    
    oampdu->payload.OAMPDU.data.loopback_control.remote_loopback_command = 0x01;

    message.mtype = OAMPDU_request;
    

    memcpy(message.data, (char*)oampdu,sizeof(*oampdu));
    
    send_message(message);
    control_postbox_size++;
    
    free(oampdu);

}

void process_input()
{
    printf("Processed Command: %s\n",command);
    if(!strcmp(command,"TRANSMIT OAMPDU"))
    {
        printf("Path: ");
        get_input();
        //transmit_oampdu();
    }
    else if(!strcmp(command,"LOOPBACK"))
    {
        loopback();
        printf("Loopback Command Sent...\n");
    }
    else if(!strcmp(command,"DISCOVERY"))
    {
        //discovery();
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
    
    command=(char*)malloc(MAX_COMMAND_SIZE*sizeof(char));
    
    while(1)
    {
        printf(">");
        get_input();
        command[strlen(command)-1]=0;
        parse_input();
    }
}
