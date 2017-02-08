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
#ifndef _MATRIX_H
#define _MATRIX_H


#include <cmath>
#include <cstring>
#include "Vectors.h"

//Basic matrix structure, everything is stored in
//single-dimension array
struct FlatMatrix{

    float m[16];
    //---------
    FlatMatrix(float e00, float e01, float e02, float e03,
               float e10, float e11, float e12, float e13,
               float e20, float e21, float e22, float e23,
               float e30, float e31, float e32, float e33){
        m[0]  = e00;  m[1]  = e01;  m[2]  = e02;   m[3]  = e03;
        m[4]  = e10;  m[5]  = e11;  m[6]  = e12;   m[7]  = e13;
        m[8]  = e20;  m[9]  = e21;  m[10] = e22;  m[11]  = e23;
        m[12] = e30;  m[13] = e31;  m[14] = e32;  m[15]  = e33;
    }
    FlatMatrix(){}
    //initialization using an array
    FlatMatrix(float * _m){
        memcpy(m, _m, 16 * sizeof(float));
    }

    float * getRow(unsigned index){
        return &(m[4*index]);
    }

    friend const FlatMatrix operator * (const FlatMatrix& left, 
                                        const FlatMatrix& right);

};
//-----------------------------------------------------
//makes similar matrix as gluLookAt
void       MatrixLookAt(Vector3D pos, Vector3D target, Vector3D up,
                        float * mat);
//replacement for gluperspective
void       MatrixPerspective(float fov, float aspect, float zNear,
                             float zFar, float * mat);
//replacement for glOrtho
void       MatrixOrtho(float left, float right, float bottom, float top,
                       float near, float far, float * mat);

//replacement for gluUnproject
Vector3D   Unproject(float x, float y, float z, 
                     float * modelview, float* projection,
                     int * viewport);

void       MatrixRotationAxis(float angle, Vector3D axis, float * mat);
void       MatrixTranslationScale(Vector3D position,
                                  Vector3D scale,
                                  float * mat);
void       MatrixDump(float * m);

void       MatrixInverse(float * m1, float * m2);

//old 2d array matrix stuff----------------------------
typedef float Matrix[4][4] ;
void       MatrixCopy(Matrix a, Matrix b);

FlatMatrix Matrix2Flat(Matrix m);
void       MatrixFlat2Matrix(FlatMatrix m, Matrix& mat);

void       MatrixIdentity(Matrix& m);
void       MatrixMultiply (Matrix _a, Matrix _b, Matrix& out);
void       MatrixTranslation(float x, float y, float z, Matrix &mat);
void       MatrixRotationY(float angle, Matrix& mat);
void       MatrixRotationY(float angle, float * mat);
void       MatrixRotationAxis(float angle, Vector3D axis, Matrix& mat);
void       MatrixScale(float x, float y, float z, Matrix &mat);
void       MatrixDump(Matrix& m);
void       Quat2Matrix(float w, float x, float y, float z, Matrix & mat);
void       MatrixInverse(Matrix& in, Matrix& out);




#endif //_MATRIX_H



