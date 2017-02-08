#ifdef WIN32
    #ifdef  _MSC_VER
        #define _CRT_SECURE_NO_DEPRECATE 1
        #pragma comment(lib,"SDL.lib")
        #pragma comment(lib,"SDLmain.lib")
        #pragma comment(lib,"OpenGl32.lib")
        #pragma comment(lib,"GLU32.lib")
        #pragma comment(lib,"openal32.lib")
        #pragma comment(lib,"libcurl_imp.lib")

        #ifdef _DEBUG
            #pragma comment(lib,"ogg_d.lib")
            #pragma comment(lib,"vorbis_d.lib")
            #pragma comment(lib,"vorbisfile_d.lib")
        #else
            #pragma comment(lib,"ogg.lib")
            #pragma comment(lib,"vorbis.lib")
            #pragma comment(lib,"vorbisfile.lib")
        #endif
    #endif
#endif



#include <ctime>
#include "CppSingleton.h"
#include "Threads.h"
#include "SDLVideo.h"
#include "Utils.h"
#ifdef __APPLE__
#include <limits.h>
#include <unistd.h>

#include <CoreFoundation/CoreFoundation.h>
#endif
#include <string>
#include <curl/curl.h>


const int WIDTH = 320;
const int HEIGHT = 480;

bool Works = true;
SDLVideo SDL;
SDL_Joystick *Joy = 0;
int JoyX = 0;
int JoyY = 0;
int MouseX, MouseY; //relative mouse coords
int _MouseX, _MouseY;
unsigned long tick;
static std::string buffer;


Singleton Game;


void ConfGL(){
    Game.init();
}
//-----------------
void RenderScreen(){
    Game.render();
    glFlush();

    SDL_GL_SwapBuffers( );
}
//-----------------
void Logic(){
    Game.logic();
}
//-----------------
static void  process_events(){
    
    SDL_Event event;

    while( SDL_PollEvent( &event ) ) {

        Game.globalKEY = -1;
        switch( event.type ) {

        case SDL_KEYDOWN:{

            Game.globalKEY = (char)event.key.keysym.unicode;
            switch( event.key.keysym.sym ) {
                default:{}
            }
        } break;
        case SDL_MOUSEBUTTONUP:{
            Vector3D pos(event.button.x, event.button.y, 0);
            Game.touches.up.add(pos);
            Game.touches.allfingersup = true;
        } break;
        case SDL_MOUSEBUTTONDOWN:{
            Vector3D pos(event.button.x, event.button.y, 0);
            Game.touches.down.add(pos);
            Game.touches.allfingersup = false;

        } break;
        case SDL_MOUSEMOTION:{
            if(SDL_GetMouseState(0, 0)&SDL_BUTTON_LMASK){
                Vector3D pos(event.button.x, event.button.y, 0);
                Game.touches.move.add(pos);
                Game.touches.allfingersup = false;
            }
        }break;


        case SDL_QUIT:{
            Game.Exit = true;
        }break;
    
        }

    }
}
//--------------------
void checkKeys(){
    Uint8 * keys;
    int JoyNum = 0;
    
    keys = SDL_GetKeyState ( NULL );
    JoyNum = SDL_NumJoysticks();

    if (JoyNum > 0) {

        //printf("%s\n", SDL_JoystickName(0));

        SDL_JoystickUpdate ();
        JoyX = SDL_JoystickGetAxis(Joy, 0);
        JoyY = SDL_JoystickGetAxis(Joy, 1);
    }

    int bm;
    bm = SDL_GetRelativeMouseState ( &MouseX,&MouseY );
    SDL_GetMouseState(&_MouseX, &_MouseY);

    Game.gamepad.v[0] = JoyX/ 1000.0f;
    Game.gamepad.v[1] = JoyY/ 1000.0f;

    //Game.gamepad.v[0] = MouseX * 10.0f;
    //Game.gamepad.v[1] = MouseY * 10.0f;


    memset(Game.OldKeys, 0, 20);
    memcpy(Game.OldKeys, Game.Keys, 20);
    memset(Game.Keys, 0, 20);

    if ( keys[SDLK_w] )    Game.Keys[0] = 1;
    if ( keys[SDLK_s] )    Game.Keys[1] = 1;
    if ( keys[SDLK_a] )    Game.Keys[2] = 1;
    if ( keys[SDLK_d] )    Game.Keys[3] = 1;
    if ( keys[SDLK_UP] )   Game.Keys[0] = 1;
    if ( keys[SDLK_DOWN])  Game.Keys[1] = 1;
    if ( keys[SDLK_LEFT])  Game.Keys[2] = 1;
    if ( keys[SDLK_RIGHT]) Game.Keys[3] = 1;
    if ( keys[SDLK_SPACE]) Game.Keys[4] = 1;
    if ( keys[SDLK_RETURN]) Game.Keys[5] = 1;
    if ( keys[SDLK_LCTRL]) Game.Keys[6] = 1;
    if ( keys[SDLK_ESCAPE]) Game.Keys[7] = 1;

    if (JoyY/1000 > 1) Game.Keys[9] = 1;
    if (JoyY/1000 < -1) Game.Keys[8] = 1;
    if (JoyX/1000 < -1) Game.Keys[10] = 1;
    if (JoyX/1000 > 1) Game.Keys[11] = 1;


    if (JoyNum){
        if (SDL_JoystickGetButton (Joy, 0)){
            Game.Keys[4] = 1;
            Game.Keys[5] = 1;
        }
        if (SDL_JoystickGetButton (Joy, 1))
            Game.Keys[6] = 1;

        if (SDL_JoystickGetButton (Joy, 2)){
            Game.Keys[7] = 1;
        }
    }
}
//------------------------
static int writer(char *data, size_t size, size_t nmemb,
                  std::string *buffer){

    int result = 0;
    if (buffer){
            buffer->append(data, size * nmemb);
            result = size * nmemb;
    }

        return result;
}

