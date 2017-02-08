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
#include "Matrix.h"
#include <cmath>
#include <cstdio>


//--------------------------------------
const FlatMatrix operator * (const FlatMatrix& left,
                                const FlatMatrix& right){ 
    FlatMatrix tmp;
    

    int k;
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            for(k = 0, tmp.m[i * 4 + j] = 0; k < 4; k++){
                tmp.m[i * 4 + j] += left.m[i * 4 + k] * right.m[k * 4  + j];
            }

        return tmp;
}


//------------------------------------
void       MatrixCopy(Matrix a, Matrix b){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            b[i][j] = a[i][j];
        }
    }

}
//---------------------------------
void MatrixIdentity(Matrix& m){
    m[0][0] = 1.0f;     m[0][1] = 0.0f;   m[0][2] = 0.0f;       m[0][3] = 0.0f;
    m[1][0] = 0.0f;     m[1][1] = 1.0f;   m[1][2] = 0.0f;       m[1][3] = 0.0f;
    m[2][0] = 0.0f;     m[2][1] = 0.0f;   m[2][2] = 1.0f;       m[2][3] = 0.0f;
    m[3][0] = 0.0f;     m[3][1] = 0.0f;   m[3][2] = 0.0f;       m[3][3] = 1.0f;

}

//---------------------------------------------------

void MatrixMultiply(Matrix _a, Matrix _b, Matrix& out){

    int k;
    for(int i = 0; i < 4; i++)
    for(int j = 0; j < 4; j++)
      for(k = 0, out[i][j] = 0; k < 4; k++)
        out[i][j] += _a[i][k] * _b[k][j];



}
//------------------------------------------------------------------
FlatMatrix Matrix2Flat(Matrix m){
    FlatMatrix fm;
    int index = 0;
    for (int i = 0; i<4;i++)
        for (int a = 0; a < 4; a++){
            fm.m[index] = m[i][a];
            index++;
        }
    return fm;

}

//-------------------------------------------------------
void MatrixFlat2Matrix(FlatMatrix m, Matrix& mat){


    int index = 0;
    for (int i=0; i<4; i++)
        for (int a=0; a<4; a++){
            mat[i][a] = m.m[index];
            index++;
        }


}
//-----------------------------------------------------------------
void       MatrixTranslationScale(Vector3D position,
                                  Vector3D scale,
                                  float * mat){
    memset(mat, 0, sizeof(float)*16);
    mat[0]  = scale.v[0];
    mat[5] = scale.v[1];
    mat[10] = scale.v[2];
    memcpy(&mat[12], position.v, 3 * sizeof(float));
    mat[15] = 1.0f;
}



//-----------------------------------------------------------------
void MatrixTranslation(float x, float y, float z, Matrix &mat){
    mat[0][0] = 1.0f;       mat[0][1] = 0.0f;   mat[0][2] = 0.0f;       mat[0][3] = 0.0f;
    mat[1][0] = 0.0f;       mat[1][1] = 1.0f;   mat[1][2] = 0.0f;       mat[1][3] = 0.0f;
    mat[2][0] = 0.0;        mat[2][1] = 0.0f;   mat[2][2] = 1.0f;       mat[2][3] = 0.0f;
    mat[3][0] = x;          mat[3][1] = y;      mat[3][2] = z;          mat[3][3] = 1.0f;
}
//--------------------------------------------------------------------------
void MatrixScale(float x, float y, float z, Matrix &mat){
    mat[0][0] = x;          mat[0][1] = 0.0f;   mat[0][2] = 0.0f;       mat[0][3] = 0.0f;
    mat[1][0] = 0.0f;       mat[1][1] = y;      mat[1][2] = 0.0f;       mat[1][3] = 0.0f;
    mat[2][0] = 0.0f;       mat[2][1] = 0.0f;   mat[2][2] = z;          mat[2][3] = 0.0f;
    mat[3][0] = 0.0f;       mat[3][1] = 0.0f;   mat[3][2] = 0.0f;       mat[3][3] = 1.0f;
}
//--------------------------------------------------------------------------
void MatrixRotationY(float angle, Matrix& mat){
    float a = cosf(angle);
    float b = sinf(angle);
    mat[0][0] = a;            mat[0][1]=0.0f; mat[0][2] = -b;           mat[0][3]=0.0f;
    mat[1][0] = 0.0f;         mat[1][1]=1.0f; mat[1][2] = 0.0f;         mat[1][3]=0.0f;
    mat[2][0] = b;            mat[2][1]=0.0f; mat[2][2] = a;            mat[2][3]=0.0f;
    mat[3][0] = 0.0f;         mat[3][1]=0.0f; mat[3][2] = 0.0f;         mat[3][3]=1.0f;
}
//----------------------------------------------------------------------------
void MatrixRotationY(float angle, float * mat){
    float a = cosf(angle);
    float b = sinf(angle);
    memset(mat, 0, sizeof(float) * 16);
    mat[0] = a;     mat[2] = -b;   
    mat[5] = 1.0f;  
    mat[8] = b;     mat[10] = a;   
    mat[15]=1.0f;
}
//----------------------------------------------------------------
void MatrixRotationAxis(float angle, Vector3D axis, float * mat){
    Vector3D V = axis;
    V.normalize();
    float x = V.x();
    float y = V.y();
    float z = V.z();

    float s = sin(angle);
    float c = cos(angle);
    float t = 1-c;
    memset(mat, 0, sizeof(float)*16);
    mat[0] =  t * (x * x) + c;    mat[1] = t * x * y - s * z;
    mat[2] =  t * x * z + s * y;
    mat[4] =  t * x * y + s * z;  mat[5] = t * y * y + c;
    mat[6] =  t * y * z - s * x;
    mat[8] =  t * x * z - s * y;  mat[9] = t * y * z + s * x;
    mat[10] = t * z * z + c;
    mat[15] = 1.0f;
}

