#include "GameScene.h"
#include "GameMenu.h"
#include "GameTheme.h"
#include "GameLevel.h"

CGameScene::CGameScene()
:m_nCurCoin(0)
{
}

CGameScene::~CGameScene()
{
	m_pUpgradeAnimate->release();
}

CGameScene* CGameScene::m_spInstance = nullptr;

Scene* CGameScene::createScene()
{
	return CGameScene::getInstance();
}

bool CGameScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	//节点信息
	m_sSignedNodeDt = sSignedNodeDt();
	//播放背景音乐
	SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/bg/BGMusic.mp3",true);
	//设置速度
	Director::getInstance()->getScheduler()->setTimeScale(1.f);
	this->createBg();
	this->createMgr();	
	this->addTouchListener();
	this->initPos();	

	//创建萝卜
	m_pRadish = CRadish::create();
	m_pRadish->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_pRadish->setPosition(m_vecPos.back().x, m_vecPos.back().y);
	this->addChild(m_pRadish);

	
	this->scheduleUpdate();
	this->scheduleOnce(CC_CALLBACK_1(CGameScene::gameStart, this), 0.f,"gameStart");
	return true;
}

void CGameScene::addTouchListener()
{
	EventListenerTouchOneByOne* pListener = EventListenerTouchOneByOne::create();
	pListener->onTouchBegan = CC_CALLBACK_2(CGameScene::onTouchBegin,this);
	pListener->onTouchEnded = CC_CALLBACK_2(CGameScene::onTouchEnd, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);
}

bool CGameScene::onTouchBegin(Touch* pTouch ,Event* pEvent)
{
	return true;
}

