
enum _multiplexerState {WAIT_FOR_TX, TX_FRAME, CHECK_PHY_LINK} multiplexerState;

void _WAIT_FOR_TX()
{
    printf("Messages in Multiplexer Queue: %d\n",multiplexer_postbox_size);
    get_multiplexer_postbox();
    multiplexer_postbox_size--;

    PACKET *packet=(PACKET*)message_get_multiplexer.data;

    printf("Received Message Type: %d\n",message_get_multiplexer.mtype);
    
    if (message_get_multiplexer.mtype == CTL_OAMI_request)
    {
        multiplexerState = TX_FRAME;
    }
    else if (message_get_multiplexer.mtype != CTL_OAMI_request && ((message_get_multiplexer.mtype == MCF_MA_DATA_request && local_mux_action == mux_FWD) || message_get_multiplexer.mtype == LBF_OAMI_request))
    {
        multiplexerState = CHECK_PHY_LINK;
    }
    else
    {
        printf("Received Invalid Packet!!!\n");
    }

}


void _TX_FRAME()
{

    printf("Sending Message through Socket...\n");
    printf("-----------------------------------------\n\n");
    
    DEBUG((PACKET*)message_get_multiplexer.data);
    
    send_socket(message_get_multiplexer.data);

    multiplexerState = WAIT_FOR_TX;
}

void _CHECK_PHY_LINK()
{
    if (local_unidirectional == TRUE && local_link_status == FAIL)
        multiplexerState = WAIT_FOR_TX;
    else
        multiplexerState = TX_FRAME;
}


void multiplexer()
{

    multiplexerState = WAIT_FOR_TX;

    while (1)
    {

        switch (multiplexerState)
        {
        case WAIT_FOR_TX:
            _WAIT_FOR_TX();
            break;

        case TX_FRAME:
            _TX_FRAME();
            break;

        case CHECK_PHY_LINK:
            _CHECK_PHY_LINK();
            break;

        }

    }

}
