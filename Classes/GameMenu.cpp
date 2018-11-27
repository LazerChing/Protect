#include "GameMenu.h"
#include "GameLevel.h"
#include "GameTheme.h"

CGameMenu::CGameMenu()
{
	m_pPopUpLayer = nullptr;
}

CGameMenu::~CGameMenu()
{
}

Scene* CGameMenu::createScene()
{
	return CGameMenu::create();
}

bool CGameMenu::init()
{
	if (!Scene::init())
	{
		return false;
	}
	m_visibleSize = Director::getInstance()->getVisibleSize();
	//播放背景音乐
	SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/bg/Accomplish.mp3",true);
	createBg();
	
	return true;
}

void CGameMenu::menuCallBack(Ref* pSender)
{
	//播放音效
	SimpleAudioEngine::getInstance()->playEffect("Music/effect/Select.mp3");
	MenuItemImage* pImage = (MenuItemImage*)pSender;
	if (0 == strcmp(pImage->getName().c_str(),"btnAdventure"))
	{
		//切换场景
		TransitionMoveInR* pTrans = TransitionMoveInR::create(0.5f, CGameTheme::createScene());
		//Director::getInstance()->pushScene(pTrans);
		Director* pDir = Director::getInstance();
		Director::getInstance()->replaceScene(pTrans);
	}
	else if (0 == strcmp(pImage->getName().c_str(), "btnBoss"))
	{
		//判断有没有上锁，上锁了就弹出提示
		if (m_pBossLooked->isVisible()) //可见表示上锁了
		{
			createPopUpLayer();
		}
	}
	else if (0 == strcmp(pImage->getName().c_str(), "btnMonsterHome"))
	{
	}
}

