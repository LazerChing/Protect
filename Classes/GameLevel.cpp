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
	//创建背景
	Sprite* pBg = Sprite::createWithSpriteFrameName("ui_S_Bg.png");
	pBg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(pBg);

	//上面的选择关卡
	Sprite* pSelectPng = Sprite::createWithSpriteFrameName("ss_bg_CN.png");
	pSelectPng->setAnchorPoint(Vec2(0.5, 1));
	pSelectPng->setPosition(visibleSize.width / 2, visibleSize.height);
	this->addChild(pSelectPng);

	//底部的云
	Sprite* pBottomCloud = Sprite::createWithSpriteFrameName(CDataMgr::getInstance()->getThemeDtMgr()->getCurThemeData()->cloudPng);
	pBottomCloud->setAnchorPoint(Vec2::ZERO);
	this->addChild(pBottomCloud);

	//返回
	MenuItemImage* pMenuBack = MenuItemImage::create("", "",[=](Ref* pSender){
		//Director::getInstance()->popScene();
		//播放音效
		SimpleAudioEngine::getInstance()->playEffect("Music/effect/Select.mp3");
		Director::getInstance()->replaceScene(CGameTheme::createScene());
	});
	pMenuBack->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("ss_back_normal.png"));
	pMenuBack->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("ss_back_pressed.png"));
	pMenuBack->setAnchorPoint(Vec2(0, 1));
	pMenuBack->setPosition(0, visibleSize.height);

	//开始游戏
	MenuItemImage* pMenuStart = MenuItemImage::create("", "",
		CC_CALLBACK_1(CGameLevel::gameStart, this));
	pMenuStart->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("ss_play_normal_CN.png"));
	pMenuStart->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("ss_play_pressed_CN.png"));
	pMenuStart->setPosition(visibleSize.width / 2, 50);

	Menu* pMenu = Menu::create(pMenuBack, pMenuStart, nullptr);
	pMenu->setPosition(Vec2::ZERO);
	this->addChild(pMenu, 6); //比点击层层级高

	this->createPage();
	this->clickLayer();
	return true;
}

void CGameLevel::createPage()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//创建
	m_pPageView = PageView::create();
	//设置内容大小
	m_pPageView->setContentSize(visibleSize);
	//设置位置
	m_pPageView->setPosition(Vec2::ZERO);

	//vector<sLevelDt*> levelVecDatas = CDataMgr::getInstance()->getLevelDtMgr()->getVecDatas();
	//得到当前主题数据
	const sThemeDt* pThemeVecDatas = CDataMgr::getInstance()->getThemeDtMgr()->getCurThemeData();

	for (int i = 0; i < pThemeVecDatas->vecLevelIDs.size(); ++i)
	{
		sLevelDt* pLevelData = CDataMgr::getInstance()->getLevelDtMgr()->getDataByID(pThemeVecDatas->vecLevelIDs[i]);
		//创建Layout
		Layout* pLayout = Layout::create();
		//设置内容大小
		pLayout->setContentSize(visibleSize);
		pLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		//设置位置
		pLayout->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		//创建背景精灵
		Sprite* pBg = Sprite::createWithSpriteFrameName(pLevelData->bgPng);
		//设置位置
		pBg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		pLayout->addChild(pBg);

		//波数
		Sprite* pWave = Sprite::createWithSpriteFrameName(pLevelData->wavePng);
		pWave->setPosition(visibleSize.width / 2 , visibleSize.height / 2 + 200);
		pLayout->addChild(pWave);

		//武器
		Sprite* pWeapon = Sprite::createWithSpriteFrameName(pLevelData->weaponPng);
		pWeapon->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 200);
		pLayout->addChild(pWeapon);

		m_pPageView->addPage(pLayout);
	}
	//添加PageView
	this->addChild(m_pPageView);
}

void CGameLevel::clickLayer()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//创建点击层
	m_pClickLayer = Layer::create();
	//通过精灵的图片大小来设置层的大小 //随便取一个图片
	Sprite* pSprite = Sprite::createWithSpriteFrameName("ss_map01.png");
	m_pClickLayer->setContentSize(pSprite->getContentSize());
	//设置层的锚点
	m_pClickLayer->setIgnoreAnchorPointForPosition(false);
	m_pClickLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	//设置位置
	m_pClickLayer->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(m_pClickLayer, 5);

	//点击事件
	EventListenerTouchOneByOne* pListener = EventListenerTouchOneByOne::create();
	//点击开始
	pListener->onTouchBegan = [=](Touch* pTouch, Event* pEvent){
		//把点击的位置存起来
		m_vBeginClickPos = pTouch->getLocation();

		//判断点击的位置在Layer层中
		if (m_pClickLayer->getBoundingBox().containsPoint(m_vBeginClickPos))
		{
			//得到当前的page的索引
			int nIndex = m_pPageView->getCurPageIndex();

			//得到page里面的容器
			Vector<Node*> VecNodes = m_pPageView->getChildren();
			Layout* pLayerout = (Layout*)VecNodes.at(nIndex);

			//取到的当前layerout放大
			if (!m_bIsScale)
			{
				ScaleTo* pScaleTo = ScaleTo::create(0.1f, 1.1f);
				pLayerout->runAction(pScaleTo);
				m_bIsScale = true;
			}
		}
		return true;
	};

	//触摸移动
	pListener->onTouchMoved = [=](Touch* pTouch, Event* pEvent){
		Vec2 clickPos = pTouch->getLocation();
		//得到当前的page的索引
		int nIndex = m_pPageView->getCurPageIndex();
		//得到page里面的容器
		Vector<Node*> VecNodes = m_pPageView->getChildren();
		Layout* pLayerout = (Layout*)VecNodes.at(nIndex);
		//如果移动距离大于5像素
		//取到的当前layerout还原大小
		ScaleTo* pScaleTo = ScaleTo::create(0.1f, 1.f);
		if (5 <= abs(clickPos.x - m_vBeginClickPos.x))
		{
			pLayerout->runAction(pScaleTo);
			m_bIsScale = false;
		}
	};

	//触摸结束
	pListener->onTouchEnded = [=](Touch* pTouch, Event* pEvent){		
		//得到当前的page的索引
		int nIndex = m_pPageView->getCurPageIndex();

		//得到page里面的容器
		Vector<Node*> VecNodes = m_pPageView->getChildren();
		Layout* pLayerout = (Layout*)VecNodes.at(nIndex);
		//取到的当前layerout还原大小
		ScaleTo* pScaleTo = ScaleTo::create(0.1f, 1.f);

		Vec2 clickPos = pTouch->getLocation();
		if (m_pClickLayer->getBoundingBox().containsPoint(clickPos))
		{			
			//判断结束后的x坐标的偏差 如果小于5像素，就表示确定
			if (5 > abs(clickPos.x - m_vBeginClickPos.x))
			{
				//设置当前关卡
				CDataMgr::getInstance()->getLevelDtMgr()->setCurLevelIndex(nIndex);
				CallFunc* pChangeScene = CallFunc::create(CC_CALLBACK_0(CGameLevel::changeScene, this));
				pLayerout->runAction(Sequence::create(pScaleTo, pChangeScene, NULL));
				//播放音效
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
	//播放音效
	SimpleAudioEngine::getInstance()->playEffect("Music/effect/Select.mp3");
	int nIndex = m_pPageView->getCurPageIndex();
	//设置当前关卡
	CDataMgr::getInstance()->getLevelDtMgr()->setCurLevelIndex(nIndex);
	changeScene();
}

