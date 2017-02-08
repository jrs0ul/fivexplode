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
#include <cmath>
#include "OnScreenBoard.h"
//#include "Coolframe.h"




void   OnScreenKeyboard::setChars(const unsigned char * newchars){
        if (newchars){
            size_t len = 0;
            len = (strlen((const char*)newchars) > 254) ? 254 : strlen((const char*)newchars);
            memcpy(characters, newchars, len);
            characters[len] = '-';
            characters[len + 1] = 'v';
            count = len + 2;
        }
}
//---------------------------
void   OnScreenKeyboard::setRowLen(unsigned newLen){
        if (newLen < count)
            rowLength = newLen;
        else
            rowLength = count;
    
    if (height < (int)(count/rowLength) * charWidth + 10)
        height = (count/rowLength) * charWidth + 57;

}
//---------------------------
void   OnScreenKeyboard::draw(PicsContainer &pics, unsigned long frm,
                              unsigned long rod,
                              unsigned long font,
                              COLOR bg){

        pics.draw(-1, getX()+32, getY(), 0, false, 
                  20 + charWidth*rowLength - 64, height-1, 0,
                  bg, bg);
        pics.draw(frm, getX() - 32, getY(), 0, false, 
              1.0f, (float)height/64.0f, 0,
              bg, bg);
        pics.draw(frm, getX() + 20 + charWidth*rowLength-32, getY(), 1, false, 
              1.0f, (float)height/64.0f, 0,
              bg, bg);

        unsigned rows = (unsigned)ceilf((count*1.0f)/rowLength);
        unsigned index = 0;
        for (unsigned i = 0; i < rows; i++){
            for (unsigned a = 0; a < rowLength; a++){
                if(index < count){
                    if (index == state){
                        pics.draw(-1, getX()+10+a*charWidth, getY()+10+i*charWidth, 0, false, 
                                  charWidth - 2, charWidth - 2, 0,
                                  COLOR(0,0,0.0f,0.8f), COLOR(0.3f,0.3f,0.3f,0.8f));
                    }


                    if (index == count - 2){
                        pics.draw(rod, getX()+16+a*charWidth,
                                  getY()+16+i*charWidth,
                                  5, false, 
                                  1.5f, 1.5f, 0,
                                  COLOR(1,1,1.0f,1.0f), COLOR(1,1,1.0f,1.0f));

                    }
                    else
                        if (index == count - 1){
                            pics.draw(rod,
                                      getX()+ 16 + a * charWidth,
                                      getY()+ 16 + i * charWidth,
                                      4, false, 
                                      1.5f, 1.5f, 0,
                                      COLOR(1,1,1.0f,1.0f), COLOR(1,1,1.0f,1.0f));
                    }
                        else
                            pics.draw(font, 
                                      getX() + 20 + a * charWidth, 
                                      getY() + 20 + i * charWidth,
                                      characters[index]-32, false, 
                                      1.0f, 1.0f, 0,
                                      COLOR(1,1,1.0f,1.0f), COLOR(1,1,1.0f,1.0f));
                index++;
            }
        }

    }
   // Coolframe(getX(),getY(),width,height, pics, frm);
}
//---------------------------
unsigned char OnScreenKeyboard::getInput(unsigned key, int mx, int my){

    if ((mx > getX()) && (mx < getX() + (int)rowLength * charWidth + 10)
        && (my > getY()) && (my < getY() + height)){
        
        state = ((my - getY()-10)/charWidth)*rowLength + ((mx - getX() - 10)/charWidth);
        
        if (state == count - 1){
            entered = true;
            pressedkey = key;
            return 0;
        }
        
        if (state == count - 2){
            pressedkey = key;
            return 8;
        }
        
        pressedkey = key;
        return characters[state];
        
            
    }

    if (key != pressedkey){

        //printf("%u\n", key); 
        if (pressedkey == 27)
            canceled = true;   
            
        if (pressedkey == 13){

            if (state == count - 1){
                    entered = true;
                    pressedkey = key;
                    return 0;
            }

                if (state == count - 2){
                    pressedkey = key;
                    return 8;
                }

                pressedkey = key;
                return characters[state];
        }

        if (pressedkey == 127){
            pressedkey = key;
            return 8;
        }


        if (count){

                switch(key) {

                case 0:{ //up
                    if (state > rowLength)
                        state -= rowLength;
                }  break;
                case 1:{ //down
                    if (state + rowLength < count)
                        state += rowLength;

                }  break;
                case 3:{ //left
                    if (state < count - 1)
                        state++;
                }  break;

                case 2:{ //right
                    if (state > 0)
                        state--;
                } break;

                
                default: pressedkey = 0;
                }
        }


            pressedkey = key;
            return key;
    }

        return 0;

    }

