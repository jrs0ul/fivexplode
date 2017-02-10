#include <wchar.h>
#include "CppSingleton.h"
#include "Utils.h"
#include "gui/Text.h"

#include <sys/stat.h>



void Singleton::setMusicVolume(float vol){
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
    MusicVolume = vol;
    ChangeVolume =true;
#else
    music.setVolume(vol);
#endif
}
//------------------------
void Singleton::playMusic(){
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
    PlayMusic = true;
#else
    music.playback();
#endif
}
//---------------------
void Singleton::playNewSong(const char * path){
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
    PlayNewSong = true;
    sprintf(songName, "data/%s.m4a", path);
#else
    music.stop();
    music.release();
    sprintf(songName, "data/%s.ogg", path);
    music.open(songName);
    music.playback();
#endif
    
}

//---------------------

void Singleton::init(){
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
    glViewport(0, 0, 320, 480);
    glOrthof(0.0, (GLfloat) 320, (GLfloat) 480, 0.0, 400, -400);
#else
    glOrtho(0.0, (GLfloat) 320, (GLfloat) 480, 0.0, 400, -400);
#endif
    
    pics.initBuffers();
        
    if (!pics.load("data/pics/list.txt"))
        puts("can't find list");
    
    ss.init(0);
    //Vector3D p(0,0,0);
    //ss.setupListener(p.v, p.v);
    ss.loadFiles("data/sfx/", "audio.txt");
#if !(TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
    music.open("data/music.ogg");
#endif
    
   
#if !(TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
    char letters[] = "1234567890QWERTYUIOPASDFGHJKL:ZXCVBNM@$&*-/\\!#?_=() .,";
    Keyboard.setChars((const unsigned char*)letters);
    Keyboard.setCharWidth(40);
    Keyboard.setRowLen(6);
    Keyboard.setpos(30, 60);
#endif
    
    
    
    char megabuf[255];
    sprintf(megabuf, "%s/score.dat", DocumentPath);
    highscores.load(megabuf, MAXSCORE);
    sprintf(megabuf, "%s/settings.cfg", DocumentPath);


   
    if (!sys.load(megabuf)){

    
    }

    changeName_button.set(150, 415, 170, 80);

    int menuY = 110;
    int menuH = 45;
    int menuS = 4;
    resume_button.set(   70, menuY,                     180, menuH);
    play_button.set(     70, menuY + menuH + menuS,     180, menuH);
    highscore_button.set(70, menuY + 2*(menuH + menuS), 180, menuH);
    options_button.set(  70, menuY + 3*(menuH + menuS), 180, menuH);
    howtoplay_button.set(70, menuY + 4*(menuH + menuS), 180, menuH);

    helpBack_button.set(5,5,60,40);

    onlineScore_button.set(75, 0, 120, 40);
    localScore_button.set(195, 0, 120, 40);
    localScore_button.pressed = true;
    resetScore_button.set(260, 430, 60, 40);

    YesNoDlg.set(20, 100, 280, 120);
    toMainMenu_button.set(0, 0, 40, 35);

    highScoresBack_button.set(5,5,60,40);
    backOptions_button.set(5,5,60,40);
    moregames_button.set(10, 360, 130, 120);
    GameCenter_button.set(0, 430, 50, 50);



    musicVolume_bar.set(20, 150, 5, 100, sys.musicVolume*100 , 2, false);
    sfxVolume_bar.set(20, 230, 5, 100, sys.sfxVolume*100 , 2, false);
    strcpy(nameToEnter, sys.nameToEnter);
    setMusicVolume(sys.musicVolume);
    ss.setVolume(sys.sfxVolume);
    playMusic();
    

    
    useAccel = false;
    
}
//-----------------------
void Singleton::drawEnterName(){
    pics.draw(BG, 0, 0, 0, false, 320/256.0f, 480/512.0f);
    WriteShadedText(60, 10, pics, 0, "Enter your name:");
#if !(TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
    WriteShadedText(90, 30, pics, 0, nameToEnter);
    WriteShadedText(90 + strlen(nameToEnter) * 11, 40, pics, 0, (rand()%2 == 0) ? "_" : " ");
    Keyboard.draw(pics, GUI, 3, 0, COLOR(0.5f, 0.5f, 0.5f, 0.9f));
#endif
    
}
//----------------------
void Singleton::submitScore(int type){
    char tmpName[12];
    strncpy(tmpName, nameToEnter, 12);
    sprintf(highscorePostParams,"name=%s&score=%d&password=", tmpName, score);
    GameCenterScore = score;
    onlineScoreType = type;
    sendScore = true;
    onlineScorePage = 0;
}
//----------------------
void Singleton::addScore(){
    char megabuf[256];
           sprintf(megabuf, "%s/score.dat", DocumentPath);
        highscores.addScore(nameToEnter, score, MAXSCORE);
        highscores.write(megabuf, MAXSCORE);
        regularHighscore_button.pressed = true;
        submitScore(0);
        onlineScore_button.pressed = true;
        localScore_button.pressed = false;
  }

//-----------------------
void Singleton::enterNameLogic(){
#if !(TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
    float mx = -1;
    float my = -1;
    if (touches.up.count()){
        mx = touches.up[0].v[0];
        my = touches.up[0].v[1];
    }
    unsigned result = Keyboard.getInput(globalKEY, mx, my);
    if (result){
        //printf("%c\n", globalKEY);
            if ((result != 8) && (result != 13) && (strlen(nameToEnter) < 11)&&(result < 127)){

                //printf("char %d\n", result);

                ss.playsound(0);
                unsigned pos = strlen(nameToEnter);
                nameToEnter[pos] = (char)result;
                nameToEnter[pos + 1] = 0;
            }
            else{
                if (result == 8){
                    ss.playsound(0);
                    nameToEnter[strlen(nameToEnter) - 1] = 0;
                }
                else{

                    /*printf("char %d\n", result);
                    if ((strlen(nameToEnter) < 11)){
                        unsigned pos = strlen(nameToEnter);
                        nameToEnter[pos] = (char)result;
                        nameToEnter[pos + 1] = 0;
                    }*/

                }
            }
    }

    
#endif
        if (TextEntered || Keyboard.entered){
            TextEntered = false;
            ss.playsound(0);
            char megabuf[255];
            
            if (strlen(nameToEnter) <= 0){
                strcpy(nameToEnter, "NONAME");
            }
            
            
            strncpy(sys.nameToEnter, nameToEnter, 11);
            strncpy(nameToEnter, sys.nameToEnter, 11);
            sprintf(megabuf, "%s/settings.cfg", DocumentPath);
            sys.write(megabuf);
                
#if !(TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
            Keyboard.reset();
#endif
            gamestate = TITLE;
            //playNewSong("music");
        }
}

