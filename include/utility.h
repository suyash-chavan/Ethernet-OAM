void print_Remote_Loopback_Control_OAMPDU(OAMPDU *packet)
{
    printf("\n\t\t\t\tOAM Remote Loopback Command:\t\t%02x",packet->payload.data.loopback_control.remote_loopback_command);
}

void print_variable_container(struct variable_container vc)
{
    printf("\n\t\t\t\tVariable Branch:\t\t%02x",vc.branch);
    printf("\n\t\t\t\tVariable Leaf:\t\t%04x",vc.leaf);
    printf("\n\t\t\t\tVariable Width:\t\t%02x",vc.width);
    printf("\n\t\t\t\tVariable Value:\t\t%04x",vc.value);             // How to print?
}

void print_Variable_Response_OAMPDU(OAMPDU *packet)
{
    for(int i=0;i<sizeof(packet->payload.data.variable_response.variable_containers)/sizeof(struct variable_container);i++)
    {
        printf("\n\nVariable Descriptor %d:",i+1);
        print_variable_container(packet->payload.data.variable_response.variable_containers[i]);
    }
}

void print_variable_descriptor(struct variable_descriptor vd)
{
    printf("\n\t\t\t\tVariable Branch:\t\t%02x",vd.variable_branch);
    printf("\n\t\t\t\tVariable Leaf:\t\t%04x",vd.variable_leaf);
}

void print_Variable_Request_OAMPDU(OAMPDU *packet)
{
    for(int i=0;i<sizeof(packet->payload.data.variable_request.variable_descriptors)/sizeof(struct variable_descriptor);i++)
    {
        printf("\n\nVariable Descriptor %d:",i+1);
        print_variable_descriptor(packet->payload.data.variable_request.variable_descriptors[i]);
    }
}

void print_link_event_tlv(struct link_event_tlv tlv)
{
    printf("\n\t\t\t\tEvent Type:\t\t%02x",tlv.event_type);
    printf("\n\t\t\t\tEvent Length:\t\t%02x",tlv.event_len);
    printf("\n\t\t\t\tEvent Time Stamp:\t\t%hu",tlv.event_time_stamp);
    printf("\n\t\t\t\tError Symbol Window:\t\t%hu",tlv.err_symbol_window);
    printf("\n\t\t\t\tError Symbol Threshold:\t\t%ul",tlv.err_symbol_threshold);
    printf("\n\t\t\t\tError Symbols:\t\t%ul",tlv.err_symbols);
    printf("\n\t\t\t\tError Running Total:\t\t%ul",tlv.err_running_total);
    printf("\n\t\t\t\tEvent Running Total:\t\t%ul",tlv.event_running_total);
}

void print_Event_Notification_OAMPDU(OAMPDU *packet)
{
    printf("\n\n\tSequence Number:\t\t\t%hu",packet->payload.data.event_notification_tlv.sequence_no);
    
    for(int i=0;i<sizeof(packet->payload.data.event_notification_tlv.link_event_tlvs)/sizeof(struct link_event_tlv);i++)
    {
        printf("\n\nEvent Notification TLV %d:",i+1);
        print_link_event_tlv(packet->payload.data.event_notification_tlv.link_event_tlvs[i]);
    }
}

void print_info_tlv(struct info_tlv tlv)
{
    printf("\n\t\t\t\tInformation Type:\t\t%02x",tlv.info_type);
    printf("\n\t\t\t\tInformation Length:\t\t%02x",tlv.info_len);
    printf("\n\t\t\t\tOAM Version:\t\t\t%02x",tlv.oam_version);
    printf("\n\t\t\t\tRevision:\t\t\t%hu",tlv.revision);
    printf("\n\t\t\t\tState:\t\t\t\t%02x",tlv.state);
    printf("\n\t\t\t\tOAM Configuration:\t\t%02x",tlv.oam_config);
    printf("\n\t\t\t\tOAMPDU Configuration:\t\t%02x",tlv.oampdu_config);
} 

void print_Information_OAMPDU(OAMPDU *packet)
{
    printf("\n\n\tLocal Information TLV:");
    print_info_tlv(packet->payload.data.information_tlv.local_info);
    
    printf("\n\n\tRemote Information TLV:");
    print_info_tlv(packet->payload.data.information_tlv.remote_info);
    
    for(int i=0;i<sizeof(packet->payload.data.information_tlv.other_info_tlv)/sizeof(struct info_tlv);i++)
    {
        printf("\n\n\tOther Information TLV %d:",i+1);
        print_info_tlv(packet->payload.data.information_tlv.other_info_tlv[i]);
    }
}

void print(OAMPDU *packet)
{
    printf("\n------------------------------------------------------------------------------------------\n");  
    
    printf("\n\tDestination Address:\n\n\t\t\t\t[ ");
    for(int i=0;i<sizeof(packet->DA);i++)
        printf(" 0x%02x ",packet->DA[i]);
    printf(" ]\n");
    
    
    printf("\n\tSource Address:\n\n\t\t\t\t[ ");
    for(int i=0;i<sizeof(packet->SA);i++)
        printf(" 0x%02x ",packet->SA[i]);
    printf(" ]\n");
    
    printf("\n\tFlags:");
    for(;;)
    {
        unsigned int flags=packet->payload.flags;
        
        for(int i=15;i>=0;i--,flags>>=1)
            printf("\n\t\t\t\tBit %d:\t%d",i,flags&1);
        break;
    }
    
    printf("\n\n\tCode:\t\t\t%02x",packet->payload.code);
    
    
    switch(packet->payload.code)
    {
        case 0:
            print_Information_OAMPDU(packet);
            break;
        
        case 1:
            print_Event_Notification_OAMPDU(packet);
            break;
            
        case 2:
            print_Variable_Request_OAMPDU(packet);
            break;
            
        case 3:
            print_Variable_Response_OAMPDU(packet);
            break;
            
        case 4:
            print_Remote_Loopback_Control_OAMPDU(packet);
            break;        
        
        default:
            printf("\n\tThis OAMPDU is Reserved or is Organization Specific.\n");
            break;
    }
    
    printf("\n\n\tFrame Check Sequence(FCS):\t\t\t\t%lu\n",packet->fcs);
    
    printf("\n------------------------------------------------------------------------------------------\n");  
}


void DEBUG(OAMPDU *packet)
{
    //OAMPDU *packet=(OAMPDU *) buf;
    
    #ifdef DEBUG_PRINT
    print(packet);
    #endif
}



