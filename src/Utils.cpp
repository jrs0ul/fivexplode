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
#include <cstring>
#include <cwchar>
#include <cmath>
#include <cstdlib>
#include "Utils.h"




double _round(double x){ return (x-floor(x))>0.5 ? ceil(x) : floor(x); }

//----------------------------------------------------
void convertLT(const wchar_t* txt, char * buf, int len) {
    
        wchar_t LTletters[] = L"ĄąČčĘęĖėĮįŠšŲųŪūŽž";

        unsigned char Codes[] = {161, 177, 200, 232, 202, 234, 204, 236, 199, 231, 169,
                             185, 217, 249, 222, 254, 174, 190};

        if (txt){
            char * bufs = 0;
            int _len = 0;
            _len = wcslen(txt);
            if (_len){
                bufs = (char *)malloc(_len + 1);
    
                for (int i = 0; i <= _len; i++) {
                    bool found = false;
                    for (int a = 0; a < (int)wcslen(LTletters); a++){
                        if (txt[i] == LTletters[a]){
                            bufs[i] = Codes[a];
                            found = true;
                            break;
                        }
                    }
                    if (!found){
                        bufs[i] = (char)txt[i];
                    }
                }
                bufs[_len] = '\0';
                if (len < (int)strlen(bufs))
                    bufs[len] = '\0';
                strncpy(buf, bufs, len);
                //printf("|%s|len %d\n",buf, _len);
                free(bufs);
            }
        }
}
//-------------------------------------------------------------
wchar_t * _wcstok(wchar_t * str, const wchar_t * delimiters, wchar_t** pointer){
        wchar_t * result = 0;
        #ifdef WIN32
            result = wcstok(str, delimiters);
        #else
            result = wcstok(str, delimiters, pointer);
        #endif
        return result;
}
//----------------------------------------------------------
void      UTF8toWchar(char* utftext, wchar_t * wchartext){

        size_t utftextLen = 0;
        if (utftext){
            utftextLen = strlen(utftext);

            #ifndef WIN32
                iconv_t cd;
                cd = iconv_open("WCHAR_T", "UTF-8");
                if (cd == (iconv_t)-1){
                    wchartext[0] = L'\0';
                    return;
                }

                if (cd){
                    if (utftextLen > 0){
                        size_t s2 = utftextLen * sizeof(wchar_t);
                        char * in = utftext;
                        char * out = (char *)wchartext;
                        size_t result = 0;
                        result = iconv(cd, &in, (size_t*)&utftextLen, &out, (size_t*)&s2);
                        //if (s2 >= sizeof (wchar_t))
                        *((wchar_t *) out) = L'\0';

                    }
                    iconv_close(cd);
                }
            #else
                    int len = 0;
                    len = MultiByteToWideChar(CP_UTF8, 0, utftext, utftextLen ,
                                    wchartext, utftextLen);
                    wchartext[len] = L'\0';
            #endif
        }
}
//-------------------------------------------------------------
    bool      ReadFileData(const char* path, char ** data){
        if (*data)
            return false;
        FILE * f = 0;
        f = fopen(path, "rb");
        if (!f)
            return false;

        unsigned long fsize = 0;
        fseek (f, 0, SEEK_END);
        fsize = ftell(f);
        rewind(f);

        if (fsize <= 0)
            return false;

        (*data) = (char *)malloc(fsize + 10);
        if (!fread(*data, 1, fsize, f)){

            free(*data);
            (*data) = 0;
            fclose(f);
            return false;
        }
        fclose(f);
        

        (*data)[fsize - 1] = '\0';

        //puts(*data);

        return true;

    }
//-------------------------------------------------------------
void GetHomePath(char * _homePath){

        char * home = 0;
        char * homepath = 0;
        char * homedrive = 0;

        home = getenv("HOME");
        if (!home){
            homedrive = getenv("HOMESHARE");
            if (!homedrive)
                homedrive = getenv("HOMEDRIVE");
            homepath = getenv("HOMEPATH");

            if ((homepath)&&(homedrive)){
                sprintf(_homePath, "%s%s/", homedrive, homepath);
            }
            else
                sprintf(_homePath, "./");
        }
        else{
            sprintf(_homePath, "%s/", home);
        }
}
//---------------------------------------------------------------
void    GetFileList(const char* path, Lines& l){
#ifndef WIN32
    DIR* Dir;
    dirent *DirEntry = 0;
    Dir = opendir(path);
    DirEntry = readdir(Dir);
    while(DirEntry){
        ptext pt;
        strncpy(pt.t, DirEntry->d_name, 255);
        l.l.add(pt);
        
        DirEntry = readdir(Dir);
    }
    closedir(Dir);
#else
    HANDLE hFind;
    WIN32_FIND_DATA FindData;

    char buf[255];
    sprintf(buf,"%s/*", path);
    hFind = FindFirstFile(buf, &FindData);

    if(hFind != INVALID_HANDLE_VALUE){
            ptext pt;
            strncpy(pt.t, FindData.cFileName, 255);
            l.l.add(pt);
    }
    while (FindNextFile(hFind, &FindData)){

            ptext pt;
            strncpy(pt.t, FindData.cFileName, 255);
            l.l.add(pt);
    }

    FindClose(hFind);
#endif


}

