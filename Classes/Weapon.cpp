#include "Weapon.h"
#include "DataMgr.h"
#include "GameScene.h"

CWeapon::CWeapon()
{
	m_pBg = nullptr;
	m_pWeapon = nullptr;
	m_pFireAnimate = nullptr;
	m_bIsMax = false;
	m_nBulletID = 0;
	m_fFireInterval = 0.f;
	m_nRange = 0;
	m_sAimedNode = sSignedNodeDt();
}

CWeapon::~CWeapon()
{
	m_pFireAnimate->release();
	m_pStopFunc->release();
	m_pShowUpgradeAnimate->release();
}

CWeapon* CWeapon::createWithData(sWeaponDt* pData)
{
	CWeapon* pWeapon = new CWeapon();
	if (nullptr != pWeapon && pWeapon->initWithData(pData))
	{
		pWeapon->autorelease();
		return pWeapon;
	}
	CC_SAFE_DELETE(pWeapon);
	return nullptr;
}

bool CWeapon::initWithData(sWeaponDt* pData)
{
	if (!Node::init())
	{
		return false;
	}

	this->m_nID = pData->nID;
	this->m_nNextID = pData->nNextID;
	if (0 == m_nNextID) //��һ�ȼ�IDΪ0��ʾ��ǰ�ȼ�Ϊ����
	{
		m_bIsMax = true;
	}
	this->m_nUpGradeCoin = pData->nUpGradeCoin;
	this->m_vecUpGradePng = pData->vecUpGradePng;
	this->m_nSellCoin = pData->nSellCoin;
	this->m_strSellPng = pData->sellPng;
	if (0 != strcmp("",pData->bgPng.c_str())) //�����ͼƬ�Ŵ���
	{
		this->m_pBg = Sprite::createWithSpriteFrameName(pData->bgPng);
		this->addChild(m_pBg);
		m_pBg->setPosition(Vec2::ZERO);
	}
	this->m_strNormalPng = pData->normalPng;
	this->m_nBulletID = pData->nBulletID;
	this->m_fFireInterval = pData->fFireInterval;
	this->m_nRange = pData->nRange;
	this->m_strType = pData->type;
	this->m_strFireMusic = pData->fireMusic;
	this->m_pWeapon = Sprite::createWithSpriteFrameName(pData->normalPng);
	m_pWeapon->setPosition(Vec2::ZERO);
	this->addChild(m_pWeapon);


	//��Ӷ���
	Animation* pFireAnimation = Animation::create();
	for (int i = 0; i < pData->vecFirePng.size();++i)
	{
		pFireAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(pData->vecFirePng[i]));
	}
	pFireAnimation->setDelayPerUnit(0.1f);
	m_pFireAnimate = Animate::create(pFireAnimation);
	m_pFireAnimate->setTag(E_ANIMATE_FIRE);
	//�ֶ�����
	m_pFireAnimate->retain();

	m_pStopFunc = CallFunc::create([=](){
		//ֹͣǰ��Ķ���
		m_pWeapon->stopAllActionsByTag(E_ANIMATE_FIRE);
		//���ԭ��
		m_pWeapon->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(m_strNormalPng));
	});
	m_pStopFunc->retain();

	//����ͼƬ�ľ���
	m_pShowUpgradeSprite = Sprite::create();
	m_pShowUpgradeSprite->setPosition(0, 50);
	//��ʼ��Ϊ���ɼ�
	m_pShowUpgradeSprite->setVisible(false);
	this->addChild(m_pShowUpgradeSprite);

	//����������
	Animation* pShowUpgradeAni = Animation::create();
	pShowUpgradeAni->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("showupgrade01.png"));
	pShowUpgradeAni->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("showupgrade02.png"));
	pShowUpgradeAni->setDelayPerUnit(0.2f);
	//һֱѭ��
	pShowUpgradeAni->setLoops(-1);
	m_pShowUpgradeAnimate = Animate::create(pShowUpgradeAni);
	
	m_pShowUpgradeSprite->runAction(m_pShowUpgradeAnimate);
	//�ֶ�����
	m_pShowUpgradeAnimate->retain();

	this->scheduleUpdate();
	//��ʱ����
	this->schedule(CC_CALLBACK_1(CWeapon::fire, this), this->m_fFireInterval, "fire");	

	return true;
}

