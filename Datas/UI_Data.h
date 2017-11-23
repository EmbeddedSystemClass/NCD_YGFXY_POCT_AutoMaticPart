#ifndef UI_D_H
#define UI_D_H

#include	"MyStack.h"
#include	"Define.h"
#include	"Intent.h"

MyRes startActivity(MyRes (* pageCreate)(Activity * thizactivity, Intent * pram), Intent * pram,
	MyRes (* childPageCreate)(Activity * thizactivity, Intent * pram));
MyRes backToActivity(char * pageName);
MyRes backToFatherActivity(void);
void destroyTopActivity(void);
char * getFatherActivityName(void);
char * getCurrentActivityName(void);
bool checkFatherActivityIs(char * pageName);
MyRes gotoChildActivity(Intent * pram,
	MyRes (* childPageCreate)(Activity * thizactivity, Intent * pram));
	
void InitActivity(Activity * activity, char * activityName, void (* pageStart)(void), 
	void (* pageInput)(unsigned char *pbuf , unsigned short len), 
	void (* pageFresh)(void),
	void (* pageHide)(void),
	void (* pageResume)(void),
	void (* pageDestroy)(void));

void activityInputFunction(unsigned char *pbuf , unsigned short len);
void activityFreshFunction(void);
	
#endif