//----------------------
void Singleton::drawMainMenu(){
    COLOR bgColor = COLOR(0.5f, 0.5f, 0.5f, 0.7f);

    pics.draw(BG, 0, 0, 0, false, 320/256.0f, 480/512.0f);
    pics.draw(11, 160, 60, 0, true);

    if (levelCreated)
        resume_button.draw(pics, MENU, 2);
    play_button.draw(pics, MENU, 8);
    highscore_button.draw(pics, MENU, 3);
    options_button.draw(pics, MENU, 4);
    howtoplay_button.draw(pics, MENU, 5);

    pics.draw(GUI, changeName_button.getX(), changeName_button.getY(),
              2, false, 
              changeName_button.w()/64.0f,
              changeName_button.h()/64.0f, 0 , bgColor, bgColor); 
    WriteShadedText(changeName_button.getX()+20, changeName_button.getY()+10, pics, 0, "Welcome,", 1.0f, 1.0f);
    WriteShadedText(changeName_button.getX()+20, changeName_button.getY()+30, pics, 0, nameToEnter, 1.0f, 1.0f);
    WriteShadedText(changeName_button.getX()+5, changeName_button.getY()+50, pics, 0, "(Tap here to change name)", 0.6f, 0.8f);
 

    moregames_button.draw(pics, 8, 0);
}
//-----------------------
void Singleton::resetHighScores(){
    char megabuf[255];
           sprintf(megabuf, "%s/score.dat", DocumentPath);
        highscores.destroy();
        highscores.write(megabuf, 10);
   }
//-----------------------
void Singleton::parseOnlineScores(HighScores& container){
    
    container.destroy();
    Xml data;
    wchar_t tmp[2024];
    //puts("parsing...");
    //puts(highScoreFromServer);
    mbstowcs(tmp, highScoreFromServer, 2024);
    data.parse(tmp);
    XmlNode * n = 0;
    n = data.root.getNode(L"Highscores");
    
    if (n){
       
        for (unsigned i = 0; i < n->childrenCount(); i++) {
            XmlNode * score = 0;
            score = n->getNode(i);
            if (score){
                char _name[13];
                XmlNode * nameNode = 0;
                nameNode = score->getNode(L"Name");
                if (nameNode){
                    wcstombs(_name, nameNode->getValue(), 12);
                    //puts(_name);
                }
                XmlNode * pointNode = 0;
                char stmp[20];
                pointNode = score->getNode(L"Points");
                if (pointNode){
                    wcstombs(stmp, pointNode->getValue(), 20);
                    long _score = atoi(stmp);
                    container.addScore(_name, _score, MAXSCORE);
                }
            }
        }
    }
    container.resetHighscoreIndex();
    data.destroy();
}

//-----------------------
void Singleton::fetchScore(int type){
    
    getHighScore = true;
    gotHighScore = false;
    waitForOnlineScoreTics = 0;
    highScoreFromServer[0] = 0;
    onlineScoreRegular.destroy();
    onlineScoreType = type;
    failToGetScore = false;
    
}

//-----------------------
void Singleton::highScoreLogic(){


#if !(TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)

    if ((!Keys[7])&&(OldKeys[7])){
        gamestate = TITLE;
        ss.playsound(0);
    }
#endif


    if (strlen(highScoreFromServer) < 1){
        if (waitForOnlineScoreTics < MAXWAITFORSCORE)
            waitForOnlineScoreTics++;
    }
    
    if ((waitForOnlineScoreTics == MAXWAITFORSCORE) || (gotHighScore)){
        
        getHighScore = false;
        waitForOnlineScoreTics = 0;
        
        if (gotHighScore){
            gotHighScore = false;
            if (strlen(highScoreFromServer) > 0){
                if (onlineScoreType == 0)
                    parseOnlineScores(onlineScoreRegular);
            }
        }
        else{
            failToGetScore = true;
        }
    }
    
    if (touches.down.count()){
        float mx = touches.down[0].v[0];
        float my = touches.down[0].v[1];
        
        if (GameCenter_button.isPointerOnTop(mx, my))
            GameCenter_button.c = COLOR(0.5f, 0.5f, 0.5f);
        
    }
    
    if (touches.allfingersup){
        GameCenter_button.c = COLOR(1,1,1);
    }
    
    if ((touches.up.count())&&(!touches.down.count())){
        float mx = touches.up[0].v[0];
        float my = touches.up[0].v[1];


        GameCenter_button.c = COLOR(1,1,1);
        
        if (GameCenter_button.isPointerOnTop(mx, my)){
            ShowGameCenter = true;
            GameCenter_button.c = COLOR(1,1,1);
        }
        
        if (YesNoDlg.active()){
            if (YesNoDlg.isPointerOnYes(mx, my)){
                resetHighScores();
                YesNoDlg.deactivate();
                ss.playsound(0);
            }
            if (YesNoDlg.isPointerOnNo(mx, my)){
                YesNoDlg.deactivate();
                ss.playsound(0);
            }
            
            return;
        }

        if (!onlineScore_button.pressed){
            if (resetScore_button.isPointerOnTop(mx, my)){
                YesNoDlg.activate();
                ss.playsound(0);
            }
        }
#ifndef LITE      
        
        
        if (onlineScorePage_button.isPointerOnTop(mx, my)){
            if (onlineScorePage > 0)
                onlineScorePage = 0;
            else {
                onlineScorePage = 1;
            }
            
            fetchScore(0);
            
            ss.playsound(0);

        }
        
           
        if (onlineScore_button.isPointerOnTop(mx, my)){
            onlineScore_button.pressed = true;
            localScore_button.pressed = false;
            waitForOnlineScoreTics = 0;
            
            fetchScore(0);
            
            ss.playsound(0);
        }
        
        if (localScore_button.isPointerOnTop(mx, my)){
            onlineScore_button.pressed = false;
            localScore_button.pressed = true;
            ss.playsound(0);
        }
#endif
        
        
        if (highScoresBack_button.isPointerOnTop(mx, my)){
            gamestate = TITLE;
            ss.playsound(0);
        }
    }
}
//------------------------
void Singleton::optionsLogic(){


#if !(TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)

    if ((!Keys[7])&&(OldKeys[7])){
        gamestate = TITLE;
        char buf[255];
        if (strlen(sys.nameToEnter) <= 0)
            strcpy(sys.nameToEnter,"Player");
        sprintf(buf, "%s/settings.cfg", DocumentPath);
        sys.write(buf);
        ss.playsound(0);

    }
#endif    
    if ((touches.up.count())&&(!touches.down.count())){
        
        float mx = touches.up[0].v[0];
        float my = touches.up[0].v[1];
        
        if (backOptions_button.isPointerOnTop(mx, my)){
            gamestate = TITLE;
            char buf[255];
            if (strlen(sys.nameToEnter) <= 0)
                strcpy(sys.nameToEnter,"NONAME");
            sprintf(buf, "%s/settings.cfg", DocumentPath);
            sys.write(buf);
            ss.playsound(0);
        }
        
             
        if (musicVolume_bar.isPointerOnNextpage(mx, my)){
            musicVolume_bar.pageDown();
            sys.musicVolume = musicVolume_bar.state()/100.0f; 
            setMusicVolume(sys.musicVolume);
            ss.playsound(0);
        }
        if (musicVolume_bar.isPointerOnPrevpage(mx, my)){
            musicVolume_bar.pageUp();
            sys.musicVolume = musicVolume_bar.state()/100.0f; 
           setMusicVolume(sys.musicVolume);
            ss.playsound(0);
            
        }
        if (sfxVolume_bar.isPointerOnNextpage(mx, my)){
            sfxVolume_bar.pageDown();
            sys.sfxVolume = sfxVolume_bar.state()/100.0f; 
            ss.setVolume(sys.sfxVolume);
            ss.playsound(0);
        }
        if (sfxVolume_bar.isPointerOnPrevpage(mx, my)){
            sfxVolume_bar.pageUp();
            sys.sfxVolume = sfxVolume_bar.state()/100.0f; 
            ss.setVolume(sys.sfxVolume);
            ss.playsound(0);
            
        }
        
    }
    if (touches.move.count()){
        musicVolume_bar.processInput(touches.move[0].v[0], touches.move[0].v[1]);
        sfxVolume_bar.processInput(touches.move[0].v[0], touches.move[0].v[1]);
        sys.musicVolume = musicVolume_bar.state()/100.0f;
        sys.sfxVolume = sfxVolume_bar.state()/100.0f;
        setMusicVolume(sys.musicVolume);
        ss.setVolume(sys.sfxVolume);

    }
    if (touches.down.count()){
        musicVolume_bar.processInput(touches.down[0].v[0], touches.down[0].v[1]);
        sfxVolume_bar.processInput(touches.down[0].v[0], touches.down[0].v[1]);
        sys.musicVolume = musicVolume_bar.state()/100.0f; 
        sys.sfxVolume = sfxVolume_bar.state()/100.0f;
        ss.setVolume(sys.sfxVolume);
        setMusicVolume(sys.musicVolume);

    }
}