void CGameMenu::createBg()
{
	Sprite* pBg = Sprite::createWithSpriteFrameName("mainbg.png");
	this->addChild(pBg);
	pBg->setPosition(m_visibleSize.width / 2, m_visibleSize.height / 2);

	//设置冒险按钮
	MenuItemImage* m_pBtnAdventure = MenuItemImage::create("", "", 
		CC_CALLBACK_1(CGameMenu::menuCallBack, this));
	m_pBtnAdventure->setPosition(182, 84);
	m_pBtnAdventure->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_adventure_normal_CN.png"));
	m_pBtnAdventure->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_adventure_pressed_CN.png"));
	m_pBtnAdventure->setName("btnAdventure");

	//设置boss模式
	MenuItemImage* m_pBtnBoss = MenuItemImage::create("", "",
		CC_CALLBACK_1(CGameMenu::menuCallBack, this));
	m_pBtnBoss->setPosition(m_visibleSize.width / 2, 84);
	m_pBtnBoss->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_boss_normal_CN.png"));
	m_pBtnBoss->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_boss_pressed_CN.png"));
	m_pBtnBoss->setName("btnBoss");

	//怪物窝
	MenuItemImage* m_pBtnMonsterHome = MenuItemImage::create("", "",
		CC_CALLBACK_1(CGameMenu::menuCallBack, this));
	m_pBtnMonsterHome->setPosition(m_visibleSize.width - 182, 84);
	m_pBtnMonsterHome->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_nest_normal_CN.png"));
	m_pBtnMonsterHome->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_nest_pressed_CN.png"));
	m_pBtnMonsterHome->setName("btnMonsterHome");	

	Menu* pMenu = Menu::create(m_pBtnAdventure,m_pBtnBoss,m_pBtnMonsterHome, nullptr);
	pMenu->setPosition(Vec2::ZERO);
	this->addChild(pMenu);

	//锁的图片
	m_pBossLooked = Sprite::createWithSpriteFrameName("locked.png");
	this->addChild(m_pBossLooked);
	m_pBossLooked->setPosition(Vec2(613, 62));

	//创建会动的那些精灵

	//小云
	m_pSmallCloud = Sprite::createWithSpriteFrameName("cloud1.png");
	this->addChild(m_pSmallCloud);
	m_pSmallCloud->setPosition(-100, m_visibleSize.height - 67);
	//不断移动
	CallFunc* pSmallCloudMove = CallFunc::create(CC_CALLBACK_0(CGameMenu::smallCloudMove, this));
	MoveBy* pSmallCloudMoveBy = MoveBy::create(12, Vec2(1100, 0));
	m_pSmallCloud->runAction(Sequence::createWithTwoActions(pSmallCloudMoveBy, pSmallCloudMove));

	//大云
	m_pBigCloud = Sprite::createWithSpriteFrameName("cloud2.png");
	this->addChild(m_pBigCloud);
	m_pBigCloud->setPosition(-150, m_visibleSize.height - 140);
	//不断移动
	CallFunc* pBigCloudMove = CallFunc::create(CC_CALLBACK_0(CGameMenu::bigCloudMove, this));
	MoveBy* pBigCloudMoveBy = MoveBy::create(25, Vec2(1300, 0));
	m_pBigCloud->runAction(Sequence::createWithTwoActions(pBigCloudMoveBy, pBigCloudMove));


	//左边叶子
	Sprite* pLeftLeaf = Sprite::createWithSpriteFrameName("leaf-1.png");
	this->addChild(pLeftLeaf);
	pLeftLeaf->setAnchorPoint(Vec2(1, 0));
	pLeftLeaf->setPosition(m_visibleSize.width / 2 - 15, m_visibleSize.height / 2 + 90);
	

	//右边叶子
	m_pRightLeaf = Sprite::createWithSpriteFrameName("leaf-3.png");
	this->addChild(m_pRightLeaf);
	m_pRightLeaf->setAnchorPoint(Vec2(0, 0));
	m_pRightLeaf->setPosition(m_visibleSize.width / 2 + 15, m_visibleSize.height / 2 + 90);

	//中间叶子
	m_pMidLeaf = Sprite::createWithSpriteFrameName("leaf-2.png");
	this->addChild(m_pMidLeaf);
	m_pMidLeaf->setAnchorPoint(Vec2(0.5, 0));
	m_pMidLeaf->setPosition(m_visibleSize.width / 2, m_visibleSize.height / 2 + 90);	

	//设置定时器 每过8s中间叶子运动一次
	this->schedule(CC_CALLBACK_1(CGameMenu::midLeafMove, this), 8.f, "midLeafMove");

	
	//中心胡萝卜
	Sprite* pCarrot = Sprite::createWithSpriteFrameName("carrot.png");
	this->addChild(pCarrot);
	pCarrot->setPosition(m_visibleSize.width / 2, m_visibleSize.height / 2 + 50);

	//保卫萝卜四个字
	Sprite* pMainBg = Sprite::createWithSpriteFrameName("mainbg_CN.png");
	this->addChild(pMainBg);
	pMainBg->setPosition(m_visibleSize.width / 2, m_visibleSize.height / 2);

	//飞鸟
	Sprite* pFlyBird = Sprite::createWithSpriteFrameName("bird.png");
	this->addChild(pFlyBird);
	pFlyBird->setPosition(Vec2(210, 470));
	MoveBy* pMoveBy = MoveBy::create(1.5f, Vec2(0, 80));
	FiniteTimeAction* pReverse = pMoveBy->reverse();
	//上下来回运动
	pFlyBird->runAction(RepeatForever::create(Sequence::createWithTwoActions(pMoveBy, pReverse)));
	
}

void CGameMenu::okCallBack(Ref* pSender)
{
	//不显示弹出层
	m_pPopUpLayer->setVisible(false);
}

void CGameMenu::rightLeafMove()
{
	RotateBy* pRightRotate = RotateBy::create(0.08f, 15);
	FiniteTimeAction* pRightRotateRev = pRightRotate->reverse();
	ActionInterval* pRightAction = Sequence::create(pRightRotate, pRightRotateRev, pRightRotate, pRightRotateRev, nullptr);
	m_pRightLeaf->runAction(pRightAction);
}

