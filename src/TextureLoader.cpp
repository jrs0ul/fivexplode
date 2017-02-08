/*
 The Disarray 
 by jrs0ul(jrs0ul ^at^ gmail ^dot^ com) 2010
 -------------------------------------------
 Sprite batcher
 mod. 2010.11.30
 */

#include <cstdio>
#include <cstring>
#include <cmath>
#include "TextureLoader.h"
#include "Vectors.h"




GLuint PicsContainer::getname(unsigned long index){
    if (index < TexNames.count())
        return TexNames[index];
    return 0;
}
//-----------------------------

PicData* PicsContainer::getInfo(unsigned long index){
    if (index < TexNames.count())
        return &PicInfo[index];
    return 0;
}
//-----------------------------
bool PicsContainer::load(const char *list){


    if (!initContainer(list))
        return false;

    for (unsigned long i = 0; i < PicInfo.count(); i++){

        Image naujas;

        unsigned short imageBits = 0;
        if (!naujas.loadTga(PicInfo[i].name,imageBits))
            printf("%s not found or corrupted by M$\n",PicInfo[i].name);
        PicInfo[i].width = naujas.width;
        PicInfo[i].height = naujas.height;


        PicInfo[i].htilew = PicInfo[i].twidth/2.0f;
        PicInfo[i].htileh = PicInfo[i].theight/2.0f;
        PicInfo[i].vframes = PicInfo[i].height/PicInfo[i].theight;
        PicInfo[i].hframes = PicInfo[i].width/PicInfo[i].twidth;

        int filtras = GL_NEAREST;
        if (PicInfo[i].filter)
            filtras = GL_LINEAR;


        glBindTexture(GL_TEXTURE_2D, TexNames[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtras );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtras );

        if (imageBits > 24)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, naujas.width, naujas.height,
                         0, GL_RGBA, GL_UNSIGNED_BYTE,naujas.data);
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, naujas.width, naujas.height,
                         0, GL_RGB, GL_UNSIGNED_BYTE,naujas.data);

        naujas.destroy();

    }
    return true;
}

//---------------------------------------------------
void PicsContainer::draw(
                long textureIndex,
                float x, float y,
                unsigned int frame,
                bool useCenter,
                float scaleX, float scaleY,
                float rotationAngle,
                COLOR upColor,
                COLOR dwColor
               ){

        SpriteBatchItem nb;

        nb.x = x;
        nb.y = y;
        nb.textureIndex = textureIndex;
        nb.frame = frame;
        nb.useCenter = useCenter;
        nb.scaleX = scaleX;
        nb.scaleY = scaleY;
        nb.rotationAngle = rotationAngle;
        nb.upColor = upColor;
        nb.dwColor = dwColor;

        batch.add(nb);
}
//----------------------------------------------------------
Vector3D CalcUvs(PicData * p, unsigned frame){

    //printf("%d\n", p->hframes);
    float hf = frame / p->hframes;

    float startx =  (frame - p->hframes * hf) * p->twidth;
    float starty = hf * p->theight;

    Vector3D result = Vector3D(
                               (startx*1.0f)/(p->width*1.0f),
                               ((startx+p->twidth)*1.0f)/(p->width*1.0f),
                               (( p->height - starty) * 1.0f ) / ( p->height * 1.0f )- 0.0001f,
                               (( p->height - starty - p->theight ) * 1.0f) / (p->height * 1.0f)
                              );
    return result;
}

//------------------------------
void PicsContainer::initBuffers(){

    
    float * _verts = (float*)malloc(MAXSPRITES * 6 * (2+2+4) * sizeof(float));
    
    VO.init(_verts, VPOSITION|VCOLOR|VTEXCOORD0, MAXSPRITES * 6, true, true,
            
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
            false
#else
            false
#endif
            );
   
    
    free(_verts);
}

