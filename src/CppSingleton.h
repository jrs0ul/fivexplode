#ifndef _CPP_SINGLETON
#define _CPP_SINGLETON


#include "audio/SoundSystem.h"
#include "audio/OggStream.h"
#include "TextureLoader.h"
#include "Vectors.h"
#include "gui/Button.h"
#include "gui/Scrollbar.h"
#include "gui/YesNoDialog.h"
#include "gui/OnScreenBoard.h"
#include "FindPath.h"
#include "SysConfig.h"
#include "TouchData.h"
#include "HighScore.h"
#include "externals.h"
#include "Xml.h"
#include "Grid.h"

#define MENU 1
#define GUI 2
#define BALLS 4
#define BRICK 5
#define DIGITS 6
#define BUTTONS 7
#define BG 12


#define BOARDY 90
#define MAXSCORE 20

enum GameStates {TITLE, GAME, HIGHSCORE,
                 OPTIONS, ENTERNAME, HELP};
class Ball{
public:
    unsigned char x,y;
    unsigned char color;
    int cnt;
};


struct Joystick{
    Vector3D pos;
    float radius;
    
    Joystick(){
        pos = Vector3D(70, 250, 0);
        radius = 50.0;
    }
};


class Singleton{
    
    void ProcessTouchUp(Vector3D * touch);
    void ProcessTouchDown(Vector3D * touch);
    void ProcessTouchMove(Vector3D * touch);
    
    void drawMainMenu();
    void drawHighScore();
    void highScoreLogic();
    void drawGame();
    void gameLogic();
    void mainMenuLogic();
    void drawLevelList();
    void AttachDefaultEntitySounds();
    void GoToNextLevel();
    void throwPowerup();
    
    void helpLogic();
    void drawHelp();
    void drawOptions();
    void optionsLogic();
    
    void makeBomb();
    void runLevel(int lev);
    void resetHighScores();
    
    void enterNameLogic();
    void drawEnterName();
    
    void parseOnlineScores(HighScores& container);
    
    void fetchScore(int type);
    void submitScore(int type);
    
    void setMusicVolume(float vol);
    void playMusic();
    void playNewSong(const char* path);
    
    
    void addScore();

    
    void drawDebugText();


    void GridDraw(int x, int y);
    void PutBall(int x,int y, char cl);
    bool Fivexplode();
    void countscore(int cnt);

    void eliminateballs();
    void eliminateballsCoS();

    bool CanGo(int x, int y);
    void BallClick(_Point p);
    void ResetBoard();
    void drawscore(int x, int y, int number);

    void checkForObelisksV();
    void checkForObelisksH();
    void checkForObelisksLVH();
    void checkForObelisksRVH();
    void CoSGlue(unsigned type, unsigned Color, unsigned &oldColor);
    void addVictim(_Point& p);

    void CoSAnimation();

public:
    bool Exit;
    
    bool ChangeVolume;
    float MusicVolume;
    bool PlayMusic;
    bool PlayNewSong;
    bool StopMusic;
    
    
    SoundSystem ss;
    OggStream music;
    
    PicsContainer pics;
    
    bool useAccel;
    
    bool sendStats;    //send my level stats
    time_t levelStart;
    time_t levelEnd;
    time_t suspendStart;
    time_t suspendEnd;
    double suspendDuration;
    
    
    int scoreBefore;
    int livesBefore;
    int bombsUsed;
    int jumps;
    char previousLevelName[100];
    char statPostParams[255];
    
    bool getHighScore; //fetch me some scores!
    bool gotHighScore; //got some xml shit
    bool sendScore;   //send my scores quickly
    
    bool failToGetScore; 
    int onlineScoreType; //what type of scores
    char highscorePostParams[255]; //post parameters to send
    int waitForOnlineScoreTics; //wait tics until timeout
    char highScoreFromServer[2024]; //xml thingie
    bool submitOnline; //do I have to send my scores online?
    int onlineScorePage;
    
    int GameCenterScore;
    
    SystemConfig sys;
    
    Vector3D pos;
    Vector3D gamepad; //input from real device
    
    HighScores highscores;
    HighScores onlineScoreRegular;
    
    
    char DocumentPath[255];
    
    char songName[255];
    
    
    Joystick joystick;
    TouchData touches;
    unsigned char Keys[20];
    unsigned char OldKeys[20];
    int activeMenuItem;
    int activeGameMode;
    float acc;
    Ball b;
    Grid grid;

