#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

// User Defined Header Files
#include "include/packet.h"
#include "include/utility.h"
#include "include/sock_send.h"
#include "Init/init.h"
#include "Init/access_memory.h"

/*
    Message Queue Handler...
*/

#include "IPC/message.h"
#include "IPC/mac_client_postbox.h"
#include "IPC/control_postbox.h"
#include "IPC/multiplexer_postbox.h"
#include "IPC/parser_postbox.h"
#include "IPC/send_handler.h"


struct _shared_memory *shared_memory;

#include "include/macros.h"
#include "Multiplexer/functions.h"

int main()
{
    // Initiating Shared Memory
    get_init();

    // Pointer to Shared Memory
    shared_memory = (struct _shared_memory *) ptr;

    // Setting Up Message Queues to other Modules
    set_multiplexer_postbox();

    // Starting State Machine
    multiplexer();

    return 0;

}