void CGameScene::onTouchEnd(Touch* pTouch, Event* pEvent)
{
	Vec2 clickPos =  pTouch->getLocation();
	Vec2 tiledPos = getTiledPosByPixel(clickPos);
	//取得目标坐标可操作性类型
	//用来返回选中的节点
	Node* pSelectedNode = nullptr;
	int nType = getTypeOfAimPos(clickPos,pSelectedNode);
	if (E_AIM_BUILD == nType || E_AIM_FORBID == nType)
	{
		//调用卡片管理者的点击事件
		m_pCardMgr->clickEvent(clickPos, nType, pSelectedNode);
	}
	else if (E_AIM_RADISH == nType)
	{
		//调用萝卜播放动画接口
		m_pRadish->playShakeAnimate();
	}
	else if (E_AIM_TOWER == nType) //选中的是炮塔
	{
		m_pCardMgr->clickEvent(clickPos, nType, pSelectedNode);
		CCLOG("update");
	}
	else if (E_AIM_TOP == nType) //选中的是最上方
	{
		CCLOG("Top");
	}
	else  if (E_AIM_OBJECT == nType)
	{
		//播放音效
		SimpleAudioEngine::getInstance()->playEffect("Music/effect/ShootSelect.mp3");
		CCLOG("sign object");
		//当前选中的节点是物品
		//判断有没有已选中的
		if (0 != m_sSignedNodeDt.nUUID)
		{
			//表示有标记的，取消标记状态
			if (0 == strcmp(m_sSignedNodeDt.nodeName.c_str(), "object"))
			{
				if (nullptr != pSelectedNode)
				{
					//通过uuid取得已经选中的object
					CObject* pObjectOld = m_pObjectMgr->findObjectByUUID(m_sSignedNodeDt.nUUID);
					//获得新选中的
					CObject* pObjectNew = (CObject*)pSelectedNode;
					//判断当前点中的是不是已选中的
					if (m_sSignedNodeDt.nodeName == pSelectedNode->getName() && 
						m_sSignedNodeDt.nUUID == pObjectNew->getUUID())
					{
						//取消已选中的
						pObjectOld->setIsSigned(false);
						clearSignedNode();
					}
					else if (nullptr != pObjectOld)
					{
						//清除标记的时候判断有没有ball类型的子弹攻击这个节点，如果有，就把这个子弹清除，并告诉这个子弹的归属者，去选择另一个目标
						Vector<Node*> VecNodes = m_pBulletMgr->getChildren();
						for (Node* pNode : VecNodes)
						{
							if (713 == pNode->getTag() ||
								714 == pNode->getTag() ||
								715 == pNode->getTag()) //ball
							{
								CBullet* pBullet = (CBullet*)pNode;
								//判断目标是否一样
								if (pBullet->getAimedNode()->nodeName == m_sSignedNodeDt.nodeName &&
									pBullet->getAimedNode()->nUUID == m_sSignedNodeDt.nUUID)
								{
									pBullet->unscheduleUpdate();
									pBullet->unschedule("timingDamage");
									//通知这个子弹的归属者，移除目标
									pBullet->getOwner()->removeAim();
									//目标节点指向空
									pBullet->clearAim();
									//这颗子弹消失
									pBullet->removeFromParent();
								}
							}
						}

						//以前的关了
						pObjectOld->setIsSigned(false);
						//新的
						CObject* pObjectNew = (CObject*)pSelectedNode;
						if (nullptr != pObjectNew)
						{
							//替换掉
							pObjectNew->setIsSigned(true);
							m_sSignedNodeDt.nodeName = pObjectNew->getName();
							m_sSignedNodeDt.nUUID = pObjectNew->getUUID();
						}
					}
				}
			}
			else if (0 == strcmp(m_sSignedNodeDt.nodeName.c_str(), "enemy"))
			{
				if (nullptr != pSelectedNode)
				{
					//通过uuid取得enemy
					CEnemy* pEnemyOld = m_pEnemyMgr->findEnemyByUUID(m_sSignedNodeDt.nUUID);
					//获得新选中的
					CEnemy* pEnemyNew = (CEnemy*)pSelectedNode;
					//判断当前点中的是不是已选中的
					if (m_sSignedNodeDt.nodeName == pSelectedNode->getName() &&
						m_sSignedNodeDt.nUUID == pEnemyNew->getUUID())
					{
						//取消已选中的
						pEnemyOld->setIsSigned(false);
						clearSignedNode();
					}
					else if (nullptr != pEnemyOld)
					{
						//清除标记的时候判断有没有ball类型的子弹攻击这个节点，如果有，就把这个子弹清除，并告诉这个子弹的归属者，去选择另一个目标
						Vector<Node*> VecNodes = m_pBulletMgr->getChildren();
						for (Node* pNode : VecNodes)
						{
							if (713 == pNode->getTag() ||
								714 == pNode->getTag() ||
								715 == pNode->getTag()) //ball
							{
								CBullet* pBullet = (CBullet*)pNode;
								//判断目标是否一样
								if (pBullet->getAimedNode()->nodeName == m_sSignedNodeDt.nodeName &&
									pBullet->getAimedNode()->nUUID == m_sSignedNodeDt.nUUID)
								{
									pBullet->unscheduleUpdate();
									pBullet->unschedule("timingDamage");
									//通知这个子弹的归属者，移除目标
									pBullet->getOwner()->removeAim();
									//目标节点指向空
									pBullet->clearAim();
									//这颗子弹消失
									pBullet->removeFromParent();
								}
							}
						}

						//以前的关了
						pEnemyOld->setIsSigned(false);
						//新的
						CObject* pObjectNew = (CObject*)pSelectedNode;
						if (nullptr != pObjectNew)
						{
							//替换掉
							pObjectNew->setIsSigned(true);
							m_sSignedNodeDt.nodeName = pObjectNew->getName();
							m_sSignedNodeDt.nUUID = pObjectNew->getUUID();
						}
					}
				}
			}
		}
		else  //没有已选中的
		{
			//直接替换
			CObject* pObject = (CObject*)pSelectedNode;
			pObject->setIsSigned(true);
			m_sSignedNodeDt.nodeName = pObject->getName();
			m_sSignedNodeDt.nUUID = pObject->getUUID();
		}
	}
	else  if (E_AIM_ENEMY == nType)
	{
		//播放音效
		SimpleAudioEngine::getInstance()->playEffect("Music/effect/ShootSelect.mp3");
		CCLOG("sign enemy");
		//当前选中的节点是敌人
		//判断有没有已选中的
		if (0 != m_sSignedNodeDt.nUUID)
		{
			//表示有标记的，取消标记状态
			if (0 == strcmp(m_sSignedNodeDt.nodeName.c_str(), "object"))
			{
				if (nullptr != pSelectedNode)
				{
					//通过uuid取得object
					CObject* pObjectOld = m_pObjectMgr->findObjectByUUID(m_sSignedNodeDt.nUUID);

					//获得新选中的
					CObject* pObjectNew = (CObject*)pSelectedNode;
					//判断当前点中的是不是已选中的
					if (m_sSignedNodeDt.nodeName == pSelectedNode->getName() &&
						m_sSignedNodeDt.nUUID == pObjectNew->getUUID())
					{
						//取消已选中的
						pObjectOld->setIsSigned(false);
						clearSignedNode();
					}
					else if (nullptr != pObjectOld)
					{
						//清除标记的时候判断有没有ball类型的子弹攻击这个节点，如果有，就把这个子弹清除，并告诉这个子弹的归属者，去选择另一个目标
						Vector<Node*> VecNodes = m_pBulletMgr->getChildren();
						for (Node* pNode : VecNodes)
						{
							if (713 == pNode->getTag() ||
								714 == pNode->getTag() ||
								715 == pNode->getTag()) //ball
							{
								CBullet* pBullet = (CBullet*)pNode;
								//判断目标是否一样
								if (pBullet->getAimedNode()->nodeName == m_sSignedNodeDt.nodeName &&
									pBullet->getAimedNode()->nUUID == m_sSignedNodeDt.nUUID)
								{
									pBullet->unscheduleUpdate();
									pBullet->unschedule("timingDamage");
									//通知这个子弹的归属者，移除目标
									pBullet->getOwner()->removeAim();
									//目标节点指向空
									pBullet->clearAim();
									//这颗子弹消失
									pBullet->removeFromParent();
								}
							}
						}

						//以前的关了
						pObjectOld->setIsSigned(false);
						//新的
						CEnemy* pEnemyNew = (CEnemy*)pSelectedNode;
						if (nullptr != pEnemyNew)
						{
							//替换掉
							pEnemyNew->setIsSigned(true);
							m_sSignedNodeDt.nodeName = pEnemyNew->getName();
							m_sSignedNodeDt.nUUID = pEnemyNew->getUUID();
						}
					}
				}
			}
			else if (0 == strcmp(m_sSignedNodeDt.nodeName.c_str(), "enemy"))
			{
				if (nullptr != pSelectedNode)
				{
					//通过uuid取得enemy
					CEnemy* pEnemyOld = m_pEnemyMgr->findEnemyByUUID(m_sSignedNodeDt.nUUID);

					//获得新选中的
					CEnemy* pEnemyNew = (CEnemy*)pSelectedNode;
					//判断当前点中的是不是已选中的
					if (m_sSignedNodeDt.nodeName == pSelectedNode->getName() &&
						m_sSignedNodeDt.nUUID == pEnemyNew->getUUID())
					{
						//取消已选中的
						pEnemyOld->setIsSigned(false);
						clearSignedNode();
					}
					else if (nullptr != pEnemyOld)
					{
						//清除标记的时候判断有没有ball类型的子弹攻击这个节点，如果有，就把这个子弹清除，并告诉这个子弹的归属者，去选择另一个目标
						Vector<Node*> VecNodes = m_pBulletMgr->getChildren();
						for (Node* pNode : VecNodes)
						{
							if (713 == pNode->getTag() ||
								714 == pNode->getTag() ||
								715 == pNode->getTag()) //ball
							{
								CBullet* pBullet = (CBullet*)pNode;
								//判断目标是否一样
								if (pBullet->getAimedNode()->nodeName == m_sSignedNodeDt.nodeName &&
									pBullet->getAimedNode()->nUUID == m_sSignedNodeDt.nUUID)
								{
									pBullet->unscheduleUpdate();
									pBullet->unschedule("timingDamage");
									//通知这个子弹的归属者，移除目标
									pBullet->getOwner()->removeAim();
									//目标节点指向空
									pBullet->clearAim();
									//这颗子弹消失
									pBullet->removeFromParent();
								}
							}
						}

						//以前的关了
						pEnemyOld->setIsSigned(false);
						//新的
						CEnemy* pEnemyNew = (CEnemy*)pSelectedNode;
						if (nullptr != pEnemyNew)
						{
							//替换掉
							pEnemyNew->setIsSigned(true);
							m_sSignedNodeDt.nodeName = pEnemyNew->getName();
							m_sSignedNodeDt.nUUID = pEnemyNew->getUUID();
						}
					}
				}
			}
		}
		else  //没有已选中的
		{
			//直接替换
			CEnemy* pEnmey = (CEnemy*)pSelectedNode;
			pEnmey->setIsSigned(true);
			m_sSignedNodeDt.nodeName = pEnmey->getName();
			m_sSignedNodeDt.nUUID = pEnmey->getUUID();
		}
	}
}

cocos2d::Vec2 CGameScene::getTiledPosByPixel(Vec2 pos)
{
	//x坐标不变
	float fX = pos.x;
	//格子的X坐标
	int nTiledX = fX / m_pTmxTiledMap->getTileSize().width;

	//y坐标
	float fY = pos.y;
	//翻转的y坐标
	int nFlipedTiledY = fY / m_pTmxTiledMap->getTileSize().height;	
	//格子的Y坐标
	int nTiledY = (m_pTmxTiledMap->getMapSize().height - 1) - nFlipedTiledY;
	return Vec2(nTiledX, nTiledY);
}

CGameScene* CGameScene::getInstance()
{
	if (nullptr == m_spInstance)
	{
		m_spInstance = CGameScene::create();
	}
	return m_spInstance;
}

