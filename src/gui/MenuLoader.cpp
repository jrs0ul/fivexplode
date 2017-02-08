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
#include "MenuLoader.h"
#include "../Xml.h"
#include "../Utils.h"
#include <cwchar>
#include <cstdio>


//-----------------------------------------------
bool MenuLoader::loadMenus(const char* filename){

    Xml menus;

    if (!menus.load(filename))
        return false;

    XmlNode * root = 0;
    root = menus.root.getNode(L"Menus");
    if (root){
        
        unsigned long count = root->childrenCount();

        for (unsigned long a = 0; a < count; a++) {

            XmlNode * menu = 0;
            menu = root->getNode(a);
            SMenu newMenu;
            if (menu){

                XmlNode * title = 0;
                XmlNode * options = 0;
                title = menu->getNode(L"Title");
                if (title){
                    char t[30];
                    convertLT(title->getValue(), t, 30);
                    newMenu.setTitle(t);
                    printf("title: %s\n",t);
                }
                options = menu->getNode(L"Options");
                
                unsigned long optionCount = 0;
                optionCount = options->childrenCount();
                //printf("options count %lu\n", optionCount);

                for ( unsigned long i = 0; i < optionCount; i++){
                    MenuItem nmi;
                    XmlNode * option = 0;
                    option = options->getNode(i);
                    if (option){
                        convertLT(option->getValue(), nmi.name, 255);
                        newMenu.add(nmi);
                        //puts(nmi.opt);
                        //puts("-----");
                    }
                }

                data.add(newMenu);
                puts("added");
            }
        }
    }
    
    menus.destroy();


    return true;
}

//---------------------------------
void MenuLoader::destroy(){


    for (unsigned i = 0; i< data.count(); i++)
        data[i].destroy();
    data.destroy();

}
