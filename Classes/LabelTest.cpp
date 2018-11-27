#include "LabelTest.h"

CLabelTest::CLabelTest()
{
}

CLabelTest::~CLabelTest()
{
}


Scene* CLabelTest::createScene()
{
	return CLabelTest::create();
}


bool CLabelTest::init()
{
	if (!Scene::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();

	Label* pLabel1 = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
	pLabel1->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 100);
	this->addChild(pLabel1);

	Label* pLabel2 = Label::createWithCharMap("ui/numyellow-hd.png", 44, 40, 46);
	pLabel2->setString("02");
	
	pLabel2->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(pLabel2);
	return true;
}
