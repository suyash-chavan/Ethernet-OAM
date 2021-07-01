#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

// For Handeling Timers and Signals
#include <signal.h>
#include <sys/time.h>

// User Defined Header Files
#include "include/packet.h"
#include "include/utility.h"
#include "Init/init.h"
#include "Control/peer.h"
#include "Init/access_memory.h"

/*
    Message Queue Handler...
*/

#include "IPC/message.h"
#include "IPC/mac_client_postbox.h"
#include "IPC/control_postbox.h"
#include "IPC/multiplexer_postbox.h"
#include "IPC/parser_postbox.h"
#include "IPC/oam_client_postbox.h"
#include "IPC/send_handler.h"

#include "MAC_Client/functions.h"

int main()
{
    set_multiplexer_postbox();
    set_mac_client_postbox();
    
    interface();
    

    return 0;

}