void CGameScene::initPos()
{
	//通过Tiled地图取得路径点
	TMXObjectGroup* pObjGroup = m_pTmxTiledMap->getObjectGroup("pathObjs");
	ValueVector vObjs = pObjGroup->getObjects();
	for (int i = 0; i < vObjs.size(); ++i)
	{
		ValueMap vMap = vObjs[i].asValueMap();
		//像素坐标  
		float fX = vMap["x"].asFloat();
		float fY = vMap["y"].asFloat();
		Vec2 pos = Vec2(fX, fY);

		//把像素坐标转化为格子坐标
		Vec2 tiledPos = getTiledPosByPixel(pos);
		//把格子坐标转换为格子中点的像素坐标
		Vec2 centerPos = getCenterPixelPosByTiled(tiledPos);
		m_vecPos.push_back(centerPos);
	}

	//通过地图得到物品，并创建物品
	TMXObjectGroup* pObject = m_pTmxTiledMap->getObjectGroup("object");
	ValueVector vObjs2 = pObject->getObjects();
	for (int i = 0; i < vObjs2.size(); ++i)
	{
		ValueMap vMap = vObjs2[i].asValueMap();
		//像素坐标 得到的是方框左下角的世界像素坐标
		float fX = vMap["x"].asFloat();
		float fY = vMap["y"].asFloat();

		//方框的大小
		float fWidth = vMap["width"].asFloat();
		float fHeight = vMap["height"].asFloat();

		//制作矩形盒子
		Rect objectRect = Rect(fX, fY, fWidth, fHeight);
		m_vecObjectRect.push_back(objectRect);

		//id
		string strID = vMap["name"].asString();
		Vec2 pos = Vec2(fX + fWidth / 2, fY + fHeight / 2);
		m_pObjectMgr->addObject(strID, pos);
	}

	//遍历地图
	Size mapSize = m_pTmxTiledMap->getMapSize();	
	//可以建塔的敌方
	for (int i = 0; i < mapSize.width;++i)
	{
		for (int j = 1; j < mapSize.height;++j)
		{
			//为0表示空的
			if (0 == m_pTmxPathLayer->getTileGIDAt(Vec2(i,j)))
			{
				//格子是否在物品里面
				bool bExist = false;
				//每个格子坐标的中心点				
				Vec2 centerPos = getCenterPixelPosByTiled(Vec2(i, j));
				for (int m = 0; m < m_vecObjectRect.size();++m)
				{
					//如果坐标在矩形盒子里面
					if (m_vecObjectRect[m].containsPoint(centerPos))
					{
						bExist = true;
						break;
					}
				}
				if (!bExist)
				{
					//可以建塔的敌方
					Sprite* pCanBuild = Sprite::createWithSpriteFrameName("select_00.png");
					pCanBuild->setPosition(centerPos);
					pCanBuild->setName("canBuildSpritePos");
					this->addChild(pCanBuild);
					m_vecCanBuildPos.push_back(centerPos);
					FadeOut* pFadeOut = FadeOut::create(1.f);
					FadeIn* pFadeIn = FadeIn::create(1.0f);
					pCanBuild->runAction(Sequence::create(
						pFadeOut, pFadeIn, pFadeOut, pFadeIn, pFadeOut,
						RemoveSelf::create(), nullptr));
				}
			}
		}
	}
}

cocos2d::Vec2 CGameScene::getCenterPixelPosByTiled(Vec2 pos)
{
	//格子地图大小
	Size mapSize = m_pTmxTiledMap->getMapSize();
	//格子大小
	Size tileSize = m_pTmxTiledMap->getTileSize();
	//像素x坐标
	float fX = pos.x*tileSize.width + tileSize.width / 2;
	//像素y坐标
	float fY = ((mapSize.height - 1) - pos.y)*tileSize.height + tileSize.height / 2;
	return Vec2(fX, fY);
}

