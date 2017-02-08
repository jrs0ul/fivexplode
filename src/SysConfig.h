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
#ifndef _SYSTEMCONFIG_H
#define _SYSTEMCONFIG_H
#include "Vectors.h"
#include "externals.h"
#include <cstring>

    class SystemConfig{


    public:
        float musicVolume;
        float sfxVolume;
        float accelerometerMultiplier;
        char nameToEnter[100];
        
        Vector3D jumpButton;
        Vector3D bombButton;
        Vector3D pauseButton;
        Vector3D joystick;


        SystemConfig(){
            musicVolume = 0.2f;
            sfxVolume = 1.0f;
            accelerometerMultiplier = 0.5f;
            strcpy(nameToEnter, "Player");
            joystick = Vector3D(70, 250, 0);

        }


        bool load(const char * config);
        bool write(const char * config);

    };



#endif //SYSTEMCONFIG_H
