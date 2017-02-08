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
#ifndef __VERTEX_OBJECT_H
#define __VERTEX_OBJECT_H


#ifdef WIN32
    #ifdef _MSC_VER
        #include <SDL_opengl.h>
    #else
        #include <SDL/SDL_opengl.h>
    #endif
#else
    #ifdef __APPLE__    
        #include <TargetConditionals.h>
        #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
            #include <OpenGLES/ES1/gl.h>
            #include <OpenGLES/ES1/glext.h>
        #else
              #include <SDL/SDL_opengl.h>
        #endif
    #else
        #include <SDL/SDL_opengl.h>
    #endif
#endif




/*basic class to wrap up Vertex Buffer Object and Vertex Array*/


enum VertexFormat{
    VEMPTY        = 0,
    VPOSITION     = 1,
    VNORMAL       = 2,
    VCOLOR        = 4,
    VTEXCOORD0    = 8
};


class VertexObject{

    char *       data;
    GLuint       vboId;

    unsigned     _vertSize;
    unsigned     _vertCount;
    GLenum       bufferType;
    VertexFormat vertexFormat;
    bool         useVBO;
    bool         is2D;
public:

            VertexObject(){ data = 0; _vertSize = 0; _vertCount = 0;
                            vboId = 0;
                            vertexFormat = VEMPTY; useVBO = true; is2D = false;}
    void    init(void * _data, unsigned format,
                 unsigned long _vertexCount, 
                 bool useDynamic,
                 bool _is2D = false,
                 bool _useVBO = true);
    //prepares data for drawing
    void    bind();
    void    unbind();

    float * lock();
    void    unlock();

    void    copyFrom(VertexObject& obj);

    void    destroy();

    unsigned vertexSize(){return _vertSize;}
    unsigned long vertexCount(){ return _vertCount;}
    char * getRawData(){return data;}
};
#endif //__VERTEX_OBJECT_H