void CGameScene::createBg()
{
	//创建地图
	m_pTmxTiledMap = TMXTiledMap::create(CDataMgr::getInstance()->getCurLevelDt()->mapName);
	m_nCurCoin = CDataMgr::getInstance()->getCurLevelDt()->nCoin;
	this->addChild(m_pTmxTiledMap);

	//通过地图里面的路径层名字取得路径层
	m_pTmxPathLayer = m_pTmxTiledMap->getLayer("path");


	Size visibleSize = Director::getInstance()->getVisibleSize();
	//菜单背景层
	m_pMenuBgLayer = Layer::create();
	//背景图
	Sprite* pMenuBg = Sprite::createWithSpriteFrameName("MenuBG.png");
	pMenuBg->setAnchorPoint(Vec2(0, 0));
	pMenuBg->setPosition(Vec2::ZERO);
	m_pMenuBgLayer->addChild(pMenuBg);

	//金币
	m_pCoinLabel = Label::createWithTTF("", "fonts/arial.ttf", 37);
	m_pCoinLabel->setAnchorPoint(Vec2(0, 0.5));
	m_pCoinLabel->setPosition(100, 45);
	m_pCoinLabel->setString(StringUtils::format("%d", m_nCurCoin));
	m_pMenuBgLayer->addChild(m_pCoinLabel);

	//波数层
	m_pWaveBgLayer = Layer::create();
	m_pWaveBgLayer->setPosition(380, 0);
	m_pMenuBgLayer->addChild(m_pWaveBgLayer);

	//波数背景图
	Sprite* pWaveBg = Sprite::createWithSpriteFrameName("MenuCenter_01_CN.png");
	pWaveBg->setPosition(Vec2(0, 37));
	m_pWaveBgLayer->addChild(pWaveBg);

	//当前波数
	m_pCurWaveLabel = Label::createWithCharMap("ui/numyellow-hd.png", 44, 40, 46);
	m_pCurWaveLabel->setString("01");
	m_pCurWaveLabel->setPosition(-80, 45);
	m_pWaveBgLayer->addChild(m_pCurWaveLabel);

	//总波数
	Label* pTotalWave = Label::createWithTTF("", "fonts/arial.ttf", 32);
	pTotalWave->setString(StringUtils::format("%d", CDataMgr::getInstance()->getCurLevelDt()->nWave));
	pTotalWave->setPosition(10, 45);
	m_pWaveBgLayer->addChild(pTotalWave);

	//加速按钮
	Sprite* pMenuAcceOffSprite = Sprite::createWithSpriteFrameName("speed11.png");
	Sprite* pMenuAcceOnSprite = Sprite::createWithSpriteFrameName("speed22.png");
	MenuItemSprite* pAcceOffMenu = MenuItemSprite::create(pMenuAcceOffSprite, pMenuAcceOffSprite);
	MenuItemSprite* pAcceOnMenu = MenuItemSprite::create(pMenuAcceOnSprite, pMenuAcceOnSprite);

	MenuItemToggle* pMenuAcceToggle = MenuItemToggle::createWithCallback([=](Ref* pRef){
		MenuItemToggle* pAcceToggle = (MenuItemToggle*)pRef;
		int nIndex = pAcceToggle->getSelectedIndex();
		if (0 == nIndex)
		{
			CCLOG("Acce Off");
			Director::getInstance()->getScheduler()->setTimeScale(1);			
		}
		else if (1 == nIndex)
		{
			CCLOG("Acce On");
			//加速逻辑
			Director::getInstance()->getScheduler()->setTimeScale(2);
		}
		//播放音效
		SimpleAudioEngine::getInstance()->playEffect("Music/effect/MenuSelect.mp3");
	}, pAcceOffMenu, pAcceOnMenu, nullptr);
	pMenuAcceToggle->setPosition(620, 37);


	//暂停图片
	m_pPauseSprite = Sprite::createWithSpriteFrameName("MenuCenter_02_CN.png");
	m_pPauseSprite->setPosition(350, 40);
	m_pMenuBgLayer->addChild(m_pPauseSprite);
	//隐藏
	m_pPauseSprite->setVisible(false);


	//暂停按钮
	//非暂停状态精灵
	Sprite* pPauseOffSprite = Sprite::createWithSpriteFrameName("pause12.png");
	//暂停状态精灵
	Sprite* pPauseOnSprite = Sprite::createWithSpriteFrameName("pause02.png");
	
	MenuItemSprite* pPauseOffMenu = MenuItemSprite::create(pPauseOnSprite, pPauseOnSprite);
	MenuItemSprite* pPauseOnMenu = MenuItemSprite::create(pPauseOffSprite, pPauseOffSprite);

	MenuItemToggle* pMenuPauseToggle = MenuItemToggle::createWithCallback([=](Ref* pRef){
		MenuItemToggle* pMenuPauseToggle = (MenuItemToggle*)pRef;
		int nIndex = pMenuPauseToggle->getSelectedIndex();
		if (0 == nIndex) //根据索引来判断当前是什么状态
		{
			//未暂停状态
			CCLOG("Not Pause");
			m_pWaveBgLayer->setVisible(true);
			m_pPauseSprite->setVisible(false);
			Director::getInstance()->resume();
			//获得暂停透明层
			LayerColor* pPauseLayerColor = (LayerColor*) this->getChildByName("pauseLayer");
			if (nullptr != pPauseLayerColor)
			{
				//移除
				pPauseLayerColor->removeFromParent();
			}
		}
		else if (1 == nIndex)
		{
			CCLOG("Pause");
			m_pWaveBgLayer->setVisible(false);
			m_pPauseSprite->setVisible(true);
			Director::getInstance()->pause();
			//添加一个透明层
			LayerColor* pPauseLayerColor = LayerColor::create(Color4B(0, 0, 0, 0));
			pPauseLayerColor->setName("pauseLayer");
			EventListenerTouchOneByOne* pListener = EventListenerTouchOneByOne::create();
			pListener->onTouchBegan = [=](Touch* pTouch, Event* pEvent){return true; };
			//设置触摸穿透
			pListener->setSwallowTouches(true);
			_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, pPauseLayerColor);
			this->addChild(pPauseLayerColor, 9);
		}
		//播放音效
		SimpleAudioEngine::getInstance()->playEffect("Music/effect/MenuSelect.mp3");
	}, pPauseOffMenu, pPauseOnMenu, nullptr);
	pMenuPauseToggle->setPosition(700, 37);

	//操作按钮（暂停）
	MenuItemImage* pMenuOperate = MenuItemImage::create("","",
		[=](Ref* pRef){
		//暂停游戏
		Director::getInstance()->pause();
		//添加一个灰色半透明层
		LayerColor* pLayerColor = LayerColor::create(Color4B(0, 0, 0, 100));
		pLayerColor->setName("LayerColor");
		EventListenerTouchOneByOne* pListener = EventListenerTouchOneByOne::create();
		pListener->onTouchBegan = [=](Touch* pTouch, Event* pEvent){return true; };
		//设置触摸穿透
		pListener->setSwallowTouches(true);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, pLayerColor);

		//添加一个界面层
		Layer* pLayer = Layer::create();
		//背景
		Sprite* pMenuBg = Sprite::createWithSpriteFrameName("menu_bg.png");
		pMenuBg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
		pLayer->addChild(pMenuBg);

		//播放音效
		SimpleAudioEngine::getInstance()->playEffect("Music/effect/MenuSelect.mp3");

		//恢复游戏按钮
		MenuItemImage* pMenuImageResume = MenuItemImage::create("","",
			[=](Ref* pRef){
			MenuItemImage* pMenuImageResume = (MenuItemImage*)pRef;
			//恢复游戏
			Director::getInstance()->resume();
			pMenuImageResume->getParent()->getParent()->getParent()->removeFromParent();
			//播放音效
			SimpleAudioEngine::getInstance()->playEffect("Music/effect/MenuSelect.mp3");
		});
		pMenuImageResume->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("menu_resume_normal_CN.png"));
		pMenuImageResume->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("menu_resume_pressed_CN.png"));
		pMenuImageResume->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 95);

		//重新游戏按钮
		MenuItemImage* pMenuImageRestart = MenuItemImage::create("", "",
			[=](Ref* pRef){
			MenuItemImage* pMenuImageRestart = (MenuItemImage*)pRef;
			//恢复游戏
			Director::getInstance()->resume();
			destroy();
			Director::getInstance()->replaceScene(CGameScene::createScene());
			//播放音效
			SimpleAudioEngine::getInstance()->playEffect("Music/effect/MenuSelect.mp3");
		});
		pMenuImageRestart->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("menu_restart_normal_CN.png"));
		pMenuImageRestart->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("menu_restart_pressed_CN.png"));
		pMenuImageRestart->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 2);

		//退出按钮
		MenuItemImage* pMenuImageQuit = MenuItemImage::create("", "",
			[=](Ref* pRef){
			MenuItemImage* pMenuImageQuit = (MenuItemImage*)pRef;
			//恢复游戏
			Director::getInstance()->resume();
			destroy();
			Director::getInstance()->replaceScene(CGameLevel::createScene());
			//播放音效
			SimpleAudioEngine::getInstance()->playEffect("Music/effect/MenuSelect.mp3");
			//播放背景音乐
			SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/bg/Accomplish.mp3", true);
		});
		pMenuImageQuit->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("menu_quit_normal_CN.png"));
		pMenuImageQuit->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("menu_quit_pressed_CN.png"));
		pMenuImageQuit->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 92);

		Menu* pMenu = Menu::create(pMenuImageResume, pMenuImageRestart, pMenuImageQuit, nullptr);
		pMenu->setPosition(Vec2::ZERO);
		pLayer->addChild(pMenu);

		pLayerColor->addChild(pLayer);
		this->addChild(pLayerColor,11);
	});
	pMenuOperate->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("menu01.png"));
	pMenuOperate->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("menu01.png"));
	pMenuOperate->setPosition(760, 37);
	

	//音乐开关
	Sprite* pMenuMusicOnSprite = Sprite::createWithSpriteFrameName("soundfx_on_CN.png");
	Sprite* pMenuMusicOffSprite = Sprite::createWithSpriteFrameName("soundfx_off_CN.png");
	MenuItemSprite* pMusicOnMenu = MenuItemSprite::create(pMenuMusicOnSprite, pMenuMusicOnSprite);
	MenuItemSprite* pMusicOffMenu = MenuItemSprite::create(pMenuMusicOffSprite, pMenuMusicOffSprite);

	MenuItemToggle* pMenuMusicToggle = MenuItemToggle::createWithCallback([=](Ref* pRef){
		MenuItemToggle* pMenuMusicToggle = (MenuItemToggle*)pRef;
		int nIndex = pMenuMusicToggle->getSelectedIndex();
		if (0 == nIndex)
		{
			CCLOG("Music On");
			SimpleAudioEngine::getInstance()->resumeAllEffects();
			SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
			SimpleAudioEngine::getInstance()->setEffectsVolume(1);
			//播放音效
			SimpleAudioEngine::getInstance()->playEffect("Music/effect/MenuSelect.mp3");
		}
		else if (1 == nIndex)
		{
			CCLOG("Music Off");
			SimpleAudioEngine::getInstance()->pauseAllEffects();
			SimpleAudioEngine::getInstance()->setEffectsVolume(0);
			SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		}
	}, pMusicOnMenu, pMusicOffMenu, nullptr);
	pMenuMusicToggle->setPosition(845, 47);
	pMenuMusicToggle->setScale(0.7f);

	Menu* pMenuTop = Menu::create(pMenuPauseToggle, pMenuAcceToggle, pMenuOperate, pMenuMusicToggle, nullptr);
	pMenuTop->setPosition(Vec2::ZERO);
	m_pMenuBgLayer->addChild(pMenuTop);

	m_pMenuBgLayer->setPosition(0, visibleSize.height - pMenuBg->getContentSize().height);
	this->addChild(m_pMenuBgLayer, 10);


	//创建升级动画
	Animation* pUpgradeAnimation = Animation::create();
	for (int i = 32; i <= 35; ++i)
	{
		string strName = StringUtils::format("air%d.png", i);
		pUpgradeAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(strName));
	}
	pUpgradeAnimation->setDelayPerUnit(0.08f);
	m_pUpgradeAnimate = Animate::create(pUpgradeAnimation);
	m_pUpgradeAnimate->retain();

	//隐藏
	m_pUpgradeAnimateSprite = Sprite::createWithSpriteFrameName("air32.png");
	this->addChild(m_pUpgradeAnimateSprite);
	m_pUpgradeAnimateSprite->setVisible(false);
}

