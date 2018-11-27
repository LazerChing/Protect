#include "Enemy.h"
#include "GameScene.h"

CEnemy::CEnemy()
:m_nPosIndex(0), m_fCurSpeedRate(1)
{
	m_nBeginTime = clock();
	m_nUUID = 0;
	m_bIsSigned = false;
}

CEnemy::~CEnemy()
{
	m_pAnimate->release();
}

CEnemy* CEnemy::createWithData(sEnemyDt* pData)
{
	CEnemy* pEnemy = new CEnemy();
	if (nullptr != pEnemy && pEnemy->initWithData(pData))
	{
		pEnemy->autorelease();
		return pEnemy;
	}
	else 
	{
		CC_SAFE_DELETE(pEnemy);
		return nullptr;
	}
}

bool CEnemy::initWithData(sEnemyDt* pData)
{
	//初始化图片
	if (!Sprite::initWithSpriteFrameName(pData->vecPngs[0]))
	{
		return false;
	}
	Size enemySize = this->getContentSize();

	this->m_nID = pData->nID;
	this->m_nHp = pData->nHp;
	this->m_nMaxHp = pData->nHp;
	this->m_fNormalSpeed = pData->fSpeed;
	this->m_vecPngs = pData->vecPngs;
	this->m_nCoin = pData->nCoin;
	this->m_strCoin = pData->strCoin;
	this->setName("enemy");
	//初始化动作
	this->initAnimate();
	this->runAction(m_pAnimate);
	//设置位置
	m_vecPathPos = CGameScene::getInstance()->getVecPathPos();
	this->setPosition(m_vecPathPos[0]);

	//精灵和出场动画
	m_pShowSprite = Sprite::createWithSpriteFrameName("mcm01.png");
	m_pShowSprite->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
	this->addChild(m_pShowSprite);

	//创建血条
	//设置进度条背景
	m_pHpBg = Sprite::createWithSpriteFrameName("MonsterHP02.png");
	this->addChild(m_pHpBg);
	m_pHpBg->setPosition(Vec2(enemySize.width / 2, enemySize.height));
	//刚出现隐藏
	m_pHpBg->setVisible(false);

	//设置进度条
	Sprite* pTimer = Sprite::createWithSpriteFrameName("MonsterHP01.png");
	m_pHpBar = ProgressTimer::create(pTimer);
	this->addChild(m_pHpBar);
	m_pHpBar->setMidpoint(Vec2(0, 0.5));
	m_pHpBar->setType(ProgressTimer::Type::BAR);
	m_pHpBar->setBarChangeRate(Vec2(1, 0));
	m_pHpBar->setPosition(Vec2(enemySize.width / 2, enemySize.height));
	//这个放在最后, 放前面可能刚开始不显示
	m_pHpBar->setPercentage(100);
	m_pHpBar->setVisible(false);

	DelayTime* pDelay = DelayTime::create(0.1f);
	ScaleTo* pScale = ScaleTo::create(0.6f, 1.4f);
	CallFuncN* pScaleOver = CallFuncN::create([=](Node* pNode){
		Sprite* pSprite = (Sprite*)pNode;
		pSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("mcm02.png"));
	});

	m_pShowSprite->runAction(Sequence::create(pDelay, pScale, pScaleOver, RemoveSelf::create(), nullptr));

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

	//怪物移动
	this->move();
	this->scheduleUpdate();
	return true;
}

void CEnemy::move()
{
	//判断是否走到了最后
	if (m_nPosIndex <= m_vecPathPos.size() - 2)
	{	
		//判断位置，x坐标，改变方向
		if (0 < m_vecPathPos[m_nPosIndex].x - m_vecPathPos[m_nPosIndex + 1].x)
		{
			//左方向
			this->setFlippedX(true);
		}
		else //右方向
		{
			this->setFlippedX(false);
		}
		//移动距离
		float fDistance = this->getPosition().getDistance(m_vecPathPos[m_nPosIndex + 1]);
		//float fDistance = this->getPosition().getDistance(m_vecPathPos[m_nPosIndex + 1]);
		//移动时间
		float fTime = fDistance/m_fNormalSpeed;
		MoveTo* pMoveTo = MoveTo::create(fTime, m_vecPathPos[m_nPosIndex + 1]);
		CallFunc* pCallFun = CallFunc::create(CC_CALLBACK_0(CEnemy::move, this));
		Sequence* pSeq = Sequence::createWithTwoActions(pMoveTo, pCallFun);
		Speed* pSpeed = Speed::create(pSeq, m_fCurSpeedRate);
		pSpeed->setTag(E_TAG_MOVE);
		this->runAction(pSpeed);
		m_nPosIndex++;
	}
	else
	{
		//萝卜扣血，敌人消失
		if (CGameScene::getInstance()->getRadish()->damage())
		{
			//游戏结束
			CGameScene::getInstance()->gameOver();			
		}
		CGameScene::getInstance()->getAnimateMgr()->playEnemyBombAnimate(this->getPosition(), this->m_strCoin);
		removeSelf();
	}
}

