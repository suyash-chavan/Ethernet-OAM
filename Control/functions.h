#define pdu_limit 10
#define max_queue_depth 7

enum _controlState {WAIT_FOR_INPUT, PROCESS, TRANSMIT} controlState;
enum _discoveryState {FAULT, ACTIVE_SEND_LOCAL_PRE, ACTIVE_SEND_LOCAL_POST, PASSIVE_WAIT_PRE, PASSIVE_WAIT_POST, SEND_LOCAL_REMOTE, SEND_LOCAL_REMOTE_OK, SEND_ANY} discoveryState;
enum _transmitState {RESET, WAIT_FOR_TX, DEC_PDU_CNT, TX_OAMPDU} transmitState;

enum _discoveryExternalState {IN_PROGRESS, COMPLETED} discoveryExternalState;
enum _selectState {PARSER, CLIENT, SELECT} selectState;
enum _boolean pdu_timer_done;

struct itimerval local_lost_link_timer;
struct itimerval pdu_timer;
struct itimerval fault_timer;

// Declaring Timer States
enum _timerState {ON, OFF} local_lost_link_timer_State, pdu_timer_State, fault_timer_State;

enum _boolean Loopback_Control_OAMPDU_Sent;

int pdu_cnt = 10; // For Debugging in Future

#define bitset(byte,nbit)   ((byte) |=  (1<<(nbit)))
#define bitclear(byte,nbit) ((byte) &= ~(1<<(nbit)))
#define bitflip(byte,nbit)  ((byte) ^=  (1<<(nbit)))
#define bitcheck(byte,nbit) ((byte) &   (1<<(nbit)))

// Interval of Timers
#define local_lost_link_timer_interval 5
#define pdu_timer_interval 1
#define fault_timer_interval 1

unsigned short local_info_revision=1;

/*
    Declaration Functions.
*/

void SEND_INFORMATION_OAMPDU();
unsigned short get_oampdu_config();
unsigned char get_oam_config();
unsigned char get_state();
unsigned int generate_flags();
void init_local_lost_link_timer();
void stop_local_lost_link_timer();
void init_pdu_timer();
void init_fault_timer();
void local_lost_link_timer_expired(int);
void pdu_timer_expired(int);
void fault_timer_expired(int);
void _INIT_TRANSMIT();
void _TX_OAMPDU();
void _DEC_PDU_CNT();
void _WAIT_FOR_TX();
void _TRANSMIT(struct _message*);
void _FAULT();
void _ACTIVE_SEND_LOCAL_PRE();
void _ACTIVE_SEND_LOCAL_POST();
void _PASSIVE_WAIT_PRE();
void _PASSIVE_WAIT_POST();
void _SEND_LOCAL_REMOTE();
void _SEND_LOCAL_REMOTE_OK();
void _SEND_ANY();
void _DISCOVERY();
void process();
void _WAIT();
void control();


void SEND_INFORMATION_OAMPDU()
{
    struct _message message;

    /*
        Create OAMPDU and attach in message...
    */
    
    
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
    oampdu->payload.OAMPDU.code = Information_OAMPDU;
    oampdu->payload.OAMPDU.data.information_tlv.local_info.info_type = 0x01;
    oampdu->payload.OAMPDU.data.information_tlv.local_info.info_len = 0x10;
    oampdu->payload.OAMPDU.data.information_tlv.local_info.oam_version = local_oam_version; // Should be stored in Memory
    oampdu->payload.OAMPDU.data.information_tlv.local_info.revision = local_tlv_revision;  // Should be stored in Memory
    oampdu->payload.OAMPDU.data.information_tlv.local_info.state = get_state();
    oampdu->payload.OAMPDU.data.information_tlv.local_info.oam_config = get_oam_config();
    oampdu->payload.OAMPDU.data.information_tlv.local_info.oampdu_config = get_oampdu_config();
    oampdu->payload.OAMPDU.data.information_tlv.local_info.revision = local_info_revision;
    
    /*
        Nedd to also copy peer info...
    */

    message.mtype = CTL_OAMI_request;

    memcpy(message.data, (char*)oampdu,sizeof(*oampdu));

    _TRANSMIT(&message);

    free(oampdu);
}

void SEND_LOOPBACK_CONTROL_OAMPDU()
{
    struct _message message;

    /*
        Create OAMPDU and attach in message...
    */

    _TRANSMIT(&message);
}

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


    if (remote_state_valid == TRUE)
    {
        if (peer.peer_local_evaluating == 1)
            bitset(flags, 5);

        if (peer.peer_local_stable == 1)
            bitset(flags, 6);
    }

    return flags;
}

