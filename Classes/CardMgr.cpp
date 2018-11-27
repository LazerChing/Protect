#include "CardMgr.h"
#include "GameScene.h"


CCardMgr::CCardMgr()
:m_bIsFirstClick(true)
{
	m_pSelectedWeapon = nullptr;
}

CCardMgr::~CCardMgr()
{
	m_pSelectAnimate->release();
	m_pBuildTowerAnimate->release();
}

bool CCardMgr::init()
{
	if (!Node::init())
	{
		return false;
	}
	//隐藏建塔卡片
	m_pCardsNode = Node::create();
	m_pCardsNode->setVisible(false);
	m_pCardsNode->setPosition(Vec2::ZERO);
	this->addChild(m_pCardsNode);

	//刚初始化就隐藏
	//this->setVisible(false);
	//得到当前关卡的数据
	const sLevelDt* pCurLevDt = CDataMgr::getInstance()->getCurLevelDt();
	for (int i = 0; i < pCurLevDt->vecCardID.size();++i)
	{
		sCardDt* pData = CDataMgr::getInstance()->getCardDtMgr()->getDataByID(pCurLevDt->vecCardID[i]);
		CCard* pCard = CCard::createWithData(pData);
		m_vecCards.push_back(pCard);
		//添加节点
		m_pCardsNode->addChild(pCard);
	}

	//先设置第一个的位置
	m_vecCards[0]->setPosition(0,m_vecCards[0]->getContentSize().height);
	for (int i = 1; i < m_vecCards.size();++i)
	{
		//设置第二个及以后的节点位置
		m_vecCards[i]->setPosition(m_vecCards[i - 1]->getPositionX() + m_vecCards[i - 1]->getContentSize().width/2
			+ m_vecCards[i]->getContentSize().width / 2, m_vecCards[i]->getContentSize().height);
	}

	//选中地方的精灵
	m_pSelectTower = Sprite::createWithSpriteFrameName("select_01.png");
	m_pSelectTower->setPosition(Vec2::ZERO);
	m_pSelectTower->setVisible(false);
	this->addChild(m_pSelectTower);

	//创建选中地方的动画
	Animation* pAnimationSelect = Animation::create();
	for (int i = 1; i <= 4; ++i)
	{
		string strName = StringUtils::format("select_%02d.png", i);
		pAnimationSelect->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(strName));
	}
	pAnimationSelect->setDelayPerUnit(0.08f);
	//一直循环
	pAnimationSelect->setLoops(-1);
	m_pSelectAnimate = Animate::create(pAnimationSelect);	
	m_pSelectAnimate->retain();	

	//创建不可操作图片
	m_pDisablePng = Sprite::createWithSpriteFrameName("forbidden.png");
	m_pDisablePng->setVisible(false);
	this->addChild(m_pDisablePng);	


	//隐藏升级卡片
	m_pUpGradeNode = Node::create();
	m_pUpGradeNode->setVisible(false);
	m_pUpGradeNode->setPosition(Vec2::ZERO);
	this->addChild(m_pUpGradeNode);
	m_pRangeSprite = Sprite::createWithSpriteFrameName("range_100.png");
	m_pRangeSprite->setScale(0.5f);
	m_pUpGradeNode->addChild(m_pRangeSprite);

	//升级卡片和出售初始化
	m_pUpgradeCoinSprite = Sprite::create();
	m_pSellSprite = Sprite::create();
	m_pUpGradeNode->addChild(m_pUpgradeCoinSprite);
	m_pUpGradeNode->addChild(m_pSellSprite);


	//创建建塔动画
	Animation* pBuildTowerAnimation= Animation::create();
	for (int i = 11; i <= 15; ++i)
	{
		string strName = StringUtils::format("air%d.png", i);
		pBuildTowerAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(strName));
	}
	pBuildTowerAnimation->setDelayPerUnit(0.05f);
	m_pBuildTowerAnimate = Animate::create(pBuildTowerAnimation);
	m_pBuildTowerAnimate->retain();


	

	return true;
}