//------------------------
void Singleton::drawHighScore(){
       
    pics.draw(BG, 0, 0, 0, false, 320/256.0f, 480/512.0f);
    highScoresBack_button.drawTextnPicture(pics, 7, BUTTONS, 0, "back");

    pics.draw(GUI, 10, 50, 2, false, 300/64.0f, 50/64.0f,0,
              COLOR(0.5f, 0.5f, 0.5f, 0.5f),
              COLOR(0.5f, 0.5f, 0.5f, 0.5f));
    pics.draw(GUI, 10, 380, 3, false, 300/64.0f, 50/64.0f,0,
              COLOR(0.5f, 0.5f, 0.5f, 0.5f),
              COLOR(0.5f, 0.5f, 0.5f, 0.5f));
    pics.draw(-1, 10, 100, 0, false, 300, 280, 0,
              COLOR(0.5f, 0.5f, 0.5f, 0.5f),
              COLOR(0.5f, 0.5f, 0.5f, 0.5f)
              );
    
    COLOR pressed = COLOR(1.0, 1.0, 1.0);
    COLOR bgColor = COLOR(1.0, 1.0, 1.0);
    COLOR notPressed = COLOR(0.5, 0.5, 0.5);
  
      
    if (onlineScore_button.pressed){
        onlineScore_button.c = bgColor;
        onlineScore_button.draw(pics, GUI, 3);
        onlineScore_button.c = pressed;
    }
    else
        onlineScore_button.c = notPressed;
    
    onlineScore_button.draw(pics, MENU, 6);
    
    if (localScore_button.pressed){
        localScore_button.c = bgColor;
        localScore_button.draw(pics, GUI, 3);
        localScore_button.c = pressed;
    }
    else
        localScore_button.c = notPressed;
    localScore_button.draw(pics, MENU, 7);
    
    
    int scorePosY = 65;
    
        if (localScore_button.pressed)
            highscores.display(pics, 0, MAXSCORE, 10, scorePosY);
        else{
            if (onlineScoreRegular.count())
                onlineScoreRegular.display(pics, 0, MAXSCORE, 10, scorePosY, (onlineScorePage)? 11 : 1);
            
            if (strlen(highScoreFromServer) < 1){
                if (!failToGetScore)
                    WriteShadedText(95, 220, pics, 0, "Loading...");
                else {
                    WriteShadedText(10, 220, pics, 0, "Could not connect server :(");
                }

            }
        }
    
       
#if (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
    GameCenter_button.draw(pics, BUTTONS, 12);
#endif
    if (!onlineScore_button.pressed)
        resetScore_button.drawTextnPicture(pics, 7, BUTTONS, 0, "reset");
    
    if (YesNoDlg.active())
        YesNoDlg.draw(pics, 0, GUI, BUTTONS, 7);
    
  
}
//-------------------------
void Singleton::GridDraw(int x,int y){
    int jmp;
    int frm=0;

    for (int a = 0; a < GRIDHEIGHT; a++){
        for (int i = 0; i < GRIDWIDTH; i++){
            pics.draw(BRICK, x+32*i, y+32*a, 0, false, 1, 1, 0,
                      ((i == pointerON.x)&&(a == pointerON.y))? COLOR(0.5f, 0.5f, 0.5f) : COLOR(1,1,1),
                      ((i == pointerON.x)&&(a == pointerON.y))? COLOR(0.5f, 0.5f, 0.5f) : COLOR(1,1,1)
                      );//brick
        }
    }

    for (int a = 0; a < GRIDHEIGHT; a++){
        for (int i = 0; i < GRIDWIDTH; i++){
            unsigned type = grid.grid[a][i].color;
            if (type){
                jmp = 0;
                if ((bmoved)&&(a==b.y)&&(i==b.x))
                    jmp = b.cnt/10;

                switch (grid.grid[a][i].state){
                    case NEW:  frm = 9; break;
                    case DEAD: frm = 18; break;
                    default:  frm = 0; break;
                }
                COLOR c = COLOR(1, 1, 1, 1);
                if (type != 9)
                    pics.draw(BALLS, x+32*i, y+32*a+jmp + (32*1.5f),
                              type - 1 + frm, false, 1, -1.5, 0,
                              COLOR(0,0,0,0.3f), COLOR(0,0,0,0.25f));//shadow
                else
                    c = CoS; 
                pics.draw(BALLS, x+32*i + 16, y+32*a+16+jmp - 10,
                          type - 1 + frm, true,
                          1 + ((type == 9) ? jmp/12.0f:0), 
                          1 + ((type == 9) ? jmp/12.0f:0), 
                          0, c, c);//bubble
            }

        }

    }
}
//--------------------------
void Singleton::drawscore(int x,int y,int number){
 
 int tmp=number;
 int mas[6]={0};
 int a=5;

 while (tmp>0){
  mas[a]=tmp%10;
  tmp=tmp/10;
  a--;
 }

 for (int i=0;i<6;i++)
  pics.draw(DIGITS,x+16*i,y,mas[i]);

}

