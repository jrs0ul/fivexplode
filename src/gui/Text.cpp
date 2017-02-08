/*
 The Disarray 
 by jrs0ul(jrs0ul ^at^ gmail ^dot^ com) 2010
 -------------------------------------------
 Text stuff
 mod. 2011.03.08
 */
#include "Text.h"


void WriteText(int x, int y,
               PicsContainer& pics, int font,
               const char* s,
               float scalex, float scaley, COLOR c1, COLOR c2){

    for (unsigned int i = 0; i < strlen(s); i++){
        pics.draw(font, x+(11*scalex)*i, y, ((unsigned char)s[i])-32,
                  false, scalex, scaley, 0.0f, c1, c2);
    }
}
//-----------------------------------------------------
void WriteShadedText(int x, int y,
                     PicsContainer& pics, int font, const char* s,
                     float scalex, float scaley,
                     COLOR c1,
                     COLOR c2,
                     COLOR shade){
    WriteText(x - 1, y - 1, pics, font, s, scalex, scaley, shade, shade);
    WriteText(x, y, pics, font, s, scalex, scaley, c1, c2);
    
    
    
}



