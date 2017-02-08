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
#include "SelectMenu.h"
#include "Text.h"
#ifdef WIN32
    #ifdef _MSC_VER
        #include <SDL.h>
    #else
        #include <SDL/SDL.h>
    #endif
#else
  #include <SDL/SDL.h>
#endif


//================================================

void SelectMenu::resize(unsigned dheight){
    int maxOptionLen = 0;
    for (unsigned i = 0; i < selection.count(); i++){
        if ((int)strlen(selection.items[i].name) > maxOptionLen)
            maxOptionLen = (int)strlen(selection.items[i].name);
    }

    if (selection.count()){

        if ((strlen(selection.getTitle())==0)
            ||(maxOptionLen > (int)strlen(selection.getTitle())))

            width = maxOptionLen * 12 + 60;
        else
            width = (int)strlen(selection.getTitle())*12 + 60;
    }
    else
        width = (int)strlen(selection.getTitle())*12 + 60;

    if (dheight==0)
        height = selection.count() * 20 + 50;
    else
        height = dheight;
}

//------------------------------------------------
void SelectMenu::init(int dx,
                          int dy,
                          SMenu& dsel,
                          unsigned char dst,
                          unsigned int dheight){


        setpos(dx,dy);


        selection.setTitle(dsel.getTitle());

        for (unsigned i = 0; i < dsel.count(); i++){

            selection.add(dsel.items[i]);
        }


        resize(dheight);

        defstate=dst;
        state=defstate;
    selected = false; 
    canceled = false;
    deactivate();
    pressedkey = 0;
    itemHeight = 20;
}
//--------------------------------------------
    void SelectMenu::reset(){
        state=defstate;
        selected=false;
        canceled=false;
    }
//--------------------------------------------
void SelectMenu::getInput(unsigned key,
                          int MouseX, int MouseY){

    if ((oldMouseX != MouseX)||(oldMouseY != MouseY)){
        if (selection.count()){
            //printf("%d\n", MouseX);
            if (((MouseX > getX() + 10) && (MouseX < getX() + width)) &&
                ((MouseY > getY() + 28)&&(MouseY < getY() + height))){

                state = (MouseY - getY() - 28) / itemHeight;
                if (state >= selection.count())
                    state = selection.count() - 1;

            }
        }
        oldMouseX = MouseX;
        oldMouseY = MouseY;
    }



    if (key != pressedkey){

        //printf("%u\n", key);
        //cia tam kad parinktu kai mygtuka atleidi
        if (pressedkey == 13) {
            if (selection.count())
                selected = true;      //enter
        }

        if (pressedkey == 27) {
            canceled=true;      //esc
        }

        if (selection.count()){
            switch(key) {
                case SDLK_UP: {
                    if (state>0) //up{
                        state--;
                    else
                        state = selection.count() - 1;
                }  break;

                case SDLK_DOWN: {
                    if (state < selection.count() - 1)  //down
                        state++;
                    else
                        state = 0;
                }
                break;

                default: pressedkey = 0;
            }

        }


        pressedkey = key;
    }

    
}
//--------------------------------------------
void SelectMenu::draw(PicsContainer &pics, unsigned long frame,
                      unsigned long pointer,
                      unsigned long font,
                      unsigned long icons,
                      COLOR c1, COLOR c2,
                      bool drawframe){



    unsigned newcount = 0;
    unsigned start = 0;

    if (icons)
        itemHeight = pics.getInfo(icons)->theight;

    int half = 0;
    if (height){

        half = (((height - 28) / itemHeight) / 2);
    }


    unsigned tmpheight = height;

    if (selection.count() * itemHeight + 28 > tmpheight){

        if ((int)state - half > 0){
            if ((int)state - half < (int)selection.count() - half * 2)
                start = state - half;
            else
                start = selection.count() - half * 2;
        }
        else
            start = 0;

        newcount = start + (half * 2);


    }
    else
        newcount = selection.count();

    //draw colorfull menu background
    pics.draw(-1, getX(), getY(),0, false,
              width, height, 0, c1, c2);
    //draw title
    WriteText(getX()+10,getY()+4, pics, font, selection.getTitle());

    //draw menu elements--------------------
    for (unsigned i = start; i < newcount; i++){
        if (i < selection.count() ){
            if (icons){
                int iconh = pics.getInfo(icons)->theight;
                /*char buf[50];
                sprintf(buf,"%d", iconh);
                puts(buf);*/
                pics.draw(icons, getX()+16,
                        getY()+28+((i-start) * iconh),
                        selection.items[i].icon);

                WriteText(getX() + 20 + iconh,
                        getY() + 28 +((i-start) * iconh),
                        pics,
                        font,
                        selection.items[i].name);

            }

            else{
                WriteText(getX() + 32, 
                          getY() + 28 + ((i - start) * 20),
                          pics, font, selection.items[i].name);
                //puts(selection.opt[i]);
            }
        }

    }//----------------------------------

    //draw up arrow
    if (start > 0)
        pics.draw(pointer, getX() + width - 18, getY() + 3, arrowFrames[1]);

    //draw down arrow
    if (newcount < selection.count())
        pics.draw(pointer, getX() + width - 18,
                           getY() + height - 19, arrowFrames[2]);


    //Draw hand(pointer)
    if ((rand() % 100) % 10 == 0)
        pics.draw(pointer, getX() + 3, 
                  getY() + 28 + ((state-start) * itemHeight),
                  arrowFrames[0]);
    else
        pics.draw(pointer, getX() + 5, 
                  getY() + 28 + ((state-start) * itemHeight),
                  arrowFrames[0]);

}

//---------------------------------------
    void SelectMenu::setTitle(const char* newTitle){

        selection.setTitle(newTitle);
    }
//---------------------------------------
int SelectMenu::run(unsigned key,
                    int MouseX, int MouseY){

    if (!selected){
        if (!active())
            activate();
        getInput(key, MouseX, MouseY);
        /*if (selected){
            deactivate();
        }*/
    }
    else{
        if (selection.items[state].function)
            (*selection.items[state].function)();
        //reset();
        return 1;
    }
    if (canceled){
        deactivate();
        reset();
        return -1;
    }

    return 0;
}