//----------------------
void Singleton::drawGame(){
    pics.draw(10, 0, 0);
    toMainMenu_button.draw(pics, BUTTONS, 11);
    int y = 40; 
    for (int i = 0; i < 3; i++){
        pics.draw(BRICK,112+i*32, y);
    }
    for (int i = 0; i < 3; i++){
        COLOR c = (nextballs[i] == 9)? CoS : COLOR(1,1,1,1);
        pics.draw(BALLS,112+i*32, y - 10, nextballs[i]-1, false, 1, 1, 0, c, c);
    }
    drawscore(10, y,highscores.getScore(0).score);
    drawscore(214, y,score);
    GridDraw(0, BOARDY);
    pics.draw(9, 0, BOARDY+10*32);
    if (gameover){
        pics.draw(MENU, 160-128, -100 + gameOverTics, 0);
    }
}
//-----------------------
void Singleton::drawOptions(){
    pics.draw(BG, 0, 0, 0, false, 320/256.0f, 480/512.0f);
    backOptions_button.drawTextnPicture(pics, 7, BUTTONS, 0, "back");
    
    pics.draw(MENU, 100, 10, 4, false, 0.8, 0.8);
    COLOR bgColor = COLOR(0.5f, 0.5f, 0.5f, 0.5f);
    
    int panelHeight = 250;
    
    pics.draw(-1, 70, 90, 0, false, 180, panelHeight, 0, bgColor, bgColor);
    pics.draw(GUI, 10, 90, 0, false, 
              60/64.0f, panelHeight/64.0f, 0, bgColor, bgColor);
    pics.draw(GUI, 250, 90, 1, false, 
              60/64.0f, panelHeight/64.0f, 0, bgColor, bgColor);
    
    WriteShadedText(100, musicVolume_bar.getY()-10, pics, 0, "Music volume:");
    musicVolume_bar.draw(pics, BUTTONS, 8, 9);
    WriteShadedText(100, sfxVolume_bar.getY()-10, pics, 0, "Sfx volume:");
    sfxVolume_bar.draw(pics, BUTTONS, 8, 9);

    
}
//----------------------
void Singleton::drawHelp(){

    pics.draw(BG, 0, 0, 0, false, 320/256.0f, 480/512.0f);
    helpBack_button.drawTextnPicture(pics, 7, BUTTONS, 0, "back");

    pics.draw(BG+1, 10, 60, 0, false);
}
//----------------------
void Singleton::helpLogic(){
    if (touches.up.count()){
        float x = touches.up[0].v[0];
        float y = touches.up[0].v[1];

        if (helpBack_button.isPointerOnTop(x, y)){
            gamestate = TITLE;
            ss.playsound(0);
        }
    }
}
//----------------
void Singleton::drawDebugText(){
    char buf[50];
    sprintf(buf, "draw calls %d", drawCalls);
    WriteShadedText(5, 5, pics, 0, buf, 0.8f, 0.8f, COLOR(1,0,0), COLOR(1,0,0));
    sprintf(buf, "free space %d", grid.freeSquares());
    WriteShadedText(5, 20, pics, 0, buf, 0.8f, 0.8f, COLOR(1,0,0), COLOR(1,0,0));
}

//-------------------
void Singleton::render(){
    
   
    glClear(GL_COLOR_BUFFER_BIT);
    switch(gamestate){
        case TITLE : drawMainMenu(); break;
        case GAME : drawGame(); break;
        case HIGHSCORE : drawHighScore(); break;
        case OPTIONS : drawOptions(); break;
        case ENTERNAME: drawEnterName(); break;
        case HELP: drawHelp(); break;
    }
    
    //drawDebugText();
    
    drawCalls = pics.drawBatch();
    
    
}
//------------------
void Singleton::ResetBoard(){
    pointerON.x = 666;
   score=0;
   oldScore = 0;
   gameOverTics = 0;
   grid.reset();
   spawnObelisk = false;
   spawnCoS = false;
   victims.destroy();
   grid.spawnFigure(rand()%7+1);
   grid.spawnFigure(rand()%7+1);
   grid.spawnFigure(rand()%7+1);

   /*grid.spawnFigure(9);
   grid.spawnFigure(9);
   grid.spawnFigure(9);
   grid.spawnFigure(9);
   grid.spawnFigure(9);
   grid.spawnFigure(9);
   grid.spawnFigure(8);
   grid.spawnFigure(8);
   grid.spawnFigure(8);
   grid.spawnFigure(8);
   grid.spawnFigure(4);
   grid.spawnFigure(4);
   grid.spawnFigure(4);
   grid.spawnFigure(4);
   grid.spawnFigure(4);
   grid.spawnFigure(4);*/

   nextballs[0]=rand()%7+1;
   nextballs[1]=rand()%7+1;
   nextballs[2]=rand()%7+1;
   spawn = true;
   lines = 0;

}

