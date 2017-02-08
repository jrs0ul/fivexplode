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
#include "Button.h"
#include "Text.h"


void Button::draw(PicsContainer& pics,
                  unsigned picindex ,
                  unsigned frame){
    PicData* d = 0;
    d = pics.getInfo(picindex);
    if (d){
        if ((d->twidth)&&(d->theight)){
            pics.draw(picindex, getX() + width / 2, getY() + height / 2,
                      frame, true, 
                      (width/(d->twidth*1.0f)) * scalex,
                      height/(d->theight*1.0f), 0.0f, c , c);
        }
    }
}
//----------------------------------

void  Button::drawTextnPicture(PicsContainer& pics, unsigned frame, unsigned index,
                               unsigned font, const char* text){
    PicData* d = 0;
    d = pics.getInfo(index);
    if (d){
        pics.draw(index, getX() + width / 2, getY() + height / 2,
                  frame, true, 
                  width/(d->twidth*1.0f), height/(d->theight*1.0f), 0.0f, c , c);
    }
    float textlen = strlen(text) * 11.5f;
  
    WriteShadedText( getX() + width/2.0f - textlen/2.0f, getY() + height/2 - 8, pics, font, text, 1.0f, 1.0f, COLOR(1,1,1));
    
}
//---------------------------------


bool Button::isPointerOnTop(int px, int py){
    if ((px > getX()) && (px < (getX() + width))
            && (py > getY()) && (py < (getY() + height)))

        return true;

    return false;
}
//---------------------------------
void Button::shiftstate(){
     if (pressed) pressed = false;
     else 
         pressed = true;
}


