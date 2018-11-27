#include "Object.h"
#include "GameScene.h"

CObject::CObject()
{
	m_nBeginTime = clock();
	m_bIsSigned = false;
	m_nUUID = 0;
}

CObject::~CObject()
{
}

CObject* CObject::createWithData(sObjectDt* pData)
{
	CObject* pObject = new CObject();
	if (nullptr != pObject && pObject->initWithData(pData))
	{
		pObject->autorelease();
		return pObject;
	}
	CC_SAFE_DELETE(pObject);
	return nullptr;
}

bool CObject::initWithData(sObjectDt* pData)
{
	if (!Sprite::initWithSpriteFrameName(pData->strPng))
	{
		return false;
	}
	this->m_strID = pData->strID;
	this->m_nCoin = pData->nCoin;
	this->m_nHp = pData->nHp;
	this->m_nMaxHp = m_nHp;
	this->m_strName = pData->strName;
	this->m_strPng = pData->strPng;
	this->m_strCoinPng = pData->strCoinPng;
	this->setName("object");

	//创建血条
	//设置进度条背景
	m_pHpBg = Sprite::createWithSpriteFrameName("MonsterHP02.png");
	this->addChild(m_pHpBg);
	m_pHpBg->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height));
	//刚出现隐藏
	m_pHpBg->setVisible(false);

	//设置进度条
	Sprite* pTimer = Sprite::createWithSpriteFrameName("MonsterHP01.png");
	m_pHpBar = ProgressTimer::create(pTimer);
	this->addChild(m_pHpBar);
	m_pHpBar->setMidpoint(Vec2(0, 0.5));
	m_pHpBar->setType(ProgressTimer::Type::BAR);
	m_pHpBar->setBarChangeRate(Vec2(1, 0));
	m_pHpBar->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height));
	//这个放在最后, 放前面可能刚开始不显示
	m_pHpBar->setPercentage(100);
	m_pHpBar->setVisible(false);

	//显示标记的精灵
	m_pSignSprite = Sprite::create();
	m_pSignSprite->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height));
	//隐藏
	m_pSignSprite->setVisible(false);
	this->addChild(m_pSignSprite);
	Animation* pSignAnimation = Animation::create();
	pSignAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("point01.png"));
	pSignAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("point02.png"));
	pSignAnimation->setDelayPerUnit(0.1f);
	pSignAnimation->setLoops(-1);
	Animate* pSignAnimate = Animate::create(pSignAnimation);
	m_pSignSprite->runAction(pSignAnimate);	

	this->scheduleUpdate();

	return true;
}

bool CObject::damage(CBullet* pBullet)
{
	m_vecHitBulUUID.push_back(pBullet->getUUID());
	m_nHp -= pBullet->getAck();
	if (0 >= m_nHp)
	{
		m_pHpBar->setPercentage(0);
		return true;
	}
	//设置血量
	float fPercentage = 100 * m_nHp / m_nMaxHp;
	m_pHpBar->setPercentage(fPercentage);
	m_pHpBg->setVisible(true);
	m_pHpBar->setVisible(true);
	//更新时间
	m_nBeginTime = clock();
	return false;
}

void CObject::update(float fDelta)
{
	if (1000 < clock() - m_nBeginTime) //1s没受到攻击就隐藏血条
	{
		m_pHpBg->setVisible(false);
		m_pHpBar->setVisible(false);
	}
}

bool CObject::isAttacked(CBullet* pBullet)
{
	for (int nUUID : m_vecHitBulUUID)
	{
		if (nUUID == pBullet->getUUID())
		{
			return true;
		}
	}
	return false;
}

void CObject::setIsSigned(bool bIsSign)
{
	m_bIsSigned = bIsSign;
	if (m_bIsSigned)
	{
		//显示标记
		m_pSignSprite->setVisible(true);
	}
	else
	{
		//不显示标记
		m_pSignSprite->setVisible(false);
	}
}

void CObject::removeSelf()
{
	//如果是被标记的，通知清除标记
	if (this->m_bIsSigned)
	{
		CGameScene::getInstance()->clearSignedNode();
	}
	this->removeFromParent();
}
