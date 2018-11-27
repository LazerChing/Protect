#include "GameTheme.h"
#include "GameLevel.h"
#include "GameMenu.h"

CGameTheme::CGameTheme()
{
}

CGameTheme::~CGameTheme()
{
}

Scene* CGameTheme::createScene()
{
	return CGameTheme::create();
}

bool CGameTheme::init()
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

	//����
	MenuItemImage* pMenuBack = MenuItemImage::create("", "", [=](Ref* pSender){
		//Director::getInstance()->popScene();
		Director::getInstance()->replaceScene(CGameMenu::createScene());
		//������Ч
		SimpleAudioEngine::getInstance()->playEffect("Music/effect/Select.mp3");
	});
	
	pMenuBack->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("theme_home_normal.png"));
	pMenuBack->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("theme_home_pressed.png"));
	pMenuBack->setAnchorPoint(Vec2(0, 1));
	pMenuBack->setPosition(0, visibleSize.height);

	Menu* pMenu = Menu::create(pMenuBack, nullptr);
	pMenu->setPosition(Vec2::ZERO);
	this->addChild(pMenu, 6); //�ȵ����㼶��

	this->createPage();
	this->clickLayer();
	return true;
}


void CGameTheme::createPage()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//����
	m_pPageView = PageView::create();
	//�������ݴ�С
	m_pPageView->setContentSize(visibleSize);
	//����λ��
	m_pPageView->setPosition(Vec2::ZERO);

	vector<sThemeDt*> themeVecDatas = CDataMgr::getInstance()->getThemeDtMgr()->getVecDatas();

	for (int i = 0; i < themeVecDatas.size(); ++i)
	{
		//����Layout
		Layout* pLayout = Layout::create();
		//�������ݴ�С
		pLayout->setContentSize(visibleSize);
		pLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		//����λ��
		pLayout->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		//������������
		Sprite* pBg = Sprite::createWithSpriteFrameName(themeVecDatas[i]->bgPng);
		//����λ��
		pBg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		pLayout->addChild(pBg);

		//��������
		Sprite* pThemeTxt = Sprite::createWithSpriteFrameName(themeVecDatas[i]->themeTxt);
		pThemeTxt->setPosition(visibleSize.width / 2 + 20, visibleSize.height / 2 + 5);
		pLayout->addChild(pThemeTxt);

		//��ǩ
		Sprite* pBookMark = Sprite::createWithSpriteFrameName(themeVecDatas[i]->bookMark);
		pBookMark->setPosition(750, 90);
		pLayout->addChild(pBookMark);

		m_pPageView->addPage(pLayout);
	}
	//���PageView
	this->addChild(m_pPageView);
}

void CGameTheme::clickLayer()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//���������
	m_pClickLayer = Layer::create();
	//ͨ�������ͼƬ��С�����ò�Ĵ�С //���ȡһ��ͼƬ
	Sprite* pSprite = Sprite::createWithSpriteFrameName("theme_pack01.png");
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
				//���õ�ǰ����
				CDataMgr::getInstance()->getThemeDtMgr()->setCurThemeIndex(nIndex);
				CallFunc* pChangeScene = CallFunc::create(CC_CALLBACK_0(CGameTheme::changeScene, this));
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

void CGameTheme::changeScene()
{
	//�л�����
	TransitionMoveInR* pTrans = TransitionMoveInR::create(0.5f, CGameLevel::createScene());
	//Director::getInstance()->pushScene(pTrans);
	Director::getInstance()->replaceScene(pTrans);
}