//----------------------
void Singleton::mainMenuLogic(){
    
    float shrink = 0.93f;
    COLOR dark = COLOR(0.6f, 0.6f, 0.6f);
    COLOR white = COLOR(1.0f, 1.0f, 1.0f);
    
    if (strlen(nameToEnter) < 1){
        gamestate = ENTERNAME;
        ActivateEditField = true;
        return;
    }
    if (strlen(nameToEnter) > 11){
        puts(nameToEnter);
        puts(sys.nameToEnter);
        nameToEnter[11]=0;
    
    }
if (touches.allfingersup){
        play_button.c = COLOR(1.0f, 1.0f, 1.0f);
        play_button.scalex = 1.0f;
        highscore_button.c = COLOR(1.0f, 1.0f, 1.0f);
        highscore_button.scalex = 1.0f;
        resume_button.c = COLOR(1.0f, 1.0f, 1.0f);
        resume_button.scalex = 1.0f;
        options_button.c = COLOR(1.0f, 1.0f, 1.0f);
        options_button.scalex = 1.0f;
        howtoplay_button.c = COLOR(1.0f, 1.0f, 1.0f);
        howtoplay_button.scalex = 1.0f;
        moregames_button.c = white;
}

    if (touches.down.count()){
        float x = touches.down[0].v[0];
        float y = touches.down[0].v[1];
        
        if (play_button.isPointerOnTop(x, y)){
            play_button.c = dark;
            play_button.scalex = shrink;
            ss.playsound(0);
        }

        if (moregames_button.isPointerOnTop(x, y)){
            moregames_button.c = dark;
            ss.playsound(0);
        }
 
        if (highscore_button.isPointerOnTop(x, y)){
            highscore_button.c = dark;
            highscore_button.scalex = shrink;
            ss.playsound(0);
        }

        if (levelCreated){
            if (resume_button.isPointerOnTop(x, y)){
                resume_button.c = dark;
                resume_button.scalex = shrink;
                ss.playsound(0);
            }
        }
        
        if (howtoplay_button.isPointerOnTop(x, y)){
            howtoplay_button.c = dark;
            howtoplay_button.scalex = shrink;
            ss.playsound(0);
        }

        if (options_button.isPointerOnTop(x, y)){
            options_button.c = dark;
            options_button.scalex = shrink;
            ss.playsound(0);
        }
    }
    
        
    if ((touches.up.count())&&(!touches.oldDown.count())){

        float x = touches.up[0].v[0];
        float y = touches.up[0].v[1];
        //printf("%f %f\n", x, y);
        
        if (changeName_button.isPointerOnTop(x, y)){
            gamestate = ENTERNAME;
            ActivateEditField = true;
            ss.playsound(0);
        }

        
        
        play_button.c = COLOR(1.0f, 1.0f, 1.0f);
        play_button.scalex = 1.0f;
        highscore_button.c = COLOR(1.0f, 1.0f, 1.0f);
        highscore_button.scalex = 1.0f;
        resume_button.c = COLOR(1.0f, 1.0f, 1.0f);
        resume_button.scalex = 1.0f;
        options_button.c = COLOR(1.0f, 1.0f, 1.0f);
        options_button.scalex = 1.0f;
        howtoplay_button.c = COLOR(1.0f, 1.0f, 1.0f);
        howtoplay_button.scalex = 1.0f;
        
       
        if (howtoplay_button.isPointerOnTop(x, y)){
            gamestate = HELP;
        }
        
        if (play_button.isPointerOnTop(x, y)){
            gamestate = GAME; 
            showBanner = true;

            levelCreated = true;
            ResetBoard();
        
        }

        if (moregames_button.isPointerOnTop(x, y)){
            launchjrs0ul = true;
        }
        
        if (highscore_button.isPointerOnTop(x, y)){
            //highscore_button.c = COLOR(1.0f, 1.0f, 1.0f);
            gamestate = HIGHSCORE;
            if (onlineScore_button.pressed){
            fetchScore(0);

            }
        }

        
        if ((resume_button.isPointerOnTop(x, y))&&(levelCreated)){
            gamestate = GAME;
            showBanner = true;
        }
        
        if (options_button.isPointerOnTop(x, y)){
           
            gamestate = OPTIONS;
        }
       
    }
}
//---------------------
void Singleton::PutBall(int x,int y, char cl){
    grid.grid[y][x].color=cl;
}
//--------------------------
void Singleton::countscore(int cnt){
    
    score += cnt * 2;

    if (score >= oldScore + ObeliskInterval){
        spawnObelisk = true;
        oldScore = (score/ObeliskInterval) * ObeliskInterval;
        //printf("%d\n", oldScore);
    }
}

//--------------
void Singleton::eliminateballs(){
    int count = (int)victims.count();
    if (count > 4){
        if (count > 5)
            spawnCoS = true;
        for (int z = 0; z < count; z++){
            /*printf("color %u x:%d y:%d\n",
                    grid.grid[victims[z].y][victims[z].x].color,
                    victims[z].x, victims[z].y);*/
            grid.grid[victims[z].y][victims[z].x].state = DEAD;
        }
        countscore(count);
        lines++;
    }
    victims.destroy();
}
//----------------------
void Singleton::addVictim(_Point& p){

    for (unsigned i = 0; i < victims.count(); i++){
        if ((p.x == victims[i].x)&&(p.y == victims[i].y))
            return;
    }
    victims.add(p);
}

//--------------------
void Singleton::eliminateballsCoS(){
    int count = (int)victims.count();
    if (count > 4){
        if (count > 5)
            spawnCoS = true;
        for (int z = 0; z < count; z++){
            /*printf("-color %u x:%d y:%d\n",
                    grid.grid[victims[z].y][victims[z].x].color,
                    victims[z].x, victims[z].y);*/
            grid.grid[victims[z].y][victims[z].x].state = DEAD;
        }
        countscore(count);
        lines++;
        victims.destroy();
    }
    else{
        DArray<_Point> tmp;
        for (int i = count-1; i >= 0; i--){
            if (grid.grid[victims[i].y][victims[i].x].color == 9){
                _Point a;
                a.x = victims[i].x; a.y = victims[i].y;
                tmp.add(a);
            }
            else
                break;
        }
        victims.destroy();
        //printf("cnt %lu\n", tmp.count());
        for (unsigned i = 0; i < tmp.count(); i++){
            _Point a;
            a.x = tmp[i].x; a.y = tmp[i].y;
            addVictim(a);

        }
        tmp.destroy();
    }
}