void CGameScene::createMgr()
{
	//创建物品管理者
	m_pObjectMgr = CObjectMgr::create();
	this->addChild(m_pObjectMgr);
	//创建敌人管理者
	m_pEnemyMgr = CEnmeyMgr::create();
	this->addChild(m_pEnemyMgr);
	//创建武器管理者
	m_pWeaponMgr = CWeaponMgr::create();
	this->addChild(m_pWeaponMgr);
	//创建卡片管理者
	m_pCardMgr = CCardMgr::create();
	this->addChild(m_pCardMgr);
	//创建子弹管理者
	m_pBulletMgr = CBulletMgr::create();
	this->addChild(m_pBulletMgr);
	//创建动画管理者
	m_pAnimateMgr = CAnimateMgr::create();
	this->addChild(m_pAnimateMgr,8);
}

cocos2d::Vec2 CGameScene::convertGLToWorld(Vec2 pos)
{
	//x坐标不变
	float fX = pos.x;
	//y坐标反过来
	float fY = m_pTmxTiledMap->getMapSize().height - pos.y;
	return Vec2(fX, fY);
}

void CGameScene::gameStart(float fDelta)
{
	float fStartTime = 0.0f;
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//添加一个透明层
	LayerColor* pLayerColor = LayerColor::create(Color4B(0, 0, 0, 0));
	pLayerColor->setName("startLayer");
	EventListenerTouchOneByOne* pListener = EventListenerTouchOneByOne::create();
	pListener->onTouchBegan = [=](Touch* pTouch, Event* pEvent){return true; };
	//设置触摸穿透
	pListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, pLayerColor);
	this->addChild(pLayerColor, 30);

	//背景
	Sprite* pCountBg = Sprite::createWithSpriteFrameName("countdown_11.png");
	pCountBg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	pLayerColor->addChild(pCountBg);

	//转圈的
	Sprite* pCircle = Sprite::createWithSpriteFrameName("countdown_12.png");
	pCircle->setAnchorPoint(Vec2(0.5, 0.5));
	pCircle->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	RotateBy* pRotate = RotateBy::create(1.f, -360);
	pCircle->runAction(RepeatForever::create(pRotate));
	pLayerColor->addChild(pCircle);

	//倒计时数字
	Sprite* pCountNum = Sprite::createWithSpriteFrameName("countdown_01.png");
	pCountNum->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	ScaleTo* pScaleToBig = ScaleTo::create(0.2f, 1.2f);
	pScaleToBig->retain();
	ScaleTo* pScaleToSmall = ScaleTo::create(0.02f, 1.1f);
	pScaleToSmall->retain();

	//播放音效
	SimpleAudioEngine::getInstance()->playEffect("Music/effect/CountDown.mp3");

	CallFunc* pCallFunc = CallFunc::create([=](){
		pCountNum->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("countdown_02.png"));
		pCountNum->setScale(1);
		//播放音效
		SimpleAudioEngine::getInstance()->playEffect("Music/effect/CountDown.mp3");
		CallFunc* pCallFunc2 = CallFunc::create([=](){
			pCountNum->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("countdown_03.png"));
			pCountNum->setScale(1);
			//播放音效
			SimpleAudioEngine::getInstance()->playEffect("Music/effect/CountDown.mp3");
			CallFunc* pTimeOver = CallFunc::create([=](){
				pCountNum->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("countdown_13.png"));
				pCountNum->setScale(1);
				//播放GO
				SimpleAudioEngine::getInstance()->playEffect("Music/effect/GO.mp3");
				RotateBy* pRotate = RotateBy::create(0.2f, -360);
				//游戏开始
				CallFunc* pGameStart = CallFunc::create([=](){
					pScaleToSmall->release();
					pScaleToBig->release();
					pLayerColor->removeFromParent();
					m_pEnemyMgr->gameStart();
				});
				Sequence* pSeq = Sequence::create(pRotate, pScaleToBig, DelayTime::create(0.78f), pGameStart, nullptr);
				pCountNum->runAction(pSeq);
			});
			Sequence* pSeq = Sequence::create(pScaleToBig, pScaleToSmall, DelayTime::create(0.78f), pTimeOver, nullptr);
			pCountNum->runAction(pSeq);
		});
		Sequence* pSeq = Sequence::create(pScaleToBig, pScaleToSmall, DelayTime::create(0.78f), pCallFunc2, nullptr);
		pCountNum->runAction(pSeq);
	});	

	Sequence* pSeq = Sequence::create(pScaleToBig, pScaleToSmall, DelayTime::create(0.78f), pCallFunc, nullptr);
	pCountNum->runAction(pSeq);
	pLayerColor->addChild(pCountNum);


	//起始位置箭头
	Vec2 pixelPos = m_vecPos[0];
	//判断位置，x坐标，改变方向
	if (0 < m_vecPos[0].x - m_vecPos[1].x)
	{
		//左方向
		m_pAnimateMgr->createArrow(Vec2(pixelPos.x - 50, pixelPos.y), -180);

		this->scheduleOnce([=](float fDelta){
			m_pAnimateMgr->createArrow(Vec2(pixelPos.x - 90, pixelPos.y), -180);
		},0.5f,"createArrow");

		this->scheduleOnce([=](float fDelta){
			m_pAnimateMgr->createArrow(Vec2(pixelPos.x - 130, pixelPos.y), -180);
		}, 1.0f, "createArrow2");
	}
	else 
	{
		if (0 > m_vecPos[0].x - m_vecPos[1].x) //右方向
		{
			m_pAnimateMgr->createArrow(Vec2(pixelPos.x + 50, pixelPos.y), 0);

			this->scheduleOnce([=](float fDelta){
				m_pAnimateMgr->createArrow(Vec2(pixelPos.x + 90, pixelPos.y), 0);
			}, 0.5f, "createArrow");

			this->scheduleOnce([=](float fDelta){
				m_pAnimateMgr->createArrow(Vec2(pixelPos.x + 130, pixelPos.y), 0);
			}, 1.0f, "createArrow2");
		}
		else //判断Y方向
		{
			if (m_vecPos[0].y > m_vecPos[1].y)
			{
				//向下
				m_pAnimateMgr->createArrow(Vec2(pixelPos.x, pixelPos.y - 50), 90);

				this->scheduleOnce([=](float fDelta){
					m_pAnimateMgr->createArrow(Vec2(pixelPos.x, pixelPos.y - 90), 90);
				}, 0.5f, "createArrow");

				this->scheduleOnce([=](float fDelta){
					m_pAnimateMgr->createArrow(Vec2(pixelPos.x, pixelPos.y - 130), 90);
				}, 1.0f, "createArrow2");
			}
			else
			{
				//向上
				m_pAnimateMgr->createArrow(Vec2(pixelPos.x, pixelPos.y + 50), -90);

				this->scheduleOnce([=](float fDelta){
					m_pAnimateMgr->createArrow(Vec2(pixelPos.x, pixelPos.y + 90), -90);
				}, 0.5f, "createArrow");

				this->scheduleOnce([=](float fDelta){
					m_pAnimateMgr->createArrow(Vec2(pixelPos.x, pixelPos.y + 130), -90);
				}, 1.0f, "createArrow2");
			}
		}
	}
}