void CEnemy::initAnimate()
{
	float fDelay = 0.2f;
	//左动作
	Animation* pAnimation = Animation::create();
	SpriteFrameCache* pCache = SpriteFrameCache::getInstance();
	pAnimation->addSpriteFrame(pCache->getSpriteFrameByName(m_vecPngs[0]));
	pAnimation->addSpriteFrame(pCache->getSpriteFrameByName(m_vecPngs[1]));
	pAnimation->setDelayPerUnit(fDelay);
	//设置一直循环
	pAnimation->setLoops(-1);
	m_pAnimate = Animate::create(pAnimation);
	//手动保留
	m_pAnimate->retain();
}

void CEnemy::update(float fDelta)
{
	if (1000 < clock() - m_nBeginTime) //1s没受到攻击就隐藏血条
	{
		m_pHpBg->setVisible(false);
		m_pHpBar->setVisible(false);
	}
}

bool CEnemy::damage(CBullet* pBullet)
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

bool CEnemy::isAttacked(CBullet* pBullet)
{
	for (int nUUID:m_vecHitBulUUID)
	{
		if (nUUID == pBullet->getUUID())
		{
			return true;
		}
	}
	return false;
}

void CEnemy::moveHindered(float fRate, float fTime, vector<string>& hitPng)
{
	Speed* pSpeed = (Speed*)this->getActionByTag(E_TAG_MOVE);
	m_fCurSpeedRate = fRate;
	pSpeed->setSpeed(m_fCurSpeedRate);

	Sprite* pSprite = (Sprite*) this->getChildByTag(2);
	if (nullptr == pSprite)
	{
		//受阻动画
		Animation* pAnimation = Animation::create();
		for (int i = 0; i < hitPng.size(); ++i)
		{
			pAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(hitPng[i]));
		}
		pAnimation->setDelayPerUnit(0.1f);
		pAnimation->setLoops(-1);
		Animate* pAnimate = Animate::create(pAnimation);
		Sprite* pSprite = Sprite::create();
		pSprite->setAnchorPoint(Vec2(0.5, 0));
		pSprite->setPosition(this->getContentSize().width / 2, 0);
		pSprite->setTag(2);
		this->addChild(pSprite);
		pSprite->runAction(pAnimate);
	}	

	this->scheduleOnce([=](float fDelta){
		Speed* pSpeed = (Speed*)this->getActionByTag(E_TAG_MOVE);
		m_fCurSpeedRate = 1;
		pSpeed->setSpeed(m_fCurSpeedRate);
		Sprite* pSprite = (Sprite*) this->getChildByTag(2);
		if (nullptr != pSprite)
		{
			pSprite->removeFromParent();
		}
	},fTime,"resumeSpeed");
}

void CEnemy::setIsSigned(bool bIsSign)
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

void CEnemy::removeSelf()
{
	//随机播放一个音效
	int nRand = rand() % 3;
	if (0 == nRand)
	{
		SimpleAudioEngine::getInstance()->playEffect("Music/effect/LandPinkDie.mp3");
	}
	else if (1 == nRand)
	{
		SimpleAudioEngine::getInstance()->playEffect("Music/effect/Fat241.mp3");
	}
	else if (2 == nRand)
	{
		SimpleAudioEngine::getInstance()->playEffect("Music/effect/Fat641.mp3");
	}
	//如果是被标记的，通知清除标记
	if (this->m_bIsSigned)
	{
		CGameScene::getInstance()->clearSignedNode();
	}
	this->removeFromParent();
}
