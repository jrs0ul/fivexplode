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
#ifndef _MENU_STRUCT_H
#define _MENU_STRUCT_H

#include <cstring>
#include "../DArray.h"

typedef void (*Func)();

//Menu item struct
struct MenuItem{
    char     name[255];  //Text in menu
    char     value[255]; //Some text value; not visible
    unsigned icon;       //icon index in iconset
    unsigned id;        //some decimal value
    Func     function;  //callback function address

    MenuItem(){
        function = 0;
    }
};

//====================
//Menu class
class SMenu{
    char menuTitle[255];
public:
    DArray<MenuItem> items; //menu items

    void        destroy(){items.destroy();}
    void        add(MenuItem& item){ items.add(item);}
    unsigned    count(){return items.count();}
    void        remove(unsigned long index){
                    if (index < items.count())
                        items.remove(index);
                }
    void        setTitle(const char * newTitle);
    const char* getTitle() {return menuTitle;};
};

 #endif //_MENU_STRUCT_H
