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
#ifndef _BASICUICONTROL_
#define _BASICUICONTROL_


//--------------------------------------

    class ActiveControl{
        //is control active
        bool activated;
    public:
        void activate(){if (!activated) activated = true;}
        bool active(){return activated;}
        void deactivate(){if (activated) activated = false;}
    };
//----------------------------------------

    class BasicControl: public ActiveControl {
        int x;
        int y;
    public:
        void setpos(int nx, int ny){x = nx; y = ny;}
        int getX(){ return x;}
        int getY(){ return y;}
    };




#endif //_BASICUICONTROL_
