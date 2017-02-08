#ifndef YESNODIALOG_H
#define YESNODIALOG_H

#include "Button.h"

class YesNoDialog: public BasicControl{
    int width;
    int height;
    char text[255];
    Button yes;
    Button no;
    
public:
    YesNoDialog(){
        strcpy(text,"Do you want to reset ?");
    }
    YesNoDialog(int _x, int _y, int w, int h){
        set(_x, _y, w, h);
        strcpy(text,"Do you want to reset ?");
    }

    void set(int _x, int _y, int w, int h);
    void setText(const char* _text){strncpy(text, _text, 254);}
    void draw(PicsContainer& pics, unsigned font, unsigned decorations,
              unsigned buttons, unsigned buttonframes, float decorationSize = 64.0f);
    bool isPointerOnYes(int px, int py);
    bool isPointerOnNo(int px, int py);
    
};



#endif //YESNODIALOG_H

