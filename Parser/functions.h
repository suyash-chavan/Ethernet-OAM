enum _parserState {WAIT_FOR_RX, PARSE, RX_OAMPDU, RX_DATA, RX_LOOPBACK} parserState;

struct _message current;

void _WAIT_FOR_RX()
{
    
    receive_socket(current.data);
    
    printf("Received through Socket...\n");
    
    DEBUG((PACKET*)current.data);

    parserState = PARSE;
}


void _PARSE()
{
    PACKET *oampdu;
    oampdu = (PACKET*)(current.data);

    if (rxOk && RxOAMPDU)
        parserState = RX_OAMPDU;
    else if (rxOk && !RxOAMPDU && local_par_action == par_FWD)
        parserState = RX_DATA;
    else if (rxOk && !RxOAMPDU && local_par_action == par_LB)
        parserState = RX_LOOPBACK;
    else if (!rxOk || (!RxOAMPDU && local_par_action == par_DISCARD) )
        parserState = WAIT_FOR_RX;

}

void _RX_OAMPDU()
{
    current.mtype = CTL_OAMI_indication;
    
    printf("Message Type Sending: CTL_OAMI_indication\n");
    printf("---------------------------------\n\n");
    
    control_postbox_size++;
    send_message(current);
    parserState = WAIT_FOR_RX;
}

void _RX_DATA()
{
    current.mtype = MCF_MA_DATA_indication;
    
    
    printf("Message Type Sending: MCF_MA_DATA_indication\n");
    printf("---------------------------------\n\n");
    
    mac_client_postbox_size++;
    
    send_message(current);
    parserState = WAIT_FOR_RX;
}

void _RX_LOOPBACK()
{
    current.mtype = LBF_OAMI_request;
    
    
    parser_postbox_size++;
    printf("Message Type Sending: LBF_OAMI_request\n");
    printf("---------------------------------\n\n");
    
    
    send_message(current);

    parserState = WAIT_FOR_RX;
}


void parser()
{

    parserState = WAIT_FOR_RX;

    while (1)
    {

        switch (parserState)
        {
        case WAIT_FOR_RX:
            _WAIT_FOR_RX();
            break;

        case PARSE:
            _PARSE();
            break;

        case RX_OAMPDU:
            _RX_OAMPDU();
            break;

        case RX_DATA:
            _RX_DATA();
            break;

        case RX_LOOPBACK:
            _RX_LOOPBACK();
            break;
        }

    }

}