    bool spawn;
    int spawnc;
    bool explode;
    int explodec;
    int lines;
    bool gameover;
    bool hidebanner;

    unsigned char nextballs[3];
    
    Button playregular_button;
    Button playrandom_button;
    Button changeName_button;
    
    Button play_button;
    Button resume_button;
    Button highscore_button;
    Button options_button;
    Button howtoplay_button;
    Button resetLevel_button;
    Button backOptions_button;
    Button controlsOptions_button;
    Button resetScore_button;
    Button onlineScorePage_button;
    
    Button helpBack_button;
    Button helpNext_button;
    Button helpPrev_button;
    
    Button regularHighscore_button;
    Button onlineScore_button;
    Button localScore_button;
    Button GameCenter_button;
    
    Button highScoresBack_button;
    Button toMainMenu_button;
    
    Button about_button;
    Button moregames_button;
    Button jrs0ul_button;
    bool launchjrs0ul;
    
    ScrollBar musicVolume_bar;
    ScrollBar sfxVolume_bar;
    ScrollBar accel_bar;
    
    YesNoDialog YesNoDlg;
    
    //-----
    Button bomb_button;
    Button jump_button;
    
    //---
    
    GameStates gamestate;
    bool bmoved; 
    bool goToNextLevel;
    bool fadeOut;
    float alpha;
    
    bool levelCreated;
    
    bool joyPressed;
    
    int TouchedLevelindex;
    
    int helpPage;
    
    OnScreenKeyboard Keyboard;
    char nameToEnter[256];
    
    bool ActivateEditField;
    bool TextEntered;
    int globalKEY;
    //---
    //
    int score;
    int oldScore;
    
    
    bool buyGame;
    
    bool ShowGameCenter;
    
    bool isPirate;
    
    Vector3D oldMoveTouch;
    
    Vector3D oldTouchDown;
    bool menumoved;
    
    int drawCalls;

    bool burbulaseina;
    int bstep;

    Path path;
    DArray<_Point> victims;
    
    bool hasadd;
    bool showBanner;
    bool spawnObelisk;

    bool spawnCoS;
    COLOR CoS;
    int CoSPhase;
    COLOR CoSData[10];
    int gameOverTics;
    _Point pointerON;

    
    
    Singleton(){
        gamestate = TITLE;
        Exit = false;
        alpha = 0.0f;
        fadeOut = false;
        goToNextLevel = false;
        levelCreated = false;
        joyPressed = false;
        helpPage = 0;
        TouchedLevelindex = -1;
        ActivateEditField = false;
        TextEntered = false;
        
        launchjrs0ul = false;
        acc = 36.0f;
        
        getHighScore = false;
        gotHighScore= false;
        waitForOnlineScoreTics = 0;
        onlineScoreType = 0;
        onlineScorePage = 0;
        sendScore = false;
        sendStats = false;
        submitOnline = false;
        failToGetScore = false;
        
        ChangeVolume = false;
        MusicVolume = 0.0f;
        PlayMusic = false;
        StopMusic = false;
        PlayNewSong = false;
        
        buyGame = false;
        GameCenterScore = 0;
        ShowGameCenter = false;
        isPirate = false;
        
        suspendDuration = 0;
        activeMenuItem = 1;
        activeGameMode = 0;
        Exit = false;
        oldMoveTouch = Vector3D(0,0,0);
        menumoved = false;
        drawCalls = 0;
       
        spawn = false;
        spawnc = 0;
        explode = false;
        explodec = 0;
        burbulaseina = false;
        bstep = 0;
        lines = 0;
        gameover = false;
        hidebanner = false;
        hasadd = false;
        showBanner = false;
        bmoved = false;
        spawnObelisk = false;
        spawnCoS = false;
        CoS = COLOR(1,0,0,0.9f);
        CoSPhase = 1;
        CoSData[0] = COLOR(1,0,0); 
        CoSData[1] = COLOR(1,1,0); 
        CoSData[2] = COLOR(0,1,0); 
        CoSData[3] = COLOR(0,0,1); 
        CoSData[4] = COLOR(1,0,1); 
    }
    
    void init();
    void logic();
    void render();
    void destroy();
    
};





#endif //_CPP_SINGLETON

