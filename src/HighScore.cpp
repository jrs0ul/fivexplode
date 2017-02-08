#include "HighScore.h"

#include <cstdio>
#include "gui/Text.h"

//-------------------------------
void HighScores::load(const char* path, unsigned count){
	FILE* failas;

	if ((failas=fopen(path,"rb"))==0){
      failas=fopen(path,"wb");
	  fclose(failas);
	}
	else{
	  
	  for (unsigned i = 0;i < count; i++){
		  Score scoras;
		  if (!feof(failas)){
			fread(&scoras,sizeof(Score),1,failas);
			if ((strcmp(scoras.name,"") != 0) && (scoras.score > 0))
				data.add(scoras);
		  }
		  else break;
	  }
	  fclose(failas);
	}

}
//------------------
void HighScores::write(const char* path, unsigned count){
	FILE* failas;

	failas = fopen(path,"wb+");
    if (!failas)
        return;
	if (count > data.count())
		count=data.count();
	for (unsigned i = 0; i < count; i++){  

		fwrite(&data[i],sizeof(Score),1,failas);
	}
	fclose(failas);

}

//--------------------
void HighScores::addScore(const char* name, long score, int maxcount){
    
    if (score){
        Score tmp(name, score);
	
        unsigned i = 0;

        if (data.count()){
            while ((score <= data[i].score) && (i<=data.count()))
                i++;
        }
    
        if (data.count() < (unsigned)maxcount)
            data.add(tmp);
	 
        if (i < data.count()){
            for (unsigned a = data.count() - 1; a > i; a--){
                strcpy(data[a].name, data[a - 1].name);
                data[a].score = data[a - 1].score;
            }
            strcpy(data[i].name, tmp.name);
            data[i].score = tmp.score;
            lastHighscoreIndex = i;
        } 
        else{
            lastHighscoreIndex = i - 1;
            
        }
    }
}

//-------------------

void HighScores::display(PicsContainer & pics, unsigned font, int count,
                         int x, int y, int startNumber){
 
  char buf[150];
  
  if (count > (int)data.count())
	  count = data.count();
  for (int i = 0; i < count;i++){
      int len = strlen(data[i].name);
      char buf2[150];
      sprintf(buf2, "%s2d. %ss %s%dld", "%", "%", "%", 20-len);
	  sprintf(buf, buf2, i+startNumber, data[i].name,data[i].score);
      
      COLOR c = ( i == lastHighscoreIndex) ? COLOR(1,1,0) : COLOR( 1,1,1);
      
      WriteShadedText(x, y+i*18, pics, font, buf, 1.0f, 1.0f, c);
  }
   
}
//-----------------------------
void HighScores::destroy(){
	data.destroy();
}

//-----------------------------
Score HighScores::getScore(int index){
	Score sc;
	if ((index>=0)&&(index < (int)data.count())){
		strcpy(sc.name,data[index].name);
		sc.score=data[index].score;
	}

	return sc;
}

