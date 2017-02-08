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

#include "SDLVideo.h"

bool SDLVideo::InitWindow(const char * title, const char * iconPath,
                          bool isWindowed, bool resizable){

    const SDL_VideoInfo* info = NULL;

    // First, initialize SDL's video subsystem.
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
        return false;

    // Let's get some video information.
    info = SDL_GetVideoInfo( );

    if( !info ){
        puts("info sucks");
        return false;
    }

    _bpp = info->vfmt->BitsPerPixel;

    printf("bpp: %d\n", _bpp);
    int rSize, gSize, bSize, aSize;
    rSize = gSize = bSize = aSize = 8;
    int buffsize = ((rSize + gSize + bSize + aSize + 7) & 0xf8);

    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 8);
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 0 );
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, rSize );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, gSize );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, bSize );
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, aSize );
    SDL_GL_SetAttribute( SDL_GL_BUFFER_SIZE, buffsize );

    _flags = SDL_OPENGL|SDL_DOUBLEBUF;
        
    if (resizable)
        _flags = _flags|SDL_RESIZABLE;

    if ( info->hw_available)
        _flags = _flags|SDL_HWSURFACE;
    else
        _flags = _flags|SDL_SWSURFACE;

    if ( info->blit_hw )
        _flags = _flags |SDL_HWACCEL;

    if (!isWindowed){
        _flags = _flags |SDL_FULLSCREEN;
        puts("lolscreen");
    }

    SDL_WM_SetCaption(title, 0);
    icon = SDL_LoadBMP(iconPath);
    if (icon){
        SDL_SetColorKey(icon, SDL_SRCCOLORKEY,
                        SDL_MapRGB(icon->format, 255, 255, 255));
        SDL_WM_SetIcon(icon, 0);
    }

    if( SDL_SetVideoMode( _width, _height,
                          _bpp, _flags ) == 0 ){
        puts("can't set video mode");
        return false;
    }

    return true;


}
//-------------------------------------------
void SDLVideo::setMetrics(unsigned w, unsigned h){
    _width = w;
    _height = h;
}
//-------------------------------------------
bool SDLVideo::resetMode(){

   if (SDL_SetVideoMode( _width, _height,
                      _bpp, _flags ) == 0)
       return false;
   return true;
}

//--------------------------------------------
void SDLVideo::Quit(){

    SDL_Quit();
}

