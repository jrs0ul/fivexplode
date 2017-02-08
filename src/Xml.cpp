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

#include <cstring>
#include <cwchar>
#include <clocale>
#include <cerrno>
#include "Xml.h"
#include "Utils.h"

    
    //------------------------------------------------------
    void CString::set(const wchar_t * t){
        destroy();
        if (t){
            int len = (int)wcslen(t);
            if (len > 0){
                text = (wchar_t *)malloc((len + 1)*sizeof(wchar_t));
                wcsncpy(text, t, len + 1);
            }
        }
    }
    //------------------------------------------------------
    void CString::destroy(){
    
        if (text){
            free(text);
            //puts("freed");
            text = 0;
        }
        
    }
    //======================================================
    XmlAttribute::XmlAttribute(const wchar_t * _name, const wchar_t * _value){
        
        value = 0;
        name = 0;
        int nameLen = (int)wcslen(_name);
        int valueLen = (int)wcslen(_value);
        if (nameLen > 0){
            name = (wchar_t *)malloc((nameLen + 1)*sizeof(wchar_t));
            wcscpy(name, _name);
        }
        if (valueLen > 0){
            value = (wchar_t *)malloc((valueLen + 1)*sizeof(wchar_t));
            wcscpy(value, _value);
        }
    }
    //------------------------------------------------------
    void XmlAttribute::set(const wchar_t * _name, const wchar_t * _value){
        destroy();
        int nameLen = (int)wcslen(_name);
        int valueLen = (int)wcslen(_value);

        if (nameLen > 0){
            name = (wchar_t *)malloc((nameLen + 1)*sizeof(wchar_t));
            wcsncpy(name, _name, nameLen + 1);
        }
        if (valueLen > 0){
            value = (wchar_t *)malloc((valueLen + 1)*sizeof(wchar_t));
            wcsncpy(value, _value, valueLen + 1);
        }

    }
    //------------------------------------------------------
    void XmlAttribute::destroy(){
        if (value){
            free(value);
            value = 0;
        }

        if (name){
            free(name);
            name = 0;
        }
    }
    //------------------------------------------------
    void XmlAttribute::setValue(const wchar_t* _value){
        if (value){
            free(value);
            value = 0;
        }
        
        int valueLen = (int)wcslen(_value);
        if (valueLen > 0) {
            value = (wchar_t *)malloc((valueLen + 1) * sizeof(wchar_t));
            wcsncpy(value, _value, valueLen + 1);
        }
    }
    
    //------------------------------------------------------
    void XmlAttribute::setName(const wchar_t* _name){
        if (name){

            free(name);
            name = 0;
        }
        
        int nameLen = (int)wcslen(_name);
        if (nameLen > 0){
            name = (wchar_t*)malloc((nameLen + 1) * sizeof(wchar_t));
            wcsncpy(name, _name, nameLen + 1);
        }

    }

    //======================================================

    void XmlNode::setValue(const wchar_t * _value){
        if (value){

            free(value);
            value = 0;
        }
        
        int valueLen = (int)wcslen(_value);
        if (valueLen > 0){
            value = (wchar_t *)malloc((valueLen + 1)*sizeof(wchar_t));
            wcsncpy(value, _value, valueLen + 1);
        }
    }
    
    //------------------------------------------------------
    void XmlNode::setName(const wchar_t * _name){
        if (name){
            free(name);
            name = 0;
        }
        
        int nameLen = (int)wcslen(_name);
        if (nameLen > 0){
            name = (wchar_t *)malloc((nameLen + 1)*sizeof(wchar_t));
            wcscpy(name, _name);
        }

    }
    //------------------------------------------------------
    void XmlNode::addAtribute(const wchar_t * _name, const wchar_t * _value){
        XmlAttribute newattr(_name, _value);
        attributes.add(newattr);
    }
    //---------------------------------------------------
    XmlAttribute * XmlNode::getAttribute(unsigned long index){
        if (index < attributes.count())
            return &attributes[index];
        return 0;
    }
    //------------------------------------------------------
    void XmlNode::addChild(XmlNode & node){
        node.parent = this;
        children.add(node);
    }  
    //------------------------------------------------------
    XmlNode * XmlNode::getNode(unsigned long index){
        if (index < children.count()){
            return &children[index];
        }
        return 0;
    }
    //------------------------------------------------------
    XmlNode * XmlNode::getNode(const wchar_t * _name){
        for (unsigned long i = 0; i < children.count(); i++){
            if (wcscmp(_name, children[i].getName()) == 0)
                return getNode(i);
        }
        return 0;
}
//--------------------------------------------------
bool XmlNode::writeAttributes(FILE * f){
    for (unsigned long i = 0; i < attributes.count(); i++){
        XmlAttribute * a = getAttribute(i);
        char * buf = (char*)malloc(wcslen(a->name) + wcslen(a->value) + 10);
        sprintf(buf, " %ls=\"%ls\"", a->name, a->value);
        int res = fwrite(buf, 1, strlen(buf), f);
        free(buf);
        if (!res)
            return false;
    }
    return true;
}
//----------------------------------------------------
bool XmlNode::write(FILE *f, unsigned depth){

        //TODO: Write tag arguments
        //TODO: UTF8

        bool hasvalue = false;
        if (value){
            if (wcslen(value) > 0)
                hasvalue = true;
        }
        bool hasname = false;
        if (name){
            if (wcslen(name) > 0)
                hasname = true;
        }
        bool haschildren = (children.count()) ? true : false;

        wchar_t * indent = 0;
        if (depth){
            indent = (wchar_t*)malloc(sizeof(wchar_t) * 6 * depth);
            for (unsigned i = 0; i < depth * 4; i++){
                indent[i] = L' ';
            }
            indent[depth * 4] = 0;
        }
        
        if ((hasvalue) || (haschildren)){
            wchar_t* buf = 0;
            char mbuf[258];
            if (hasname){
                
                int len = (wcslen(indent) + wcslen(name) + 5);
                buf = (wchar_t*)malloc(sizeof(wchar_t) * len);
               
                
                swprintf(buf, 
                #ifndef __MINGW32__ 
                len, 
                #endif
                L"%ls<%ls", indent, name);
               
                wcstombs(mbuf, buf, 256);
                free(buf);
                buf = 0;
                int size = (int)strlen(mbuf);
                if (!fwrite(mbuf, 1, size, f))
                    return false;
                
                writeAttributes(f);
                
                if (haschildren){
                    fwrite(" >\n", 1, 3, f);
                }
                else{
                    fwrite(" >", 1, 2, f);
                }
                
            }
            if (haschildren){
                //puts("write children");
                for (unsigned long i = 0; i < children.count(); i++){
                    children[i].write(f, depth + 1);
                }
            }
            else{
                int len = wcslen(value) + 1;
                buf = (wchar_t*)malloc(sizeof(wchar_t) * len);
                swprintf(buf, 
                        #ifndef __MINGW32__ 
                        len,
                        #endif
                        L"%ls", value);
                wcstombs(mbuf, buf, 256);
                free(buf);
                buf = 0;
                fwrite(mbuf, 1, strlen(mbuf), f);
                //puts(mbuf);
            } 
            if (hasname){
                int len = 0;
                len = (wcslen(indent) + wcslen(name) + 5);
               
                buf = (wchar_t*)malloc(sizeof(wchar_t) * len);
                if (haschildren){
                  
                    swprintf(buf,
                    #ifndef __MINGW32__ 
                    len,
                    #endif
                    L"%ls</%ls>\n", indent, name);
                   
                }
                else{
                    swprintf(buf,
                    #ifndef __MINGW32__ 
                    len,
                    #endif
                    L"</%ls>\n", name);
                }
                wcstombs(mbuf, buf, 256);
                free(buf);
                buf = 0;
                fwrite(mbuf, 1, strlen(mbuf), f);
            }

        }//has value or children
        else{
            wchar_t * buf = 0;
            char mbuf[1024];
            if (hasname){
                int len = 0;
                len = (wcslen(indent) + wcslen(name) + 5);
                buf = (wchar_t*)malloc(sizeof(wchar_t) * len);
                
                swprintf(buf,
                #ifndef __MINGW32__
                         len,
                        #endif
                             L"%ls<%ls", indent, name);
               
                wcstombs(mbuf, buf, 256);
                free(buf);
                buf = 0;
                fwrite(mbuf, 1, strlen(mbuf), f);
                
                writeAttributes(f);
                
                fwrite(" />\n", 1, 4, f);
            }

        }

        if (indent){
            free(indent);
            indent = 0;
        }

        return true;


    }
    //------------------------------------------------------
    void XmlNode::destroy(){

        //puts("free value");
        if (value){
            //printf("%ls\n",value);
            free(value);
            value = 0;
        }
       // else puts("fail");


        //puts("free name");
        if (name){
            //puts("ok");
            free(name);
            name = 0;
        }


        for (unsigned long i = 0; i < attributes.count(); i++){
            attributes[i].destroy();
        }
        attributes.destroy();

        for (unsigned long i = 0; i < children.count(); i++){
           // puts("----child------");
            children[i].destroy();
        }
        children.destroy();
    }
    //-----------------------------------------------------
      bool Xml::load(const char* filename){

        char*  mbuf = 0;
        if (!ReadFileData(filename, &mbuf))
            return false;

        wchar_t * buff = 0;
        size_t mbuflen = 0;
        if (mbuf)
            mbuflen = strlen(mbuf);
        if (mbuflen <= 0){
            free(mbuf);
            return false;
        }

        buff = (wchar_t *)malloc((mbuflen + 1) * sizeof(wchar_t));
        if (mbuf){
            UTF8toWchar(mbuf, buff);
            //mbstowcs(buff, mbuf, mbuflen+1);
            free(mbuf);
        }
        parse(buff, true);

        return true;

      }


