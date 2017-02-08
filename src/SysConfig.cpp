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
#include "SysConfig.h"
#include <cwchar>
#include <cstdio>
#include "Xml.h"


    bool SystemConfig::load(const char * config){
        
        Xml conf;


        if (!conf.load(config))
            return false;

        XmlNode * settings = 0;
        settings = conf.root.getNode(L"Settings");
        if (settings){

            wchar_t tmp[255];
            char ctmp[255];

            XmlNode * nod = 0;
            nod = settings->getNode(L"MusicVolume");
            if (nod){
                wcscpy(tmp, nod->getValue());
                wcstombs(ctmp, tmp, 100);
                musicVolume = atof(ctmp);
            }
            nod = 0;
            
            nod = settings->getNode(L"SfxVolume");
            if (nod){
                wcscpy(tmp, nod->getValue());
                wcstombs(ctmp, tmp, 100);
                sfxVolume = atof(ctmp);
            }
            nod = 0;

            nod = settings->getNode(L"Accelerometer");
            if (nod){
                wcscpy(tmp, nod->getValue());
                wcstombs(ctmp, tmp, 100);
                accelerometerMultiplier = atof(ctmp);
            }
            nod = 0;
            nod = settings->getNode(L"HighscoreName");
            if (nod){
                wcscpy(tmp, nod->getValue());
                wcstombs(nameToEnter, tmp, 100);
            }
            nod = 0;
            
            nod = settings->getNode(L"Joystick");
            if (nod){
                wcscpy(tmp, nod->getAttribute(0)->getValue());
                wcstombs(ctmp, tmp, 100);
                joystick.v[0] = atof(ctmp);
                wcscpy(tmp, nod->getAttribute(1)->getValue());
                wcstombs(ctmp, tmp, 100);
                joystick.v[1] = atof(ctmp);
            }
            nod = 0;
            nod = settings->getNode(L"Bomb");
            if (nod){
                wcscpy(tmp, nod->getAttribute(0)->getValue());
                wcstombs(ctmp, tmp, 100);
                bombButton.v[0] = atof(ctmp);
                wcscpy(tmp, nod->getAttribute(1)->getValue());
                wcstombs(ctmp, tmp, 100);
                bombButton.v[1] = atof(ctmp);
            }
            nod = 0;
            nod = settings->getNode(L"Jump");
            if (nod){
                wcscpy(tmp, nod->getAttribute(0)->getValue());
                wcstombs(ctmp, tmp, 100);
                jumpButton.v[0] = atof(ctmp);
                wcscpy(tmp, nod->getAttribute(1)->getValue());
                wcstombs(ctmp, tmp, 100);
                jumpButton.v[1] = atof(ctmp);
            }
            nod = 0;
            nod = settings->getNode(L"Pause");
            if (nod){
                wcscpy(tmp, nod->getAttribute(0)->getValue());
                wcstombs(ctmp, tmp, 100);
                pauseButton.v[0] = atof(ctmp);
                wcscpy(tmp, nod->getAttribute(1)->getValue());
                wcstombs(ctmp, tmp, 100);
                pauseButton.v[1] = atof(ctmp);
            }
            
        }

        conf.destroy();

        return true;
    }
//------------------------------------------
    bool SystemConfig::write(const char * config){

        char buf[255];
        wchar_t wbuf[255];
        Xml conf;

        XmlNode Settings;
        Settings.setName(L"Settings");
        Settings.setValue(L"\n");

        XmlNode _width;
        sprintf(buf, "%f", musicVolume);
        mbstowcs(wbuf, buf, 255);
        _width.setName(L"MusicVolume");
        _width.setValue(wbuf);
        Settings.addChild(_width);
        
        XmlNode _sfxVolume;
        sprintf(buf, "%f", sfxVolume);
        mbstowcs(wbuf, buf, 255);
        _sfxVolume.setName(L"SfxVolume");
        _sfxVolume.setValue(wbuf);
        Settings.addChild(_sfxVolume);

        XmlNode _height;
        sprintf(buf, "%f", accelerometerMultiplier);
        mbstowcs(wbuf, buf, 255);
        _height.setName(L"Accelerometer");
        _height.setValue(wbuf);
        Settings.addChild(_height);
        
        XmlNode _name;
        mbstowcs(wbuf, nameToEnter, 255);
        _name.setName(L"HighscoreName");
        _name.setValue(wbuf);
        Settings.addChild(_name);
        
        XmlNode _joystick;
        _joystick.setName(L"Joystick");
        sprintf(buf, "%f", joystick.v[0]);
        mbstowcs(wbuf, buf, 255);
        _joystick.addAtribute(L"x", wbuf);
        sprintf(buf, "%f", joystick.v[1]);
        mbstowcs(wbuf, buf, 255);
        _joystick.addAtribute(L"y", wbuf);
        Settings.addChild(_joystick);
        
        XmlNode _bomb;
        _bomb.setName(L"Bomb");
        sprintf(buf, "%f", bombButton.v[0]);
        mbstowcs(wbuf, buf, 255);
        _bomb.addAtribute(L"x", wbuf);
        sprintf(buf, "%f", bombButton.v[1]);
        mbstowcs(wbuf, buf, 255);
        _bomb.addAtribute(L"y", wbuf);
        Settings.addChild(_bomb);
        
        
        XmlNode _jump;
        _jump.setName(L"Jump");
        sprintf(buf, "%f", jumpButton.v[0]);
        mbstowcs(wbuf, buf, 255);
        _jump.addAtribute(L"x", wbuf);
        sprintf(buf, "%f", jumpButton.v[1]);
        mbstowcs(wbuf, buf, 255);
        _jump.addAtribute(L"y", wbuf);
        Settings.addChild(_jump);
        
        XmlNode _pause;
        _pause.setName(L"Pause");
        sprintf(buf, "%f", pauseButton.v[0]);
        mbstowcs(wbuf, buf, 255);
        _pause.addAtribute(L"x", wbuf);
        sprintf(buf, "%f", pauseButton.v[1]);
        mbstowcs(wbuf, buf, 255);
        _pause.addAtribute(L"y", wbuf);
        Settings.addChild(_pause);

      
       conf.root.addChild(Settings);


        if (!conf.write(config)){
            conf.destroy();
            return false;
        
        }
        conf.destroy();
        return true;
    }

