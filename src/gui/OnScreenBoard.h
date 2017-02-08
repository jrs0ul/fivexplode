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
#ifndef ONSCREENKEYBOARD_H
#define ONSCREENKEYBOARD_H

#include "BasicUiControl.h"
#include "../TextureLoader.h"


    class OnScreenKeyboard:public BasicControl{

        unsigned char characters[256];
        unsigned      count;
        unsigned      rowLength;
        unsigned      state;
        unsigned      pressedkey;
        int           charWidth;

    public:
        bool          entered;
        bool          canceled;
        int           width;
        int           height;

        void          setChars(const unsigned char * newchars);
        void          setRowLen(unsigned newLen);
        void          setCharWidth(int newWidth){charWidth = newWidth;}
        void          draw(PicsContainer &pics, unsigned long frm,
                           unsigned long rod,
                           unsigned long font,
                           COLOR bg = COLOR(0,0,0.9f,0.8f));
        unsigned char getInput(unsigned key, int mx, int my);


        OnScreenKeyboard(){
            count = 0;
            rowLength = 0;
            state = 0;
            pressedkey = 0;
            entered = false;
            canceled = false;
            width = 250;
            height = 120;
            charWidth = 18;
        }

        void reset(){
            state = 0;
            pressedkey = 0;
            entered = false;
            canceled = false;

        }


    };


#endif //ONSCREENKEYBOARD_H