void CCardMgr::clickEvent(const Vec2& inClickPos, const int& nType, Node*& pSelectedNode)
{
	//判断是否是第一次点击   建塔和升级塔都不可见
	if (!m_pCardsNode->isVisible() && !m_pUpGradeNode->isVisible())
	{
		Vec2 clickTiledPos = CGameScene::getInstance()->getTiledPosByPixel(inClickPos);//点击位置的格子坐标
		//将点击位置转化为中心点
		Vec2 centerPixelPos = CGameScene::getInstance()->getCenterPixelPosByTiled(clickTiledPos);
		Size tiledMapSize = CGameScene::getInstance()->getTiledMapSize();
		Size tileSize = CGameScene::getInstance()->getTiledSize();

		if (E_AIM_BUILD == nType) //能显示
		{
			//把forbidden设置为不可见
			m_pDisablePng->setVisible(false);

			//判断显示的图片是可以购买还是不可以购买
			for (int i = 0; i < m_vecCards.size(); ++i)
			{
				m_vecCards[i]->updateView(CGameScene::getInstance()->getCurCoin());
			}


			//优化卡片的显示

			//先设置Y坐标
			//如果点击的是最上方 
			if (1 == clickTiledPos.y)
			{
				for (int i = 0; i < m_vecCards.size(); ++i)
				{
					//设置节点Y位置
					m_vecCards[i]->setPositionY(-m_vecCards[i]->getContentSize().height);
				}
			}
			else
			{
				for (int i = 0; i < m_vecCards.size(); ++i)
				{
					//设置节点Y位置
					m_vecCards[i]->setPositionY(m_vecCards[i]->getContentSize().height);
				}
			}

			//再设置X坐标
			//如果点击处的x坐标小于卡片数量的一半，卡片就贴着左边显示
			if (clickTiledPos.x < m_vecCards.size() / 2)
			{				
				if (0 == clickTiledPos.x)
				{
					m_vecCards[0]->setPositionX(0);
				}
				else
				{
					//相差的格子数
					int nDisNum = m_vecCards.size() / 2 - clickTiledPos.x;
					float fDisX = -nDisNum * tileSize.width;
					m_vecCards[0]->setPositionX(fDisX);
				}
				for (int i = 1; i < m_vecCards.size(); ++i)
				{
					//设置第二个及以后的节点位置
					m_vecCards[i]->setPositionX(m_vecCards[i - 1]->getPositionX() + m_vecCards[i - 1]->getContentSize().width / 2
						+ m_vecCards[i]->getContentSize().width / 2);
				}
			}
			else if (tiledMapSize.width - 1 - clickTiledPos.x < m_vecCards.size() / 2)  //如果点击处的倒数x坐标小于卡片数量的一般，卡片就贴着右边显示
			{
				//相差的格子数
				int nDisNum = tiledMapSize.width - 1 - clickTiledPos.x;
				float fDisX = nDisNum * tileSize.width;
				//先设置最后一个卡片的位置
				m_vecCards.back()->setPositionX(fDisX);
				for (int i = m_vecCards.size() - 2; i >= 0; --i)
				{
					m_vecCards[i]->setPositionX(m_vecCards[i + 1]->getPositionX() - m_vecCards[i + 1]->getContentSize().width / 2
						- m_vecCards[i]->getContentSize().width / 2);
				}
			}
			else //一般情况
			{
				float fX = 0.0f;
				//遍历卡片容器，获得所有卡片宽度之和
				for (int i = 0; i < m_vecCards.size(); ++i)
				{
					fX += m_vecCards[i]->getContentSize().width;
				}
				m_vecCards[0]->setPositionX(-fX / 2 + m_vecCards[0]->getContentSize().width / 2);
				for (int i = 1; i < m_vecCards.size(); ++i)
				{
					//设置第二个及以后的节点位置
					m_vecCards[i]->setPositionX(m_vecCards[i - 1]->getPositionX() + m_vecCards[i - 1]->getContentSize().width / 2
						+ m_vecCards[i]->getContentSize().width / 2);
				}
			}


			//设置当前位置为点击位置（中心像素）
			this->setPosition(centerPixelPos);
			m_pCardsNode->setPosition(Vec2::ZERO);
			m_pCardsNode->setVisible(true);
			//卡片全部放大
			for (int i = 0; i < m_vecCards.size();++i)
			{
				//先记下当前位置坐标
				Vec2 tempPos = m_vecCards[i]->getPosition();
				m_vecCards[i]->setPosition(0,0);

				m_vecCards[i]->setScale(0.1f);
				MoveTo* pMoveTo = MoveTo::create(0.1f, tempPos);
				ScaleTo* pScaleTo = ScaleTo::create(0.1f, 1.0f);
				Spawn* pAction = Spawn::createWithTwoActions(pMoveTo, pScaleTo);
				m_vecCards[i]->runAction(pAction);
			}

			m_pSelectTower->setVisible(true);
			//执行动画
			m_pSelectTower->runAction(m_pSelectAnimate);

			//第一次点击完了，就不是第一次点击了
			m_bIsFirstClick = false;
		}
		else if (E_AIM_FORBID == nType)
		{
			//显示不可操作图片
			//设置当前位置为点击位置
			this->setPosition(inClickPos);
			m_pDisablePng->stopAllActions();
			m_pDisablePng->setPosition(Vec2::ZERO);
			//显示不可操作图片 显示 0.8s 隐藏
			CallFunc* pShow = CallFunc::create([=](){
				//还原回来
				m_pDisablePng->setOpacity(255);
				m_pDisablePng->setVisible(true);
				//播放音效
				SimpleAudioEngine::getInstance()->playEffect("Music/effect/SelectFault.mp3");
			});

			FadeOut* pFadeOut = FadeOut::create(0.8f);

			CallFunc* pHide = CallFunc::create([=](){
				m_pDisablePng->setVisible(false);
			});
			FiniteTimeAction* pDisableAction = Sequence::create(pShow, pFadeOut, pHide, nullptr);
			m_pDisablePng->runAction(pDisableAction);
		}
		else if (E_AIM_TOWER == nType) //选中的是塔
		{
			//把forbidden设置为不可见
			m_pDisablePng->setVisible(false);			
			if (nullptr != pSelectedNode)
			{
				ScaleTo* pScaleToRange = ScaleTo::create(0.1f, 1.0f);
				m_pRangeSprite->runAction(Sequence::create(pScaleToRange, nullptr));
				m_pSelectTower->setVisible(true);
				//执行动画
				m_pSelectTower->runAction(m_pSelectAnimate);

				CWeapon* pWeapon = (CWeapon*)pSelectedNode;
				m_pSelectedWeapon = pWeapon;//设置当前选中的武器
				
				bool bCanUpGrade = false;
				//升级图片
				string strUpGradePng = pWeapon->getUpGradePng(bCanUpGrade);
				
				m_pUpgradeCoinSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(strUpGradePng));
				
				//销售图片
				string strSellPng = pWeapon->getStrSellPng();
				m_pSellSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(strSellPng));
				//优化位置
				
				//如果点击的在最上方
				if (1 == clickTiledPos.y)
				{
					//如果点击的在最右边
					if (tiledMapSize.width - 1 == clickTiledPos.x)
					{
						//设置相对父节点的位置
						m_pUpgradeCoinSprite->setPosition(-m_pUpgradeCoinSprite->getContentSize().width, 0);
					}
					else //一般情况
					{						
						m_pUpgradeCoinSprite->setPosition(m_pUpgradeCoinSprite->getContentSize().width, 0);
					}
					m_pSellSprite->setPosition(0, -m_pSellSprite->getContentSize().height);
				}
				else if (tiledMapSize.height - 1 == clickTiledPos.y)  //点击位置在最下方
				{
					//如果点击的在最右边
					if (tiledMapSize.width - 1 == clickTiledPos.x)
					{
						m_pSellSprite->setPosition(-m_pSellSprite->getContentSize().width, 0);
					}
					else
					{
						m_pSellSprite->setPosition(m_pSellSprite->getContentSize().width, 0);;
					}
					m_pUpgradeCoinSprite->setPosition(0, m_pUpgradeCoinSprite->getContentSize().height);
				}
				else
				{
					//一般情况
					m_pUpgradeCoinSprite->setPosition(0, m_pUpgradeCoinSprite->getContentSize().height);
					m_pSellSprite->setPosition(0, -m_pSellSprite->getContentSize().height);
				}

				//显示炮塔升级
				//设置当前位置为点击位置
				this->setPosition(centerPixelPos);
				m_pUpGradeNode->setVisible(true);
				m_pUpGradeNode->setPosition(Vec2::ZERO);

				//先记下当前升级图片位置坐标
				Vec2 tempUpgradePos = m_pUpgradeCoinSprite->getPosition();
				m_pUpgradeCoinSprite->setPosition(0, 0);

				m_pUpgradeCoinSprite->setScale(0.1f);
				MoveTo* pMoveTo = MoveTo::create(0.1f, tempUpgradePos);
				ScaleTo* pScaleTo = ScaleTo::create(0.1f, 1.0f);
				Spawn* pAction = Spawn::createWithTwoActions(pMoveTo, pScaleTo);
				m_pUpgradeCoinSprite->runAction(pAction);

				//记下出售图片位置
				Vec2 tempSellPos = m_pSellSprite->getPosition();
				m_pSellSprite->setPosition(0, 0);

				m_pSellSprite->setScale(0.1f);
				MoveTo* pMoveToSell = MoveTo::create(0.1f, tempSellPos);
				ScaleTo* pScaleToSell = ScaleTo::create(0.1f, 1.0f);
				Spawn* pActionSell = Spawn::createWithTwoActions(pMoveToSell, pScaleToSell);
				m_pSellSprite->runAction(pActionSell);

				//播放音效
				SimpleAudioEngine::getInstance()->playEffect("Music/effect/TowerSelect.mp3");

				CCLOG("WeaponID %d", pWeapon->getID());				
			}

			//第一次点击完了，就不是第一次点击了
			m_bIsFirstClick = false;
		}

	}
	else  //第二次点击，判断不在点击层中
	{
		//判断是否创建卡片
		if (m_pCardsNode->isVisible())
		{
			//判断点击的不是建塔
			//把世界坐标转换为节点坐标，因为节点上可能有多个精灵，如果节点坐标转为世界坐标需要转换多次
			Vec2 nodePos = this->convertToNodeSpace(inClickPos);
			//遍历容器
			for (int i = 0; i < m_vecCards.size(); ++i)
			{
				if (m_vecCards[i]->getBoundingBox().containsPoint(nodePos))
				{	
					//判断金币够不够
					if (m_vecCards[i]->canBuy(CGameScene::getInstance()->getCurCoin()))
					{
						CCLOG("%d", m_vecCards[i]->getID());
						//进行建塔
						Vec2 tiledPos = CGameScene::getInstance()->getTiledPosByPixel(this->getPosition());
						Vec2 centerPixelPos = CGameScene::getInstance()->getCenterPixelPosByTiled(tiledPos);
						//先把金币保留下来
						int nCoin = m_vecCards[i]->getNeedCoin();
						CGameScene::getInstance()->getWeaponMgr()->addWeapon(m_vecCards[i]->getWeaponID(), centerPixelPos);
						//减少金币  减钱逻辑在最后
						CGameScene::getInstance()->reduceCoin(nCoin);
						//移除可建塔的坐标
						CGameScene::getInstance()->removeCanBuildPos(centerPixelPos);
						//播放建塔动画
						Sprite* pBuildTowerSprite = Sprite::create();
						pBuildTowerSprite->setPosition(Vec2::ZERO);
						this->addChild(pBuildTowerSprite);
						pBuildTowerSprite->runAction(Sequence::create(m_pBuildTowerAnimate, RemoveSelf::create(), nullptr));
						//播放音效
						SimpleAudioEngine::getInstance()->playEffect("Music/effect/TowerBulid.mp3");
						break;
					}
					else
					{
						//播放音效，提示不可购买
						SimpleAudioEngine::getInstance()->playEffect("Music/effect/SelectFault.mp3");
					}
				}
			}
		
			//卡片全部缩小
			for (int i = 0; i < m_vecCards.size(); ++i)
			{
				MoveTo* pMoveTo = MoveTo::create(0.1f, Vec2::ZERO);
				ScaleTo* pScaleTo = ScaleTo::create(0.1f, 0.1f);
				Spawn* pAction = Spawn::createWithTwoActions(pMoveTo, pScaleTo);
				CallFunc* pActionOver = CallFunc::create([=](){
					//停止动画
					m_pSelectTower->stopAllActions();
					m_pSelectTower->setVisible(false);
					//隐藏
					m_pCardsNode->setVisible(false);
				});
				m_vecCards[i]->runAction(Sequence::createWithTwoActions(pAction,pActionOver));
			}
			//播放音效
			SimpleAudioEngine::getInstance()->playEffect("Music/effect/TowerDeselect.mp3");
		}
		else if (m_pUpGradeNode->isVisible()) //升级卡片的
		{
			//转化点击点为节点坐标系
			Vec2 nodePos = this->convertToNodeSpace(inClickPos);
			//如果是升级
			if (m_pUpgradeCoinSprite->getBoundingBox().containsPoint(nodePos))
			{
				if (nullptr != m_pSelectedWeapon) //当前选中的武器不为空
				{
					bool bCanUpgrade = false;
					m_pSelectedWeapon->getUpGradePng(bCanUpgrade);
					if (bCanUpgrade) //如果能升级
					{
						Vec2 tiledPos = CGameScene::getInstance()->getTiledPosByPixel(this->getPosition());
						Vec2 centerPixelPos = CGameScene::getInstance()->getCenterPixelPosByTiled(tiledPos);
						//先把金币保留下来
						int nCoin = m_pSelectedWeapon->getUpGradeCoin();
						//调用升级接口
						CGameScene::getInstance()->getWeaponMgr()->upgradeWeapon(m_pSelectedWeapon, centerPixelPos);
						//金币减少  减钱逻辑在最后
						CGameScene::getInstance()->reduceCoin(nCoin);

						//升级动画
						//向上的动画
						Sprite* pUpgradeUp = Sprite::createWithSpriteFrameName("air31.png");
						pUpgradeUp->setAnchorPoint(Vec2(0.5, 0));
						pUpgradeUp->setPosition(Vec2(0, 30));
						this->addChild(pUpgradeUp);
						ScaleBy* pScaleBy = ScaleBy::create(0.32f, 1.f, 1.5f);
						pUpgradeUp->runAction(Sequence::create(pScaleBy, RemoveSelf::create(), nullptr));
						//左右的动画
						//调用GameScene里的函数，因为层级关系，会覆盖炮塔
						CGameScene::getInstance()->playUpgradeWeaponAnimate(this->getPosition());
						//播放音效
						SimpleAudioEngine::getInstance()->playEffect("Music/effect/TowerUpdata.mp3");
					}
					else
					{
						//播放音效，提示不可升级
						SimpleAudioEngine::getInstance()->playEffect("Music/effect/SelectFault.mp3");
					}
				}
			}
			else if (m_pSellSprite->getBoundingBox().containsPoint(nodePos))
			{
				if (nullptr != m_pSelectedWeapon) //当前选中的武器不为空
				{
					//先把金币保留下来
					int nCoin = m_pSelectedWeapon->getSellCoin();
					//调用出售接口
					CGameScene::getInstance()->getWeaponMgr()->sellWeapon(m_pSelectedWeapon);
					//金币增加  增钱逻辑在最后
					CGameScene::getInstance()->addCoin(nCoin);
					//播放移除炮塔动画
					Sprite* pBuildTowerSprite = Sprite::create();
					pBuildTowerSprite->setPosition(Vec2::ZERO);
					this->addChild(pBuildTowerSprite);
					pBuildTowerSprite->runAction(Sequence::create(m_pBuildTowerAnimate, RemoveSelf::create(), nullptr));
					//播放音效
					SimpleAudioEngine::getInstance()->playEffect("Music/effect/TowerSell.mp3");
				}
			}

			CallFunc* pScaleStart = CallFunc::create([=](){
				//升级图标缩小
				ScaleTo* pScaleTo = ScaleTo::create(0.1f, 0.5f);
				m_pRangeSprite->runAction(pScaleTo);
				MoveTo* pMoveTo2 = MoveTo::create(0.1f, Vec2::ZERO);
				ScaleTo* pScaleTo2 = ScaleTo::create(0.1f, 0.1f);
				Spawn* pAction2 = Spawn::createWithTwoActions(pMoveTo2, pScaleTo2);
				m_pUpgradeCoinSprite->runAction(pAction2->clone());
				m_pSellSprite->runAction(pAction2->clone());
			});
			
			CallFunc* pScaleOver = CallFunc::create([=](){
				m_pSelectTower->stopAllActions();
				m_pSelectTower->setVisible(false);
				m_pUpGradeNode->setVisible(false);				
			});
			m_pRangeSprite->runAction(Sequence::create(pScaleStart, DelayTime::create(0.1f), pScaleOver, nullptr));
			//播放音效
			SimpleAudioEngine::getInstance()->playEffect("Music/effect/TowerDeselect.mp3");
		}

		//第二次点击完了，就是第一点击了
		m_bIsFirstClick = true;
	}
}
