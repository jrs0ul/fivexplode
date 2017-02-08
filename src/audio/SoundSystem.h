/*
 The Disarray 
 by jrs0ul(jrs0ul ^at^ gmail ^dot^ com) 2010
 -------------------------------------------
 sound fx
 mod. 2010.10.27
 */


#ifndef SOUND_SYSTEM_H
#define SOUND_SYSTEM_H

#ifdef WIN32
    #ifdef _MSC_VER
        #include <alc.h>
        #include <al.h>
    #else
        #include <AL/alc.h>
        #include <AL/al.h>
    #endif
    #elif __APPLE__
        #include <OpenAL/alc.h>
        #include <OpenAL/al.h>
    #else
        #include <AL/alc.h>
        #include <AL/al.h>
#endif


#include "../DArray.h"

struct SoundData{
    char name[255];
};

//----------

class SoundSystem{

    ALCdevice* alcdev;
    ALCcontext* alccont;

    ALuint* buffers;
    ALuint* sources;
    DArray<SoundData> audioInfo;

    char* LoadOGG(char *fileName,  ALsizei & size, ALenum &format, ALsizei &freq);

    float volume;


public:
    SoundSystem(){alcdev=0; buffers=0; sources=0; volume = 0.9f; }
    bool init(ALCchar* dev);
    void loadFiles(const char* BasePath, const char* list);
    void setupListener(float * pos, float * orientation);
    void setSoundPos(unsigned int index, float * pos);
    void playsound(unsigned int index, bool loop=false);
    void freeData();
    bool isPlaying(unsigned int index);
    ALuint getBuffer(unsigned int index){return buffers[index];}
    void stopAll();
    void exit();
    void setVolume(float vol){volume = (vol > 1.0f)?1.0f:((vol < 0.0f)?0:vol);}
    float getVolume(){return volume;};

};




#endif //SOUND_SYSTEM_H
