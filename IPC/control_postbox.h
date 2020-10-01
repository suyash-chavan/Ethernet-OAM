key_t key_control;

int msgid_control;

struct _message message_get_control;

void init_control_postbox()
{
    system("rm -f CONTROL");
    
    system("touch CONTROL");    

  	msgctl(msgid_control, IPC_RMID, NULL);

}

void set_control_postbox()
{    
	key_control = ftok("CONTROL", 65);
	
	msgid_control = msgget(key_control, 0666 | IPC_CREAT);
	
}

void post_control_postbox(struct _message message_post_control)
{
	printf("MSGSEND return: %d",(int)msgsnd(msgid_control, &message_post_control, sizeof(message_post_control), 0));
	
}

ssize_t get_control_postbox()
{
    ssize_t x=(msgrcv(msgid_control, &message_get_control, sizeof(message_get_control), 0, 0));
    printf("MESSAGE RECEIVE SIZE: %d \n",(int)x);
	return x;
}