//----------------------------------------------------------------
void MatrixRotationAxis(float angle, Vector3D axis, Matrix& mat){
    Vector3D V = axis;
    V.normalize();
    float x = V.x();
    float y = V.y();
    float z = V.z();


    float s= sin(angle);
    float c= cos(angle);
    float t= 1-c;
    mat[0][0]=t*(x*x)+c;    mat[0][1]=t*x*y-s*z;    mat[0][2]=t*x*z+s*y;    mat[0][3]=  0;
    mat[1][0]=t*x*y+s*z;    mat[1][1]=t*y*y+c;      mat[1][2]=t*y*z-s*x;    mat[1][3]=  0;
    mat[2][0]=t*x*z-s*y;    mat[2][1]=t*y*z+s*x;    mat[2][2]=t*z*z+c;      mat[2][3]=0;
    mat[3][0]=0;            mat[3][1]=0;            mat[3][2]=0;            mat[3][3]=1;
}
//--------------------------------------------------
void       MatrixLookAt(Vector3D pos, Vector3D target, Vector3D up, float* mat){

    Vector3D newDir = target - pos;
    newDir.normalize();
    Vector3D right = up ^ newDir;
    right.normalize();
    Vector3D newUp = newDir ^ right;


    mat[0] = -right.x(); mat[1] = newUp.x(); mat[2]  = -newDir.x(); mat[3] = 0;
    mat[4] = -right.y(); mat[5] = newUp.y(); mat[6]  = -newDir.y(); mat[7] = 0;
    mat[8] = -right.z(); mat[9] = newUp.z(); mat[10] = -newDir.z(); mat[11] = 0;
    mat[12] = (right * pos); 
    mat[13] = -(newUp * pos);
    mat[14] = (newDir * pos); 
    mat[15] = 1;
    
}
//-----------------------------------------------------

void       MatrixPerspective(float fov, float aspect, float zNear, float zFar,
                             float * mat){

    float piover360 = 0.0087266462599716478846184538424431;

    float yScale = 1.0f/(tan(fov * piover360));
    float ndepth = zNear - zFar; //negative depth

    mat[0] = yScale / aspect; mat[1] = 0; mat[2]  = 0; mat[3] = 0;
    mat[4] = 0; mat[5] = yScale; mat[6]  = 0; mat[7] = 0;

    mat[8] = 0; mat[9] = 0; mat[10] = (zFar + zNear) / ndepth; mat[11] = -1;

    mat[12] = 0; mat[13] = 0; mat[14] = 2.0f * (zNear * zFar) / ndepth; mat[15] = 0;


}
//-----------------------------------------

