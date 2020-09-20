/*
    This Program is administrator of OAM 
    sublayer.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "	include/oampdu.h"
#include "include/utility.h"
#include "Client/functions.h"

int main()
{
    int send_fd;
    send_fd = open(SEND_FILE, O_CREAT|O_WRONLY);
    
     interface();
}