int CGameScene::getTypeOfAimPos(Vec2 pixelPos, Node*& pNode)
{
	//把像素坐标转化为格子中心坐标
	//先转化为格子坐标
	Vec2 tiledPos = getTiledPosByPixel(pixelPos);
	//获得中心坐标
	Vec2 centerPos = getCenterPixelPosByTiled(tiledPos);

	//目标类型
	int nType = E_AIM_NONE;

	if (0 == tiledPos.y)
	{
		nType = E_AIM_TOP;//状态栏
		return nType;
	}

	if (m_pRadish->getSpriteWorldBox().containsPoint(pixelPos))
	{
		nType = E_AIM_RADISH;
		return nType;
	}

	//得到已有的炮塔
	Vector<Node*> VecWeapons = m_pWeaponMgr->getChildren();
	//判断点击的地方有没有炮塔
	for (int i = 0; i < VecWeapons.size();++i)
	{
		CWeapon* pWeapon = (CWeapon*)VecWeapons.at(i);
		//并且是第一次点击
		if (centerPos == pWeapon->getPosition())
		{
			//表示有炮塔
			nType = E_AIM_TOWER;
			pNode = VecWeapons.at(i);
			return nType;
		}
	}

	//得到已有的敌人
	Vector<Node*> VecEnemys = m_pEnemyMgr->getChildren();
	//判断点击的地方有没有敌人
	for (int i = 0; i < VecEnemys.size(); ++i)
	{
		CEnemy* pEnemy = (CEnemy*)VecEnemys.at(i);
		if (pEnemy->getBoundingBox().containsPoint(pixelPos) && m_pCardMgr->getIsFirstClick())
		{
			//表示有敌人
			nType = E_AIM_ENEMY;
			pNode = VecEnemys.at(i);
			return nType;
		}
	}

	//判断像素在不在路径上
	int nGid = m_pTmxPathLayer->getTileGIDAt(tiledPos);

	//如果没有炮塔，判断在不在路径上
	if (0 == nGid) //0表示不在路径上
	{
		//判断坐标是否在物品里面
		bool bInObject = false;
		for (int m = 0; m < m_vecObjectRect.size(); ++m)
		{
			//如果坐标在矩形盒子里面  并且是第一次点击的
			bool bContain = m_vecObjectRect[m].containsPoint(centerPos);
			if (m_vecObjectRect[m].containsPoint(centerPos) && m_pCardMgr->getIsFirstClick())
			{
				bInObject = true;
				//找到该物品
				CObject* pObject = m_pObjectMgr->findObjectByPos(pixelPos);
				pNode = pObject;
				break;
			}
		}
		if (bInObject)
		{
			nType = E_AIM_OBJECT;
		}
		else
			nType = E_AIM_BUILD;
	}
	else
	{
		nType = E_AIM_FORBID;
	}
	return nType;
}

bool CGameScene::removeCanBuildPos(Vec2 centerPixelPos)
{
	for (int i = 0; i < m_vecCanBuildPos.size();++i)
	{
		if (centerPixelPos == m_vecCanBuildPos[i])
		{
			m_vecCanBuildPos.erase(m_vecCanBuildPos.begin() + i);
			return true;
		}
	}
	return false;
}

void CGameScene::destroy()
{
	m_spInstance = nullptr;
}

void CGameScene::reduceCoin(int nCoin)
{
	m_nCurCoin -= nCoin;
	//更新金币显示
	m_pCoinLabel->setString(StringUtils::format("%d", m_nCurCoin));
	//更新是否能升级武器
	m_pWeaponMgr->checkUpgrade();
}

void CGameScene::addCoin(int nCoin)
{
	m_nCurCoin += nCoin;
	//更新金币显示
	m_pCoinLabel->setString(StringUtils::format("%d", m_nCurCoin));
	//更新是否能升级武器
	m_pWeaponMgr->checkUpgrade();
}

void CGameScene::playUpgradeWeaponAnimate(Vec2 pos)
{
	m_pUpgradeAnimateSprite->setVisible(true);
	m_pUpgradeAnimateSprite->setPosition(pos);
	CallFunc* pAnimateOver = CallFunc::create([=](){
		m_pUpgradeAnimateSprite->setVisible(false);
	});
	m_pUpgradeAnimateSprite->runAction(Sequence::create(m_pUpgradeAnimate, pAnimateOver, nullptr));
}

void CGameScene::update(float fDelta)
{
	collision();
}

