const int OAMPDU_MAX = 1600;
const char Slow_Protocols_Multicast[] = {0X01,0X80,0Xc2,0X00,0X00,0X02};
const char Slow_Protocols_Type[] = {0X88,0X09};
const char OAM_subtype = 0x03;

/* Information TLV */
struct info_tlv{
    unsigned char info_type;
    unsigned char info_len;
    unsigned char oam_version;
    unsigned short revision;
    unsigned char state;
    unsigned char oam_config;
    unsigned short oampdu_config;
};



/* Link Event TLV */
struct link_event_tlv{
    unsigned char event_type;
    unsigned char event_len;
    unsigned short event_time_stamp;
    unsigned long long err_symbol_window;
    unsigned long long err_symbol_threshold;
    unsigned long long err_symbols;
    unsigned long long err_running_total;
    unsigned int event_running_total;
};



/* Variable Descriptor - Variable Request OAMPDUs */
struct variable_descriptor{
    unsigned char variable_branch;
    unsigned short variable_leaf;
};



/* Variable Container - Variable Response OAMPDUs */
struct variable_container{
    unsigned char branch;
    unsigned short leaf;
    unsigned char width;
    unsigned char value[128];
};



/* Data Field OAMPDU vary */
union Data{
    
    /* For Information OAMPDUs*/
    struct _information_tlv{
        struct info_tlv local_info;
        struct info_tlv remote_info;
        struct info_tlv *other_info_tlv;    
    } information_tlv;
    
    /* Event Notification OAMPDUs */
    struct _event_notification_tlv{
        unsigned short sequence_no;
        struct link_event_tlv *link_event_tlvs;
    } event_notification_tlv;
    
    /* Variable Request OAMPDUs */
    struct _variable_request{
        struct variable_descriptor *variable_descriptors;
    } variable_request;
    
    /* Variable Response OAMPDUs */
    struct _variable_response{
        struct variable_container *variable_containers;
    } variable_response;
    
    struct _loopback_control{
        unsigned char remote_loopback_command;
    } loopback_control;
};

struct _oampdu{
    unsigned char subtype;
    unsigned int flags;
	unsigned char code;
	union Data data;
};

struct _mac{
    char data[1500];
};

union _payload{
    struct _oampdu OAMPDU;
    struct _mac MAC;    
};

/* OAMPDU defination */
typedef struct _packet{
    unsigned char DA[6];
    unsigned char length[2];
	unsigned char SA[6];
	union _payload payload;
	unsigned int fcs;
} PACKET;