//-----------------
void Singleton::checkForObelisksV(){
    int count = (int)victims.count();
    if (count < 5)
        return;
    _Point p;

    int minY = GRIDHEIGHT + 1;
    int maxY = -1;
    for(unsigned i = 0; i < victims.count(); i++){
        if (victims[i].y > maxY)
            maxY = victims[i].y;
        if (victims[i].y < minY)
            minY = victims[i].y;
    }

    int firstObeliskY = minY - 1;
    int lastObeliskY = maxY + 1;
    if ((firstObeliskY >= 0) && (firstObeliskY - 1 < GRIDHEIGHT)){
        if (grid.grid[firstObeliskY][victims[0].x].color == 8){
            p.x = victims[0].x;
            p.y = firstObeliskY;
            addVictim(p);
        }
    }
    if ((lastObeliskY >= 0) && (lastObeliskY < GRIDHEIGHT)){
        if (grid.grid[lastObeliskY][victims[count - 1].x].color == 8){
            p.x = victims[count - 1].x;
            p.y = lastObeliskY;
            addVictim(p);
        }
    }
}
//--------------------
void Singleton::checkForObelisksH(){
    int count = (int)victims.count();
    if (count < 5)
        return;
    _Point p;

    int minX = GRIDWIDTH + 1;
    int maxX = -1;
    for(unsigned i = 0; i < victims.count(); i++){
        if (victims[i].x > maxX)
            maxX = victims[i].x;
        if (victims[i].x < minX)
            minX = victims[i].x;
    }


    int firstObeliskX = minX - 1;
    int lastObeliskX = maxX + 1;
    if ((firstObeliskX >= 0) && (firstObeliskX < GRIDWIDTH)){
        if (grid.grid[victims[0].y][firstObeliskX].color == 8){
            p.x = firstObeliskX;
            p.y = victims[0].y;
            addVictim(p);
        }
    }
    if ((lastObeliskX >= 0) && (lastObeliskX < GRIDWIDTH)){
        if (grid.grid[victims[count-1].y][lastObeliskX].color == 8){
            p.x = lastObeliskX;
            p.y = victims[count - 1].y;
            addVictim(p);
        }
    }
}
//-------------------
void Singleton::checkForObelisksLVH(){
    int count = (int)victims.count();
    if (count < 5)
        return;
    _Point p;


    int minX = GRIDWIDTH + 1;
    int maxX = -1;
    int minY = GRIDHEIGHT + 1;
    int maxY = -1;

    for(unsigned i = 0; i < victims.count(); i++){
        if (victims[i].x > maxX)
            maxX = victims[i].x;
        if (victims[i].x < minX)
            minX = victims[i].x;
        if (victims[i].y > maxY)
            maxY = victims[i].y;
        if (victims[i].y < minY)
            minY = victims[i].y;
    }


    int firstObeliskX = minX - 1;
    int firstObeliskY = minY - 1;
    int lastObeliskX = maxX + 1;
    int lastObeliskY = maxY + 1;

    if ((firstObeliskX >= 0) && (firstObeliskX < GRIDWIDTH)&&
        (firstObeliskY >= 0) && (firstObeliskY < GRIDHEIGHT)){
        if (grid.grid[firstObeliskY][firstObeliskX].color == 8){
            p.x = firstObeliskX;
            p.y = firstObeliskY;
            addVictim(p);
        }
    }
    if ((lastObeliskX >= 0) && (lastObeliskX < GRIDWIDTH)&&
        (lastObeliskY >= 0) && (lastObeliskY < GRIDHEIGHT)){
        if (grid.grid[lastObeliskY][lastObeliskX].color == 8){
            p.x = lastObeliskX;
            p.y = lastObeliskY;
            addVictim(p);
        }
    }
}
//-------------------
void Singleton::checkForObelisksRVH(){
    int count = (int)victims.count();
    if (count < 5)
        return;
    _Point p;

    int minX = GRIDWIDTH + 1;
    int maxX = -1;
    int minY = GRIDHEIGHT + 1;
    int maxY = -1;

    for(unsigned i = 0; i < victims.count(); i++){
        if (victims[i].x > maxX)
            maxX = victims[i].x;
        if (victims[i].x < minX)
            minX = victims[i].x;
        if (victims[i].y > maxY)
            maxY = victims[i].y;
        if (victims[i].y < minY)
            minY = victims[i].y;
    }

    int firstObeliskX = maxX + 1;
    int firstObeliskY = minY - 1;
    int lastObeliskX = minX - 1;
    int lastObeliskY = maxY + 1;

    if ((firstObeliskX >= 0) && (firstObeliskX < GRIDWIDTH)&&
        (firstObeliskY >= 0) && (firstObeliskY < GRIDHEIGHT)){
        if (grid.grid[firstObeliskY][firstObeliskX].color == 8){
            p.x = firstObeliskX;
            p.y = firstObeliskY;
            addVictim(p);
        }
    }
    if ((lastObeliskX >= 0) && (lastObeliskX < GRIDWIDTH)&&
        (lastObeliskY >= 0) && (lastObeliskY < GRIDHEIGHT)){
        if (grid.grid[lastObeliskY][lastObeliskX].color == 8){
            p.x = lastObeliskX;
            p.y = lastObeliskY;
            addVictim(p);
        }
    }


}
//-----------------
//Color of Space glues creatures and they explode :)
void Singleton::CoSGlue(unsigned type, unsigned Color, unsigned &oldColor){

    if (oldColor == 9){
        if (((int)victims.count())-1 > 0){
            //...[O]...[9][O]...
            //    |     |  |
            //  tmp     |  Color
            //          |
            //          oldColor
            int index = (int)victims.count() - 2;
            unsigned tmp = grid.grid[victims[index].y][victims[index].x].color;

            while ((tmp == 9)&&(index > 0)){
                index--;
                tmp = grid.grid[victims[index].y][victims[index].x].color;
            }

            if ((tmp != Color)&&(Color!=9)){
                oldColor = Color;
                _Point last;
                last.x = victims[victims.count()-1].x;
                last.y = victims[victims.count()-1].y;
                switch(type){
                    case 0 : checkForObelisksV(); break;
                    case 1 : checkForObelisksH(); break;
                    case 2 : checkForObelisksLVH(); break;
                    case 3 : checkForObelisksRVH(); break;
                }
                eliminateballsCoS();//eliminate stuff but don't delete last CoS
                addVictim(last);
            }
            else{
                oldColor = tmp;
            }
        }
        else
            if (victims.count()){
                oldColor = Color;
            }
    }
}

