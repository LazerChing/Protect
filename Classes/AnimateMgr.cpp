#include "AnimateMgr.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
CAnimateMgr::CAnimateMgr()
{
}

CAnimateMgr::~CAnimateMgr()
{
	m_pEnemyBombAnimate->release();
}

bool CAnimateMgr::init()
{
	if (!Node::init())
	{
		return false;
	}

	Animation* pEnemyBombAnimation = Animation::create();
	for (int i = 1; i <= 5; ++i)
	{
		string strName = StringUtils::format("air%02d.png", i);
		pEnemyBombAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(strName));
	}
	pEnemyBombAnimation->setDelayPerUnit(0.08f);
	m_pEnemyBombAnimate = Animate::create(pEnemyBombAnimation);
	m_pEnemyBombAnimate->retain();

	return true;
}

void CAnimateMgr::playAnimateByVecPng(Vec2 pos, vector<string>& vecPngs)
{
	Animation* pAnimation = Animation::create();
	for (int i = 0; i < vecPngs.size(); i++)
	{
		pAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(vecPngs[i]));
	}
	pAnimation->setDelayPerUnit(0.05f);
	Animate* pAnimate = Animate::create(pAnimation);
	Sprite* pSprite = Sprite::create();
	pSprite->setPosition(pos);
	this->addChild(pSprite);
	pSprite->runAction(Sequence::create(pAnimate, RemoveSelf::create(), nullptr));
}

void CAnimateMgr::playEnemyBombAnimate(Vec2 pos, string strCoin)
{
	Sprite* pSprite = Sprite::create();
	pSprite->setPosition(pos);
	this->addChild(pSprite);
	pSprite->runAction(Sequence::createWithTwoActions(m_pEnemyBombAnimate, RemoveSelf::create()));

	Sprite* pCoinSprite = Sprite::createWithSpriteFrameName(strCoin);
	pCoinSprite->setPosition(pos);
	this->addChild(pCoinSprite);
	MoveBy* pMoveBy = MoveBy::create(0.5f, Vec2(0, 70));
	EaseOut* pEaseOut = EaseOut::create(pMoveBy, 2.5f);
	pCoinSprite->runAction(Sequence::create(pEaseOut, RemoveSelf::create(), nullptr));
}

void CAnimateMgr::playFinalWaveAnimate()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Sprite* pSprite = Sprite::createWithSpriteFrameName("finalwave.png");
	pSprite->setPosition(-200, visibleSize.height / 2);
	this->addChild(pSprite);

	MoveTo* pMoveIn = MoveTo::create(0.15f, Vec2(visibleSize.width / 2, visibleSize.height / 2));
	DelayTime* pDelay = DelayTime::create(1.5f);
	MoveTo* pMoveOut = MoveTo::create(0.15f, Vec2(visibleSize.width + 200, visibleSize.height / 2));

	Sequence* pSeq = Sequence::create(pMoveIn, pDelay, pMoveOut, RemoveSelf::create(), nullptr);
	pSprite->runAction(pSeq);
	//²¥·ÅÒôÐ§
	SimpleAudioEngine::getInstance()->playEffect("Music/effect/Finalwave.mp3");
}

void CAnimateMgr::createArrow(Vec2 pos, float fRotate)
{
	Sprite* pArrow = Sprite::createWithSpriteFrameName("arrow.png");
	this->addChild(pArrow);
	pArrow->setPosition(pos);
	pArrow->setRotation(fRotate);
	FadeOut* pFadeOut = FadeOut::create(1.f);
	FadeIn* pFadeIn = FadeIn::create(1.0f);
	pArrow->runAction(Sequence::create(
		pFadeOut, pFadeIn, pFadeOut, pFadeIn, pFadeOut,
		RemoveSelf::create(), nullptr));
}
