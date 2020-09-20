key_t key_parser;

int msgid_parser;

struct _message message_get_parser;

void init_parser_postbox()
{
    system("rm -f PARSER");
    
    system("touch PARSER");  

   	msgctl(msgid_parser, IPC_RMID, NULL);


}

void set_parser_postbox()
{  
    
	key_parser = ftok("PARSER", 65);
	
	msgid_parser = msgget(key_parser, 0666 | IPC_CREAT);
}

void post_parser_postbox(struct _message message_post_parser)
{
	msgsnd(msgid_parser, &message_post_parser, sizeof(message_post_parser), 0);
}

void get_parser_postbox()
{
	msgrcv(msgid_parser, &message_get_parser, sizeof(message_get_parser), 0, 0);
}

