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
	//���ؽ�����Ƭ
	m_pCardsNode = Node::create();
	m_pCardsNode->setVisible(false);
	m_pCardsNode->setPosition(Vec2::ZERO);
	this->addChild(m_pCardsNode);

	//�ճ�ʼ��������
	//this->setVisible(false);
	//�õ���ǰ�ؿ�������
	const sLevelDt* pCurLevDt = CDataMgr::getInstance()->getCurLevelDt();
	for (int i = 0; i < pCurLevDt->vecCardID.size();++i)
	{
		sCardDt* pData = CDataMgr::getInstance()->getCardDtMgr()->getDataByID(pCurLevDt->vecCardID[i]);
		CCard* pCard = CCard::createWithData(pData);
		m_vecCards.push_back(pCard);
		//��ӽڵ�
		m_pCardsNode->addChild(pCard);
	}

	//�����õ�һ����λ��
	m_vecCards[0]->setPosition(0,m_vecCards[0]->getContentSize().height);
	for (int i = 1; i < m_vecCards.size();++i)
	{
		//���õڶ������Ժ�Ľڵ�λ��
		m_vecCards[i]->setPosition(m_vecCards[i - 1]->getPositionX() + m_vecCards[i - 1]->getContentSize().width/2
			+ m_vecCards[i]->getContentSize().width / 2, m_vecCards[i]->getContentSize().height);
	}

	//ѡ�еط��ľ���
	m_pSelectTower = Sprite::createWithSpriteFrameName("select_01.png");
	m_pSelectTower->setPosition(Vec2::ZERO);
	m_pSelectTower->setVisible(false);
	this->addChild(m_pSelectTower);

	//����ѡ�еط��Ķ���
	Animation* pAnimationSelect = Animation::create();
	for (int i = 1; i <= 4; ++i)
	{
		string strName = StringUtils::format("select_%02d.png", i);
		pAnimationSelect->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(strName));
	}
	pAnimationSelect->setDelayPerUnit(0.08f);
	//һֱѭ��
	pAnimationSelect->setLoops(-1);
	m_pSelectAnimate = Animate::create(pAnimationSelect);	
	m_pSelectAnimate->retain();	

	//�������ɲ���ͼƬ
	m_pDisablePng = Sprite::createWithSpriteFrameName("forbidden.png");
	m_pDisablePng->setVisible(false);
	this->addChild(m_pDisablePng);	


	//����������Ƭ
	m_pUpGradeNode = Node::create();
	m_pUpGradeNode->setVisible(false);
	m_pUpGradeNode->setPosition(Vec2::ZERO);
	this->addChild(m_pUpGradeNode);
	m_pRangeSprite = Sprite::createWithSpriteFrameName("range_100.png");
	m_pRangeSprite->setScale(0.5f);
	m_pUpGradeNode->addChild(m_pRangeSprite);

	//������Ƭ�ͳ��۳�ʼ��
	m_pUpgradeCoinSprite = Sprite::create();
	m_pSellSprite = Sprite::create();
	m_pUpGradeNode->addChild(m_pUpgradeCoinSprite);
	m_pUpGradeNode->addChild(m_pSellSprite);


	//������������
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
	//�ж��Ƿ��ǵ�һ�ε��   �����������������ɼ�
	if (!m_pCardsNode->isVisible() && !m_pUpGradeNode->isVisible())
	{
		Vec2 clickTiledPos = CGameScene::getInstance()->getTiledPosByPixel(inClickPos);//���λ�õĸ�������
		//�����λ��ת��Ϊ���ĵ�
		Vec2 centerPixelPos = CGameScene::getInstance()->getCenterPixelPosByTiled(clickTiledPos);
		Size tiledMapSize = CGameScene::getInstance()->getTiledMapSize();
		Size tileSize = CGameScene::getInstance()->getTiledSize();

		if (E_AIM_BUILD == nType) //����ʾ
		{
			//��forbidden����Ϊ���ɼ�
			m_pDisablePng->setVisible(false);

			//�ж���ʾ��ͼƬ�ǿ��Թ����ǲ����Թ���
			for (int i = 0; i < m_vecCards.size(); ++i)
			{
				m_vecCards[i]->updateView(CGameScene::getInstance()->getCurCoin());
			}


			//�Ż���Ƭ����ʾ

			//������Y����
			//�������������Ϸ� 
			if (1 == clickTiledPos.y)
			{
				for (int i = 0; i < m_vecCards.size(); ++i)
				{
					//���ýڵ�Yλ��
					m_vecCards[i]->setPositionY(-m_vecCards[i]->getContentSize().height);
				}
			}
			else
			{
				for (int i = 0; i < m_vecCards.size(); ++i)
				{
					//���ýڵ�Yλ��
					m_vecCards[i]->setPositionY(m_vecCards[i]->getContentSize().height);
				}
			}

			//������X����
			//����������x����С�ڿ�Ƭ������һ�룬��Ƭ�����������ʾ
			if (clickTiledPos.x < m_vecCards.size() / 2)
			{				
				if (0 == clickTiledPos.x)
				{
					m_vecCards[0]->setPositionX(0);
				}
				else
				{
					//���ĸ�����
					int nDisNum = m_vecCards.size() / 2 - clickTiledPos.x;
					float fDisX = -nDisNum * tileSize.width;
					m_vecCards[0]->setPositionX(fDisX);
				}
				for (int i = 1; i < m_vecCards.size(); ++i)
				{
					//���õڶ������Ժ�Ľڵ�λ��
					m_vecCards[i]->setPositionX(m_vecCards[i - 1]->getPositionX() + m_vecCards[i - 1]->getContentSize().width / 2
						+ m_vecCards[i]->getContentSize().width / 2);
				}
			}
			else if (tiledMapSize.width - 1 - clickTiledPos.x < m_vecCards.size() / 2)  //���������ĵ���x����С�ڿ�Ƭ������һ�㣬��Ƭ�������ұ���ʾ
			{
				//���ĸ�����
				int nDisNum = tiledMapSize.width - 1 - clickTiledPos.x;
				float fDisX = nDisNum * tileSize.width;
				//���������һ����Ƭ��λ��
				m_vecCards.back()->setPositionX(fDisX);
				for (int i = m_vecCards.size() - 2; i >= 0; --i)
				{
					m_vecCards[i]->setPositionX(m_vecCards[i + 1]->getPositionX() - m_vecCards[i + 1]->getContentSize().width / 2
						- m_vecCards[i]->getContentSize().width / 2);
				}
			}
			else //һ�����
			{
				float fX = 0.0f;
				//������Ƭ������������п�Ƭ���֮��
				for (int i = 0; i < m_vecCards.size(); ++i)
				{
					fX += m_vecCards[i]->getContentSize().width;
				}
				m_vecCards[0]->setPositionX(-fX / 2 + m_vecCards[0]->getContentSize().width / 2);
				for (int i = 1; i < m_vecCards.size(); ++i)
				{
					//���õڶ������Ժ�Ľڵ�λ��
					m_vecCards[i]->setPositionX(m_vecCards[i - 1]->getPositionX() + m_vecCards[i - 1]->getContentSize().width / 2
						+ m_vecCards[i]->getContentSize().width / 2);
				}
			}


			//���õ�ǰλ��Ϊ���λ�ã��������أ�
			this->setPosition(centerPixelPos);
			m_pCardsNode->setPosition(Vec2::ZERO);
			m_pCardsNode->setVisible(true);
			//��Ƭȫ���Ŵ�
			for (int i = 0; i < m_vecCards.size();++i)
			{
				//�ȼ��µ�ǰλ������
				Vec2 tempPos = m_vecCards[i]->getPosition();
				m_vecCards[i]->setPosition(0,0);

				m_vecCards[i]->setScale(0.1f);
				MoveTo* pMoveTo = MoveTo::create(0.1f, tempPos);
				ScaleTo* pScaleTo = ScaleTo::create(0.1f, 1.0f);
				Spawn* pAction = Spawn::createWithTwoActions(pMoveTo, pScaleTo);
				m_vecCards[i]->runAction(pAction);
			}

			m_pSelectTower->setVisible(true);
			//ִ�ж���
			m_pSelectTower->runAction(m_pSelectAnimate);

			//��һ�ε�����ˣ��Ͳ��ǵ�һ�ε����
			m_bIsFirstClick = false;
		}
		else if (E_AIM_FORBID == nType)
		{
			//��ʾ���ɲ���ͼƬ
			//���õ�ǰλ��Ϊ���λ��
			this->setPosition(inClickPos);
			m_pDisablePng->stopAllActions();
			m_pDisablePng->setPosition(Vec2::ZERO);
			//��ʾ���ɲ���ͼƬ ��ʾ 0.8s ����
			CallFunc* pShow = CallFunc::create([=](){
				//��ԭ����
				m_pDisablePng->setOpacity(255);
				m_pDisablePng->setVisible(true);
				//������Ч
				SimpleAudioEngine::getInstance()->playEffect("Music/effect/SelectFault.mp3");
			});

			FadeOut* pFadeOut = FadeOut::create(0.8f);

			CallFunc* pHide = CallFunc::create([=](){
				m_pDisablePng->setVisible(false);
			});
			FiniteTimeAction* pDisableAction = Sequence::create(pShow, pFadeOut, pHide, nullptr);
			m_pDisablePng->runAction(pDisableAction);
		}
		else if (E_AIM_TOWER == nType) //ѡ�е�����
		{
			//��forbidden����Ϊ���ɼ�
			m_pDisablePng->setVisible(false);			
			if (nullptr != pSelectedNode)
			{
				ScaleTo* pScaleToRange = ScaleTo::create(0.1f, 1.0f);
				m_pRangeSprite->runAction(Sequence::create(pScaleToRange, nullptr));
				m_pSelectTower->setVisible(true);
				//ִ�ж���
				m_pSelectTower->runAction(m_pSelectAnimate);

				CWeapon* pWeapon = (CWeapon*)pSelectedNode;
				m_pSelectedWeapon = pWeapon;//���õ�ǰѡ�е�����
				
				bool bCanUpGrade = false;
				//����ͼƬ
				string strUpGradePng = pWeapon->getUpGradePng(bCanUpGrade);
				
				m_pUpgradeCoinSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(strUpGradePng));
				
				//����ͼƬ
				string strSellPng = pWeapon->getStrSellPng();
				m_pSellSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(strSellPng));
				//�Ż�λ��
				
				//�������������Ϸ�
				if (1 == clickTiledPos.y)
				{
					//�������������ұ�
					if (tiledMapSize.width - 1 == clickTiledPos.x)
					{
						//������Ը��ڵ��λ��
						m_pUpgradeCoinSprite->setPosition(-m_pUpgradeCoinSprite->getContentSize().width, 0);
					}
					else //һ�����
					{						
						m_pUpgradeCoinSprite->setPosition(m_pUpgradeCoinSprite->getContentSize().width, 0);
					}
					m_pSellSprite->setPosition(0, -m_pSellSprite->getContentSize().height);
				}
				else if (tiledMapSize.height - 1 == clickTiledPos.y)  //���λ�������·�
				{
					//�������������ұ�
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
					//һ�����
					m_pUpgradeCoinSprite->setPosition(0, m_pUpgradeCoinSprite->getContentSize().height);
					m_pSellSprite->setPosition(0, -m_pSellSprite->getContentSize().height);
				}

				//��ʾ��������
				//���õ�ǰλ��Ϊ���λ��
				this->setPosition(centerPixelPos);
				m_pUpGradeNode->setVisible(true);
				m_pUpGradeNode->setPosition(Vec2::ZERO);

				//�ȼ��µ�ǰ����ͼƬλ������
				Vec2 tempUpgradePos = m_pUpgradeCoinSprite->getPosition();
				m_pUpgradeCoinSprite->setPosition(0, 0);

				m_pUpgradeCoinSprite->setScale(0.1f);
				MoveTo* pMoveTo = MoveTo::create(0.1f, tempUpgradePos);
				ScaleTo* pScaleTo = ScaleTo::create(0.1f, 1.0f);
				Spawn* pAction = Spawn::createWithTwoActions(pMoveTo, pScaleTo);
				m_pUpgradeCoinSprite->runAction(pAction);

				//���³���ͼƬλ��
				Vec2 tempSellPos = m_pSellSprite->getPosition();
				m_pSellSprite->setPosition(0, 0);

				m_pSellSprite->setScale(0.1f);
				MoveTo* pMoveToSell = MoveTo::create(0.1f, tempSellPos);
				ScaleTo* pScaleToSell = ScaleTo::create(0.1f, 1.0f);
				Spawn* pActionSell = Spawn::createWithTwoActions(pMoveToSell, pScaleToSell);
				m_pSellSprite->runAction(pActionSell);

				//������Ч
				SimpleAudioEngine::getInstance()->playEffect("Music/effect/TowerSelect.mp3");

				CCLOG("WeaponID %d", pWeapon->getID());				
			}

			//��һ�ε�����ˣ��Ͳ��ǵ�һ�ε����
			m_bIsFirstClick = false;
		}

	}
	else  //�ڶ��ε�����жϲ��ڵ������
	{
		//�ж��Ƿ񴴽���Ƭ
		if (m_pCardsNode->isVisible())
		{
			//�жϵ���Ĳ��ǽ���
			//����������ת��Ϊ�ڵ����꣬��Ϊ�ڵ��Ͽ����ж�����飬����ڵ�����תΪ����������Ҫת�����
			Vec2 nodePos = this->convertToNodeSpace(inClickPos);
			//��������
			for (int i = 0; i < m_vecCards.size(); ++i)
			{
				if (m_vecCards[i]->getBoundingBox().containsPoint(nodePos))
				{	
					//�жϽ�ҹ�����
					if (m_vecCards[i]->canBuy(CGameScene::getInstance()->getCurCoin()))
					{
						CCLOG("%d", m_vecCards[i]->getID());
						//���н���
						Vec2 tiledPos = CGameScene::getInstance()->getTiledPosByPixel(this->getPosition());
						Vec2 centerPixelPos = CGameScene::getInstance()->getCenterPixelPosByTiled(tiledPos);
						//�Ȱѽ�ұ�������
						int nCoin = m_vecCards[i]->getNeedCoin();
						CGameScene::getInstance()->getWeaponMgr()->addWeapon(m_vecCards[i]->getWeaponID(), centerPixelPos);
						//���ٽ��  ��Ǯ�߼������
						CGameScene::getInstance()->reduceCoin(nCoin);
						//�Ƴ��ɽ���������
						CGameScene::getInstance()->removeCanBuildPos(centerPixelPos);
						//���Ž�������
						Sprite* pBuildTowerSprite = Sprite::create();
						pBuildTowerSprite->setPosition(Vec2::ZERO);
						this->addChild(pBuildTowerSprite);
						pBuildTowerSprite->runAction(Sequence::create(m_pBuildTowerAnimate, RemoveSelf::create(), nullptr));
						//������Ч
						SimpleAudioEngine::getInstance()->playEffect("Music/effect/TowerBulid.mp3");
						break;
					}
					else
					{
						//������Ч����ʾ���ɹ���
						SimpleAudioEngine::getInstance()->playEffect("Music/effect/SelectFault.mp3");
					}
				}
			}
		
			//��Ƭȫ����С
			for (int i = 0; i < m_vecCards.size(); ++i)
			{
				MoveTo* pMoveTo = MoveTo::create(0.1f, Vec2::ZERO);
				ScaleTo* pScaleTo = ScaleTo::create(0.1f, 0.1f);
				Spawn* pAction = Spawn::createWithTwoActions(pMoveTo, pScaleTo);
				CallFunc* pActionOver = CallFunc::create([=](){
					//ֹͣ����
					m_pSelectTower->stopAllActions();
					m_pSelectTower->setVisible(false);
					//����
					m_pCardsNode->setVisible(false);
				});
				m_vecCards[i]->runAction(Sequence::createWithTwoActions(pAction,pActionOver));
			}
			//������Ч
			SimpleAudioEngine::getInstance()->playEffect("Music/effect/TowerDeselect.mp3");
		}
		else if (m_pUpGradeNode->isVisible()) //������Ƭ��
		{
			//ת�������Ϊ�ڵ�����ϵ
			Vec2 nodePos = this->convertToNodeSpace(inClickPos);
			//���������
			if (m_pUpgradeCoinSprite->getBoundingBox().containsPoint(nodePos))
			{
				if (nullptr != m_pSelectedWeapon) //��ǰѡ�е�������Ϊ��
				{
					bool bCanUpgrade = false;
					m_pSelectedWeapon->getUpGradePng(bCanUpgrade);
					if (bCanUpgrade) //���������
					{
						Vec2 tiledPos = CGameScene::getInstance()->getTiledPosByPixel(this->getPosition());
						Vec2 centerPixelPos = CGameScene::getInstance()->getCenterPixelPosByTiled(tiledPos);
						//�Ȱѽ�ұ�������
						int nCoin = m_pSelectedWeapon->getUpGradeCoin();
						//���������ӿ�
						CGameScene::getInstance()->getWeaponMgr()->upgradeWeapon(m_pSelectedWeapon, centerPixelPos);
						//��Ҽ���  ��Ǯ�߼������
						CGameScene::getInstance()->reduceCoin(nCoin);

						//��������
						//���ϵĶ���
						Sprite* pUpgradeUp = Sprite::createWithSpriteFrameName("air31.png");
						pUpgradeUp->setAnchorPoint(Vec2(0.5, 0));
						pUpgradeUp->setPosition(Vec2(0, 30));
						this->addChild(pUpgradeUp);
						ScaleBy* pScaleBy = ScaleBy::create(0.32f, 1.f, 1.5f);
						pUpgradeUp->runAction(Sequence::create(pScaleBy, RemoveSelf::create(), nullptr));
						//���ҵĶ���
						//����GameScene��ĺ�������Ϊ�㼶��ϵ���Ḳ������
						CGameScene::getInstance()->playUpgradeWeaponAnimate(this->getPosition());
						//������Ч
						SimpleAudioEngine::getInstance()->playEffect("Music/effect/TowerUpdata.mp3");
					}
					else
					{
						//������Ч����ʾ��������
						SimpleAudioEngine::getInstance()->playEffect("Music/effect/SelectFault.mp3");
					}
				}
			}
			else if (m_pSellSprite->getBoundingBox().containsPoint(nodePos))
			{
				if (nullptr != m_pSelectedWeapon) //��ǰѡ�е�������Ϊ��
				{
					//�Ȱѽ�ұ�������
					int nCoin = m_pSelectedWeapon->getSellCoin();
					//���ó��۽ӿ�
					CGameScene::getInstance()->getWeaponMgr()->sellWeapon(m_pSelectedWeapon);
					//�������  ��Ǯ�߼������
					CGameScene::getInstance()->addCoin(nCoin);
					//�����Ƴ���������
					Sprite* pBuildTowerSprite = Sprite::create();
					pBuildTowerSprite->setPosition(Vec2::ZERO);
					this->addChild(pBuildTowerSprite);
					pBuildTowerSprite->runAction(Sequence::create(m_pBuildTowerAnimate, RemoveSelf::create(), nullptr));
					//������Ч
					SimpleAudioEngine::getInstance()->playEffect("Music/effect/TowerSell.mp3");
				}
			}

			CallFunc* pScaleStart = CallFunc::create([=](){
				//����ͼ����С
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
			//������Ч
			SimpleAudioEngine::getInstance()->playEffect("Music/effect/TowerDeselect.mp3");
		}

		//�ڶ��ε�����ˣ����ǵ�һ�����
		m_bIsFirstClick = true;
	}
}