void CWeapon::fire(float fDelta)
{
	//�Ȳ�����û�б���ǵĽڵ�
	sSignedNodeDt pSignedNode = CGameScene::getInstance()->getSignedNodeDt();
	if (0 != pSignedNode.nUUID)
	{
		Node* pNode = nullptr;
		int nUUID = 0;
		if (0 == strcmp(pSignedNode.nodeName.c_str(), "object"))
		{
			pNode = CGameScene::getInstance()->getObjectMgr()->findObjectByUUID(pSignedNode.nUUID);
			CObject* pObject = (CObject*)pNode;
			if (nullptr != pObject)
			{
				nUUID = pObject->getUUID();
			}
		}
		else if (0 == strcmp(pSignedNode.nodeName.c_str(), "enemy"))
		{
			pNode = CGameScene::getInstance()->getEnemyMgr()->findEnemyByUUID(pSignedNode.nUUID);
			CEnemy* pEnemy = (CEnemy*)pNode;
			if (nullptr != pEnemy)
			{
				nUUID = pEnemy->getUUID();
			}
		}
		if (nullptr != pNode)
		{
			//����У��жϾ��빻����
			//�õ���Χ�е��ĸ������꣬�ֱ��жϺ������ľ���
			Rect boundBox = pNode->getBoundingBox();
			//���½�����
			Vec2 LBPos = boundBox.origin;
			//���Ͻǵ�����
			Vec2 LTPos = Vec2(boundBox.origin.x, boundBox.origin.y + boundBox.size.height);
			//���½ǵ�����
			Vec2 RBPos = Vec2(boundBox.origin.x + boundBox.size.width, boundBox.origin.y);
			//���Ͻǵ�����
			Vec2 RTPos = Vec2(boundBox.origin.x + boundBox.size.width,
				boundBox.origin.y + boundBox.size.height);
			//������������
			Vec2 MidPos = Vec2(boundBox.getMidX(), boundBox.getMidY());

			if (/*m_nRange >= this->getPosition().getDistance(LBPos) ||
				m_nRange >= this->getPosition().getDistance(LTPos) ||
				m_nRange >= this->getPosition().getDistance(RBPos) ||
				m_nRange >= this->getPosition().getDistance(RTPos)*/
				m_nRange>=this->getPosition().getDistance(MidPos)
				)
			{
				//���빻				
				//�������䶯��
				m_pWeapon->runAction(Sequence::create(m_pFireAnimate, m_pStopFunc, nullptr));
				//������������������ӵ�
				if (0 == strcmp(m_strType.c_str(), "ball")) //�ӵ���һ���ߵ�
				{
					//��ǰ��Ŀ��ڵ�Ϊ�ղŷ��䣬���򲻷���
					if (0 == m_sAimedNode.nUUID)
					{
						//����Ŀ��
						m_sAimedNode = pSignedNode;
						CGameScene::getInstance()->getBulletMgr()->
							addAimedBullet(this, pNode, nUUID);
					}
				}
				else
				{
					//����ӵ�
					CGameScene::getInstance()->getBulletMgr()->addBullet(this->m_nBulletID, this->getPosition(), MidPos, pNode->getName());
				}
				//������Ч
				SimpleAudioEngine::getInstance()->playEffect(m_strFireMusic.c_str());
			}
			else
			{
				//���벻�������ҵ��� �����˷���
				if (nullptr != this->findOneEnemy()) //�ҵ�����
				{
					//�������䶯��
					m_pWeapon->runAction(Sequence::create(m_pFireAnimate, m_pStopFunc, nullptr));
					//������������������ӵ�
					if (0 == strcmp(m_strType.c_str(), "ball")) //�ӵ���һ���ߵ�
					{
						//��ǰĿ��ڵ�Ϊ�ղŷ���
						if (0 == m_sAimedNode.nUUID)
						{
							//����Ŀ��
							m_sAimedNode.nodeName = this->findOneEnemy()->getName();
							m_sAimedNode.nUUID = this->findOneEnemy()->getUUID();
							CGameScene::getInstance()->getBulletMgr()->
								addAimedBullet(this, this->findOneEnemy(), this->findOneEnemy()->getUUID());
						}
					}
					else
					{
						//����ӵ�
						CGameScene::getInstance()->getBulletMgr()->
							addBullet(this->m_nBulletID, this->getPosition(), this->findOneEnemy()->getPosition(),"enemy");
					}
					//������Ч
					SimpleAudioEngine::getInstance()->playEffect(m_strFireMusic.c_str());
				}
			}
		}		
	}
	else
	{
		//�����ʱ���жϹ�����Χ����ĵ���
		if (nullptr != this->findOneEnemy()) //�ҵ�����
		{
			//�������䶯��
			m_pWeapon->runAction(Sequence::create(m_pFireAnimate, m_pStopFunc, nullptr));
			//������������������ӵ�
			if (0 == strcmp(m_strType.c_str(), "ball")) //�ӵ���һ���ߵ�
			{
				//��ǰĿ��ڵ�Ϊ�ղŷ���
				if (0 == m_sAimedNode.nUUID)
				{
					//����Ŀ��
					m_sAimedNode.nodeName = this->findOneEnemy()->getName();
					m_sAimedNode.nUUID = this->findOneEnemy()->getUUID();
					CGameScene::getInstance()->getBulletMgr()->
						addAimedBullet(this, this->findOneEnemy(), this->findOneEnemy()->getUUID());
				}			
			}
			else
			{
				//����ӵ�
				CGameScene::getInstance()->getBulletMgr()->
					addBullet(this->m_nBulletID, this->getPosition(),this->findOneEnemy()->getPosition(),"enemy");
			}
			//������Ч
			SimpleAudioEngine::getInstance()->playEffect(m_strFireMusic.c_str());
		}
	}
}