//---------------------------------------------------------------
void    GetDirectoryList(const char* path, Lines& l){

#ifndef WIN32
    unsigned char isFolder = 0x4;
    DIR* Dir;
    dirent *DirEntry = 0;
    Dir = opendir(path);
    DirEntry = readdir(Dir);
    while(DirEntry){
        if ( DirEntry->d_type == isFolder){
            ptext pt;
            strncpy(pt.t, DirEntry->d_name, 255);
            l.l.add(pt);
        }
        
        DirEntry = readdir(Dir);
    }
    closedir(Dir);
#else
    HANDLE hFind;
    WIN32_FIND_DATA FindData;

    char buf[255];
    sprintf(buf,"%s/*", path);
    hFind = FindFirstFile(buf, &FindData);

    if(hFind != INVALID_HANDLE_VALUE){
        if (FindData.dwFileAttributes & 0x0010){
            ptext pt;
            strncpy(pt.t, FindData.cFileName, 255);
            l.l.add(pt);
        }
    }
    while (FindNextFile(hFind, &FindData)){

        if (FindData.dwFileAttributes & 0x0010){
            ptext pt;
            strncpy(pt.t, FindData.cFileName, 255);
            l.l.add(pt);

        }
    }

    FindClose(hFind);
#endif

    
}
//----------------------------------------------------------------
void MakeDir(const char * path){
    #ifdef WIN32
        CreateDirectory(path, 0);
    #else
        mkdir(path, S_IRUSR|S_IWUSR|S_IXUSR);
    #endif

}

//--------------------------------------------------------------

bool CirclesColide(float x1,float y1,float radius1, float x2, float y2, float radius2){

     float difx = (float) fabs (x1 - x2);
     float  dify = (float) fabs (y1 - y2);
     float   distance = (float) sqrt (difx * difx + dify * dify);

     if   (distance < (radius1 + radius2))
        return   true;

     return   false;

}
//-----------------------------------------------
bool    CollisionSphere_Sphere(Vector3D pos1, float radius1,
                               Vector3D pos2, float radius2){

     float  difx = (float) fabs (pos1.v[0] - pos2.v[0]);
     float  dify = (float) fabs (pos1.v[1] - pos2.v[1]);
     float  difz = (float) fabs (pos1.v[2] - pos2.v[2]);

     float   distance = (float) sqrt (difx * difx + dify * dify + difz * difz);

     if   (distance < (radius1 + radius2))
        return   true;

     return   false;

}
//------------------------------------------------
int CollisionRay_Traingle( Vector3D rayOrigin, Vector3D rayDir,
                           Vector3D vert0,Vector3D vert1,
                           Vector3D vert2, float &t){

    float u,v;

    Vector3D tvec, pvec, qvec;
    float det,inv_det;

    Vector3D edge1 = vert1 - vert0;
    Vector3D edge2 = vert2 - vert0;

    pvec = rayDir ^ edge2;

    det = edge1 * pvec;

    if (det > -0.000001 && det < 0.000001)
        return 0;
    inv_det = 1.0f/det;
    tvec = rayOrigin - vert0;
    u = tvec * pvec;
    u  *= inv_det;
    if (u <0.0 || u>1.0f)
        return 0;

    qvec = tvec ^ edge1;

    v = rayDir * qvec;
    v *= inv_det;
    if (v < 0.0 || u + v > 1.0)
        return 0;

    t = edge2 * qvec;
    t *= inv_det;

    return 1;
}

//-----------------------------------------------

float   CalcFrustumSphereRadius(float _near, float _far,
                                float fov){
    float viewLen = _far - _near;
    float hviewLen = viewLen * 0.5f;
    float frustumHeight = viewLen * tan((fov * 0.0174532925f) * 0.5f);
    Vector3D P(0.0f, 0.0f, _near + hviewLen);
    Vector3D Q(frustumHeight, frustumHeight, viewLen);
    Vector3D Diff = P - Q;

    return Diff.length();
}
//--------------------------------------------------------
Vector3D    CalcFrustumSphereCenter(float _near, float _far,
                                    Vector3D camPos, Vector3D camLook){


    float hviewLen = (_far - _near) * 0.5f;
    float distanceFromCam = hviewLen + _near;
    // calculate the center of the sphere
    Vector3D temp = Vector3D(camLook.v[0] * distanceFromCam,
                             camLook.v[1] * distanceFromCam,
                             camLook.v[2] * distanceFromCam);
    return camPos + temp;

}



