key_t key_oam_client;

int msgid_oam_client;

struct _message message_get_oam_client;

void init_oam_client_postbox()
{
    system("rm -f OAM_CLIENT");
    
    system("touch OAM_CLIENT"); 

    msgctl(msgid_oam_client, IPC_RMID, NULL);

}

void set_oam_client_postbox()
{    
	key_oam_client = ftok("OAM_CLIENT", 65);
	
	msgid_oam_client = msgget(key_oam_client, 0666 | IPC_CREAT);
}

void post_oam_client_postbox(struct _message message_post_oam_client)
{
	msgsnd(msgid_oam_client, &message_post_oam_client, sizeof(message_post_oam_client), 0);
}

void get_oam_client_postbox()
{
	msgrcv(msgid_oam_client, &message_get_oam_client, sizeof(message_get_oam_client), 0, 0);
}