//-------------
bool Singleton::Fivexplode(){

    unsigned cl,clold;
    int cnt;
    _Point p;

    cl = 0;
    cnt = 0;
    int a = 0;

    int tmplines = lines;

    //naikinam vertikaliai
    for (a = 0; a < 10; a++) {
        for (int i = 0; i < 10; i++){
            clold = cl;
            cl = grid.grid[i][a].color;

            CoSGlue(0, cl, clold);
            
            if (((cl == clold)||(cl==9)) && (cl!=0) && (i > 0)){
                p.x = a;  p.y = i;
                addVictim(p);
            }
            else{
                checkForObelisksV();
                eliminateballs();
                if (cl!=0){
                    p.x = a;  p.y = i;
                    addVictim(p);
                }
            }
        } 
    }
    checkForObelisksV();
    eliminateballs();

    cnt=0;
    //naikinam horizontaliai 
    for (a = 0; a < 10; a++){ 
        for (int i = 0; i < 10; i++){
            clold = cl;
            cl = grid.grid[a][i].color;

            CoSGlue(1, cl, clold);
            if (((cl == clold)||(cl==9)) && (cl!=0) && (i > 0)){
                p.x=i;  p.y=a;
                addVictim(p);
            }
            else {
                checkForObelisksH();
                eliminateballs();
                if (cl!=0){
                    p.x = i;  p.y = a;
                    addVictim(p);
                }
            }
        }
    }
    checkForObelisksH();
    eliminateballs();
    //==============
    //istrizai, is desines
    cnt=0;

    for (a = 4; a < 10; a++){  
        for (int i = a; i >= 0; i--){
            clold = cl; 
            cl = grid.grid[a-i][i].color;

            
    
            CoSGlue(3, cl, clold);
            if (((cl == clold)||(cl==9)) && (cl!=0) && (i<a)){
                p.x = i;  p.y = a-i;
                addVictim(p); 

            }
            else {

                checkForObelisksRVH();
                eliminateballs();
                if (cl!=0){
                    p.x=i;  p.y=a-i;
                    addVictim(p); 
                }
            } 
        }
    }
    //--
    for (a = 1; a < 6; a++){  
        for (int i = 9; i >= a; i--){
            clold=cl; 
            cl=grid.grid[a+(9-i)][i].color;

            CoSGlue(3, cl, clold);
            if (((cl==clold)||(cl==9))&&(cl!=0)&&(i<9)){
                p.x=i;  p.y=a+(9-i);
                addVictim(p); 
            }
            else {
                checkForObelisksRVH();
                eliminateballs();
                if (cl!=0){
                    p.x = i;  p.y = a+(9-i);
                    addVictim(p);
                }
            }
        } 
    }
    checkForObelisksRVH();
    eliminateballs();
    //puts("crashpoint3");
    //==============
   //istrizai, is kaires
    cnt=0;
   //--
    for (a = 1; a < 6; a++){  
        for (int i=0; i<10-a;i++){
            clold = cl; 
            cl = grid.grid[a+i][i].color;
 
            CoSGlue(2, cl, clold);
            if (((cl==clold)||(cl==9))&&(cl!=0)&&(i>0)){
                p.x=i;  p.y=a+i;
                addVictim(p); 
            }
            else{
                checkForObelisksLVH();
                eliminateballs();
                if (cl!=0){
                    p.x = i;  p.y = a+i;
                    addVictim(p);
                }
            }
        } 
    }
    //--
    for (a=0;a<6;a++){  
        for (int i=0; i<10-a; i++){
            clold=cl; 
            cl=grid.grid[i][i+a].color;
            CoSGlue(2, cl, clold);
            
            if (((cl==clold)||(cl==9))&&(cl!=0)&&(i>0)){
                p.x=i+a;  p.y=i;
                addVictim(p); 
            }
            else {

                checkForObelisksLVH();
                eliminateballs();
                p.x=i+a;  p.y=i;
                if (cl!=0){
                    addVictim(p);
                }

            }
        } 
    }

    checkForObelisksLVH();
    eliminateballs();
    //puts("crashpoint6");
    //===
    
    if (tmplines < lines)
        return true;
    else
        return false;
}
//-----------------------------------------------
bool Singleton::CanGo(int x,int y){

    _Point start,dest;
    start.x=b.x;
    start.y=b.y;
    dest.x=x;
    dest.y=y;

    bool ** bmap = 0;

    //puts("yo");
       bmap = (bool **)malloc( 10 * sizeof(bool*));
    for (unsigned i = 0; i < 10; i++){
        bmap[i] = (bool*)malloc(10 * sizeof(bool));
        for (unsigned a = 0; a < 10; a++){
            bmap[i][a] = (grid.grid[i][a].color == 0) ? true : false;
        }
    }
    //puts("crashh");
    path.destroy();
    bool result = path.findPath(start, dest, bmap, 10, 10);
    //puts("bandicut");

     if (bmap){
        for (unsigned i = 0; i < 10; i++)
            free(bmap[i]);
        free(bmap);
        bmap = 0;
    }

    return result;

}

