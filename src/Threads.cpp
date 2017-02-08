/*
 The Disarray 
 by jrs0ul(jrs0ul ^at^ gmail ^dot^ com) 2010
 -------------------------------------------
 Threads
 mod. 2010.11.30
 */

#include "Threads.h"


void Thread::create( THREADFUNC (*routine)(void*), void * args){
#ifdef _WIN32
    thread_handle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)routine, args, 0, &thread_ID);
#else
    pthread_create(&thread,
                   0,
                   routine,
                   args);
    
#endif
}
//-------------------------

void Thread::stop(){
    #ifdef _WIN32
        CloseHandle(thread_handle);
    #else
        pthread_cancel(thread);
    #endif
}