void CGameMenu::midLeafMove(float fDelta)
{
	//摆动两次
	RotateBy* pMidRotate = RotateBy::create(0.08f, 15);
	FiniteTimeAction* pMidRotateRev = pMidRotate->reverse();
	//3秒后右边叶子执行动作
	DelayTime* pMidDelay = DelayTime::create(2.5f);
	CallFunc* pRightMove = CallFunc::create(CC_CALLBACK_0(CGameMenu::rightLeafMove, this));
	ActionInterval* pMidAction = Sequence::create(pMidRotate, pMidRotateRev, pMidRotate, pMidRotateRev, pMidDelay, pRightMove, nullptr);
	m_pMidLeaf->runAction(pMidAction);
}

void CGameMenu::smallCloudMove()
{
	m_pSmallCloud->setPosition(-50, m_visibleSize.height - 67);
	DelayTime* pDelayTime = DelayTime::create(3.0f);
	CallFunc* pSmallCloudMove = CallFunc::create(CC_CALLBACK_0(CGameMenu::smallCloudMove, this));
	MoveBy* pSmallCloudMoveBy = MoveBy::create(12, Vec2(1100, 0));
	m_pSmallCloud->runAction(Sequence::create(pDelayTime, pSmallCloudMoveBy, pSmallCloudMove, nullptr));
}

void CGameMenu::bigCloudMove()
{
	m_pBigCloud->setPosition(-150, m_visibleSize.height - 140);
	DelayTime* pDelayTime = DelayTime::create(4.0f);
	CallFunc* pBigCloudMove = CallFunc::create(CC_CALLBACK_0(CGameMenu::bigCloudMove, this));
	MoveBy* pBigCloudMoveBy = MoveBy::create(25, Vec2(1300, 0));
	m_pBigCloud->runAction(Sequence::create(pDelayTime, pBigCloudMoveBy, pBigCloudMove, nullptr));
}

void CGameMenu::createPopUpLayer()
{
	m_pPopUpLayer = Layer::create();
	this->addChild(m_pPopUpLayer, 10);
	m_pPopUpLayer->setIgnoreAnchorPointForPosition(true);
	m_pPopUpLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_pPopUpLayer->setPosition(m_visibleSize.width / 2, m_visibleSize.height / 2 + 20);
	//弹出的窗口背景
	Sprite* pBossExplainBg = Sprite::createWithSpriteFrameName("popup_bg.png");
	//弹出的窗口文字
	Sprite* pBossExplainTxt = Sprite::createWithSpriteFrameName("boss_locked_CN.png");
	//设置相对父节点的位置
	pBossExplainTxt->setPosition(120, 64);

	//点击 知道了
	//点击 知道了
	MenuItemImage* pBtnOK = MenuItemImage::create("", "",
		[=](Ref* pSender){
		m_pPopUpLayer->removeFromParent();
		//播放音效
		SimpleAudioEngine::getInstance()->playEffect("Music/effect/Select.mp3");
	});
	//设置相对父节点的位置
	pBtnOK->setPosition(70, -90);
	pBtnOK->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("popup_btnbg_normal.png"));
	pBtnOK->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("popup_btnbg_pressed.png"));
	//设置要显示的文字（知道了）
	Sprite* pOkTxt = Sprite::createWithSpriteFrameName("popup_btnbg_CN.png");
	pOkTxt->setPosition(70, -90);
	Menu* pMenuOK = Menu::create(pBtnOK, nullptr);
	pMenuOK->setPosition(Vec2::ZERO);

	m_pPopUpLayer->setContentSize(pBossExplainBg->getContentSize());
	m_pPopUpLayer->addChild(pBossExplainBg);
	m_pPopUpLayer->addChild(pBossExplainTxt);
	m_pPopUpLayer->addChild(pMenuOK);
	m_pPopUpLayer->addChild(pOkTxt);

	//设置触摸穿透
	EventListenerTouchOneByOne* pListener = EventListenerTouchOneByOne::create();
	pListener->onTouchBegan = [=](Touch* pTouch, Event* pEvent)
	{
		return true;
	};
	//触摸穿透
	pListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, m_pPopUpLayer);
}