void CWeapon::update(float fDelta)
{
	//�ж��������� ������ת������
	if (0 == strcmp(m_strType.c_str(), "bottle") ||
		0 == strcmp(m_strType.c_str(), "fBottle"))
	{
		bool bCanShootSign = false; //�Ƿ��ܹ�����ǽڵ�

		Node* pNode = nullptr;
		//�Ȳ�����û�б���ǵĽڵ�
		sSignedNodeDt pSignedNode = CGameScene::getInstance()->getSignedNodeDt();
		if (0 != pSignedNode.nUUID)
		{
			if (0 == strcmp(pSignedNode.nodeName.c_str(), "object"))
			{
				pNode = CGameScene::getInstance()->getObjectMgr()->findObjectByUUID(pSignedNode.nUUID);
			}
			else if (0 == strcmp(pSignedNode.nodeName.c_str(), "enemy"))
			{
				pNode = CGameScene::getInstance()->getEnemyMgr()->findEnemyByUUID(pSignedNode.nUUID);
			}
			if (nullptr != pNode)
			{
				//����У��жϾ��빻����
				//�õ���Χ�е��ĸ������꣬�ֱ��жϺ������ľ���
				Rect boundBox = pNode->getBoundingBox();
				//���½�����
				Vec2 LBPos = boundBox.origin;
				//���Ͻǵ�����
				Vec2 LTPos = Vec2(boundBox.origin.x, boundBox.origin.y + boundBox.size.height);
				//���½ǵ�����
				Vec2 RBPos = Vec2(boundBox.origin.x + boundBox.size.width, boundBox.origin.y);
				//���Ͻǵ�����
				Vec2 RTPos = Vec2(boundBox.origin.x + boundBox.size.width,
					boundBox.origin.y + boundBox.size.height);
				//������������
				Vec2 MidPos = Vec2(boundBox.getMidX(), boundBox.getMidY());

				if (/*m_nRange >= this->getPosition().getDistance(LBPos) ||
					m_nRange >= this->getPosition().getDistance(LTPos) ||
					m_nRange >= this->getPosition().getDistance(RBPos) ||
					m_nRange >= this->getPosition().getDistance(RTPos)*/
					m_nRange >= this->getPosition().getDistance(MidPos)
					)
				{
					bCanShootSign = true;
				}
			}
		}

		if (bCanShootSign)
		{
			Vec2 startPos = this->getPosition();
			Vec2 endPos = pNode->getPosition();
			Vec2 deltaPos = endPos - startPos;
			//�õ���x�Ļ���
			float fRadians = deltaPos.getAngle();
			float fDegree = CC_RADIANS_TO_DEGREES(fRadians);
			m_pWeapon->setRotation(90 - fDegree);
		}
		else
		{
			if (nullptr == this->findOneEnemy())
			{
				return;
			}

			Vec2 startPos = this->getPosition();
			Vec2 endPos = this->findOneEnemy()->getPosition();
			Vec2 deltaPos = endPos - startPos;
			//�õ���x�Ļ���
			float fRadians = deltaPos.getAngle();
			float fDegree = CC_RADIANS_TO_DEGREES(fRadians);
			m_pWeapon->setRotation(90 - fDegree);
		}
	}
}