//-------------------------------------------------
    void Xml::analizeFirstTagSplit(CString & element, 
                                   bool& isClosingTag,
                                   bool& isSingletonTag,
                                   CString& NAME, XmlNode& attributeBox){

        int len = wcslen(element.get()) + 1;
        if (len > 0){
            wchar_t * firstpart = 0;
            firstpart = (wchar_t *)malloc(len * sizeof(wchar_t));
            wcsncpy(firstpart, element.get(), len);
            wchar_t * tt = 0;
            wchar_t * stt = 0;

            tt = _wcstok(firstpart, L" \t", &stt);
            if (tt){
                if (tt[0] == L'/'){
                    memmove(tt, tt+1, (wcslen(tt)) * sizeof(wchar_t));
                    isClosingTag = true;
                }

                NAME.set(tt);
                tt = _wcstok(0, L" \t", &stt);

                if (tt){
                    //get first attribute name
                    attributeBox.addAtribute(tt, L"");
                }
                else{
                    const wchar_t * nameval = 0;
                    nameval = NAME.get();
                    if (nameval){
                        if (nameval[wcslen(nameval) - 1] == L'/'){
                            wchar_t * newname = 0;
                            int len = wcslen(nameval) + 1;
                            if (len > 0){
                                newname = (wchar_t *)malloc(len * sizeof(wchar_t));
                                wcsncpy(newname, nameval, len);
                                newname[wcslen(newname) - 1] = 0;
                                NAME.set(newname);
                                isSingletonTag = true;
                                free(newname);
                            }
                        }
                    }
                }
                free(firstpart);
            }
        }
    }
    //--------------------------------------
    void Xml::analizeTagSplit(CString & element, 
                              XmlNode& attributeBox){

        wchar_t * part = 0;
        int len = wcslen(element.get()) + 1;
        if (len > 0){
            part = (wchar_t *)malloc(len * sizeof(wchar_t));
            wcscpy(part, element.get());

            wchar_t * at = 0; 
            wchar_t * stt = 0;

            at = _wcstok(part, L"\" \t", &stt);
            if (at){
                XmlAttribute * a = 0;
                a = attributeBox.getAttribute(attributeBox.attributeCount()-1);
                a ->setValue(at);
                at = _wcstok(0, L"\" \t", &stt);
                if (at){
                   attributeBox.addAtribute(at, L"");
                }
            }
            free(part);
        }
    }
