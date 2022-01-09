#include "modconfig.h"
#include<stdio.h>       
#include<stdlib.h>     
#include<unistd.h>    
#include<sys/types.h>   
#include<sys/stat.h>     
#include<fcntl.h>     
#include<termios.h>    
#include<errno.h>      
#include<string.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include "jprint.h"

void signalHandler( int signum )
{
    //jprint(MOD_JPRINT_MAIN,MOD_DEBUG_WARN,"sig INT signalHandler recv!\r\n");

    //jprint_init();
   //exit(0);  
}

int run_master()
{
    jprint_init();

    signal(SIGUSR1, signalHandler);//kill -USR1 2581
    signal(SIGPIPE,SIG_IGN);

    while (1)//main loop
    {
        printf("hello!\r\n");
        sleep(1);
    }
}

int main(int argc,char* argv[])
{
#if (MOD_DAEMON_DEBUG !=1)    
    int pid = fork();
    
    if(pid < 0) {
        jprint(MOD_JPRINT_MAIN,MOD_DEBUG_FATAL,"can't fork main!\r\n");
        return -1;
    }
    
    // grandpa
    if(pid > 0) {
        int status = 0;
        waitpid(pid, &status, 0);
        exit(0);
    }
    
    // father
    pid = fork();
    
    if(pid < 0) {

        return -1;
    }
    
    if(pid > 0) {
        jprint(MOD_JPRINT_MAIN,MOD_DEBUG_INFO,"father process exit!\r\n");
        exit(0);
    }
#endif
    // son
    jprint(MOD_JPRINT_MAIN,MOD_DEBUG_INFO,"son(daemon) process running.!\r\n");

    if (run_master() != 0) {
        jprint(MOD_JPRINT_MAIN,MOD_DEBUG_FATAL,"daemon run master\r\n");
        return -1;
    }

    return 0;
}