void init_local_lost_link_timer()
{
    signal(SIGALRM, local_lost_link_timer_expired);

    local_lost_link_timer.it_value.tv_sec = local_lost_link_timer_interval;
    local_lost_link_timer.it_value.tv_usec = 0;
    local_lost_link_timer.it_interval = local_lost_link_timer.it_value;

    if (setitimer(ITIMER_REAL, &local_lost_link_timer, NULL) == -1)
    {
        printf("Error in setting Alarm - init_local_lost_link_timer\n");
        exit(EXIT_FAILURE);
    }

    local_lost_link_timer_State = ON;
    local_lost_link_timer_done = FALSE;
}

void stop_local_lost_link_timer()
{
    printf("IN STOP LOCAL LOST LINK TIMER \n ---------------------------------\n\n");
    signal(SIGALRM, local_lost_link_timer_expired);

    local_lost_link_timer.it_value.tv_sec = 0;
    local_lost_link_timer.it_value.tv_usec = 0;
    local_lost_link_timer.it_interval = local_lost_link_timer.it_value;

    if (setitimer(ITIMER_REAL, &local_lost_link_timer, NULL) == -1)
    {
        printf("Error in stopping Alarm - stop_local_lost_link_timer\n");
        exit(EXIT_FAILURE);
    }

    local_lost_link_timer_State = OFF;
    local_lost_link_timer_done = FALSE;
}

void init_pdu_timer()
{
    if (signal(SIGALRM, pdu_timer_expired) == SIG_ERR)
    {
        printf("Unable to catch Signal");
        exit(EXIT_FAILURE);
    }

    pdu_timer.it_value.tv_sec = pdu_timer_interval;
    pdu_timer.it_value.tv_usec = 0;
    pdu_timer.it_interval = pdu_timer.it_value;

    if (setitimer(ITIMER_REAL, &pdu_timer, NULL) == -1)
    {
        printf("Error in setting Alarm - init_pdu_timer\n");
        exit(EXIT_FAILURE);
    }

    pdu_timer_State = ON;
    pdu_timer_done = FALSE;

}

void init_fault_timer()
{
    signal(SIGALRM, fault_timer_expired);

    fault_timer.it_value.tv_sec = fault_timer_interval;
    fault_timer.it_value.tv_usec =  0;
    fault_timer.it_interval = fault_timer.it_value;
    
    if(multiplexer_postbox_size<max_queue_depth)
    {
        printf("Multiplexer Queue Depth: %d\n",multiplexer_postbox_size);
        PACKET *oampdu;
        oampdu = (PACKET *) malloc(sizeof(PACKET));
        
        memset(oampdu,0,sizeof(*oampdu));

        for (int i = 0; i < 6; i++)
            oampdu->DA[i] = Slow_Protocols_Multicast[i];

        for (int i = 0; i < 2; i++)
            oampdu->length[i] = Slow_Protocols_Type[i];

        oampdu->payload.OAMPDU.subtype = OAM_subtype;

        for (int i = 0; i < 2; i++)
            oampdu->SA[i] = source_address[i];

        oampdu->payload.OAMPDU.flags = generate_flags();
        oampdu->payload.OAMPDU.code = Information_OAMPDU;
        oampdu->payload.OAMPDU.data.information_tlv.local_info.info_type = 0x00;

        struct _message message;

        message.mtype = CTL_OAMI_request;

        memcpy((PACKET*)message.data, oampdu, sizeof(*oampdu));

        _TRANSMIT(&message);

        free(oampdu);
    }

    if (setitimer(ITIMER_REAL, &fault_timer, NULL) == -1)
    {
        printf("Error in setting Alarm - init_fault_timer\n");
        exit(EXIT_FAILURE);
    }

}

void stop_fault_timer()
{
    
    printf("IN STOP FAULT TIMER \n---------------------------------\n\n");
    fault_timer.it_value.tv_sec = 0;
    fault_timer.it_value.tv_usec =  0;
    fault_timer.it_interval = fault_timer.it_value;
    fault_timer_State=OFF;
}

void local_lost_link_timer_expired(int sig)
{
    local_lost_link_timer_done = TRUE;
    discoveryState = FAULT;
}