//---------------------------------------------

    void Xml::analizeLastTagSplit(CString & element, 
                                 /* bool& isClosingTag,
                                  bool& isSingletonTag,
                                  CString& NAME, */XmlNode& attributeBox){

        int len = wcslen(element.get()) + 1;
        if (len > 0){
            wchar_t * lastpart = 0;
            lastpart = (wchar_t*)malloc(len * sizeof(wchar_t));
            wcsncpy(lastpart, element.get(), len);
            wchar_t * tt = 0;
            wchar_t * stt = 0;
            tt = _wcstok(lastpart, L"\" \t", &stt);

            if (tt){
                //TODO: ....
                attributeBox.getAttribute(attributeBox.attributeCount()-1)->setValue(tt);
            }
            free(lastpart);
        }

    }

//--------------------------------------------------
    bool Xml::parseLine(const wchar_t * line,
                        XmlNode& attributeBox,
                        XmlNode** currentNode){

        wchar_t * str = 0;
        int len = 0;
        if (line)
            len = wcslen(line);
        if(len > 0){
            str = (wchar_t *)malloc((len + 1)  * sizeof(wchar_t));
            wcsncpy(str, line, len + 1);
        }
        //printf("%ls\n", str);

        //get value
        wchar_t* tmp = 0;
        wchar_t* ptmp = 0;
        if (str){
            tmp = _wcstok(str, L"<", &ptmp);
        }

        if (tmp){
            CString VALUE;
            VALUE.set(tmp);
            //get tag
            tmp = _wcstok(0, L"<", &ptmp);

            if (tmp){ 
                CString NAME;
                NAME.set(tmp);
                //backup tag
                wchar_t * tagline = 0;
                int _len = wcslen(tmp);
                if (_len > 0){
                    tagline = (wchar_t *)malloc((_len + 1) * sizeof(wchar_t));
                    wcsncpy(tagline, tmp, _len + 1);
                    //try to parse tag attributes
                    DArray<CString> tagsplit;
                    wchar_t * atr = 0;
                    wchar_t * patr = 0;

                    atr = _wcstok(tagline, L"=", &patr);
                    while (atr){
                        CString ts;
                        ts.set(atr);
                        tagsplit.add(ts);
                        atr = _wcstok(0, L"=", &patr);
                    }

                    bool isClosingTag = false;
                    bool isSingletonTag = false;


                    //analize and destroy array
                    for (unsigned long a = 0; a < tagsplit.count(); a++){

                        //analise first part
                        if (a == 0){
                            analizeFirstTagSplit(tagsplit[a], isClosingTag, isSingletonTag, NAME, attributeBox);
                        }
                        //end of firstpart
                        else{
                            if (a != tagsplit.count() - 1 ){
                                //parse the rest of attributes
                                analizeTagSplit(tagsplit[a], attributeBox);
                            }
                            //analise last part
                            if ((a == tagsplit.count() - 1) && (tagsplit.count() > 1)){
                                analizeLastTagSplit(tagsplit[a], /*isClosingTag, isSingletonTag, NAME,*/ attributeBox);
                            }
                        }
                        tagsplit[a].destroy();
                    }
                    tagsplit.destroy();
                    //let's detect tag type 
                    wchar_t * t = 0;
                    t = wcschr(tmp, L'/');
                    if (isClosingTag){
                        (*currentNode)->setValue(VALUE.get());
                        (*currentNode) = (*currentNode)->getParent();
                    }
                    else{
                        if (t){ //single
                            XmlNode newn;
                            (*currentNode)->addChild(newn);
                            for (unsigned long z = 0; z < attributeBox.attributeCount(); z++){
                                (*currentNode)->getLatestChild()->addAtribute(attributeBox.getAttribute(z)->getName(),
                                                                              attributeBox.getAttribute(z)->getValue());
                            }
                            attributeBox.destroy();
                            (*currentNode)->getLatestChild()->setValue(VALUE.get());
                            (*currentNode)->getLatestChild()->setName(NAME.get());
                        } 
                        else{ 
                    
                            if ((tmp[0] != L'?')&&(tmp[0] != L'!')){ //xml trash
                                //opening
                                XmlNode newn;
                                (*currentNode)->addChild(newn);
                                for (unsigned long z = 0; z < attributeBox.attributeCount(); z++){
                                    (*currentNode)->getLatestChild()->addAtribute(attributeBox.getAttribute(z)->getName(),
                                                                                  attributeBox.getAttribute(z)->getValue());
                                }
                                (*currentNode) = (*currentNode)->getLatestChild();
                                (*currentNode)->setValue(VALUE.get());
                                (*currentNode)->setName(NAME.get());
                            }

                            attributeBox.destroy();
                        }
                    }

                    free(tagline);
                    NAME.destroy();

                }
                VALUE.destroy();
            } //_len > 0
            VALUE.destroy();
        } //if (tmp)
            if (str)
                free(str);
        return true;

    }

