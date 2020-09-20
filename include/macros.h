#define rxOk (reception_status == receiveOK)
#define valid_pdu_req ((local_pdu != RX_INFO && pdu_req == NORMAL && pdu_cnt != 0) || (local_pdu == ANY && pdu_req == CRITICAL))
#define RxOAMPDU (!strcmp(oampdu->DA,Slow_Protocols_Multicast) && !strcmp(oampdu->LengthorType,Slow_Protocols_Type) && oampdu->subtype == OAM_subtype)

#define BEGIN                                       shared_memory->BEGIN
#define local_critical_event                        shared_memory->local_critical_event
#define local_dying_gasp                            shared_memory->local_dying_gasp
#define local_lost_link_timer_done                  shared_memory->local_lost_link_timer_done
#define local_satisfied                             shared_memory->local_satisfied
#define local_stable                                shared_memory->local_stable
#define remote_stable                               shared_memory->remote_stable
#define remote_state_valid                          shared_memory->remote_state_valid
#define local_unidirectional                        shared_memory->local_unidirectional
#define reception_status                            shared_memory->reception_status
#define local_link_status                           shared_memory->local_link_status
#define local_oam_enable                            shared_memory->local_oam_enable
#define local_oam_mode                              shared_memory->local_oam_mode
#define local_par_action                            shared_memory->local_par_action
#define local_mux_action                            shared_memory->local_mux_action
#define local_pdu                                   shared_memory->local_pdu
#define pdu_req                                     shared_memory->pdu_req
#define control_postbox_size                        shared_memory->control_postbox_size
#define parser_postbox_size                         shared_memory->parser_postbox_size
#define multiplexer_postbox_size                    shared_memory->multiplexer_postbox_size
#define oam_client_postbox_size                     shared_memory->oam_client_postbox_size
#define mac_client_postbox_size                        shared_memory->mac_client_postbox_size   

#define Information_OAMPDU 							0x00
#define Event_Notification_OAMPDU 					0x01
#define Variable_Request_OAMPDU						0x02
#define Variable_Response_OAMPDU					0x03
#define Loopback_Control_OAMPDU						0x04