//-------------------------------
void produceVertices(float x, float y,
                     float rotationAngle,
                     bool useCenter,
                     float scaleX, float scaleY,
                     float htilew, float htileh,
                     float twidth, float theight,
                     float * array){
    
    if (rotationAngle == 0.0f){
        if (useCenter){
            float hwidth = htilew * scaleX;
            float hheight = htileh * scaleY;
            
            
            array[0] = x - hwidth; 
            array[1] = y - hheight;
            
            array[2] = x + hwidth; 
            array[3] = y - hheight;
            
            array[4] = x + hwidth; 
            array[5] = y + hheight;
            
            array[6] = x - hwidth; 
            array[7] = y - hheight;
            
            array[8] = x + hwidth; 
            array[9] = y + hheight;
            
            array[10] = x - hwidth; 
            array[11] = y + hheight;
        }
        else{
            
            array[0] = x;
            array[1] = y;
            
            array[2] = x + twidth * scaleX;
            array[3] = y;
            
            array[4] = x + twidth * scaleX; 
            array[5] = y + theight * scaleY;
            
            array[6] = x;
            array[7] = y;
            
            array[8] = x + twidth * scaleX; 
            array[9] = y + theight * scaleY;
            
            
            array[10] = x; 
            array[11] = y + theight * scaleY;
        }
        
    }
    else{
        
        //TODO: non-centered rotation
        
        float angle = rotationAngle * 0.0174532925 + 3.14f;
        
        if (useCenter){
            float hwidth = htilew * scaleX;
            float hheight = htileh * scaleY;
            
            float co = cosf(angle);
            float si = sinf(angle);
            float cos_rot_w = co * hwidth;
            float cos_rot_h = co * hheight;
            float sin_rot_w = si * hwidth;
            float sin_rot_h = si * hheight;
            
            
            array[0] = x + cos_rot_w - sin_rot_h; 
            array[1] = y + sin_rot_w + cos_rot_h;
            
            array[2] = x - cos_rot_w - sin_rot_h; 
            array[3] = y - sin_rot_w + cos_rot_h;
            
            array[4] = x - cos_rot_w + sin_rot_h; 
            array[5] = y - sin_rot_w - cos_rot_h;
            
            array[6] = x + cos_rot_w - sin_rot_h; 
            array[7] = y + sin_rot_w + cos_rot_h;
            
            array[8] = x - cos_rot_w + sin_rot_h; 
            array[9] = y - sin_rot_w - cos_rot_h;
            
            array[10] = x + cos_rot_w + sin_rot_h; 
            array[11] = y + sin_rot_w - cos_rot_h;
        }
        else{
            
            float _width = twidth * scaleX;
            float _height = theight * scaleY;
            
            float co = cosf(angle);
            float si = sinf(angle);
            float cos_rot_w = co * _width;
            float cos_rot_h = co * _height;
            float sin_rot_w = si * _width;
            float sin_rot_h = si * _height;
            
            //TODO: fix this
            
            array[0] = x; 
            array[1] = y;
            
            array[2] = x - cos_rot_w - sin_rot_h; 
            array[3] = y - sin_rot_w + cos_rot_h;
            
            array[4] = x - cos_rot_w + sin_rot_h; 
            array[5] = y - sin_rot_w - cos_rot_h;
            
            array[6] = x; 
            array[7] = y;
            
            array[8] = x - cos_rot_w + sin_rot_h; 
            array[9] = y - sin_rot_w - cos_rot_h;
            
            array[10] = x + cos_rot_w + sin_rot_h; 
            array[11] = y + sin_rot_w - cos_rot_h;
            
        }
        
    }

}
//--------------------------------------------
void PicsContainer::drawVO(unsigned &_count, long texIndex){
    
    if ((texIndex >= 0) && (texIndex < (long)count())){
        glBindTexture(GL_TEXTURE_2D, TexNames[texIndex]);
        
    }
    else{
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    VO.bind();
    glDrawArrays(GL_TRIANGLES, 0, _count * 6);
    VO.unbind();
    _count = 0;
}


//----------------------------------------------------------
int PicsContainer::drawBatch(/*ShaderProgram * justcolor,
                              ShaderProgram * texcolor,*/
                              int method){
    int calls = 0;
        switch(method){
                
            default:{
                
                unsigned _count = 0;
                Vector3D uv(0.0f, 0.0f, 0.0f, 0.0f);
                float htilew, htileh;
                float twidth, theight;
                
                long texIndex = -1;
               

                //memset()
                for (unsigned long i = 0; i < batch.count(); i++){
                
                    PicData * p = 0;
                    
                    if ((texIndex != batch[i].textureIndex) && (_count)){
                        drawVO(_count, texIndex);
                        calls++;
                    }
                    
                    texIndex = batch[i].textureIndex;
                    
                    htilew = 0.5f; htileh = 0.5f;
                    twidth = 1; theight = 1;
                    
                    if ((batch[i].textureIndex >= 0) && ( batch[i].textureIndex < (long)count())){
                        p = &PicInfo[batch[i].textureIndex];
                        //printf("tex index %ld\n", batch[i].textureIndex);
                    }
                    
                    if(p){
                        uv = CalcUvs(p, batch[i].frame);
                        htilew = p->twidth / 2.0f; htileh = p->theight / 2.0f;
                        twidth = p->twidth; theight = p->theight;
                    }
                    float v[12];
                    produceVertices(batch[i].x, batch[i].y, batch[i].rotationAngle, batch[i].useCenter, batch[i].scaleX,
                                    batch[i].scaleY, htilew, htileh, twidth, theight, v);

                    float * ptr = VO.lock();
                    unsigned vsize = 8;
                    unsigned vindex = _count * 6 * vsize;
                    Vector3D uv_;
                    uv_.v[0] = uv.v[0]; uv_.v[1] = uv.v[2];
                    memcpy(&ptr[vindex], v, 2 * sizeof(float));
                    memcpy(&ptr[vindex + 2], batch[i].upColor.c, 4 * sizeof(float));
                    memcpy(&ptr[vindex + 6], uv_.v, 2 * sizeof(float));
                    vindex+= vsize;
                    uv_.v[0] = uv.v[1]; uv_.v[1] = uv.v[2];
                    memcpy(&ptr[vindex], &v[2], 2 * sizeof(float));
                    memcpy(&ptr[vindex + 2], batch[i].upColor.c, 4 * sizeof(float));
                    memcpy(&ptr[vindex + 6], uv_.v, 2 * sizeof(float));
                    
                    vindex+= vsize;
                    uv_.v[0] = uv.v[1]; uv_.v[1] = uv.v[3];
                    memcpy(&ptr[vindex], &v[4], 2 * sizeof(float));
                    memcpy(&ptr[vindex + 2], batch[i].dwColor.c, 4 * sizeof(float));
                    memcpy(&ptr[vindex + 6], uv_.v, 2 * sizeof(float));
                   
                    vindex+= vsize;
                    uv_.v[0] = uv.v[0]; uv_.v[1] = uv.v[2];
                    memcpy(&ptr[vindex], &v[6], 2 * sizeof(float));
                    memcpy(&ptr[vindex + 2], batch[i].upColor.c, 4 * sizeof(float));
                    memcpy(&ptr[vindex + 6], uv_.v, 2 * sizeof(float));
                    vindex+= vsize;
                    uv_.v[0] = uv.v[1]; uv_.v[1] = uv.v[3];
                    memcpy(&ptr[vindex], &v[8], 2 * sizeof(float));
                    memcpy(&ptr[vindex + 2], batch[i].dwColor.c, 4 * sizeof(float));
                    memcpy(&ptr[vindex + 6], uv_.v, 2 * sizeof(float));
                    
                    vindex+= vsize;
                    uv_.v[0] = uv.v[0]; uv_.v[1] = uv.v[3];
                    memcpy(&ptr[vindex], &v[10], 2 * sizeof(float));
                    memcpy(&ptr[vindex + 2], batch[i].dwColor.c, 4 * sizeof(float));
                    memcpy(&ptr[vindex + 6], uv_.v, 2 * sizeof(float));
                    VO.unlock();
                    
                    _count++;
                
                    if (_count == MAXSPRITES - 1){
                        drawVO(_count, texIndex);
                        calls++;
                    }
                }
                drawVO(_count, texIndex);
                calls++;
                
            } break;    
                
    }
    batch.destroy();
    return calls;
}
//-----------------------------------------------------
void PicsContainer::resizeContainer(unsigned long index,
                                    int twidth, int theight, int filter,
                                    const char * name,
                                    bool createTextures,
                                    GLuint texname){

    if (PicInfo.count() < index + 1){

            GLuint glui = texname;
            PicData p;
            p.twidth = twidth;
            p.theight = theight;
            p.filter = filter;
            for (unsigned i = PicInfo.count(); i < index + 1; i++){
                PicInfo.add(p);
                TexNames.add(glui);
            }

            if (createTextures)
                glGenTextures(1, ((GLuint *)TexNames.getData()) + index);

            char * copy = (char*)malloc(strlen(name)+1);
            strcpy(copy, name);
            char * res = 0;
            res = strtok(copy, "/");
            while (res){
                strcpy(PicInfo[index].name, res);
                res = strtok(0, "/");
            }
            free(copy);

        }
        else{
            PicData * pp = &PicInfo[index];
            pp->twidth = twidth;
            pp->theight = theight;
            pp->filter = filter;
            char * copy = (char*)malloc(strlen(name)+1);
            strcpy(copy, name);
            char * res = 0;
            res = strtok(copy, "/");
            while (res){
                strcpy(pp->name, res);
                res = strtok(0, "/");
            }
            free(copy);
            
            if (glIsTexture(TexNames[index]))
                glDeleteTextures(1, ((GLuint *)TexNames.getData()) + index);
            if (createTextures)
                glGenTextures(1, ((GLuint *)TexNames.getData()) + index);
            else
                 *(((GLuint *)TexNames.getData()) + index) = texname;

        }


}


//-----------------------------------------------------
bool PicsContainer::loadFile(const char* file, unsigned long index,
                             int twidth, int theight, int filter){


        Image naujas;

        unsigned short imageBits=0;


        if (!naujas.loadTga(file, imageBits)){
            printf("%s not found or corrupted by M$\n", file);
            return false;
        }


        resizeContainer(index, twidth, theight, filter, file);
       
        PicInfo[index].width = naujas.width;
        PicInfo[index].height = naujas.height;


        PicInfo[index].htilew =PicInfo[index].twidth/2.0f;
        PicInfo[index].htileh =PicInfo[index].theight/2.0f;
        PicInfo[index].vframes=PicInfo[index].height/PicInfo[index].theight;
        PicInfo[index].hframes=PicInfo[index].width/PicInfo[index].twidth;

        int filtras = GL_NEAREST;
        if (PicInfo[index].filter){
            filtras = GL_LINEAR;
        }


        glBindTexture(GL_TEXTURE_2D, TexNames[index]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtras );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtras );

        GLint border = 0;

        if (imageBits > 24)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, naujas.width, naujas.height,
                 border, GL_RGBA, GL_UNSIGNED_BYTE,naujas.data);
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, naujas.width, naujas.height,
                 border, GL_RGB, GL_UNSIGNED_BYTE,naujas.data);

        naujas.destroy();

        return true;

}
//---------------------------------------------------
void PicsContainer::makeTexture(Image& img,
                 const char * name,
                 unsigned long index,
                 int twidth, int theight, int filter){

    resizeContainer(index, twidth, theight, filter, name);
       
    PicInfo[index].width = img.width;
    PicInfo[index].height = img.height;


    PicInfo[index].htilew = PicInfo[index].twidth/2.0f;
    PicInfo[index].htileh = PicInfo[index].theight/2.0f;
    PicInfo[index].vframes = PicInfo[index].height/PicInfo[index].theight;
    PicInfo[index].hframes = PicInfo[index].width/PicInfo[index].twidth;

    int filtras = GL_NEAREST;
    if (PicInfo[index].filter){
        filtras = GL_LINEAR;
    }


    glBindTexture(GL_TEXTURE_2D, TexNames[index]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtras );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtras );

    GLint border = 0;

    if (img.bits > 24)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width, img.height,
                     border, GL_RGBA, GL_UNSIGNED_BYTE, img.data);
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.width, img.height,
                 border, GL_RGB, GL_UNSIGNED_BYTE, img.data);

}

