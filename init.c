#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "Init/init.h"
#include "Init/config_parser.h"
#include "IPC/message.h"
#include "IPC/oam_client_postbox.h"
#include "IPC/mac_client_postbox.h"
#include "IPC/multiplexer_postbox.h"
#include "IPC/parser_postbox.h"
#include "IPC/control_postbox.h"

int main()
{        
    init_multiplexer_postbox();
    init_parser_postbox();
    init_control_postbox();
    init_mac_client_postbox();
    init_oam_client_postbox();     
    
    /* the size (in bytes) of shared memory object */
    const int SIZE = (int)sizeof(struct _shared_memory); 
  
    /* name of the shared memory object */
    const char* name = "/OAM";
  
    /* shared memory file descriptor */
    int shm_fd; 
  
    /* pointer to shared memory obect */
    void* ptr; 
  
    /* create the shared memory object */
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
  
    /* configure the size of the shared memory object */
    ftruncate(shm_fd, SIZE); 
  
    /* memory map the shared memory object */
    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0); 
    
    /* Initialise the Memory */
    config_parser(ptr);
    
    return 0;
    
}
