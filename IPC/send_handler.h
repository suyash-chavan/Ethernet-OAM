void send_message(struct _message message)
{
	switch(message.mtype)
	{
		case MCF_MA_DATA_indication:
			post_mac_client_postbox(message);
			break;

		case CTL_OAMI_indication:
			post_control_postbox(message);
			break;

		case LBF_OAMI_request:
			post_multiplexer_postbox(message);
			break;

		case CTL_OAMI_request:
			post_multiplexer_postbox(message);
			break;

		case MCF_MA_DATA_request:
			post_multiplexer_postbox(message);
			break;
			
		case OAMPDU_indication:
		    post_mac_client_postbox(message);
		    break;
		    
		case OAMPDU_request:
		    post_control_postbox(message);
		    break;	    

	}

}
