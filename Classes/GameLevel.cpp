#include "GameLevel.h"
#include "GameScene.h"
#include "GameTheme.h"

CGameLevel::CGameLevel()
{
	m_bIsScale = false;
}

CGameLevel::~CGameLevel()
{
}

Scene* CGameLevel::createScene()
{
	return CGameLevel::create();
}

bool CGameLevel::init()
{
	if (!Scene::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	//��������
	Sprite* pBg = Sprite::createWithSpriteFrameName("ui_S_Bg.png");
	pBg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(pBg);

	//�����ѡ��ؿ�
	Sprite* pSelectPng = Sprite::createWithSpriteFrameName("ss_bg_CN.png");
	pSelectPng->setAnchorPoint(Vec2(0.5, 1));
	pSelectPng->setPosition(visibleSize.width / 2, visibleSize.height);
	this->addChild(pSelectPng);

	//�ײ�����
	Sprite* pBottomCloud = Sprite::createWithSpriteFrameName(CDataMgr::getInstance()->getThemeDtMgr()->getCurThemeData()->cloudPng);
	pBottomCloud->setAnchorPoint(Vec2::ZERO);
	this->addChild(pBottomCloud);

	//����
	MenuItemImage* pMenuBack = MenuItemImage::create("", "",[=](Ref* pSender){
		//Director::getInstance()->popScene();
		//������Ч
		SimpleAudioEngine::getInstance()->playEffect("Music/effect/Select.mp3");
		Director::getInstance()->replaceScene(CGameTheme::createScene());
	});
	pMenuBack->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("ss_back_normal.png"));
	pMenuBack->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("ss_back_pressed.png"));
	pMenuBack->setAnchorPoint(Vec2(0, 1));
	pMenuBack->setPosition(0, visibleSize.height);

	//��ʼ��Ϸ
	MenuItemImage* pMenuStart = MenuItemImage::create("", "",
		CC_CALLBACK_1(CGameLevel::gameStart, this));
	pMenuStart->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("ss_play_normal_CN.png"));
	pMenuStart->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("ss_play_pressed_CN.png"));
	pMenuStart->setPosition(visibleSize.width / 2, 50);

	Menu* pMenu = Menu::create(pMenuBack, pMenuStart, nullptr);
	pMenu->setPosition(Vec2::ZERO);
	this->addChild(pMenu, 6); //�ȵ����㼶��

	this->createPage();
	this->clickLayer();
	return true;
}

void CGameLevel::createPage()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//����
	m_pPageView = PageView::create();
	//�������ݴ�С
	m_pPageView->setContentSize(visibleSize);
	//����λ��
	m_pPageView->setPosition(Vec2::ZERO);

	//vector<sLevelDt*> levelVecDatas = CDataMgr::getInstance()->getLevelDtMgr()->getVecDatas();
	//�õ���ǰ��������
	const sThemeDt* pThemeVecDatas = CDataMgr::getInstance()->getThemeDtMgr()->getCurThemeData();

	for (int i = 0; i < pThemeVecDatas->vecLevelIDs.size(); ++i)
	{
		sLevelDt* pLevelData = CDataMgr::getInstance()->getLevelDtMgr()->getDataByID(pThemeVecDatas->vecLevelIDs[i]);
		//����Layout
		Layout* pLayout = Layout::create();
		//�������ݴ�С
		pLayout->setContentSize(visibleSize);
		pLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		//����λ��
		pLayout->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		//������������
		Sprite* pBg = Sprite::createWithSpriteFrameName(pLevelData->bgPng);
		//����λ��
		pBg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		pLayout->addChild(pBg);

		//����
		Sprite* pWave = Sprite::createWithSpriteFrameName(pLevelData->wavePng);
		pWave->setPosition(visibleSize.width / 2 , visibleSize.height / 2 + 200);
		pLayout->addChild(pWave);

		//����
		Sprite* pWeapon = Sprite::createWithSpriteFrameName(pLevelData->weaponPng);
		pWeapon->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 200);
		pLayout->addChild(pWeapon);

		m_pPageView->addPage(pLayout);
	}
	//���PageView
	this->addChild(m_pPageView);
}

