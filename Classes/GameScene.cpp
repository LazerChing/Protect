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

	//�ڵ���Ϣ
	m_sSignedNodeDt = sSignedNodeDt();
	//���ű�������
	SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/bg/BGMusic.mp3",true);
	//�����ٶ�
	Director::getInstance()->getScheduler()->setTimeScale(1.f);
	this->createBg();
	this->createMgr();	
	this->addTouchListener();
	this->initPos();	

	//�����ܲ�
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
	//ȡ��Ŀ������ɲ���������
	//��������ѡ�еĽڵ�
	Node* pSelectedNode = nullptr;
	int nType = getTypeOfAimPos(clickPos,pSelectedNode);
	if (E_AIM_BUILD == nType || E_AIM_FORBID == nType)
	{
		//���ÿ�Ƭ�����ߵĵ���¼�
		m_pCardMgr->clickEvent(clickPos, nType, pSelectedNode);
	}
	else if (E_AIM_RADISH == nType)
	{
		//�����ܲ����Ŷ����ӿ�
		m_pRadish->playShakeAnimate();
	}
	else if (E_AIM_TOWER == nType) //ѡ�е�������
	{
		m_pCardMgr->clickEvent(clickPos, nType, pSelectedNode);
		CCLOG("update");
	}
	else if (E_AIM_TOP == nType) //ѡ�е������Ϸ�
	{
		CCLOG("Top");
	}
	else  if (E_AIM_OBJECT == nType)
	{
		//������Ч
		SimpleAudioEngine::getInstance()->playEffect("Music/effect/ShootSelect.mp3");
		CCLOG("sign object");
		//��ǰѡ�еĽڵ�����Ʒ
		//�ж���û����ѡ�е�
		if (0 != m_sSignedNodeDt.nUUID)
		{
			//��ʾ�б�ǵģ�ȡ�����״̬
			if (0 == strcmp(m_sSignedNodeDt.nodeName.c_str(), "object"))
			{
				if (nullptr != pSelectedNode)
				{
					//ͨ��uuidȡ���Ѿ�ѡ�е�object
					CObject* pObjectOld = m_pObjectMgr->findObjectByUUID(m_sSignedNodeDt.nUUID);
					//�����ѡ�е�
					CObject* pObjectNew = (CObject*)pSelectedNode;
					//�жϵ�ǰ���е��ǲ�����ѡ�е�
					if (m_sSignedNodeDt.nodeName == pSelectedNode->getName() && 
						m_sSignedNodeDt.nUUID == pObjectNew->getUUID())
					{
						//ȡ����ѡ�е�
						pObjectOld->setIsSigned(false);
						clearSignedNode();
					}
					else if (nullptr != pObjectOld)
					{
						//�����ǵ�ʱ���ж���û��ball���͵��ӵ���������ڵ㣬����У��Ͱ�����ӵ����������������ӵ��Ĺ����ߣ�ȥѡ����һ��Ŀ��
						Vector<Node*> VecNodes = m_pBulletMgr->getChildren();
						for (Node* pNode : VecNodes)
						{
							if (713 == pNode->getTag() ||
								714 == pNode->getTag() ||
								715 == pNode->getTag()) //ball
							{
								CBullet* pBullet = (CBullet*)pNode;
								//�ж�Ŀ���Ƿ�һ��
								if (pBullet->getAimedNode()->nodeName == m_sSignedNodeDt.nodeName &&
									pBullet->getAimedNode()->nUUID == m_sSignedNodeDt.nUUID)
								{
									pBullet->unscheduleUpdate();
									pBullet->unschedule("timingDamage");
									//֪ͨ����ӵ��Ĺ����ߣ��Ƴ�Ŀ��
									pBullet->getOwner()->removeAim();
									//Ŀ��ڵ�ָ���
									pBullet->clearAim();
									//����ӵ���ʧ
									pBullet->removeFromParent();
								}
							}
						}

						//��ǰ�Ĺ���
						pObjectOld->setIsSigned(false);
						//�µ�
						CObject* pObjectNew = (CObject*)pSelectedNode;
						if (nullptr != pObjectNew)
						{
							//�滻��
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
					//ͨ��uuidȡ��enemy
					CEnemy* pEnemyOld = m_pEnemyMgr->findEnemyByUUID(m_sSignedNodeDt.nUUID);
					//�����ѡ�е�
					CEnemy* pEnemyNew = (CEnemy*)pSelectedNode;
					//�жϵ�ǰ���е��ǲ�����ѡ�е�
					if (m_sSignedNodeDt.nodeName == pSelectedNode->getName() &&
						m_sSignedNodeDt.nUUID == pEnemyNew->getUUID())
					{
						//ȡ����ѡ�е�
						pEnemyOld->setIsSigned(false);
						clearSignedNode();
					}
					else if (nullptr != pEnemyOld)
					{
						//�����ǵ�ʱ���ж���û��ball���͵��ӵ���������ڵ㣬����У��Ͱ�����ӵ����������������ӵ��Ĺ����ߣ�ȥѡ����һ��Ŀ��
						Vector<Node*> VecNodes = m_pBulletMgr->getChildren();
						for (Node* pNode : VecNodes)
						{
							if (713 == pNode->getTag() ||
								714 == pNode->getTag() ||
								715 == pNode->getTag()) //ball
							{
								CBullet* pBullet = (CBullet*)pNode;
								//�ж�Ŀ���Ƿ�һ��
								if (pBullet->getAimedNode()->nodeName == m_sSignedNodeDt.nodeName &&
									pBullet->getAimedNode()->nUUID == m_sSignedNodeDt.nUUID)
								{
									pBullet->unscheduleUpdate();
									pBullet->unschedule("timingDamage");
									//֪ͨ����ӵ��Ĺ����ߣ��Ƴ�Ŀ��
									pBullet->getOwner()->removeAim();
									//Ŀ��ڵ�ָ���
									pBullet->clearAim();
									//����ӵ���ʧ
									pBullet->removeFromParent();
								}
							}
						}

						//��ǰ�Ĺ���
						pEnemyOld->setIsSigned(false);
						//�µ�
						CObject* pObjectNew = (CObject*)pSelectedNode;
						if (nullptr != pObjectNew)
						{
							//�滻��
							pObjectNew->setIsSigned(true);
							m_sSignedNodeDt.nodeName = pObjectNew->getName();
							m_sSignedNodeDt.nUUID = pObjectNew->getUUID();
						}
					}
				}
			}
		}
		else  //û����ѡ�е�
		{
			//ֱ���滻
			CObject* pObject = (CObject*)pSelectedNode;
			pObject->setIsSigned(true);
			m_sSignedNodeDt.nodeName = pObject->getName();
			m_sSignedNodeDt.nUUID = pObject->getUUID();
		}
	}
	else  if (E_AIM_ENEMY == nType)
	{
		//������Ч
		SimpleAudioEngine::getInstance()->playEffect("Music/effect/ShootSelect.mp3");
		CCLOG("sign enemy");
		//��ǰѡ�еĽڵ��ǵ���
		//�ж���û����ѡ�е�
		if (0 != m_sSignedNodeDt.nUUID)
		{
			//��ʾ�б�ǵģ�ȡ�����״̬
			if (0 == strcmp(m_sSignedNodeDt.nodeName.c_str(), "object"))
			{
				if (nullptr != pSelectedNode)
				{
					//ͨ��uuidȡ��object
					CObject* pObjectOld = m_pObjectMgr->findObjectByUUID(m_sSignedNodeDt.nUUID);

					//�����ѡ�е�
					CObject* pObjectNew = (CObject*)pSelectedNode;
					//�жϵ�ǰ���е��ǲ�����ѡ�е�
					if (m_sSignedNodeDt.nodeName == pSelectedNode->getName() &&
						m_sSignedNodeDt.nUUID == pObjectNew->getUUID())
					{
						//ȡ����ѡ�е�
						pObjectOld->setIsSigned(false);
						clearSignedNode();
					}
					else if (nullptr != pObjectOld)
					{
						//�����ǵ�ʱ���ж���û��ball���͵��ӵ���������ڵ㣬����У��Ͱ�����ӵ����������������ӵ��Ĺ����ߣ�ȥѡ����һ��Ŀ��
						Vector<Node*> VecNodes = m_pBulletMgr->getChildren();
						for (Node* pNode : VecNodes)
						{
							if (713 == pNode->getTag() ||
								714 == pNode->getTag() ||
								715 == pNode->getTag()) //ball
							{
								CBullet* pBullet = (CBullet*)pNode;
								//�ж�Ŀ���Ƿ�һ��
								if (pBullet->getAimedNode()->nodeName == m_sSignedNodeDt.nodeName &&
									pBullet->getAimedNode()->nUUID == m_sSignedNodeDt.nUUID)
								{
									pBullet->unscheduleUpdate();
									pBullet->unschedule("timingDamage");
									//֪ͨ����ӵ��Ĺ����ߣ��Ƴ�Ŀ��
									pBullet->getOwner()->removeAim();
									//Ŀ��ڵ�ָ���
									pBullet->clearAim();
									//����ӵ���ʧ
									pBullet->removeFromParent();
								}
							}
						}

						//��ǰ�Ĺ���
						pObjectOld->setIsSigned(false);
						//�µ�
						CEnemy* pEnemyNew = (CEnemy*)pSelectedNode;
						if (nullptr != pEnemyNew)
						{
							//�滻��
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
					//ͨ��uuidȡ��enemy
					CEnemy* pEnemyOld = m_pEnemyMgr->findEnemyByUUID(m_sSignedNodeDt.nUUID);

					//�����ѡ�е�
					CEnemy* pEnemyNew = (CEnemy*)pSelectedNode;
					//�жϵ�ǰ���е��ǲ�����ѡ�е�
					if (m_sSignedNodeDt.nodeName == pSelectedNode->getName() &&
						m_sSignedNodeDt.nUUID == pEnemyNew->getUUID())
					{
						//ȡ����ѡ�е�
						pEnemyOld->setIsSigned(false);
						clearSignedNode();
					}
					else if (nullptr != pEnemyOld)
					{
						//�����ǵ�ʱ���ж���û��ball���͵��ӵ���������ڵ㣬����У��Ͱ�����ӵ����������������ӵ��Ĺ����ߣ�ȥѡ����һ��Ŀ��
						Vector<Node*> VecNodes = m_pBulletMgr->getChildren();
						for (Node* pNode : VecNodes)
						{
							if (713 == pNode->getTag() ||
								714 == pNode->getTag() ||
								715 == pNode->getTag()) //ball
							{
								CBullet* pBullet = (CBullet*)pNode;
								//�ж�Ŀ���Ƿ�һ��
								if (pBullet->getAimedNode()->nodeName == m_sSignedNodeDt.nodeName &&
									pBullet->getAimedNode()->nUUID == m_sSignedNodeDt.nUUID)
								{
									pBullet->unscheduleUpdate();
									pBullet->unschedule("timingDamage");
									//֪ͨ����ӵ��Ĺ����ߣ��Ƴ�Ŀ��
									pBullet->getOwner()->removeAim();
									//Ŀ��ڵ�ָ���
									pBullet->clearAim();
									//����ӵ���ʧ
									pBullet->removeFromParent();
								}
							}
						}

						//��ǰ�Ĺ���
						pEnemyOld->setIsSigned(false);
						//�µ�
						CEnemy* pEnemyNew = (CEnemy*)pSelectedNode;
						if (nullptr != pEnemyNew)
						{
							//�滻��
							pEnemyNew->setIsSigned(true);
							m_sSignedNodeDt.nodeName = pEnemyNew->getName();
							m_sSignedNodeDt.nUUID = pEnemyNew->getUUID();
						}
					}
				}
			}
		}
		else  //û����ѡ�е�
		{
			//ֱ���滻
			CEnemy* pEnmey = (CEnemy*)pSelectedNode;
			pEnmey->setIsSigned(true);
			m_sSignedNodeDt.nodeName = pEnmey->getName();
			m_sSignedNodeDt.nUUID = pEnmey->getUUID();
		}
	}
}

cocos2d::Vec2 CGameScene::getTiledPosByPixel(Vec2 pos)
{
	//x���겻��
	float fX = pos.x;
	//���ӵ�X����
	int nTiledX = fX / m_pTmxTiledMap->getTileSize().width;

	//y����
	float fY = pos.y;
	//��ת��y����
	int nFlipedTiledY = fY / m_pTmxTiledMap->getTileSize().height;	
	//���ӵ�Y����
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
	//ͨ��Tiled��ͼȡ��·����
	TMXObjectGroup* pObjGroup = m_pTmxTiledMap->getObjectGroup("pathObjs");
	ValueVector vObjs = pObjGroup->getObjects();
	for (int i = 0; i < vObjs.size(); ++i)
	{
		ValueMap vMap = vObjs[i].asValueMap();
		//��������  
		float fX = vMap["x"].asFloat();
		float fY = vMap["y"].asFloat();
		Vec2 pos = Vec2(fX, fY);

		//����������ת��Ϊ��������
		Vec2 tiledPos = getTiledPosByPixel(pos);
		//�Ѹ�������ת��Ϊ�����е����������
		Vec2 centerPos = getCenterPixelPosByTiled(tiledPos);
		m_vecPos.push_back(centerPos);
	}

	//ͨ����ͼ�õ���Ʒ����������Ʒ
	TMXObjectGroup* pObject = m_pTmxTiledMap->getObjectGroup("object");
	ValueVector vObjs2 = pObject->getObjects();
	for (int i = 0; i < vObjs2.size(); ++i)
	{
		ValueMap vMap = vObjs2[i].asValueMap();
		//�������� �õ����Ƿ������½ǵ�������������
		float fX = vMap["x"].asFloat();
		float fY = vMap["y"].asFloat();

		//����Ĵ�С
		float fWidth = vMap["width"].asFloat();
		float fHeight = vMap["height"].asFloat();

		//�������κ���
		Rect objectRect = Rect(fX, fY, fWidth, fHeight);
		m_vecObjectRect.push_back(objectRect);

		//id
		string strID = vMap["name"].asString();
		Vec2 pos = Vec2(fX + fWidth / 2, fY + fHeight / 2);
		m_pObjectMgr->addObject(strID, pos);
	}

	//������ͼ
	Size mapSize = m_pTmxTiledMap->getMapSize();	
	//���Խ����ĵз�
	for (int i = 0; i < mapSize.width;++i)
	{
		for (int j = 1; j < mapSize.height;++j)
		{
			//Ϊ0��ʾ�յ�
			if (0 == m_pTmxPathLayer->getTileGIDAt(Vec2(i,j)))
			{
				//�����Ƿ�����Ʒ����
				bool bExist = false;
				//ÿ��������������ĵ�				
				Vec2 centerPos = getCenterPixelPosByTiled(Vec2(i, j));
				for (int m = 0; m < m_vecObjectRect.size();++m)
				{
					//��������ھ��κ�������
					if (m_vecObjectRect[m].containsPoint(centerPos))
					{
						bExist = true;
						break;
					}
				}
				if (!bExist)
				{
					//���Խ����ĵз�
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
	//���ӵ�ͼ��С
	Size mapSize = m_pTmxTiledMap->getMapSize();
	//���Ӵ�С
	Size tileSize = m_pTmxTiledMap->getTileSize();
	//����x����
	float fX = pos.x*tileSize.width + tileSize.width / 2;
	//����y����
	float fY = ((mapSize.height - 1) - pos.y)*tileSize.height + tileSize.height / 2;
	return Vec2(fX, fY);
}

void CGameScene::createBg()
{
	//������ͼ
	m_pTmxTiledMap = TMXTiledMap::create(CDataMgr::getInstance()->getCurLevelDt()->mapName);
	m_nCurCoin = CDataMgr::getInstance()->getCurLevelDt()->nCoin;
	this->addChild(m_pTmxTiledMap);

	//ͨ����ͼ�����·��������ȡ��·����
	m_pTmxPathLayer = m_pTmxTiledMap->getLayer("path");


	Size visibleSize = Director::getInstance()->getVisibleSize();
	//�˵�������
	m_pMenuBgLayer = Layer::create();
	//����ͼ
	Sprite* pMenuBg = Sprite::createWithSpriteFrameName("MenuBG.png");
	pMenuBg->setAnchorPoint(Vec2(0, 0));
	pMenuBg->setPosition(Vec2::ZERO);
	m_pMenuBgLayer->addChild(pMenuBg);

	//���
	m_pCoinLabel = Label::createWithTTF("", "fonts/arial.ttf", 37);
	m_pCoinLabel->setAnchorPoint(Vec2(0, 0.5));
	m_pCoinLabel->setPosition(100, 45);
	m_pCoinLabel->setString(StringUtils::format("%d", m_nCurCoin));
	m_pMenuBgLayer->addChild(m_pCoinLabel);

	//������
	m_pWaveBgLayer = Layer::create();
	m_pWaveBgLayer->setPosition(380, 0);
	m_pMenuBgLayer->addChild(m_pWaveBgLayer);

	//��������ͼ
	Sprite* pWaveBg = Sprite::createWithSpriteFrameName("MenuCenter_01_CN.png");
	pWaveBg->setPosition(Vec2(0, 37));
	m_pWaveBgLayer->addChild(pWaveBg);

	//��ǰ����
	m_pCurWaveLabel = Label::createWithCharMap("ui/numyellow-hd.png", 44, 40, 46);
	m_pCurWaveLabel->setString("01");
	m_pCurWaveLabel->setPosition(-80, 45);
	m_pWaveBgLayer->addChild(m_pCurWaveLabel);

	//�ܲ���
	Label* pTotalWave = Label::createWithTTF("", "fonts/arial.ttf", 32);
	pTotalWave->setString(StringUtils::format("%d", CDataMgr::getInstance()->getCurLevelDt()->nWave));
	pTotalWave->setPosition(10, 45);
	m_pWaveBgLayer->addChild(pTotalWave);

	//���ٰ�ť
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
			//�����߼�
			Director::getInstance()->getScheduler()->setTimeScale(2);
		}
		//������Ч
		SimpleAudioEngine::getInstance()->playEffect("Music/effect/MenuSelect.mp3");
	}, pAcceOffMenu, pAcceOnMenu, nullptr);
	pMenuAcceToggle->setPosition(620, 37);


	//��ͣͼƬ
	m_pPauseSprite = Sprite::createWithSpriteFrameName("MenuCenter_02_CN.png");
	m_pPauseSprite->setPosition(350, 40);
	m_pMenuBgLayer->addChild(m_pPauseSprite);
	//����
	m_pPauseSprite->setVisible(false);


	//��ͣ��ť
	//����ͣ״̬����
	Sprite* pPauseOffSprite = Sprite::createWithSpriteFrameName("pause12.png");
	//��ͣ״̬����
	Sprite* pPauseOnSprite = Sprite::createWithSpriteFrameName("pause02.png");
	
	MenuItemSprite* pPauseOffMenu = MenuItemSprite::create(pPauseOnSprite, pPauseOnSprite);
	MenuItemSprite* pPauseOnMenu = MenuItemSprite::create(pPauseOffSprite, pPauseOffSprite);

	MenuItemToggle* pMenuPauseToggle = MenuItemToggle::createWithCallback([=](Ref* pRef){
		MenuItemToggle* pMenuPauseToggle = (MenuItemToggle*)pRef;
		int nIndex = pMenuPauseToggle->getSelectedIndex();
		if (0 == nIndex) //�����������жϵ�ǰ��ʲô״̬
		{
			//δ��ͣ״̬
			CCLOG("Not Pause");
			m_pWaveBgLayer->setVisible(true);
			m_pPauseSprite->setVisible(false);
			Director::getInstance()->resume();
			//�����ͣ͸����
			LayerColor* pPauseLayerColor = (LayerColor*) this->getChildByName("pauseLayer");
			if (nullptr != pPauseLayerColor)
			{
				//�Ƴ�
				pPauseLayerColor->removeFromParent();
			}
		}
		else if (1 == nIndex)
		{
			CCLOG("Pause");
			m_pWaveBgLayer->setVisible(false);
			m_pPauseSprite->setVisible(true);
			Director::getInstance()->pause();
			//���һ��͸����
			LayerColor* pPauseLayerColor = LayerColor::create(Color4B(0, 0, 0, 0));
			pPauseLayerColor->setName("pauseLayer");
			EventListenerTouchOneByOne* pListener = EventListenerTouchOneByOne::create();
			pListener->onTouchBegan = [=](Touch* pTouch, Event* pEvent){return true; };
			//���ô�����͸
			pListener->setSwallowTouches(true);
			_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, pPauseLayerColor);
			this->addChild(pPauseLayerColor, 9);
		}
		//������Ч
		SimpleAudioEngine::getInstance()->playEffect("Music/effect/MenuSelect.mp3");
	}, pPauseOffMenu, pPauseOnMenu, nullptr);
	pMenuPauseToggle->setPosition(700, 37);

	//������ť����ͣ��
	MenuItemImage* pMenuOperate = MenuItemImage::create("","",
		[=](Ref* pRef){
		//��ͣ��Ϸ
		Director::getInstance()->pause();
		//���һ����ɫ��͸����
		LayerColor* pLayerColor = LayerColor::create(Color4B(0, 0, 0, 100));
		pLayerColor->setName("LayerColor");
		EventListenerTouchOneByOne* pListener = EventListenerTouchOneByOne::create();
		pListener->onTouchBegan = [=](Touch* pTouch, Event* pEvent){return true; };
		//���ô�����͸
		pListener->setSwallowTouches(true);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, pLayerColor);

		//���һ�������
		Layer* pLayer = Layer::create();
		//����
		Sprite* pMenuBg = Sprite::createWithSpriteFrameName("menu_bg.png");
		pMenuBg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
		pLayer->addChild(pMenuBg);

		//������Ч
		SimpleAudioEngine::getInstance()->playEffect("Music/effect/MenuSelect.mp3");

		//�ָ���Ϸ��ť
		MenuItemImage* pMenuImageResume = MenuItemImage::create("","",
			[=](Ref* pRef){
			MenuItemImage* pMenuImageResume = (MenuItemImage*)pRef;
			//�ָ���Ϸ
			Director::getInstance()->resume();
			pMenuImageResume->getParent()->getParent()->getParent()->removeFromParent();
			//������Ч
			SimpleAudioEngine::getInstance()->playEffect("Music/effect/MenuSelect.mp3");
		});
		pMenuImageResume->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("menu_resume_normal_CN.png"));
		pMenuImageResume->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("menu_resume_pressed_CN.png"));
		pMenuImageResume->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 95);

		//������Ϸ��ť
		MenuItemImage* pMenuImageRestart = MenuItemImage::create("", "",
			[=](Ref* pRef){
			MenuItemImage* pMenuImageRestart = (MenuItemImage*)pRef;
			//�ָ���Ϸ
			Director::getInstance()->resume();
			destroy();
			Director::getInstance()->replaceScene(CGameScene::createScene());
			//������Ч
			SimpleAudioEngine::getInstance()->playEffect("Music/effect/MenuSelect.mp3");
		});
		pMenuImageRestart->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("menu_restart_normal_CN.png"));
		pMenuImageRestart->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("menu_restart_pressed_CN.png"));
		pMenuImageRestart->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 2);

		//�˳���ť
		MenuItemImage* pMenuImageQuit = MenuItemImage::create("", "",
			[=](Ref* pRef){
			MenuItemImage* pMenuImageQuit = (MenuItemImage*)pRef;
			//�ָ���Ϸ
			Director::getInstance()->resume();
			destroy();
			Director::getInstance()->replaceScene(CGameLevel::createScene());
			//������Ч
			SimpleAudioEngine::getInstance()->playEffect("Music/effect/MenuSelect.mp3");
			//���ű�������
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
	

	//���ֿ���
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
			//������Ч
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


	//������������
	Animation* pUpgradeAnimation = Animation::create();
	for (int i = 32; i <= 35; ++i)
	{
		string strName = StringUtils::format("air%d.png", i);
		pUpgradeAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(strName));
	}
	pUpgradeAnimation->setDelayPerUnit(0.08f);
	m_pUpgradeAnimate = Animate::create(pUpgradeAnimation);
	m_pUpgradeAnimate->retain();

	//����
	m_pUpgradeAnimateSprite = Sprite::createWithSpriteFrameName("air32.png");
	this->addChild(m_pUpgradeAnimateSprite);
	m_pUpgradeAnimateSprite->setVisible(false);
}

