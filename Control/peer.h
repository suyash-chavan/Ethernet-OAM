struct _peer
{
    unsigned char peer_local_stable;
    
    unsigned char peer_local_evaluating; 

    unsigned short peer_oam_version;
    
    unsigned short peer_info_revision;

    unsigned char peer_source_address[6];
    
    enum _local_mux_action peer_mux_action;

    enum _local_par_action peer_par_action;

    enum _local_oam_mode peer_oam_mode;

    enum _boolean peer_unidirectional;

    enum _boolean peer_loopback_support;

    enum _boolean peer_link_event_support;

    enum _boolean peer_variable_retrieval_support;

    unsigned short peer_max_oampdu_size;

	   
} peer;