//----------
void Singleton::BallClick(_Point p){


    int x = 0;
    int y=0;

    if ((p.x < 320)&&(p.x > 0)&&
          (p.y < BOARDY+32*10)&&(p.y > BOARDY)){

        x = pointerON.x;//(p.x) / 32;
        y = pointerON.y;//(p.y-BOARDY) / 32;
        

        unsigned _color = grid.grid[y][x].color;
        unsigned state = grid.grid[y][x].state;
        if (state != IDLE)
            return;

        if (!bmoved){ //not selected
            if ((_color) && (_color != 8)){
                bmoved = true;
                b.color = _color;
                b.x = x;
                b.y = y;
            }
        }
        else{ //select other, or....
            if ((_color) && (_color != 8)){//not empty
                b.color = _color;
                b.x = x;
                b.y = y;
            }
            else{//empty, so let's put figure here
                if (CanGo(x,y)){
                    burbulaseina = true;
                    bstep = 0;   
                }
                else{
                    ss.playsound(3);//cant put here
                }
            }
        }
    }

}
//---------------------
void Singleton::CoSAnimation(){

    float color_speed = 0.02f;
    if (((CoS.c[0] > CoSData[CoSPhase].c[0]+0.2f)||(CoS.c[0] < CoSData[CoSPhase].c[0]-0.2f))
        ||((CoS.c[1] > CoSData[CoSPhase].c[1]+0.2f)||(CoS.c[1] < CoSData[CoSPhase].c[1]-0.2f))
        ||((CoS.c[2] > CoSData[CoSPhase].c[2]+0.2f)||(CoS.c[2] < CoSData[CoSPhase].c[2]-0.2f))){

        if (CoS.c[0] > CoSData[CoSPhase].c[0])
            CoS.c[0] -= color_speed;
        else
            CoS.c[0] += color_speed;
        if (CoS.c[1] > CoSData[CoSPhase].c[1])
            CoS.c[1] -= color_speed;
        else
            CoS.c[1] += color_speed;
        if (CoS.c[2] > CoSData[CoSPhase].c[2])
            CoS.c[2] -= color_speed;
        else
            CoS.c[2] += color_speed;
    }
    else{
        CoSPhase++;
    
        if (CoSPhase > 4)
            CoSPhase = 0;
        //printf("phase %d\n", CoSPhase);
    }

}
//---------------------
void Singleton::gameLogic(){


    CoSAnimation();

    //bounce
    if (bmoved) {
        b.cnt++;
        if (b.cnt > MaxBounceFrameTic){
            b.cnt = 0;
            ss.playsound(1);
        }
    }

  //-------------
    if (touches.down.count()){
        float mx = touches.down[0].v[0];
        float my = touches.down[0].v[1];

        if (toMainMenu_button.isPointerOnTop(mx, my)){
            toMainMenu_button.c = COLOR(0.5f, 0.5f, 0.5f);
        }

        
        if((!spawn)&&(!gameover)&&(!burbulaseina)&&(!explode)){
            if ((mx < 320) && (mx > 0)&&
                (my < BOARDY + 32 * GRIDHEIGHT) && (my > BOARDY)){

                pointerON.x = mx / 32;
                pointerON.y = (my-BOARDY) / 32;

            }
        }

    }

//----------------
    if (touches.move.count()){

        float mx = touches.move[0].v[0];
        float my = touches.move[0].v[1];
        if((!spawn)&&(!gameover)&&(!burbulaseina)&&(!explode)){
            if ((mx < 320) && (mx > 0)&&
                (my < BOARDY + 32 * GRIDHEIGHT) && (my > BOARDY)){

                pointerON.x = mx / 32;
                pointerON.y = (my-BOARDY) / 32;

            }
        }

    }
//----------------

    if ((touches.up.count())&&(!touches.down.count())){
        float mx = touches.up[0].v[0];
        float my = touches.up[0].v[1];

        toMainMenu_button.c = COLOR(1.0f, 1.0f, 1.0f);

        if (!gameover){

            
            if((!spawn)&&(!gameover)&&(!burbulaseina)&&(!explode)){
                _Point p;
                p.x = mx;
                p.y = my;
                BallClick(p);
            }


            if (toMainMenu_button.isPointerOnTop(mx, my)){
                gamestate = TITLE;
                hidebanner = true;
                ss.playsound(0);
            }
        }
        else{
            if (gameOverTics > 100){
                gameover = false;
                levelCreated = false;
                gamestate = HIGHSCORE;
                hidebanner = true;
                return;
            }
        }
        pointerON.x = 666;
    }
//----
    if (gameover){
        if (gameOverTics < 200)
            gameOverTics++;
    }
//--------
    if ((grid.freeSquares() <= 3)&&(!gameover)){
        gameover = true;
        onlineScoreType = 0;
        addScore();
    }


//---------
    if (spawn) {
      spawnc++;
      if (spawnc==10){
        spawn=false;
        for (int i=0;i<10;i++)
         for (int a=0;a<10;a++)
           if (grid.grid[i][a].state == NEW)
            grid.grid[i][a].state = IDLE;
        spawnc=0;

      }
    }
    //----
    if (explode) {
        explodec++;
        if (explodec > MaxExplosionFrameTic){
            explode = false;
            grid.cleanDead();
            explodec=0;
        }
    }
    //----

    if (burbulaseina){
        if (bstep==0)
            PutBall(b.x,b.y,0);
        else
            PutBall(path.parent[bstep].x, path.parent[bstep].y,0);
        bstep++;

        PutBall(path.parent[bstep].x, path.parent[bstep].y,b.color);

        if (bstep == (int)path.parent.count()){
            burbulaseina = false;
            bmoved = false;
      
            if(Fivexplode()){
                explode = true;
                ss.playsound(2);
            }
         
            if (!explode){
                grid.spawnFigure(nextballs[0]);
                grid.spawnFigure(nextballs[1]);
                grid.spawnFigure(nextballs[2]);
                nextballs[0] = rand()%7+1;
                nextballs[1] = rand()%7+1;
                nextballs[2] = rand()%7+1;
                if (spawnObelisk){
                    unsigned which = rand()%3;
                    nextballs[which] = 8;
                    spawnObelisk = false;
                }

                if (spawnCoS){
                    spawnCoS = false;
                    unsigned which = rand()%3;
                    nextballs[which] = 9;
                }    

                spawn = true;
                if (Fivexplode()){
                    ss.playsound(2);
                    explode = true;
                }
            }//!explode

        }//buble reached end of it's path
    }//burbulas eina

}

//---------------------
void Singleton::logic(){
#if !(TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
    if (music.playing())
        music.update();
#endif
    
    
    
    switch(gamestate){
        case TITLE: mainMenuLogic(); break;
        case GAME: gameLogic(); break;
        case HIGHSCORE: highScoreLogic(); break;
        case OPTIONS: optionsLogic(); break;
        case ENTERNAME: enterNameLogic(); break;
        case HELP: helpLogic(); break;
    }
    
    touches.oldDown.destroy();
    for (unsigned long i = 0; i < touches.down.count(); i++ ){
        Vector3D v = touches.down[i];
        touches.oldDown.add(v);
    }
    touches.up.destroy();
    touches.down.destroy();
    touches.move.destroy();
}
//-------------------------
void Singleton::destroy(){
    
    touches.up.destroy();
    touches.down.destroy();
    touches.move.destroy();
    touches.oldDown.destroy();
    highscores.destroy();
    onlineScoreRegular.destroy();
    victims.destroy();
    
#if (!TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
    music.stop();
    music.release();
#endif
    ss.exit();
    
    
    pics.destroy();
}