void       MatrixOrtho(float left, float right, float bottom, float top,
                       float near, float far, float * mat){

    float tx = (right + left) / (right - left);
    float ty = (top + bottom) / (top - bottom);
    float tz = (far + near) / (far - near);

    mat[0] = 2.0f/(right-left); mat[1] = 0;            mat[2] = 0; mat[3] = 0;
    mat[4] = 0;            mat[5] = 2.0f/(top-bottom); mat[6] = 0; mat[7] = 0;
    mat[8] = 0;            mat[9] = 0;            mat[10] = -2.0f/(far-near); mat[11] = 0;
    mat[12] = -tx;         mat[13] = -ty;           mat[14] = -tz; mat[15] = 1;

}
//------------------------------------------
Vector3D       Unproject(float x, float y, float z, 
                     float * modelview, float* projection,
                     int * viewport){

    Vector3D in, out;

    in = Vector3D((x - viewport[0]) * 2 / viewport[2] - 1.0f,
        (y - viewport[1]) * 2 / viewport[3] - 1.0f,
        2 * z - 1.0f);

    FlatMatrix mw;
    FlatMatrix proj;
    FlatMatrix m;
    memcpy(mw.m, modelview, sizeof(float) * 16);
    memcpy(proj.m, projection, sizeof(float) * 16);

    m = mw * proj;
    //puts("regular------");
    //MatrixDump(m.m);
    FlatMatrix fi;
    Matrix inv;
    MatrixInverse(m.m, fi.m);
    //puts("inverse------");
    //MatrixDump(fi.m);
    MatrixFlat2Matrix(fi, inv);
    in.transform(inv);

    if (in.v[3] != 0.0f){
        out.v[0] = in.v[0]/in.v[3];
        out.v[1] = in.v[1]/in.v[3];
        out.v[2] = in.v[2]/in.v[3];
    }

    return out; 

}



//-----------------------------------------
void       MatrixDump(Matrix& _m){
    
    printf("%2.3f %2.3f %2.3f %2.3f\n %2.3f %2.3f %2.3f %2.3f\n %2.3f %2.3f %2.3f %2.3f\n %2.3f %2.3f %2.3f %2.3f\n\n",
           _m[0][0], _m[0][1], _m[0][2], _m[0][3],
           _m[1][0], _m[1][1], _m[1][2], _m[1][3],
           _m[2][0], _m[2][1], _m[2][2], _m[2][3],
           _m[3][0], _m[3][1], _m[3][2], _m[3][3]
    );

}
//-----------------------------------------
void       MatrixDump(float * _m){
    
    printf("%2.3f %2.3f %2.3f %2.3f\n %2.3f %2.3f %2.3f %2.3f\n %2.3f %2.3f %2.3f %2.3f\n %2.3f %2.3f %2.3f %2.3f\n\n",
           _m[0],  _m[1],  _m[2],  _m[3],
           _m[4],  _m[5],  _m[6],  _m[7],
           _m[8],  _m[9],  _m[10], _m[11],
           _m[12], _m[13], _m[14], _m[15]
    );

}

//---------------------------------------------------------
void       Quat2Matrix(float w, float x, float y, float z, Matrix & mat){
    

    float xx    = x * x;
    float xy    = x * y;
    float xz    = x * z;
    float xw    = x * w;

    float yy      = y * y;
    float yz      = y * z;
    float yw      = y * w;

    float zz      = z * z;
    float zw      = z * w;

    mat[0][0]  = 1 - 2 * ( yy + zz );
    mat[0][1]  =     2 * ( xy - zw );
    mat[0][2]  =     2 * ( xz + yw );

    mat[1][0]  =     2 * ( xy + zw );
    mat[1][1]  = 1 - 2 * ( xx + zz );
    mat[1][2]  =     2 * ( yz - xw );

    mat[2][0]  =     2 * ( xz - yw );
    mat[2][1]  =     2 * ( yz + xw );
    mat[2][2] = 1 - 2 * ( xx + yy );

    mat[0][3]  = mat[1][3] = mat[2][3] = mat[3][0] = mat[3][1] = mat[3][2] = 0;
    mat[3][3] = 1;


}



//------------------------------------------
//Actualy it does not work quite well...
void      MatrixInverse(Matrix & in, Matrix & out){

    out[0][0] = in[0][0]; out[0][1] = in[1][0]; out[0][2] = in[2][0]; out[0][3] = 0;
    out[1][0] = in[0][1]; out[1][1] = in[1][1]; out[1][2] = in[2][1]; out[1][3] = 0;
    out[2][0] = in[0][2]; out[2][1] = in[1][2]; out[2][2] = in[2][2]; out[2][3] = 0;
    Vector3D a = Vector3D(in[0][0], in[0][1], in[0][2]);
    Vector3D b = Vector3D(in[1][0], in[1][1], in[1][2]);
    Vector3D c = Vector3D(in[2][0], in[2][1], in[2][2]);
    Vector3D pos = Vector3D(in[3][0], in[3][1], in[3][2]);
    out[3][0] = -(a * pos);
    out[3][1] = -(b * pos);
    out[3][2] = -(c * pos);
    out[3][3] = 1;

}