//----------------------------
THREADFUNC sendScore(void * args){

    


    CURLcode result;
    CURL * cu = 0;

    cu = curl_easy_init();
    curl_easy_setopt( cu, CURLOPT_POST, 1);

    char request[1024];
    sprintf(request,"%sccbecc7045106b%d2eba09e27b8a0a5a6",
            Game.highscorePostParams,1-1);
    //puts(request);

    curl_easy_setopt( cu, CURLOPT_POSTFIELDSIZE, strlen(request) );

    curl_easy_setopt( cu, CURLOPT_POSTFIELDS, request ) ;

    char url[255];
    sprintf(url, "http://jrs0ul.com/score/FiveXplodePC.php?mode=add&type=%d", 0);
    curl_easy_setopt(cu, CURLOPT_URL, url);
    curl_easy_setopt(cu, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(cu, CURLOPT_WRITEFUNCTION, writer);
    curl_easy_setopt(cu, CURLOPT_WRITEDATA, &buffer);


    result = curl_easy_perform(cu);
    if (cu){

        //puts(buffer.c_str());
        Game.getHighScore = true;
        Game.gotHighScore = false;
        Game.failToGetScore = false;
        Game.waitForOnlineScoreTics = 0;
        strcpy(Game.highScoreFromServer,"");
        buffer = "";
        curl_easy_cleanup(cu);
    }

    return 0;
}
//-------------------------
THREADFUNC getScore(void * args){


    CURLcode result;
    CURL * cu = 0;

    cu = curl_easy_init();

    char url[255];
    sprintf(url, "http://jrs0ul.com/score/FiveXplodePC.php?mode=get&page=%d&type=%d", Game.onlineScorePage ,Game.onlineScoreType);
    
    curl_easy_setopt(cu, CURLOPT_URL, url);
    curl_easy_setopt(cu, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(cu, CURLOPT_WRITEFUNCTION, writer);
    curl_easy_setopt(cu, CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt(cu, CURLOPT_FAILONERROR, true);
   
    result = curl_easy_perform(cu);
 

    /*FILE* f = fopen("failas.txt","wt+");
    fprintf(f, "buffer size: %d",buffer.size());
    fprintf(f, "buffer contents: %s",buffer.c_str());
    fclose(f);*/
    if (/*strlen(buffer.c_str()) > 0*/!buffer.empty()){
        strncpy(Game.highScoreFromServer, buffer.c_str(), 2024);
        Game.gotHighScore = true; 
    }

    if (cu)
        curl_easy_cleanup(cu);

    return 0;
}




//--------------------
int main( int   argc, char *argv[] ){
    
    srand(time(0));


    char buf[255];
    GetHomePath(buf);
    sprintf(Game.DocumentPath, "%s.fexplode", buf);
    MakeDir(Game.DocumentPath);
#ifdef __APPLE__
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX)){
        puts("Um...Error?");
    }
    CFRelease(resourcesURL);
    chdir(path);
#endif

    SDL.setMetrics(WIDTH, HEIGHT);
    if (!SDL.InitWindow("FiveXplode 1.01", "icon.bmp")){
        Works = false;
    }

    SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    if(SDL_NumJoysticks() > 0){
        Joy = SDL_JoystickOpen(0);
    }
    SDL_EnableUNICODE(SDL_ENABLE);
    ConfGL();
    //LoadExtensions();
    

     while (!Game.Exit){
        if ((SDL_GetTicks() > tick)){

            checkKeys();
            Logic();
            tick = SDL_GetTicks() + 13;
        }
        SDL_Delay(1);

        if (Game.sendScore){
            Thread t;
            t.create(&sendScore, 0);
            Game.sendScore = false;
        }

        if (Game.launchjrs0ul){
            Game.launchjrs0ul = false;
#ifdef __APPLE__
            system("open -a Safari http://jrs0ul.com/en/projects/");
#else
    #ifdef WIN32
            system("explorer http://jrs0ul.com/en/projects/");
    #else
            system("firefox http://jrs0ul.com/en/projects/");
    #endif

#endif
        }

        if (Game.getHighScore){

            Game.getHighScore = true;
            Game.gotHighScore = false;
            Game.failToGetScore = false;
            Game.waitForOnlineScoreTics = 0;
            strcpy(Game.highScoreFromServer,"");
            buffer = "";
            Thread t;

            //puts("let's get some");
            t.create(&getScore, 0);
            Game.getHighScore = false;
        }

        RenderScreen();
        process_events();
    }

    Game.destroy();

    SDL.Quit();


    return 0;
}