std::string CWeapon::getUpGradePng(bool& bCanUpGrade)
{
	//�жϵ�ǰ�ȼ��Ƿ������ȼ�
	if (m_bIsMax)
	{
		bCanUpGrade = false; //��������
		return m_vecUpGradePng[0];
	}
	//�������������жϵ�ǰ����Ƿ��ܹ�����
	int nCurCoin = CGameScene::getInstance()->getCurCoin();
    if (nCurCoin >= m_nUpGradeCoin)
	{
		bCanUpGrade = true; //������
		return  m_vecUpGradePng[1];
    }
	bCanUpGrade = false;
	return m_vecUpGradePng[0];
}

void CWeapon::checkUpgrade()
{
	bool bCanUpGrade = false;
	//�жϵ�ǰ�ȼ��Ƿ������ȼ�
	if (m_bIsMax)
	{
		bCanUpGrade = false; //��������
	}
	else
	{
		//�������������жϵ�ǰ����Ƿ��ܹ�����
		int nCurCoin = CGameScene::getInstance()->getCurCoin();
		if (nCurCoin >= m_nUpGradeCoin)
		{
			bCanUpGrade = true; //������
		}
		else
		{
			bCanUpGrade = false;
		}
	}

	if (bCanUpGrade)//���������
	{
		//��ʾ��������
		m_pShowUpgradeSprite->setVisible(true);
	}
	else
		m_pShowUpgradeSprite->setVisible(false);
}

CEnemy* CWeapon::findOneEnemy()
{
	Vector<Node*> Vec_Nodes = CGameScene::getInstance()->getEnemyMgr()->getChildren();
	for (int i = 0; i < Vec_Nodes.size();++i)
	{
		//ȡ�þ���
		float fDistance = Vec_Nodes.at(i)->getPosition().getDistance(this->getPosition());
		if (fDistance <= m_nRange)
		{
			CEnemy* pEnemy = (CEnemy*)Vec_Nodes.at(i);
			return pEnemy;
		}
	}
	return nullptr;
}

vector<CEnemy*>& CWeapon::findThreeEnemy()
{
	vector<CEnemy*> vecEnemys;
	int nCurNum = 0; //��ǰ�ҵ��ĵ��˸���
	Vector<Node*> Vec_Nodes = CGameScene::getInstance()->getEnemyMgr()->getChildren();
	for (int i = 0; i < Vec_Nodes.size(); ++i)
	{
		//ȡ�þ���
		float fDistance = Vec_Nodes.at(i)->getPosition().getDistance(this->getPosition());
		if (fDistance <= m_nRange)
		{
			CEnemy* pEnemy = (CEnemy*)Vec_Nodes.at(i);
			vecEnemys.push_back(pEnemy);
			nCurNum++;
			if (3 == nCurNum)
			{
				break;
			}
		}
	}
	return vecEnemys;
}
