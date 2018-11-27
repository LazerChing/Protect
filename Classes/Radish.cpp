#include "Radish.h"
#include <string>
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;;
CRadish::CRadish()
:m_nCurLife(10)
{
}

CRadish::~CRadish()
{
	m_pShakeAnimate->release();
}

bool CRadish::init()
{
	if (!Node::init())
	{
		return false;
	}

	m_strNormalPng = "hlb10.png";
	m_pRadishSprite = Sprite::createWithSpriteFrameName(m_strNormalPng);
	m_pRadishSprite->setAnchorPoint(Vec2(0.5, 0));
	m_pRadishSprite->setPosition(15, 0);
	this->addChild(m_pRadishSprite);
	
	//不能直接在这先得到矩形盒，还没挂载到父节点上，应该在用的时候再计算
	//得到图片大小
	/*Rect picRect = m_pRadishSprite->getTextureRect();
	Rect oldBox = m_pRadishSprite->getBoundingBox();
	Vec2 oldPos = oldBox.origin;
	Vec2 worldPos = m_pRadishSprite->convertToWorldSpaceAR(oldPos);
	m_RadishWorldBox = Rect(worldPos, picRect.size);*/


	m_pLifeSprite = Sprite::createWithSpriteFrameName("BossHP10.png");
	m_pLifeSprite->setPosition(80, 50);
	this->addChild(m_pLifeSprite);

	Animation* pShakeAnimation = Animation::create();
	string strName = "";
	for (int i = 11; i <= 18; ++i)
	{
		strName = StringUtils::format("hlb%d.png", i);
		pShakeAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(strName));
	}
	pShakeAnimation->setDelayPerUnit(0.03f);
	
	m_pShakeAnimate = Animate::create(pShakeAnimation);
	m_pShakeAnimate->retain();
	
	
	return true;
}

bool CRadish::damage()
{
	m_nCurLife--;
	if (0 >= m_nCurLife)
	{
		return true;
	}
	string strLifePng = StringUtils::format("BossHP%02d.png", m_nCurLife);
	m_pLifeSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(strLifePng));

	string strRadishPng = StringUtils::format("hlb%d.png", m_nCurLife);
	if (7 == m_nCurLife)
	{
		strRadishPng = "hlb6.png";
	}
	if (5 == m_nCurLife)
	{
		strRadishPng = "hlb4.png";
	}
	m_pRadishSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(strRadishPng));
	return false;
}

void CRadish::playShakeAnimate()
{
	//不是满血就返回
	if (m_nCurLife < 10)
	{
		return;
	}
	CallFunc* pPlayOver = CallFunc::create([=](){
		m_pRadishSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(m_strNormalPng));
	});
	m_pRadishSprite->runAction(Sequence::create(m_pShakeAnimate, pPlayOver, nullptr));
	//播放音效
	int nRand = rand() % 3 + 1;
	string strFile = StringUtils::format("Music/effect/carrot%d.mp3", nRand);
	SimpleAudioEngine::getInstance()->playEffect(strFile.c_str());
}

Rect CRadish::getSpriteWorldBox()
{
	Rect picRect = m_pRadishSprite->getTextureRect();
	Rect oldBox = m_pRadishSprite->getBoundingBox();
	Vec2 oldPos = oldBox.origin;
	Vec2 worldPos = m_pRadishSprite->convertToWorldSpaceAR(oldPos);
	Rect newBox = Rect(worldPos, picRect.size);
	return newBox;
}