//-----------------------------------------------------
bool PicsContainer::loadFile(const char* file, unsigned long index,
                             int twidth, int theight,
                             const char* basePath, int filter){

    

    char buf[255];
    sprintf(buf, "%s%s", basePath, file);

    if (!loadFile(buf, index, twidth, theight, filter)){
        sprintf(buf, "base/pics/%s", file);
        puts("Let's try base/");

        if (!loadFile(buf, index, twidth,  theight, filter))
            return false;

    }

    return true;

}
//--------------------------------------------------
bool PicsContainer::loadFile(unsigned long index,
                             const char * BasePath){

    Image naujas;

    unsigned short imageBits = 0;


    char dir[50];
    char buf[214];

    sprintf(dir, "%spics/", BasePath);
    sprintf(buf, "%s%s", dir, PicInfo[index].name);

    if (!naujas.loadTga(buf, imageBits)){
        sprintf(buf, "base/pics/%s", PicInfo[index].name);
        puts("Let's try base/");
        if (!naujas.loadTga(buf, imageBits)){
            printf("%s not found or corrupted by M$\n", buf);
            return false;
        }

    }
        
    PicInfo[index].width = naujas.width;
    PicInfo[index].height = naujas.height;


    PicInfo[index].htilew = PicInfo[index].twidth / 2.0f;
    PicInfo[index].htileh = PicInfo[index].theight / 2.0f;
    PicInfo[index].vframes = PicInfo[index].height / PicInfo[index].theight;
    PicInfo[index].hframes = PicInfo[index].width / PicInfo[index].twidth;

    int filtras = GL_NEAREST;
    if (PicInfo[index].filter)
        filtras = GL_LINEAR;


    glBindTexture(GL_TEXTURE_2D, TexNames[index]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtras );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtras );

    if (imageBits > 24)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, naujas.width, naujas.height,
                     0, GL_RGBA, GL_UNSIGNED_BYTE,naujas.data);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, naujas.width, naujas.height,
                     0, GL_RGB, GL_UNSIGNED_BYTE,naujas.data);

    naujas.destroy();

    return true;

}
//---------------------------------
void PicsContainer::attachTexture(GLuint textureID, unsigned long index,
                                  int width, int height,
                                 int twidth, int theight, int filter){

    resizeContainer(index, twidth, theight, filter, "lol", false, textureID);
    PicInfo[index].width = width;
    PicInfo[index].height = height;


    PicInfo[index].htilew = PicInfo[index].twidth / 2.0f;
    PicInfo[index].htileh = PicInfo[index].theight / 2.0f;
    PicInfo[index].vframes = PicInfo[index].height / PicInfo[index].theight;
    PicInfo[index].hframes = PicInfo[index].width / PicInfo[index].twidth;



    int filtras = GL_NEAREST;
    if (PicInfo[index].filter)
        filtras = GL_LINEAR;

    glBindTexture(GL_TEXTURE_2D, TexNames[index]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtras );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtras );
    glBindTexture(GL_TEXTURE_2D, 0);



}