void CGameScene::collision()
{
	//遍历所有的子弹和所有的敌人
	Vector<Node*> Vec_Bullets = m_pBulletMgr->getChildren();
	Vector<Node*> Vec_Enemys = m_pEnemyMgr->getChildren();
	for (Node* enemyNode : Vec_Enemys)
	{
		CEnemy* pEnemy = (CEnemy*)enemyNode;
		for (Node* bulletNode : Vec_Bullets)
		{
			CBullet* pBullet = (CBullet*)bulletNode;
			//之前没有碰撞过
			if (!pEnemy->isAttacked(pBullet) &&
				pEnemy->getBoundingBox().containsPoint(pBullet->getPosition()))
			{
				bulletTouchEnemy(pBullet, pEnemy);
			}
		}
	}

	//遍历所有子弹和所有物品
	//Vector<Node*> Vec_Bullets = m_pBulletMgr->getChildren();
	Vector<Node*> Vec_Objects = m_pObjectMgr->getChildren();
	for (Node* bulletNode : Vec_Bullets)
	{
		CBullet* pBullet = (CBullet*)bulletNode;
		if (0 == strcmp(pBullet->getAim().c_str(),"enemy")) //如果子弹发射时的目标是敌人，根据子弹类型来判断是否和物品进行碰撞
		{
			if (0 != strcmp(pBullet->getType().c_str(), "fan")) //如果不是穿透类的子弹，子弹就不与物品进行碰撞
			{
				continue;
			}
		}
		for (Node* objectNode : Vec_Objects)
		{
			CObject* pObject = (CObject*)objectNode;
			//这几种物品不判断
			if (0 == strcmp(pObject->getStrID().c_str(), "601") ||
				0 == strcmp(pObject->getStrID().c_str(), "617") ||
				0 == strcmp(pObject->getStrID().c_str(), "618"))
			{
				continue;
			}
			//之前没有碰撞过
			if (!pObject->isAttacked(pBullet) &&
				pObject->getBoundingBox().containsPoint(pBullet->getPosition()))
			{
				bulletTouchObject(pBullet, pObject);
			}
		}		
	}
}

void CGameScene::bulletTouchEnemy(CBullet* pBullet, CEnemy* pEnemy)
{
	//记下子弹的位置
	Vec2 bulPos = pBullet->getPosition();
	vector<string> vec_Pngs = pBullet->getBombPngs();
	//判断子弹类型
	if (0 == strcmp(pBullet->getType().c_str(), "bottle"))
	{
		//对敌人造成伤害
		if (pEnemy->damage(pBullet))
		{
			//播放敌人爆炸动画
			m_pAnimateMgr->playEnemyBombAnimate(pEnemy->getPosition(),pEnemy->getCoinPng());
			//加钱
			addCoin(pEnemy->getCoin());
			//敌人死亡
			pEnemy->removeSelf();
		}
		//子弹消失
		pBullet->removeFromParent();
		//播放子弹击中动画
		m_pAnimateMgr->playAnimateByVecPng(bulPos, vec_Pngs);
	}
	else if (0 == strcmp(pBullet->getType().c_str(), "shit"))
	{
		//对敌人造成伤害
		if (pEnemy->damage(pBullet))
		{
			//播放敌人爆炸动画
			m_pAnimateMgr->playEnemyBombAnimate(pEnemy->getPosition(), pEnemy->getCoinPng());
			//加钱
			addCoin(pEnemy->getCoin());
			//敌人死亡
			pEnemy->removeSelf();
		}
		else
		{
			vector<string> vec_hitPng = pBullet->getHitPng();
			//移动受阻
			pEnemy->moveHindered(pBullet->getReduceSpeedRate(), pBullet->getContinueTime(), vec_hitPng);
		}
		//子弹消失
		pBullet->removeFromParent();
		//播放子弹击中动画
		m_pAnimateMgr->playAnimateByVecPng(bulPos, vec_Pngs);
	}
	else if (0 == strcmp(pBullet->getType().c_str(), "fan"))
	{
		//对敌人造成伤害
		if (pEnemy->damage(pBullet))
		{
			//播放敌人爆炸动画
			m_pAnimateMgr->playEnemyBombAnimate(pEnemy->getPosition(), pEnemy->getCoinPng());
			//加钱
			addCoin(pEnemy->getCoin());
			//敌人死亡
			pEnemy->removeSelf();
		}  
		//子弹不消失
		//播放子弹击中动画
		m_pAnimateMgr->playAnimateByVecPng(bulPos, vec_Pngs);
	}
	else if (0 == strcmp(pBullet->getType().c_str(), "star")) //子弹造成范围伤害
	{
		//查找子弹附近范围内的敌人，造成伤害
		pBullet->damageAroundEnemy(150);
		pBullet->removeFromParent();
		//播放子弹击中动画
		m_pAnimateMgr->playAnimateByVecPng(bulPos, vec_Pngs);
	}
}

void CGameScene::bulletTouchObject(CBullet* pBullet, CObject* pObject)
{
	//记下子弹的位置
	Vec2 bulPos = pBullet->getPosition();
	vector<string> vec_Pngs = pBullet->getBombPngs();

	//对物品造成伤害
	if (pObject->damage(pBullet))
	{
		//播放爆炸动画
		m_pAnimateMgr->playEnemyBombAnimate(pObject->getPosition(), pObject->getCoinPng());
		//加钱
		addCoin(pObject->getCoin());
		//出去容器中物体的矩形盒
		for (int i = 0; i < m_vecObjectRect.size();++i)
		{
			if (m_vecObjectRect[i].containsPoint(pObject->getPosition()))
			{
				m_vecObjectRect.erase(m_vecObjectRect.begin() + i);
				break;
			}
		}
		//物体消失
		pObject->removeSelf();
	}
	//判断子弹类型 如果不是fan类型的炮塔子弹，子弹就消失
	if (0 != strcmp(pBullet->getType().c_str(), "fan"))
	{
		//子弹消失
		pBullet->removeFromParent();
	}
	//播放子弹击中动画
	m_pAnimateMgr->playAnimateByVecPng(bulPos, vec_Pngs);
}

void CGameScene::gameWin()
{
	//播放音效
	SimpleAudioEngine::getInstance()->playEffect("Music/effect/Perfect.mp3");

	Size visibleSize = Director::getInstance()->getVisibleSize();
	//暂停游戏
	Director::getInstance()->pause();
	//添加一个灰色半透明层
	LayerColor* pLayerColor = LayerColor::create(Color4B(0, 0, 0, 100));
	EventListenerTouchOneByOne* pListener = EventListenerTouchOneByOne::create();
	pListener->onTouchBegan = [=](Touch* pTouch, Event* pEvent){return true; };
	//设置触摸穿透
	pListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, pLayerColor);
	this->addChild(pLayerColor, 20);
	//添加一个界面层
	Layer* pLayer = Layer::create();
	//背景
	Sprite* pBg = Sprite::createWithSpriteFrameName("win_bg.png");
	pBg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	pLayer->addChild(pBg);

	//波数背景
	Sprite* pLableBg = Sprite::createWithSpriteFrameName("win_bg_CN.png");
	pLableBg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	pLayer->addChild(pLableBg);

	//波数label
	Label* pWaveLabel = Label::createWithCharMap("ui/numyellow-hd.png", 44, 40, 46);
	pWaveLabel->setString(StringUtils::format("%02d",m_pEnemyMgr->getCurWaveNum()));
	pWaveLabel->setPosition(visibleSize.width / 2 + 5, visibleSize.height / 2 + 27);
	pLayer->addChild(pWaveLabel);

	//总波数
	Label* pTotalWaveLabel = Label::createWithTTF("", "fonts/arial.ttf", 37);
	pTotalWaveLabel->setString(StringUtils::format("%d", CDataMgr::getInstance()->getCurLevelDt()->nWave));
	pTotalWaveLabel->setPosition(visibleSize.width / 2 + 93, visibleSize.height / 2 + 27);
	pLayer->addChild(pTotalWaveLabel);

	//关卡
	Label* pLevelLabel = Label::createWithTTF("", "fonts/arial.ttf", 37);
	int nCurLevel = CDataMgr::getInstance()->getLevelDtMgr()->getCurLevelIndex() + 1;
	pLevelLabel->setString(StringUtils::format("%d", nCurLevel));
	pLevelLabel->setPosition(visibleSize.width / 2 - 55, visibleSize.height / 2 - 28);
	pLayer->addChild(pLevelLabel);

	//金牌
	Sprite* pGold = Sprite::createWithSpriteFrameName("gainhonor_3.png");
	pGold->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 155);
	pLayer->addChild(pGold);


	//选择关卡
	MenuItemImage* pMenuImageReSelect = MenuItemImage::create("", "",
		[=](Ref* pRef){
		//恢复游戏
		Director::getInstance()->resume();
		destroy();
		//播放音乐
		SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/bg/Accomplish.mp3", true);
		Director::getInstance()->replaceScene(CGameLevel::createScene());
	});
	pMenuImageReSelect->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("select_normal_CN.png"));
	pMenuImageReSelect->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("select_pressed_CN.png"));
	pMenuImageReSelect->setPosition(visibleSize.width / 2 - 115, visibleSize.height / 2 - 107);

	//继续游戏，下一关
	MenuItemImage* pMenuImageContinue = MenuItemImage::create("", "",
		[=](Ref* pRef){
		//恢复游戏
		Director::getInstance()->resume();
		destroy();
		CDataMgr::getInstance()->nextLevel();
		Director::getInstance()->replaceScene(CGameScene::createScene());
	});
	pMenuImageContinue->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("continue_normal_CN.png"));
	pMenuImageContinue->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("continue_pressed_CN.png"));
	pMenuImageContinue->setPosition(visibleSize.width / 2 + 115, visibleSize.height / 2 - 107);

	Menu* pMenu = Menu::create(pMenuImageReSelect, pMenuImageContinue, nullptr);
	pMenu->setPosition(Vec2::ZERO);
	pLayer->addChild(pMenu);

	pLayerColor->addChild(pLayer);
}

