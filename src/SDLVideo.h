/***************************************************************************
 *   Copyright (C) 2009 by jrs0ul                                          *
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

#ifndef _SDL_VIDEO_WRAP_H
#define _SDL_VIDEO_WRAP_H

#ifdef WIN32
    #ifdef _MSC_VER
        #include <SDL.h>
    #else
        #include <SDL/SDL.h>
    #endif
#else
  #include <SDL/SDL.h>
#endif

class SDLVideo{

    int _bpp;
    int _flags;

    unsigned _width;
    unsigned _height;

    SDL_Surface * icon;

public:

    SDLVideo(){_width = 640; _height = 480;}

    unsigned width(){return _width;}
    unsigned height(){return _height;}
    int colorBits(){return _bpp;}
    int flags(){return _flags;}
    void setMetrics(unsigned w, unsigned h);

    bool resetMode();


    bool InitWindow(const char * title, const char * iconPath,
                    bool isWindowed = true, bool resizable = false);
    void Quit();

};


#endif //_SDL_VIDEO_WRAP_H