//--------------------------------
int PicsContainer::findByName(const char* picname, bool debug){
    unsigned long start = 0;

    if (!PicInfo.count())
        return -1;

    
    while ((strcmp(PicInfo[start].name, picname) != 0) && (start < PicInfo.count())){
        if (debug)
            puts(PicInfo[start].name);
        start++;
    }

    if (start == PicInfo.count())
        return -1;

    return start;
}
//---------------------------------------
bool PicsContainer::initContainer(const char *list){
    FILE* failas=fopen(list,"rt");

    int result = 0;

    if (!failas)
        return false;

    while (!feof(failas)){
        PicData data;
        data.name[0] = 0;
        result = fscanf(failas,"%s\n",data.name);
        result = fscanf(failas,"%d %d %d\n",
                        &data.theight, &data.twidth,
                        &data.filter);
        PicInfo.add(data);
    }

    fclose(failas);

    for (unsigned long i = 0; i < PicInfo.count(); i++ ){
        GLuint glui = 0;
        TexNames.add(glui);
    }

    glGenTextures(PicInfo.count(), (GLuint *)TexNames.getData());

    return true;

}

//----------------------------------
void PicsContainer::destroy(){
    for (unsigned long i = 0; i < TexNames.count(); i++){
        if (glIsTexture(TexNames[i]))
            glDeleteTextures(1, ((GLuint *)TexNames.getData()) + i);
    }
    TexNames.destroy();
    batch.destroy();
    PicInfo.destroy();
    VO.destroy();
}

//-------------------------------
void PicsContainer::remove(unsigned long index){
    if (index < TexNames.count()){
        if (glIsTexture(TexNames[index]))
            glDeleteTextures(1, ((GLuint *)TexNames.getData()) + index);

        TexNames.remove(index);
        PicInfo.remove(index);
    }

}



