key_t key_multiplexer;

int msgid_multiplexer;

struct _message message_get_multiplexer;

void init_multiplexer_postbox()
{
    system("rm -f MULTIPLEXER");
    
    system("touch MULTIPLEXER");    

	msgctl(msgid_multiplexer, IPC_RMID, NULL);

}

void set_multiplexer_postbox()
{    
	key_multiplexer = ftok("MULTIPLEXER", 65);
	
	msgid_multiplexer = msgget(key_multiplexer, 0666 | IPC_CREAT);
}

void post_multiplexer_postbox(struct _message message_post_multiplexer)
{
	msgsnd(msgid_multiplexer, &message_post_multiplexer, sizeof(message_post_multiplexer), 0);
}

void get_multiplexer_postbox()
{
	msgrcv(msgid_multiplexer, &message_get_multiplexer, sizeof(message_get_multiplexer), 0, 0);
}