void CGameScene::gameOver()
{
	//播放音效
	SimpleAudioEngine::getInstance()->playEffect("Music/effect/Lose.mp3");

	Size visibleSize = Director::getInstance()->getVisibleSize();
	//暂停游戏
	Director::getInstance()->pause();
	//添加一个灰色半透明层
	LayerColor* pLayerColor = LayerColor::create(Color4B(0, 0, 0, 100));
	EventListenerTouchOneByOne* pListener = EventListenerTouchOneByOne::create();
	pListener->onTouchBegan = [=](Touch* pTouch, Event* pEvent){return true; };
	//设置触摸穿透
	pListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, pLayerColor);
	this->addChild(pLayerColor, 20);
	//添加一个界面层
	Layer* pLayer = Layer::create();
	//背景
	Sprite* pBg = Sprite::createWithSpriteFrameName("lose_bg.png");
	pBg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	pLayer->addChild(pBg);

	//波数背景
	Sprite* pLableBg = Sprite::createWithSpriteFrameName("lose_bg_CN.png");
	pLableBg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	pLayer->addChild(pLableBg);

	//波数label
	Label* pWaveLabel = Label::createWithCharMap("ui/numyellow-hd.png", 44, 40, 46);
	pWaveLabel->setString(StringUtils::format("%02d", m_pEnemyMgr->getCurWaveNum() - 1));
	pWaveLabel->setPosition(visibleSize.width / 2 + 5, visibleSize.height / 2 + 27);
	pLayer->addChild(pWaveLabel);

	//总波数
	Label* pTotalWaveLabel = Label::createWithTTF("", "fonts/arial.ttf", 37);
	pTotalWaveLabel->setString(StringUtils::format("%d", CDataMgr::getInstance()->getCurLevelDt()->nWave));
	pTotalWaveLabel->setPosition(visibleSize.width / 2 + 93, visibleSize.height / 2 + 27);
	pLayer->addChild(pTotalWaveLabel);

	//关卡
	Label* pLevelLabel = Label::createWithTTF("", "fonts/arial.ttf", 37);
	int nCurLevel = CDataMgr::getInstance()->getLevelDtMgr()->getCurLevelIndex() + 1;
	pLevelLabel->setString(StringUtils::format("%d", nCurLevel));
	pLevelLabel->setPosition(visibleSize.width / 2 - 55, visibleSize.height / 2 - 28);
	pLayer->addChild(pLevelLabel);

	//选择关卡
	MenuItemImage* pMenuImageReSelect = MenuItemImage::create("", "",
		[=](Ref* pRef){
		//恢复游戏
		Director::getInstance()->resume();
		destroy();
		//播放音乐
		SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/bg/Accomplish.mp3", true);
		Director::getInstance()->replaceScene(CGameLevel::createScene());
	});
	pMenuImageReSelect->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("select_normal_CN.png"));
	pMenuImageReSelect->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("select_pressed_CN.png"));
	pMenuImageReSelect->setPosition(visibleSize.width / 2 - 115, visibleSize.height / 2 - 107);

	//重试
	MenuItemImage* pMenuImageRetry = MenuItemImage::create("", "",
		[=](Ref* pRef){
		//恢复游戏
		Director::getInstance()->resume();
		destroy();
		Director::getInstance()->replaceScene(CGameScene::createScene());
	});
	pMenuImageRetry->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("retry_normal_CN.png"));
	pMenuImageRetry->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("retry_pressed_CN.png"));
	pMenuImageRetry->setPosition(visibleSize.width / 2 + 115, visibleSize.height / 2 - 107);

	Menu* pMenu = Menu::create(pMenuImageReSelect, pMenuImageRetry, nullptr);
	pMenu->setPosition(Vec2::ZERO);
	pLayer->addChild(pMenu);

	pLayerColor->addChild(pLayer);
}

void CGameScene::clearSignedNode()
{
	//清除标记的时候判断有没有ball类型的子弹攻击这个节点，如果有，就把这个子弹清除，并告诉这个子弹的归属者，去选择另一个目标
	if (0 != m_sSignedNodeDt.nUUID)
	{
		Vector<Node*> VecNodes = m_pBulletMgr->getChildren();








































































		+
			+++++++3..............................+++




















			+++++++++++++++++++














































































			.

















































































































































































































































































































































































































































33+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++---
		for (Node* pNode : VecNodes)
		{
			if (713 == pNode->getTag() ||
				714 == pNode->getTag() ||
				715 == pNode->getTag()) //ball
			{
				CBullet* pBullet = (CBullet*)pNode;
				//判断目标是否一样
				if (pBullet->getAimedNode()->nodeName == m_sSignedNodeDt.nodeName &&
					pBullet->getAimedNode()->nUUID == m_sSignedNodeDt.nUUID)
				{
					pBullet->unscheduleUpdate();
					pBullet->unschedule("timingDamage");
					//通知这个子弹的归属者，移除目标
					pBullet->getOwner()->removeAim();
					//目标节点指向空
					pBullet->clearAim();
					//这颗子弹消失
					pBullet->removeFromParent();
				}
			}
		}
	}
	m_sSignedNodeDt.nodeName = "";
	m_sSignedNodeDt.nUUID = 0;
}
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++----------++++


++++---0--000++