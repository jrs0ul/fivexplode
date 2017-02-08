/***************************************************************************
 *   Copyright (C) 2008 by jrs0ul   *
 *   jrs0ul@gmail.com   *
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
#ifndef _UTILS_H
#define _UTILS_H


#ifdef WIN32
    #include <windows.h>
#else
    #include <iconv.h>
    #include <dirent.h>
    #include <sys/stat.h>
    #include <sys/types.h>
#endif

#include "Vectors.h"
#include "DArray.h"

struct _POINT{
    int x; int y;
};

struct ptext{
    char t[255];
};

struct Lines{
    DArray<ptext> l;
};


//converts wide character string to ascii string with special characters
//which represent lithuanian letters. Usefull for bitmap font
void convertLT(const wchar_t* txt, char * buf, int len);

//multiplatform version of wcstok, in Windblows it uses wcstok with 3 
//arguments and with 2 in other oses
wchar_t * _wcstok(wchar_t * str, const wchar_t * delimiters, wchar_t** pointer);

//multiplatform function converts multi byte utf8 string (char*) to 
//wide char string (wchar_t *)
void      UTF8toWchar(char* utftext, wchar_t * wchartext);

//reads file to array, data is allocated in function
//use free() to free the memory
bool      ReadFileData(const char* path, char ** data);

//Multiplatform function gets user home path.
void GetHomePath(char * _homePath);

//Multiplatform directory creation
void    MakeDir(const char * path);

double _round(double x);

void    GetFileList(const char * path, Lines& l);
void    GetDirectoryList(const char* path, Lines& l);

bool    CirclesColide(float x1,float y1,float radius1, float x2, float y2, float radius2);

bool    CollisionSphere_Sphere(Vector3D pos1, float radius1,
                               Vector3D pos2, float radius2);
int     CollisionRay_Traingle( Vector3D rayOrigin, Vector3D rayDir,
                               Vector3D vert0, Vector3D vert1,
                               Vector3D vert2, float &t);

float       CalcFrustumSphereRadius(float near, float far, float fov);
Vector3D    CalcFrustumSphereCenter(float near, float far,
                                    Vector3D camPos, Vector3D camLook);


#endif //UTILS_H
