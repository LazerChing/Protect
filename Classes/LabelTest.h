#ifndef __LABELTEST_H__
#define __LABELTEST_H__
#include "cocos2d.h"
USING_NS_CC;
class CLabelTest : Scene
{
public:
	CLabelTest();
	~CLabelTest();
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(CLabelTest);
private:

};

#endif