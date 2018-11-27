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
	//���ű�������
	SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/bg/Accomplish.mp3",true);
	createBg();
	
	return true;
}

void CGameMenu::menuCallBack(Ref* pSender)
{
	//������Ч
	SimpleAudioEngine::getInstance()->playEffect("Music/effect/Select.mp3");
	MenuItemImage* pImage = (MenuItemImage*)pSender;
	if (0 == strcmp(pImage->getName().c_str(),"btnAdventure"))
	{
		//�л�����
		TransitionMoveInR* pTrans = TransitionMoveInR::create(0.5f, CGameTheme::createScene());
		//Director::getInstance()->pushScene(pTrans);
		Director* pDir = Director::getInstance();
		Director::getInstance()->replaceScene(pTrans);
	}
	else if (0 == strcmp(pImage->getName().c_str(), "btnBoss"))
	{
		//�ж���û�������������˾͵�����ʾ
		if (m_pBossLooked->isVisible()) //�ɼ���ʾ������
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

	//����ð�հ�ť
	MenuItemImage* m_pBtnAdventure = MenuItemImage::create("", "", 
		CC_CALLBACK_1(CGameMenu::menuCallBack, this));
	m_pBtnAdventure->setPosition(182, 84);
	m_pBtnAdventure->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_adventure_normal_CN.png"));
	m_pBtnAdventure->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_adventure_pressed_CN.png"));
	m_pBtnAdventure->setName("btnAdventure");

	//����bossģʽ
	MenuItemImage* m_pBtnBoss = MenuItemImage::create("", "",
		CC_CALLBACK_1(CGameMenu::menuCallBack, this));
	m_pBtnBoss->setPosition(m_visibleSize.width / 2, 84);
	m_pBtnBoss->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_boss_normal_CN.png"));
	m_pBtnBoss->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_boss_pressed_CN.png"));
	m_pBtnBoss->setName("btnBoss");

	//������
	MenuItemImage* m_pBtnMonsterHome = MenuItemImage::create("", "",
		CC_CALLBACK_1(CGameMenu::menuCallBack, this));
	m_pBtnMonsterHome->setPosition(m_visibleSize.width - 182, 84);
	m_pBtnMonsterHome->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_nest_normal_CN.png"));
	m_pBtnMonsterHome->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_nest_pressed_CN.png"));
	m_pBtnMonsterHome->setName("btnMonsterHome");	

	Menu* pMenu = Menu::create(m_pBtnAdventure,m_pBtnBoss,m_pBtnMonsterHome, nullptr);
	pMenu->setPosition(Vec2::ZERO);
	this->addChild(pMenu);

	//����ͼƬ
	m_pBossLooked = Sprite::createWithSpriteFrameName("locked.png");
	this->addChild(m_pBossLooked);
	m_pBossLooked->setPosition(Vec2(613, 62));

	//�����ᶯ����Щ����

	//С��
	m_pSmallCloud = Sprite::createWithSpriteFrameName("cloud1.png");
	this->addChild(m_pSmallCloud);
	m_pSmallCloud->setPosition(-100, m_visibleSize.height - 67);
	//�����ƶ�
	CallFunc* pSmallCloudMove = CallFunc::create(CC_CALLBACK_0(CGameMenu::smallCloudMove, this));
	MoveBy* pSmallCloudMoveBy = MoveBy::create(12, Vec2(1100, 0));
	m_pSmallCloud->runAction(Sequence::createWithTwoActions(pSmallCloudMoveBy, pSmallCloudMove));

	//����
	m_pBigCloud = Sprite::createWithSpriteFrameName("cloud2.png");
	this->addChild(m_pBigCloud);
	m_pBigCloud->setPosition(-150, m_visibleSize.height - 140);
	//�����ƶ�
	CallFunc* pBigCloudMove = CallFunc::create(CC_CALLBACK_0(CGameMenu::bigCloudMove, this));
	MoveBy* pBigCloudMoveBy = MoveBy::create(25, Vec2(1300, 0));
	m_pBigCloud->runAction(Sequence::createWithTwoActions(pBigCloudMoveBy, pBigCloudMove));


	//���Ҷ��
	Sprite* pLeftLeaf = Sprite::createWithSpriteFrameName("leaf-1.png");
	this->addChild(pLeftLeaf);
	pLeftLeaf->setAnchorPoint(Vec2(1, 0));
	pLeftLeaf->setPosition(m_visibleSize.width / 2 - 15, m_visibleSize.height / 2 + 90);
	

	//�ұ�Ҷ��
	m_pRightLeaf = Sprite::createWithSpriteFrameName("leaf-3.png");
	this->addChild(m_pRightLeaf);
	m_pRightLeaf->setAnchorPoint(Vec2(0, 0));
	m_pRightLeaf->setPosition(m_visibleSize.width / 2 + 15, m_visibleSize.height / 2 + 90);

	//�м�Ҷ��
	m_pMidLeaf = Sprite::createWithSpriteFrameName("leaf-2.png");
	this->addChild(m_pMidLeaf);
	m_pMidLeaf->setAnchorPoint(Vec2(0.5, 0));
	m_pMidLeaf->setPosition(m_visibleSize.width / 2, m_visibleSize.height / 2 + 90);	

	//���ö�ʱ�� ÿ��8s�м�Ҷ���˶�һ��
	this->schedule(CC_CALLBACK_1(CGameMenu::midLeafMove, this), 8.f, "midLeafMove");

	
	//���ĺ��ܲ�
	Sprite* pCarrot = Sprite::createWithSpriteFrameName("carrot.png");
	this->addChild(pCarrot);
	pCarrot->setPosition(m_visibleSize.width / 2, m_visibleSize.height / 2 + 50);

	//�����ܲ��ĸ���
	Sprite* pMainBg = Sprite::createWithSpriteFrameName("mainbg_CN.png");
	this->addChild(pMainBg);
	pMainBg->setPosition(m_visibleSize.width / 2, m_visibleSize.height / 2);

	//����
	Sprite* pFlyBird = Sprite::createWithSpriteFrameName("bird.png");
	this->addChild(pFlyBird);
	pFlyBird->setPosition(Vec2(210, 470));
	MoveBy* pMoveBy = MoveBy::create(1.5f, Vec2(0, 80));
	FiniteTimeAction* pReverse = pMoveBy->reverse();
	//���������˶�
	pFlyBird->runAction(RepeatForever::create(Sequence::createWithTwoActions(pMoveBy, pReverse)));
	
}

void CGameMenu::okCallBack(Ref* pSender)
{
	//����ʾ������
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
	//�ڶ�����
	RotateBy* pMidRotate = RotateBy::create(0.08f, 15);
	FiniteTimeAction* pMidRotateRev = pMidRotate->reverse();
	//3����ұ�Ҷ��ִ�ж���
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
	//�����Ĵ��ڱ���
	Sprite* pBossExplainBg = Sprite::createWithSpriteFrameName("popup_bg.png");
	//�����Ĵ�������
	Sprite* pBossExplainTxt = Sprite::createWithSpriteFrameName("boss_locked_CN.png");
	//������Ը��ڵ��λ��
	pBossExplainTxt->setPosition(120, 64);

	//��� ֪����
	//��� ֪����
	MenuItemImage* pBtnOK = MenuItemImage::create("", "",
		[=](Ref* pSender){
		m_pPopUpLayer->removeFromParent();
		//������Ч
		SimpleAudioEngine::getInstance()->playEffect("Music/effect/Select.mp3");
	});
	//������Ը��ڵ��λ��
	pBtnOK->setPosition(70, -90);
	pBtnOK->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("popup_btnbg_normal.png"));
	pBtnOK->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("popup_btnbg_pressed.png"));
	//����Ҫ��ʾ�����֣�֪���ˣ�
	Sprite* pOkTxt = Sprite::createWithSpriteFrameName("popup_btnbg_CN.png");
	pOkTxt->setPosition(70, -90);
	Menu* pMenuOK = Menu::create(pBtnOK, nullptr);
	pMenuOK->setPosition(Vec2::ZERO);

	m_pPopUpLayer->setContentSize(pBossExplainBg->getContentSize());
	m_pPopUpLayer->addChild(pBossExplainBg);
	m_pPopUpLayer->addChild(pBossExplainTxt);
	m_pPopUpLayer->addChild(pMenuOK);
	m_pPopUpLayer->addChild(pOkTxt);

	//���ô�����͸
	EventListenerTouchOneByOne* pListener = EventListenerTouchOneByOne::create();
	pListener->onTouchBegan = [=](Touch* pTouch, Event* pEvent)
	{
		return true;
	};
	//������͸
	pListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, m_pPopUpLayer);
}
