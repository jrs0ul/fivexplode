/*
 The Disarray 
 by jrs0ul(jrs0ul ^at^ gmail ^dot^ com) 2010
 -------------------------------------------
 TGA loader/saver
 mod. 2010.11.30
 */
#ifndef IMAGE_H
#define IMAGE_H


/*struct TgaHeader{
    unsigned char fieldDescSize;
    unsigned char colormaptype;
    unsigned char imageCode; //0=none,1=indexed,2=rgb,3=grey,+8=rle packed
    unsigned short mapStart;
    unsigned short mapLength;
    unsigned char  palettebits;
    unsigned short xStart;
    unsigned short yStart;
    unsigned short width;
    unsigned short height;
    unsigned char imageBits;
    unsigned char imageDesc;
};*/



class Image{
public:
    unsigned short width;
    unsigned short height;
    unsigned char* data;
    unsigned short bits;
    Image(){ data=0; width=0; height=0;}
    bool loadTga(const char* name, unsigned short& imageBits );
    bool saveTga(const char* name);
    void destroy();
};


#endif //IMAGE_H