void CGameScene::createMgr()
{
	//������Ʒ������
	m_pObjectMgr = CObjectMgr::create();
	this->addChild(m_pObjectMgr);
	//�������˹�����
	m_pEnemyMgr = CEnmeyMgr::create();
	this->addChild(m_pEnemyMgr);
	//��������������
	m_pWeaponMgr = CWeaponMgr::create();
	this->addChild(m_pWeaponMgr);
	//������Ƭ������
	m_pCardMgr = CCardMgr::create();
	this->addChild(m_pCardMgr);
	//�����ӵ�������
	m_pBulletMgr = CBulletMgr::create();
	this->addChild(m_pBulletMgr);
	//��������������
	m_pAnimateMgr = CAnimateMgr::create();
	this->addChild(m_pAnimateMgr,8);
}

cocos2d::Vec2 CGameScene::convertGLToWorld(Vec2 pos)
{
	//x���겻��
	float fX = pos.x;
	//y���귴����
	float fY = m_pTmxTiledMap->getMapSize().height - pos.y;
	return Vec2(fX, fY);
}

void CGameScene::gameStart(float fDelta)
{
	float fStartTime = 0.0f;
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//���һ��͸����
	LayerColor* pLayerColor = LayerColor::create(Color4B(0, 0, 0, 0));
	pLayerColor->setName("startLayer");
	EventListenerTouchOneByOne* pListener = EventListenerTouchOneByOne::create();
	pListener->onTouchBegan = [=](Touch* pTouch, Event* pEvent){return true; };
	//���ô�����͸
	pListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, pLayerColor);
	this->addChild(pLayerColor, 30);

	//����
	Sprite* pCountBg = Sprite::createWithSpriteFrameName("countdown_11.png");
	pCountBg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	pLayerColor->addChild(pCountBg);

	//תȦ��
	Sprite* pCircle = Sprite::createWithSpriteFrameName("countdown_12.png");
	pCircle->setAnchorPoint(Vec2(0.5, 0.5));
	pCircle->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	RotateBy* pRotate = RotateBy::create(1.f, -360);
	pCircle->runAction(RepeatForever::create(pRotate));
	pLayerColor->addChild(pCircle);

	//����ʱ����
	Sprite* pCountNum = Sprite::createWithSpriteFrameName("countdown_01.png");
	pCountNum->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	ScaleTo* pScaleToBig = ScaleTo::create(0.2f, 1.2f);
	pScaleToBig->retain();
	ScaleTo* pScaleToSmall = ScaleTo::create(0.02f, 1.1f);
	pScaleToSmall->retain();

	//������Ч
	SimpleAudioEngine::getInstance()->playEffect("Music/effect/CountDown.mp3");

	CallFunc* pCallFunc = CallFunc::create([=](){
		pCountNum->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("countdown_02.png"));
		pCountNum->setScale(1);
		//������Ч
		SimpleAudioEngine::getInstance()->playEffect("Music/effect/CountDown.mp3");
		CallFunc* pCallFunc2 = CallFunc::create([=](){
			pCountNum->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("countdown_03.png"));
			pCountNum->setScale(1);
			//������Ч
			SimpleAudioEngine::getInstance()->playEffect("Music/effect/CountDown.mp3");
			CallFunc* pTimeOver = CallFunc::create([=](){
				pCountNum->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("countdown_13.png"));
				pCountNum->setScale(1);
				//����GO
				SimpleAudioEngine::getInstance()->playEffect("Music/effect/GO.mp3");
				RotateBy* pRotate = RotateBy::create(0.2f, -360);
				//��Ϸ��ʼ
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


	//��ʼλ�ü�ͷ
	Vec2 pixelPos = m_vecPos[0];
	//�ж�λ�ã�x���꣬�ı䷽��
	if (0 < m_vecPos[0].x - m_vecPos[1].x)
	{
		//����
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
		if (0 > m_vecPos[0].x - m_vecPos[1].x) //�ҷ���
		{
			m_pAnimateMgr->createArrow(Vec2(pixelPos.x + 50, pixelPos.y), 0);

			this->scheduleOnce([=](float fDelta){
				m_pAnimateMgr->createArrow(Vec2(pixelPos.x + 90, pixelPos.y), 0);
			}, 0.5f, "createArrow");

			this->scheduleOnce([=](float fDelta){
				m_pAnimateMgr->createArrow(Vec2(pixelPos.x + 130, pixelPos.y), 0);
			}, 1.0f, "createArrow2");
		}
		else //�ж�Y����
		{
			if (m_vecPos[0].y > m_vecPos[1].y)
			{
				//����
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
				//����
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
	//����������ת��Ϊ������������
	//��ת��Ϊ��������
	Vec2 tiledPos = getTiledPosByPixel(pixelPos);
	//�����������
	Vec2 centerPos = getCenterPixelPosByTiled(tiledPos);

	//Ŀ������
	int nType = E_AIM_NONE;

	if (0 == tiledPos.y)
	{
		nType = E_AIM_TOP;//״̬��
		return nType;
	}

	if (m_pRadish->getSpriteWorldBox().containsPoint(pixelPos))
	{
		nType = E_AIM_RADISH;
		return nType;
	}

	//�õ����е�����
	Vector<Node*> VecWeapons = m_pWeaponMgr->getChildren();
	//�жϵ���ĵط���û������
	for (int i = 0; i < VecWeapons.size();++i)
	{
		CWeapon* pWeapon = (CWeapon*)VecWeapons.at(i);
		//�����ǵ�һ�ε��
		if (centerPos == pWeapon->getPosition())
		{
			//��ʾ������
			nType = E_AIM_TOWER;
			pNode = VecWeapons.at(i);
			return nType;
		}
	}

	//�õ����еĵ���
	Vector<Node*> VecEnemys = m_pEnemyMgr->getChildren();
	//�жϵ���ĵط���û�е���
	for (int i = 0; i < VecEnemys.size(); ++i)
	{
		CEnemy* pEnemy = (CEnemy*)VecEnemys.at(i);
		if (pEnemy->getBoundingBox().containsPoint(pixelPos) && m_pCardMgr->getIsFirstClick())
		{
			//��ʾ�е���
			nType = E_AIM_ENEMY;
			pNode = VecEnemys.at(i);
			return nType;
		}
	}

	//�ж������ڲ���·����
	int nGid = m_pTmxPathLayer->getTileGIDAt(tiledPos);

	//���û���������ж��ڲ���·����
	if (0 == nGid) //0��ʾ����·����
	{
		//�ж������Ƿ�����Ʒ����
		bool bInObject = false;
		for (int m = 0; m < m_vecObjectRect.size(); ++m)
		{
			//��������ھ��κ�������  �����ǵ�һ�ε����
			bool bContain = m_vecObjectRect[m].containsPoint(centerPos);
			if (m_vecObjectRect[m].containsPoint(centerPos) && m_pCardMgr->getIsFirstClick())
			{
				bInObject = true;
				//�ҵ�����Ʒ
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
	//���½����ʾ
	m_pCoinLabel->setString(StringUtils::format("%d", m_nCurCoin));
	//�����Ƿ�����������
	m_pWeaponMgr->checkUpgrade();
}

void CGameScene::addCoin(int nCoin)
{
	m_nCurCoin += nCoin;
	//���½����ʾ
	m_pCoinLabel->setString(StringUtils::format("%d", m_nCurCoin));
	//�����Ƿ�����������
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
	//�������е��ӵ������еĵ���
	Vector<Node*> Vec_Bullets = m_pBulletMgr->getChildren();
	Vector<Node*> Vec_Enemys = m_pEnemyMgr->getChildren();
	for (Node* enemyNode : Vec_Enemys)
	{
		CEnemy* pEnemy = (CEnemy*)enemyNode;
		for (Node* bulletNode : Vec_Bullets)
		{
			CBullet* pBullet = (CBullet*)bulletNode;
			//֮ǰû����ײ��
			if (!pEnemy->isAttacked(pBullet) &&
				pEnemy->getBoundingBox().containsPoint(pBullet->getPosition()))
			{
				bulletTouchEnemy(pBullet, pEnemy);
			}
		}
	}

	//���������ӵ���������Ʒ
	//Vector<Node*> Vec_Bullets = m_pBulletMgr->getChildren();
	Vector<Node*> Vec_Objects = m_pObjectMgr->getChildren();
	for (Node* bulletNode : Vec_Bullets)
	{
		CBullet* pBullet = (CBullet*)bulletNode;
		if (0 == strcmp(pBullet->getAim().c_str(),"enemy")) //����ӵ�����ʱ��Ŀ���ǵ��ˣ������ӵ��������ж��Ƿ����Ʒ������ײ
		{
			if (0 != strcmp(pBullet->getType().c_str(), "fan")) //������Ǵ�͸����ӵ����ӵ��Ͳ�����Ʒ������ײ
			{
				continue;
			}
		}
		for (Node* objectNode : Vec_Objects)
		{
			CObject* pObject = (CObject*)objectNode;
			//�⼸����Ʒ���ж�
			if (0 == strcmp(pObject->getStrID().c_str(), "601") ||
				0 == strcmp(pObject->getStrID().c_str(), "617") ||
				0 == strcmp(pObject->getStrID().c_str(), "618"))
			{
				continue;
			}
			//֮ǰû����ײ��
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
	//�����ӵ���λ��
	Vec2 bulPos = pBullet->getPosition();
	vector<string> vec_Pngs = pBullet->getBombPngs();
	//�ж��ӵ�����
	if (0 == strcmp(pBullet->getType().c_str(), "bottle"))
	{
		//�Ե�������˺�
		if (pEnemy->damage(pBullet))
		{
			//���ŵ��˱�ը����
			m_pAnimateMgr->playEnemyBombAnimate(pEnemy->getPosition(),pEnemy->getCoinPng());
			//��Ǯ
			addCoin(pEnemy->getCoin());
			//��������
			pEnemy->removeSelf();
		}
		//�ӵ���ʧ
		pBullet->removeFromParent();
		//�����ӵ����ж���
		m_pAnimateMgr->playAnimateByVecPng(bulPos, vec_Pngs);
	}
	else if (0 == strcmp(pBullet->getType().c_str(), "shit"))
	{
		//�Ե�������˺�
		if (pEnemy->damage(pBullet))
		{
			//���ŵ��˱�ը����
			m_pAnimateMgr->playEnemyBombAnimate(pEnemy->getPosition(), pEnemy->getCoinPng());
			//��Ǯ
			addCoin(pEnemy->getCoin());
			//��������
			pEnemy->removeSelf();
		}
		else
		{
			vector<string> vec_hitPng = pBullet->getHitPng();
			//�ƶ�����
			pEnemy->moveHindered(pBullet->getReduceSpeedRate(), pBullet->getContinueTime(), vec_hitPng);
		}
		//�ӵ���ʧ
		pBullet->removeFromParent();
		//�����ӵ����ж���
		m_pAnimateMgr->playAnimateByVecPng(bulPos, vec_Pngs);
	}
	else if (0 == strcmp(pBullet->getType().c_str(), "fan"))
	{
		//�Ե�������˺�
		if (pEnemy->damage(pBullet))
		{
			//���ŵ��˱�ը����
			m_pAnimateMgr->playEnemyBombAnimate(pEnemy->getPosition(), pEnemy->getCoinPng());
			//��Ǯ
			addCoin(pEnemy->getCoin());
			//��������
			pEnemy->removeSelf();
		}  
		//�ӵ�����ʧ
		//�����ӵ����ж���
		m_pAnimateMgr->playAnimateByVecPng(bulPos, vec_Pngs);
	}
	else if (0 == strcmp(pBullet->getType().c_str(), "star")) //�ӵ���ɷ�Χ�˺�
	{
		//�����ӵ�������Χ�ڵĵ��ˣ�����˺�
		pBullet->damageAroundEnemy(150);
		pBullet->removeFromParent();
		//�����ӵ����ж���
		m_pAnimateMgr->playAnimateByVecPng(bulPos, vec_Pngs);
	}
}

void CGameScene::bulletTouchObject(CBullet* pBullet, CObject* pObject)
{
	//�����ӵ���λ��
	Vec2 bulPos = pBullet->getPosition();
	vector<string> vec_Pngs = pBullet->getBombPngs();

	//����Ʒ����˺�
	if (pObject->damage(pBullet))
	{
		//���ű�ը����
		m_pAnimateMgr->playEnemyBombAnimate(pObject->getPosition(), pObject->getCoinPng());
		//��Ǯ
		addCoin(pObject->getCoin());
		//��ȥ����������ľ��κ�
		for (int i = 0; i < m_vecObjectRect.size();++i)
		{
			if (m_vecObjectRect[i].containsPoint(pObject->getPosition()))
			{
				m_vecObjectRect.erase(m_vecObjectRect.begin() + i);
				break;
			}
		}
		//������ʧ
		pObject->removeSelf();
	}
	//�ж��ӵ����� �������fan���͵������ӵ����ӵ�����ʧ
	if (0 != strcmp(pBullet->getType().c_str(), "fan"))
	{
		//�ӵ���ʧ
		pBullet->removeFromParent();
	}
	//�����ӵ����ж���
	m_pAnimateMgr->playAnimateByVecPng(bulPos, vec_Pngs);
}

void CGameScene::gameWin()
{
	//������Ч
	SimpleAudioEngine::getInstance()->playEffect("Music/effect/Perfect.mp3");

	Size visibleSize = Director::getInstance()->getVisibleSize();
	//��ͣ��Ϸ
	Director::getInstance()->pause();
	//���һ����ɫ��͸����
	LayerColor* pLayerColor = LayerColor::create(Color4B(0, 0, 0, 100));
	EventListenerTouchOneByOne* pListener = EventListenerTouchOneByOne::create();
	pListener->onTouchBegan = [=](Touch* pTouch, Event* pEvent){return true; };
	//���ô�����͸
	pListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, pLayerColor);
	this->addChild(pLayerColor, 20);
	//���һ�������
	Layer* pLayer = Layer::create();
	//����
	Sprite* pBg = Sprite::createWithSpriteFrameName("win_bg.png");
	pBg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	pLayer->addChild(pBg);

	//��������
	Sprite* pLableBg = Sprite::createWithSpriteFrameName("win_bg_CN.png");
	pLableBg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	pLayer->addChild(pLableBg);

	//����label
	Label* pWaveLabel = Label::createWithCharMap("ui/numyellow-hd.png", 44, 40, 46);
	pWaveLabel->setString(StringUtils::format("%02d",m_pEnemyMgr->getCurWaveNum()));
	pWaveLabel->setPosition(visibleSize.width / 2 + 5, visibleSize.height / 2 + 27);
	pLayer->addChild(pWaveLabel);

	//�ܲ���
	Label* pTotalWaveLabel = Label::createWithTTF("", "fonts/arial.ttf", 37);
	pTotalWaveLabel->setString(StringUtils::format("%d", CDataMgr::getInstance()->getCurLevelDt()->nWave));
	pTotalWaveLabel->setPosition(visibleSize.width / 2 + 93, visibleSize.height / 2 + 27);
	pLayer->addChild(pTotalWaveLabel);

	//�ؿ�
	Label* pLevelLabel = Label::createWithTTF("", "fonts/arial.ttf", 37);
	int nCurLevel = CDataMgr::getInstance()->getLevelDtMgr()->getCurLevelIndex() + 1;
	pLevelLabel->setString(StringUtils::format("%d", nCurLevel));
	pLevelLabel->setPosition(visibleSize.width / 2 - 55, visibleSize.height / 2 - 28);
	pLayer->addChild(pLevelLabel);

	//����
	Sprite* pGold = Sprite::createWithSpriteFrameName("gainhonor_3.png");
	pGold->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 155);
	pLayer->addChild(pGold);


	//ѡ��ؿ�
	MenuItemImage* pMenuImageReSelect = MenuItemImage::create("", "",
		[=](Ref* pRef){
		//�ָ���Ϸ
		Director::getInstance()->resume();
		destroy();
		//��������
		SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/bg/Accomplish.mp3", true);
		Director::getInstance()->replaceScene(CGameLevel::createScene());
	});
	pMenuImageReSelect->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("select_normal_CN.png"));
	pMenuImageReSelect->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("select_pressed_CN.png"));
	pMenuImageReSelect->setPosition(visibleSize.width / 2 - 115, visibleSize.height / 2 - 107);

	//������Ϸ����һ��
	MenuItemImage* pMenuImageContinue = MenuItemImage::create("", "",
		[=](Ref* pRef){
		//�ָ���Ϸ
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
	//������Ч
	SimpleAudioEngine::getInstance()->playEffect("Music/effect/Lose.mp3");

	Size visibleSize = Director::getInstance()->getVisibleSize();
	//��ͣ��Ϸ
	Director::getInstance()->pause();
	//���һ����ɫ��͸����
	LayerColor* pLayerColor = LayerColor::create(Color4B(0, 0, 0, 100));
	EventListenerTouchOneByOne* pListener = EventListenerTouchOneByOne::create();
	pListener->onTouchBegan = [=](Touch* pTouch, Event* pEvent){return true; };
	//���ô�����͸
	pListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, pLayerColor);
	this->addChild(pLayerColor, 20);
	//���һ�������
	Layer* pLayer = Layer::create();
	//����
	Sprite* pBg = Sprite::createWithSpriteFrameName("lose_bg.png");
	pBg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	pLayer->addChild(pBg);

	//��������
	Sprite* pLableBg = Sprite::createWithSpriteFrameName("lose_bg_CN.png");
	pLableBg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	pLayer->addChild(pLableBg);

	//����label
	Label* pWaveLabel = Label::createWithCharMap("ui/numyellow-hd.png", 44, 40, 46);
	pWaveLabel->setString(StringUtils::format("%02d", m_pEnemyMgr->getCurWaveNum() - 1));
	pWaveLabel->setPosition(visibleSize.width / 2 + 5, visibleSize.height / 2 + 27);
	pLayer->addChild(pWaveLabel);

	//�ܲ���
	Label* pTotalWaveLabel = Label::createWithTTF("", "fonts/arial.ttf", 37);
	pTotalWaveLabel->setString(StringUtils::format("%d", CDataMgr::getInstance()->getCurLevelDt()->nWave));
	pTotalWaveLabel->setPosition(visibleSize.width / 2 + 93, visibleSize.height / 2 + 27);
	pLayer->addChild(pTotalWaveLabel);

	//�ؿ�
	Label* pLevelLabel = Label::createWithTTF("", "fonts/arial.ttf", 37);
	int nCurLevel = CDataMgr::getInstance()->getLevelDtMgr()->getCurLevelIndex() + 1;
	pLevelLabel->setString(StringUtils::format("%d", nCurLevel));
	pLevelLabel->setPosition(visibleSize.width / 2 - 55, visibleSize.height / 2 - 28);
	pLayer->addChild(pLevelLabel);

	//ѡ��ؿ�
	MenuItemImage* pMenuImageReSelect = MenuItemImage::create("", "",
		[=](Ref* pRef){
		//�ָ���Ϸ
		Director::getInstance()->resume();
		destroy();
		//��������
		SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/bg/Accomplish.mp3", true);
		Director::getInstance()->replaceScene(CGameLevel::createScene());
	});
	pMenuImageReSelect->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("select_normal_CN.png"));
	pMenuImageReSelect->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("select_pressed_CN.png"));
	pMenuImageReSelect->setPosition(visibleSize.width / 2 - 115, visibleSize.height / 2 - 107);

	//����
	MenuItemImage* pMenuImageRetry = MenuItemImage::create("", "",
		[=](Ref* pRef){
		//�ָ���Ϸ
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
	//�����ǵ�ʱ���ж���û��ball���͵��ӵ���������ڵ㣬����У��Ͱ�����ӵ����������������ӵ��Ĺ����ߣ�ȥѡ����һ��Ŀ��
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
				//�ж�Ŀ���Ƿ�һ��
				if (pBullet->getAimedNode()->nodeName == m_sSignedNodeDt.nodeName &&
					pBullet->getAimedNode()->nUUID == m_sSignedNodeDt.nUUID)
				{
					pBullet->unscheduleUpdate();
					pBullet->unschedule("timingDamage");
					//֪ͨ����ӵ��Ĺ����ߣ��Ƴ�Ŀ��
					pBullet->getOwner()->removeAim();
					//Ŀ��ڵ�ָ���
					pBullet->clearAim();
					//����ӵ���ʧ
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