void CGameLevel::clickLayer()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//���������
	m_pClickLayer = Layer::create();
	//ͨ�������ͼƬ��С�����ò�Ĵ�С //���ȡһ��ͼƬ
	Sprite* pSprite = Sprite::createWithSpriteFrameName("ss_map01.png");
	m_pClickLayer->setContentSize(pSprite->getContentSize());
	//���ò��ê��
	m_pClickLayer->setIgnoreAnchorPointForPosition(false);
	m_pClickLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	//����λ��
	m_pClickLayer->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(m_pClickLayer, 5);

	//����¼�
	EventListenerTouchOneByOne* pListener = EventListenerTouchOneByOne::create();
	//�����ʼ
	pListener->onTouchBegan = [=](Touch* pTouch, Event* pEvent){
		//�ѵ����λ�ô�����
		m_vBeginClickPos = pTouch->getLocation();

		//�жϵ����λ����Layer����
		if (m_pClickLayer->getBoundingBox().containsPoint(m_vBeginClickPos))
		{
			//�õ���ǰ��page������
			int nIndex = m_pPageView->getCurPageIndex();

			//�õ�page���������
			Vector<Node*> VecNodes = m_pPageView->getChildren();
			Layout* pLayerout = (Layout*)VecNodes.at(nIndex);

			//ȡ���ĵ�ǰlayerout�Ŵ�
			if (!m_bIsScale)
			{
				ScaleTo* pScaleTo = ScaleTo::create(0.1f, 1.1f);
				pLayerout->runAction(pScaleTo);
				m_bIsScale = true;
			}
		}
		return true;
	};

	//�����ƶ�
	pListener->onTouchMoved = [=](Touch* pTouch, Event* pEvent){
		Vec2 clickPos = pTouch->getLocation();
		//�õ���ǰ��page������
		int nIndex = m_pPageView->getCurPageIndex();
		//�õ�page���������
		Vector<Node*> VecNodes = m_pPageView->getChildren();
		Layout* pLayerout = (Layout*)VecNodes.at(nIndex);
		//����ƶ��������5����
		//ȡ���ĵ�ǰlayerout��ԭ��С
		ScaleTo* pScaleTo = ScaleTo::create(0.1f, 1.f);
		if (5 <= abs(clickPos.x - m_vBeginClickPos.x))
		{
			pLayerout->runAction(pScaleTo);
			m_bIsScale = false;
		}
	};

	//��������
	pListener->onTouchEnded = [=](Touch* pTouch, Event* pEvent){		
		//�õ���ǰ��page������
		int nIndex = m_pPageView->getCurPageIndex();

		//�õ�page���������
		Vector<Node*> VecNodes = m_pPageView->getChildren();
		Layout* pLayerout = (Layout*)VecNodes.at(nIndex);
		//ȡ���ĵ�ǰlayerout��ԭ��С
		ScaleTo* pScaleTo = ScaleTo::create(0.1f, 1.f);

		Vec2 clickPos = pTouch->getLocation();
		if (m_pClickLayer->getBoundingBox().containsPoint(clickPos))
		{			
			//�жϽ������x�����ƫ�� ���С��5���أ��ͱ�ʾȷ��
			if (5 > abs(clickPos.x - m_vBeginClickPos.x))
			{
				//���õ�ǰ�ؿ�
				CDataMgr::getInstance()->getLevelDtMgr()->setCurLevelIndex(nIndex);
				CallFunc* pChangeScene = CallFunc::create(CC_CALLBACK_0(CGameLevel::changeScene, this));
				pLayerout->runAction(Sequence::create(pScaleTo, pChangeScene, NULL));
				//������Ч
				SimpleAudioEngine::getInstance()->playEffect("Music/effect/Select.mp3");
			}
		}
		pLayerout->runAction(pScaleTo);
		m_bIsScale = false;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, m_pClickLayer);
}

void CGameLevel::changeScene()
{
	//TransitionMoveInR* pTrans = TransitionMoveInR::create(0.3f, CGameScene::createScene());
	CGameScene::destroy();
	Director::getInstance()->replaceScene(CGameScene::createScene());
	//CCLOG("changeScene");
}

void CGameLevel::gameStart(Ref* pSender)
{
	//������Ч
	SimpleAudioEngine::getInstance()->playEffect("Music/effect/Select.mp3");
	int nIndex = m_pPageView->getCurPageIndex();
	//���õ�ǰ�ؿ�
	CDataMgr::getInstance()->getLevelDtMgr()->setCurLevelIndex(nIndex);
	changeScene();
}