void pdu_timer_expired(int sig)
{
    printf("IN STOP PDU TIMER EXIPRED \n ---------------------------------\n\n");
        
    pdu_timer_done = TRUE;

    if (pdu_timer_done == TRUE && (local_pdu == RX_INFO || pdu_cnt < pdu_limit))
    {
        _INIT_TRANSMIT();
    }
    else if (pdu_timer_done == TRUE && local_pdu != RX_INFO && pdu_cnt == 10 && multiplexer_postbox_size<max_queue_depth)
    {
        _INIT_TRANSMIT();
        SEND_INFORMATION_OAMPDU();
    }

}

void fault_timer_expired(int sig)
{

    printf("IN FAULT TIMER EXPIRED\n");
    if (local_link_status == FAIL)
        init_fault_timer();
    else
    {
        if (local_oam_mode == ACTIVE)
        {
            discoveryState = ACTIVE_SEND_LOCAL_PRE;
        }
        else if (local_oam_mode == PASSIVE)
        {
            discoveryState = PASSIVE_WAIT_PRE;
        }

        controlState = TRANSMIT;
    }
}

void _INIT_TRANSMIT()
{
    init_pdu_timer();
    pdu_cnt = pdu_limit;
    pdu_timer_done = FALSE;
}

void _TX_OAMPDU(struct _message* message)
{
    DEBUG((PACKET*)message->data);
    multiplexer_postbox_size++;
    send_message(*message);
}

void _DEC_PDU_CNT()
{
    if (pdu_req == NORMAL)
        pdu_cnt--;

    transmitState = TX_OAMPDU;
}

void _WAIT_FOR_TX()
{
    if (valid_pdu_req)
    {
        transmitState = DEC_PDU_CNT;
    }

    transmitState = DEC_PDU_CNT;
}

void _TRANSMIT(struct _message* message)
{

    while (1)
    {
        switch (transmitState)
        {

        case WAIT_FOR_TX:
            _WAIT_FOR_TX();
            break;

        case DEC_PDU_CNT:
            _DEC_PDU_CNT();
            break;

        case TX_OAMPDU:
            _TX_OAMPDU(message);
            return ;
        }

        if (transmitState == WAIT_FOR_TX)
            break;
    }

}

void _FAULT()
{
    if (local_link_status == FAIL)
        local_pdu = LF_INFO;
    else
        local_pdu = RX_INFO;

    local_stable = FALSE;
    BEGIN = FALSE;

    if (local_pdu == LF_INFO)
    {
        init_fault_timer();
        controlState = WAIT_FOR_INPUT;
    }
    else if (local_link_status == OK)
    {
        stop_fault_timer();
        printf("local_link_status is OK\n");

        if (local_oam_mode == ACTIVE)
        {
            printf("DTE is Active\n");
            discoveryState = ACTIVE_SEND_LOCAL_PRE;
        }
        else if (local_oam_mode == PASSIVE)
            discoveryState = PASSIVE_WAIT_PRE;

        controlState = TRANSMIT;
    }

}

void _ACTIVE_SEND_LOCAL_PRE()
{
    local_pdu = INFO;

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
    oampdu->payload.OAMPDU.code = Information_OAMPDU;
    oampdu->payload.OAMPDU.data.information_tlv.local_info.info_type = 0x01;
    oampdu->payload.OAMPDU.data.information_tlv.local_info.info_len = 0x10;
    oampdu->payload.OAMPDU.data.information_tlv.local_info.oam_version = local_oam_version; // Should be stored in Memory
    oampdu->payload.OAMPDU.data.information_tlv.local_info.revision = local_tlv_revision;  // Should be stored in Memory
    oampdu->payload.OAMPDU.data.information_tlv.local_info.state = get_state();
    oampdu->payload.OAMPDU.data.information_tlv.local_info.oam_config = get_oam_config();
    oampdu->payload.OAMPDU.data.information_tlv.local_info.oampdu_config = get_oampdu_config();

    struct _message message;

    message.mtype = CTL_OAMI_request;

    memcpy(message.data, (char*)oampdu,sizeof(*oampdu));

    _TRANSMIT(&message);

    free(oampdu);

    controlState = WAIT_FOR_INPUT;

    discoveryState = ACTIVE_SEND_LOCAL_POST;
}

void _ACTIVE_SEND_LOCAL_POST()
{

    if (remote_state_valid == TRUE)
        discoveryState = SEND_LOCAL_REMOTE;
    else
        controlState = WAIT_FOR_INPUT;

    controlState = TRANSMIT;
}

void _PASSIVE_WAIT_PRE()
{
    local_pdu = RX_INFO;

    controlState = WAIT_FOR_INPUT;
}