//-----------------------------------------
//kindof works
//This function is based on Cramer's rule
void MatrixInverse(float * mat , float * dst){
    float   tmp[12]; // temp array for pairs
    float   src[16]; // array of transpose source matrix 
    // transpose matrix
    for (int i = 0; i < 4; i++) {
        src[i]      = mat[i * 4];
        src[i + 4]  = mat[i * 4 + 1];
        src[i + 8]  = mat[i * 4 + 2];
        src[i + 12] = mat[i * 4 + 3];
    }
    //calculate pairs for first 8 elements (cofactors)
    tmp[0]  =   src[10] * src[15];
    tmp[1]  =   src[11] * src[14];
    tmp[2]  =   src[9]  * src[15];
    tmp[3]  =   src[11] * src[13];
    tmp[4]  =   src[9]  * src[14];
    tmp[5]  =   src[10] * src[13];
    tmp[6]  =   src[8]  * src[15];
    tmp[7]  =   src[11] * src[12];
    tmp[8]  =   src[8]  * src[14];
    tmp[9]  =   src[10] * src[12];
    tmp[10] =   src[8]  * src[13];
    tmp[11] =   src[9]  * src[12];
    // calculate first 8 elements (cofactors)
    dst[0]  =   tmp[0]*src[5]   +  tmp[3]*src[6]   + tmp[4]*src[7];
    dst[0] -=   tmp[1]*src[5]   +  tmp[2]*src[6]   + tmp[5]*src[7];
    dst[1]  =   tmp[1]*src[4]   +  tmp[6]*src[6]   + tmp[9]*src[7];
    dst[1] -=   tmp[0]*src[4]   +  tmp[7]*src[6]   + tmp[8]*src[7];
    dst[2]  =   tmp[2]*src[4]   +  tmp[7]*src[5]   + tmp[10]*src[7];
    dst[2] -=   tmp[3]*src[4]   +  tmp[6]*src[5]   + tmp[11]*src[7];
    dst[3]  =   tmp[5]*src[4]   +  tmp[8]*src[5]   + tmp[11]*src[6];
    dst[3] -=   tmp[4]*src[4]   +  tmp[9]*src[5]   + tmp[10]*src[6];
    dst[4]  =   tmp[1]*src[1]   +  tmp[2]*src[2]   + tmp[5]*src[3];
    dst[4] -=   tmp[0]*src[1]   +  tmp[3]*src[2]   + tmp[4]*src[3];
    dst[5]  =   tmp[0]*src[0]   +  tmp[7]*src[2]   + tmp[8]*src[3];
    dst[5] -=   tmp[1]*src[0]   +  tmp[6]*src[2]   + tmp[9]*src[3];
    dst[6]  =   tmp[3]*src[0]   +  tmp[6]*src[1]   + tmp[11]*src[3];
    dst[6] -=   tmp[2]*src[0]   +  tmp[7]*src[1]   + tmp[10]*src[3];
    dst[7]  =   tmp[4]*src[0]   +  tmp[9]*src[1]   + tmp[10]*src[2];
    dst[7] -=   tmp[5]*src[0]   +  tmp[8]*src[1]   + tmp[11]*src[2];
    //calculate pairs for second 8 elements (cofactors) 
    tmp[0]   =   src[2] * src[7];
    tmp[1]   =   src[3] * src[6];
    tmp[2]   =   src[1] * src[7];
    tmp[3]   =   src[3] * src[5];
    tmp[4]   =   src[1] * src[6];
    tmp[5]   =   src[2] * src[5];
    tmp[6]   =   src[0] * src[7];
    tmp[7]   =   src[3] * src[4];
    tmp[8]   =   src[0] * src[6];
    tmp[9]   =   src[2] * src[4];
    tmp[10]  =   src[0] * src[5];
    tmp[11]  =   src[1] * src[4];
    // calculate second 8 elements (cofactors) */
    dst[8] =          tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15];
    dst[8] -=         tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15];
    dst[9] =          tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15];
    dst[9] -=         tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15];
    dst[10] =         tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15];
    dst[10]-=         tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15];
    dst[11] =         tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14];
    dst[11]-=         tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14];
    dst[12] =         tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9];
    dst[12]-=         tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10];
    dst[13] =         tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10];
    dst[13]-=         tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8];
    dst[14] =         tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8];
    dst[14]-=         tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9];
    dst[15] =         tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9];
    dst[15]-=         tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8];
    // calculate determinant 
    float det = src[0]*dst[0]+src[1]*dst[1]+src[2]*dst[2]+src[3]*dst[3];
    // calculate matrix inverse 
    det = 1/det;
    for (int j = 0; j < 16; j++)
       dst[j] *= det;

}

