key_t key_mac_client;

int msgid_mac_client;

struct _message message_get_mac_client;

void init_mac_client_postbox()
{
    system("rm -f MAC_CLIENT");
    
    system("touch MAC_CLIENT");  

    msgctl(msgid_mac_client, IPC_RMID, NULL);

}

void set_mac_client_postbox()
{
    
	key_mac_client = ftok("MAC_CLIENT", 65);
	
	msgid_mac_client = msgget(key_mac_client, 0666 | IPC_CREAT);
}

void post_mac_client_postbox(struct _message message_post_mac_client)
{
	msgsnd(msgid_mac_client, &message_post_mac_client, sizeof(message_post_mac_client), 0);
}
ssize_t get_mac_client_postbox()
{
    ssize_t x=(msgrcv(msgid_mac_client, &message_get_mac_client, sizeof(message_get_mac_client), 0, 0));
    printf("MESSAGE RECEIVE SIZE: %d \n",(int)x);
	return x;
}