void _PASSIVE_WAIT_POST()
{
    if (remote_state_valid == TRUE)
    {
        discoveryState = SEND_LOCAL_REMOTE;
        controlState = WAIT_FOR_INPUT;
    }
    else
    {
        controlState = TRANSMIT;
    }

}


void _SEND_LOCAL_REMOTE()
{
    local_pdu = INFO;
    local_stable = FALSE;

    SEND_INFORMATION_OAMPDU();

    /*
        Cross Check;
    */

    if (local_satisfied == TRUE)
        discoveryState = SEND_LOCAL_REMOTE_OK;
    else
        controlState = WAIT_FOR_INPUT;
}

void _SEND_LOCAL_REMOTE_OK()
{
    local_pdu = INFO;
    local_stable = TRUE;

    /*
        Check the Local and Remote TLVs and take actions...
    */

    if (local_satisfied == TRUE && remote_stable == TRUE)
    {
        discoveryState = SEND_ANY;
    }
    else if (local_satisfied == FALSE)
    {
        discoveryState = SEND_LOCAL_REMOTE;
    }
    else
        controlState = WAIT_FOR_INPUT;
}

void _SEND_ANY()
{
    local_pdu = ANY;

    discoveryExternalState = COMPLETED;

    if (local_satisfied == FALSE)
    {
        discoveryState = SEND_LOCAL_REMOTE;
        discoveryExternalState = IN_PROGRESS;
    }
    else if (local_satisfied == FALSE && remote_stable == FALSE)
    {
        discoveryState = SEND_LOCAL_REMOTE_OK;
        discoveryExternalState = IN_PROGRESS;
    }
    else
        controlState = WAIT_FOR_INPUT;

    if (BEGIN == TRUE)
        BEGIN = FALSE;
}

void _DISCOVERY()
{
    if (local_link_status == FAIL)
        discoveryState = FAULT;
        

    switch (discoveryState)
    {
    case FAULT:
        printf("Entered in Fault\n");
        _FAULT();
        break;

    case ACTIVE_SEND_LOCAL_PRE:
        printf("Entered in Active Send Local Pre\n");
        _ACTIVE_SEND_LOCAL_PRE();
        break;

    case ACTIVE_SEND_LOCAL_POST:
        printf("Entered in Active Send Local Post\n");
        _ACTIVE_SEND_LOCAL_POST();
        break;

    case PASSIVE_WAIT_PRE:
        printf("Entered in Passive Wait Pre\n");
        _PASSIVE_WAIT_PRE();
        break;

    case PASSIVE_WAIT_POST:
        printf("Entered Passive Wait Post\n");
        _PASSIVE_WAIT_POST();
        break;

    case SEND_LOCAL_REMOTE:
    printf("Entered in Send Local Remote\n");
        _SEND_LOCAL_REMOTE();
        break;

    case SEND_LOCAL_REMOTE_OK:
    printf("Entered in Send Local remote OK\n");
        _SEND_LOCAL_REMOTE_OK();
        break;

    case SEND_ANY:
    printf("Entered in Send Any\n");
        _SEND_ANY();
        break;
    }
}

void set_peer_info()
{
    PACKET* oampdu = (PACKET*)message_get_control.data;

    peer.peer_oam_version = oampdu->payload.OAMPDU.data.information_tlv.local_info.oam_version;

    unsigned char peer_state = oampdu->payload.OAMPDU.data.information_tlv.local_info.state;

    switch (peer_state & 3)
    {
    case 0:
        peer.peer_par_action = par_FWD;
        break;

    case 1:
        peer.peer_par_action = par_LB;
        break;

    case 2:
        peer.peer_par_action = par_DISCARD;
        break;
    }

    switch (bitcheck(peer_state, 2))
    {
    case 0:
        peer.peer_mux_action = mux_FWD;
        break;

    case 1:
        peer.peer_mux_action = mux_DISCARD;
    }

    unsigned char peer_oam_config = oampdu->payload.OAMPDU.data.information_tlv.local_info.oam_config;

    switch (bitcheck(peer_oam_config, 0))
    {
    case 0:
        peer.peer_oam_mode = ACTIVE;
        break;

    case 1:
        peer.peer_oam_mode = PASSIVE;
        break;

    }

    switch (bitcheck(peer_oam_config, 1))
    {
    case 0:
        peer.peer_unidirectional = FALSE;
        break;

    case 1:
        peer.peer_unidirectional = TRUE;
        break;

    }

    switch (bitcheck(peer_oam_config, 2))
    {
    case 0:
        peer.peer_loopback_support = FALSE;
        break;

    case 1:
        peer.peer_loopback_support = TRUE;
        break;
    }

    switch (bitcheck(peer_oam_config, 3))
    {
    case 0:
        peer.peer_link_event_support = FALSE;
        break;

    case 1:
        peer.peer_link_event_support = TRUE;
        break;
    }

    switch (bitcheck(peer_oam_config, 4))
    {
    case 0:
        peer.peer_variable_retrieval_support = FALSE;
        break;

    case 1:
        peer.peer_variable_retrieval_support = TRUE;
        break;
    }

    unsigned short peer_oampdu_config = oampdu->payload.OAMPDU.data.information_tlv.local_info.oampdu_config;

    for (int i = 0; i < 11; i++)
    {
        if (bitcheck(peer_oam_config, i))
            peer.peer_max_oampdu_size++;

        peer.peer_max_oampdu_size <<= 1;
    }

}

