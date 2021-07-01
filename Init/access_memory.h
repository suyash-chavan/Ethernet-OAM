void* ptr;

void get_init()
{
    const int SIZE = sizeof(struct _shared_memory); 
  
    /* name of the shared memory object */
    const char* name = "/OAM"; 
  
    /* shared memory file descriptor */
    int shm_fd; 
  
    /* pointer to shared memory object */
    
    /* open the shared memory object */
    shm_fd = shm_open(name, O_RDWR, 0666);
  
    /* memory map the shared memory object */
    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    if(ptr==NULL)
    {
        printf("Shared Memory is not allocated...\n");
        exit(EXIT_FAILURE);
    }   
}
