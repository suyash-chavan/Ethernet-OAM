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
#include "include/oampdu.h"
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


struct _shared_memory *shared_memory;

#include "include/macros.h"
#include "Control/functions.h"

int main()
{
    // Initiating Shared Memory
    get_init();

    // Pointer to Shared Memory
    shared_memory = (struct _shared_memory *) ptr;

    // Setting Up Message Queues to other Modules
    set_multiplexer_postbox();
    set_control_postbox();
    set_oam_client_postbox();
	    
    // Starting State Machine
    control();

    return 0;

}