//--------------------------------------------------
    bool Xml::parse(wchar_t * buff, bool freebuf){
        
        wchar_t * p = 0;

        DArray<CString> items;

        wchar_t * crap = 0;
        if (buff){
            p = _wcstok(buff, L">", &crap);
        
            while (p){

                if (wcslen(p) > 0){

                    CString s;
                    s.set(p);
                    items.add(s);
                }
                p = _wcstok(0, L">", &crap);
            }
            if (freebuf)
                free(buff);
        }

        //-------------------for each line---------------------------
        //TODO: check closing tags for match
        //TODO: what if tags has no value
    
        XmlNode * currentNode = &root;
        //node to collect attributes
        XmlNode attributeBox;

        for(unsigned i = 0; i< items.count(); i++){
            parseLine(items[i].get(), attributeBox, &currentNode);
        } 
        //------------------------------------
        //puts("destroy items");
        for (unsigned long i = 0; i < items.count(); i++)
            items[i].destroy();
        items.destroy();

        //puts("XML DONE");

        return true;
    }
    

    //------------------------------------------------------
    bool Xml::write(const char * filename){

        FILE * f = 0;

        f = fopen(filename, "wb+");
        if (!f)
            return false;

        char header[] = "<?xml version=\"1.0\"?>\n";
        fwrite(header, 1, strlen(header), f);
        root.write(f, 0);

        fclose(f);

        return true;
    }

    //------------------------------------------------------
    void Xml::destroy(){

        root.destroy();

    }