void HANDLE_Information_OAMPDU()
{
    PACKET* oampdu = (PACKET*)message_get_control.data;
    
    if (oampdu->payload.OAMPDU.data.information_tlv.local_info.info_type == 0x01 && oampdu->payload.OAMPDU.data.information_tlv.local_info.revision > peer.peer_info_revision)
    {
        set_peer_info();
        remote_state_valid=TRUE;
        oampdu->payload.OAMPDU.data.information_tlv.local_info.revision = peer.peer_info_revision;
    }
    
    controlState=TRANSMIT;

}

void HANDLE_Event_Notification_OAMPDU()
{
    message_get_control.mtype=OAMPDU_indication;
    send_message(message_get_control);
}

void HANDLE_Variable_Request_OAMPDU()
{
    message_get_control.mtype=OAMPDU_indication;
    send_message(message_get_control);
}

void HANDLE_Variable_Response_OAMPDU()
{
    message_get_control.mtype=OAMPDU_indication;
    send_message(message_get_control);
}

void HANDLE_Loopback_Control_OAMPDU()
{
    if (Loopback_Control_OAMPDU_Sent == TRUE)
    {
        unsigned int l, m;

        unsigned int *p = (unsigned int*)shared_memory->source_address;
        l = *p;
        l += shared_memory->source_address[4] + shared_memory->source_address[5];

        unsigned int *q = (unsigned int*)peer.peer_source_address;
        m = *q;
        m = m + peer.peer_source_address[4] + peer.peer_source_address[5];

        if (l < m)
            return ;

    }


    local_par_action = par_LB;
    local_mux_action = mux_DISCARD;

    SEND_INFORMATION_OAMPDU();
}

void _PROCESS()
{

    PACKET* oampdu = (PACKET*)message_get_control.data;

    switch (oampdu->payload.OAMPDU.code)
    {
    case Information_OAMPDU:
        HANDLE_Information_OAMPDU();
        break;

    case Event_Notification_OAMPDU:
        HANDLE_Event_Notification_OAMPDU();
        break;

    case Variable_Request_OAMPDU:
        HANDLE_Variable_Request_OAMPDU();
        break;

    case Variable_Response_OAMPDU:
        HANDLE_Variable_Response_OAMPDU();
        break;

    case Loopback_Control_OAMPDU:
        if(message_get_control.mtype == OAMPDU_request)
        {
            printf("\nSENDING LOOPBACK REQUEST TO PEER DTE!!!\n");
            message_get_control.mtype = CTL_OAMI_request;
            _TRANSMIT(&message_get_control);
        }
        else
            HANDLE_Loopback_Control_OAMPDU();
        controlState=WAIT_FOR_INPUT;
        break;
    }
}

void _WAIT()
{
    while(get_control_postbox()==(ssize_t)-1)
        ;
        
    control_postbox_size--;
    
    printf("RECEIVED OAMPDU FROM OTHER SIDE...\nMessages in Control Queue: %d",control_postbox_size);

    controlState = PROCESS;
}

void control()
{
    local_lost_link_timer_State = OFF;
    pdu_timer_State = OFF;
    fault_timer_State = OFF;
    Loopback_Control_OAMPDU_Sent = FALSE;
    peer.peer_info_revision=0;

    _INIT_TRANSMIT();

    controlState = TRANSMIT;
    transmitState = WAIT_FOR_TX;

    discoveryExternalState = IN_PROGRESS;

    discoveryState = FAULT;

    while (1)
    {
        switch (controlState)
        {
        case WAIT_FOR_INPUT:
            _WAIT();
            break;

        case PROCESS:
            _PROCESS();
            break;

        case TRANSMIT:
            _DISCOVERY();
            break;
        }
    }
}
