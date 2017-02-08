/***************************************************************************
 *   Copyright (C) 2008 by jrs0ul                                          *
 *   jrs0ul@gmail.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef SELECT_MENU_H
#define SELECT_MENU_H

#include "BasicUiControl.h"
#include "MenuStruct.h" //structure of menu data
#include "../TextureLoader.h"


//--------------------------------------
//Basic selection menu
class SelectMenu:public BasicControl{

public:

    SMenu selection;    //menu data
    int width;          //dialog width
    int height;         //dialog height
    unsigned defstate;  //default option
    unsigned state;     //selected menu state

    bool selected;  //true, when item selected
    bool canceled;  //true, when menu canceled
    unsigned pressedkey;
    int itemHeight;

    int oldMouseX;
    int oldMouseY;

    unsigned arrowFrames[3]; //what frames to use for pointer and arrows



    SelectMenu(){
        arrowFrames[0] = 0;
        arrowFrames[1] = 1;
        arrowFrames[2] = 2;
    }
    //initialises menu
    void init(int dx, int dy,
              SMenu& dsel,
              unsigned char dst,       //default menu state
              unsigned int dheight = 0 //default menu height
                 );
    //resets selection that has been made
    void reset();
    void getInput(unsigned key,
                  int MouseX = 0, int MouseY = 0);
    //draws menu
    void draw(PicsContainer &pics, unsigned long frame,
              unsigned long pointer,
              unsigned long font, unsigned long icons = 0,
              COLOR c1 = COLOR(0.0f, 0.0f, 0.9f, 0.8f),
              COLOR c2 = COLOR(0.0f, 0.0f, 0.0f, 0.8f),
              bool drawframe = true);

    void destroy(){selection.destroy();}

    void setTitle(const char* newTitle);
    //reshapes menu
    void resize(unsigned dheight = 0);

    int  run(unsigned key, 
             int MouseX = 0, int MouseY = 0);

};

#endif //SELECT_MENU_H
