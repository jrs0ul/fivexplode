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

#ifndef _XML_H
#define _XML_H

#include "DArray.h"


class CString{
    wchar_t * text;
public:
    void destroy();
    void set(const wchar_t * t);
    const wchar_t * get(){ return (text) ? text : 0;}
    CString(){
        text = 0;
    }
};

class XmlNode;

class XmlAttribute{
    wchar_t * name;
    wchar_t * value;
public:
              XmlAttribute(){
                name = 0;
                value = 0;
              }
              XmlAttribute(const wchar_t * _name, const wchar_t * _value);
    void      set(const wchar_t * _name, const wchar_t * _value);
    void      setName(const wchar_t * _name);
    void      setValue(const wchar_t * _value);
    wchar_t * getValue(){return (value) ? value : 0;}
    wchar_t * getName(){return (name) ? name : 0;}
    void      destroy();

    friend class XmlNode;
};
//----------------------
class Xml;
//-----------------------------------------
class XmlNode{
    DArray<XmlAttribute> attributes;
    wchar_t *            name;
    wchar_t *            value;
    XmlNode *            parent;
    DArray<XmlNode>      children;

    bool                 write(FILE * f, unsigned depth);
    bool                 writeAttributes(FILE * f);

public:
    //adds attribute to attributes array, like id="1" in <tag id="1">
    void           addAtribute(const wchar_t* _name, const wchar_t* _value);
    //sets tagged value
    void           setValue(const wchar_t * _value);
    //sets tag name
    void           setName(const wchar_t * _name);
    //adds some tagged tag
    void           addChild(XmlNode & node);

    //gets tagged value
    const wchar_t *   getValue(){return (value) ? value : 0;}
    //gets tag name
    const wchar_t *   getName(){return (name) ? name : 0;}

    XmlNode *      getParent(){return parent;}
    XmlNode *      getLatestChild(){return &children[children.count()-1];}

    //gets node by index, if node doesn't exist 0 is returned.
    XmlNode *      getNode(unsigned long index);
    //gets node by name, if there isn't such node, 0 is returned.
    XmlNode *      getNode(const wchar_t * _name);

    //gets tag attribute by index
    XmlAttribute * getAttribute(unsigned long index);
    //get tag atribute by name
    //XmlAttribute * getAttribute(const wchar_t * _name);


    unsigned long  childrenCount(){return children.count();}
    unsigned long  attributeCount(){return attributes.count();}
    void           destroy();

                   XmlNode(){
                     value = 0;
                     name = 0;
                     parent = 0;
                  }

    friend class Xml;
};
//---------------------------------------

class Xml{
    
    bool parseLine(const wchar_t * line, XmlNode& attributeBox,
                   XmlNode** currentNode);
    void analizeFirstTagSplit(CString & element,
                          bool & isClosingTag, bool & isSingleTonTag,
                          CString & NAME, XmlNode& attributeBox);
    void analizeLastTagSplit(CString & element,
                          /*bool & isClosingTag, 
                          bool & isSingleTonTag,
                          CString & NAME,*/ XmlNode& attributeBox);
    void analizeTagSplit(CString & element,
                         XmlNode& attributeBox);
public:
    XmlNode root;

    bool parse(wchar_t * buff, bool freebuff = false);
    bool load(const char * filename);
    bool write(const char * filename);
    void destroy();

};


#endif //_XML_H
