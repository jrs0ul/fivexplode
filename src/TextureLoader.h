/*
 The Disarray 
 by jrs0ul(jrs0ul ^at^ gmail ^dot^ com) 2010
 -------------------------------------------
 Sprite batcher
 mod. 2010.09.27
 */
#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#ifdef WIN32
    #ifdef _MSC_VER
        #include <SDL_opengl.h>
    #else
        #include <SDL/SDL_opengl.h>
    #endif
#else
    #ifdef __APPLE__    
        #include <TargetConditionals.h>
        #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
            #include <OpenGLES/ES1/gl.h>
            #include <OpenGLES/ES1/glext.h>
        #else
              #include <SDL/SDL_opengl.h>
        #endif
    #else
        #include <SDL/SDL_opengl.h>
    #endif
#endif


#include "Image.h"
#include "DArray.h"
#include "Colors.h"
#include "VertexObject.h"
//#include "ShaderProgram.h"

struct PicData{
    char name[255];
    int twidth;
    int theight;
    int width;
    int height;
    int filter;
    //additional data for faster rendering
    float htilew;
    float htileh;
    int vframes;
    int hframes;
    float partX;
    float partY;
};
//-------------------------
struct SpriteBatchItem{
    float x;
    float y;
    long textureIndex;
    unsigned int frame;
    bool useCenter;
    float scaleX ;
    float scaleY ;
    float rotationAngle;
    COLOR upColor;
    COLOR dwColor;
    //bool drawn;

    SpriteBatchItem(){
        x = 0;
        y = 0;
        textureIndex = 0;
        frame = 0;
        useCenter = false;
        //drawn = false;
        scaleX = 1.0f;
        scaleY = 1.0f ;
        rotationAngle = 0.0f;
        upColor = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
        dwColor = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
    }
};
//==================================

#define MAXSPRITES 600

class PicsContainer{
    DArray<GLuint> TexNames;
    DArray<PicData> PicInfo;
    
    DArray<SpriteBatchItem> batch;
    
    VertexObject VO;
    
    void drawVO(unsigned &_count, long texIndex);

    void drawVA(void * vertices, void * uvs, void *colors,
                unsigned uvsCount, unsigned vertexCount);

    void resizeContainer(unsigned long index,
                         int twidth, int theight, int filter,
                         const char * name,
                         bool createTextures = true,
                         GLuint texname = 0);

public:
    PicsContainer(){}
    bool initContainer(const char* list);
    //load textures from list in textfile
    bool load(const char* list);

    void initBuffers();

    void destroy();

    //adds sprite to batch
    void draw( long textureIndex,
                float x, float y,
                unsigned int frame = 0,
                bool useCenter = false,
                float scaleX = 1.0f, float scaleY = 1.0f,
                float rotationAngle = 0.0f,
                COLOR upColor = COLOR(1.0f, 1.0f, 1.0f, 1.0f),
                COLOR dwColor = COLOR(1.0f, 1.0f, 1.0f, 1.0f)
               );
    //draws all sprites in batch
    int drawBatch(/*ShaderProgram * justcolor,
                   ShaderProgram * texcolor,*/
                   int method = 0);

    GLuint getname(unsigned long index);
    PicData* getInfo(unsigned long index);
    unsigned long count(){return PicInfo.count();}
    int findByName(const char* picname, bool debug = false);
    //to make textures from images loaded in defferent threads
    void makeTexture(Image& img, 
                     const char * name,
                     unsigned long index,
                     int twidth, int theight, int filter=0);

    bool loadFile(const char* file, unsigned long index,
                  int twidth, int theight,
                  const char * basePath, int filter = 0); //???
    //loads images using special array loaded from file
    bool loadFile(unsigned long index, const char * BasePath);
    //no need for base folder
    bool loadFile(const char* file, unsigned long index,
                  int twidth, int theight, int filter = 0);
    //attach already created texture
    void attachTexture(GLuint textureID, unsigned long index,
                       int width, int height,
                       int twidth, int theight, int filter = 0);
    void remove(unsigned long index);

};


#endif //TEXTURE_LOADER_H



