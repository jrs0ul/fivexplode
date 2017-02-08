#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <cstring>
#include "DArray.h"
#include "TextureLoader.h"


//------------------------------------
struct Score{
	char name[11];
	long score;

	Score(){
		strncpy(name, "", 11);
		score=0;
	}
	Score(const char* _name, long _score){
		strncpy(name, _name, 11);
		score=_score;
	}
	
};
//----------------------------------------
class HighScores{
	DArray<Score> data;
    int lastHighscoreIndex;
public:
    
    HighScores(){ lastHighscoreIndex = -1;}
	Score getScore(int index);
	void load(const char* path, unsigned count);
	void write(const char* path, unsigned count);
	void addScore(const char* name, long score, int maxcount);
	void display(PicsContainer & pics, unsigned font, int count, int x, int y, int startNumber = 1);
    unsigned long count(){return data.count();}
	void destroy();
    void resetHighscoreIndex(){lastHighscoreIndex = -1;}
};




#endif //HIGHSCORE_H