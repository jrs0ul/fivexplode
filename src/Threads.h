/*
 The Disarray 
 by jrs0ul(jrs0ul ^at^ gmail ^dot^ com) 2010
 -------------------------------------------
 Threads
 mod. 2010.11.30
 */

#ifndef _THREADS_H
#define _THREADS_H


#ifdef _WIN32
    #include <windows.h>
    #define THREADFUNC DWORD
#else
    #include <pthread.h>
    #define THREADFUNC void*
#endif



class Thread{

public:

    #ifndef _WIN32
        pthread_t thread;
    #else
        DWORD thread_ID;

        HANDLE thread_handle;
    #endif

    void create(THREADFUNC (*routine)(void*), void * args);
    void stop();


};




#endif //_THREADS_H
