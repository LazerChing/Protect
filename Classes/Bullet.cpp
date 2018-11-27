#include "Bullet.h"
#include "GameScene.h"

CBullet::CBullet()
{
	m_nID = 0;
	m_nAck = 0;
	m_fSpeed = 0;
	m_fReduceSpeedRate = 0;
	m_nUUID = 0;
	m_fContinueTime = 0.0f;
	m_fRotation = 0.0f;
	m_nWeaponUUID = 0;
}

CBullet::~CBullet()
{
	m_pBulAnimate->release();
	m_pBombAnimate->release();
}


CBullet* CBullet::createWithData(sBulletDt* pData)
{
	CBullet* pBullet = new CBullet();
	if (nullptr!=pBullet && pBullet->initWithData(pData))
	{
		pBullet->autorelease();
		return pBullet;
	}
	CC_SAFE_DELETE(pBullet);
	return nullptr;
}

bool CBullet::initWithData(sBulletDt* pData)
{
	if (!Node::init())
	{
		return false;
	}
	this->m_nID = pData->nID;
	this->m_nAck = pData->nAck;
	this->m_fSpeed = pData->fSpeed;
	this->m_fReduceSpeedRate = pData->fReduceSpeedRate;
	this->m_strType = pData->strType;
	this->m_vecAnimatePng = pData->vec_animatePng;
	this->m_vecBombPng = pData->vec_bombPng;
	this->m_vecHitPng = pData->vec_hitPng;
	this->m_fContinueTime = pData->fContinueTime;
	this->setName("bullet");

	//�ӵ��˶�����
	Animation* pAnimation = Animation::create();
	for (int i = 0; i < m_vecAnimatePng.size(); i++)
	{
		pAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(m_vecAnimatePng[i]));
	}
	pAnimation->setLoops(-1);
	pAnimation->setDelayPerUnit(0.05f);
	m_pBulAnimate = Animate::create(pAnimation);
	m_pBulAnimate->retain();

	//�ӵ���ը����
	Animation* pAnimationBomb = Animation::create();
	for (int i = 0; i < m_vecBombPng.size(); i++)
	{
		pAnimationBomb->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(m_vecBombPng[i]));
	}
	pAnimationBomb->setDelayPerUnit(0.05f);
	m_pBombAnimate = Animate::create(pAnimationBomb);
	m_pBombAnimate->retain();

	m_pBullet = Sprite::create();
	m_pBullet->runAction(m_pBulAnimate);
	this->addChild(m_pBullet);

	this->scheduleUpdate();
	return true;
}

void CBullet::update(float fDelta)
{
	//�ж��ӵ�����
	if (0 == strcmp(m_strType.c_str(), "fan"))
	{
		m_fRotation += 10; //��ת�ӵ�
		if (360 <= m_fRotation)
		{
			m_fRotation = 0.0f;
		}
		this->setRotation(m_fRotation);
	}
	else if (0 == strcmp(m_strType.c_str(), "ball")) //�ӵ���һ���ߵ�
	{
		
		//���жϵ�ǰ��û��Ŀ��
		if (0 != m_sAimedNode.nUUID)
		{
			//�жϵ�ǰ�ӵ������������ڲ���
			CWeapon* pWeapon = (CWeapon*)CGameScene::getInstance()->getWeaponMgr()->findWeaponByUUID(m_nWeaponUUID);
			if (nullptr != pWeapon)
			{
				//���ж�Ŀ�껹�治����
				//�ж�Ŀ������
				if (m_sAimedNode.nodeName == "enemy")
				{
					CEnemy* pEnemy = (CEnemy*)CGameScene::getInstance()->getEnemyMgr()->findEnemyByUUID(m_sAimedNode.nUUID);
					//���Ŀ�껹����
					if (nullptr != pEnemy)
					{
						//���жϵ�ǰ��Ŀ���Ƿ����Լ��Ĺ�����Χ֮��
						Vec2 startPos = this->getPosition();
						Vec2 endPos = pEnemy->getPosition();
						float fDistance = startPos.getDistance(endPos);
						if (pWeapon->getRange() >= fDistance)
						{
							Vec2 deltaPos = endPos - startPos;
							//�õ���x�Ļ���
							float fRadians = deltaPos.getAngle();
							float fDegree = CC_RADIANS_TO_DEGREES(fRadians);
							this->setRotation(90 - fDegree);
							//�����Լ��ĳ���Ϊ���볤��
							//����Ŵ����С�ı���
							float fRate = fDistance / this->m_pBullet->getContentSize().height;
							this->setScaleY(fRate);
						}
						else  //�����ڹ�����Χ֮��
						{
							//�Լ�Ҫ��ʧ��ͬʱ������������������ѡ����Ŀ��
							clearAim();
							unscheduleUpdate();
							pWeapon->removeAim();
							this->removeFromParent();
						}
					}
					else  //Ŀ�겻�����ˣ������������ڹ�����Ŀ���������������
					{
						//֪ͨ����������͵��ӵ�
						Vector<Node*> Vec_Nodes = CGameScene::getInstance()->getBulletMgr()->getChildren();
						for (Node* pNode : Vec_Nodes)
						{
							if (713 == pNode->getTag() ||
								714 == pNode->getTag() ||
								715 == pNode->getTag()) //ball
							{
								CBullet* pBullet = (CBullet*)pNode;
								//�ж�Ŀ���Ƿ�һ��
								if (pBullet->getAimedNode()->nodeName == m_sAimedNode.nodeName &&
									pBullet->getAimedNode()->nUUID == m_sAimedNode.nUUID)
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
				}
				else if (m_sAimedNode.nodeName == "object")
				{
					CObject* pObject = (CObject*)CGameScene::getInstance()->getObjectMgr()->findObjectByUUID(m_sAimedNode.nUUID);
					//���Ŀ�껹����
					if (nullptr != pObject)
					{
						//���жϵ�ǰ��Ŀ���Ƿ����Լ��Ĺ�����Χ֮��
						Vec2 startPos = this->getPosition();
						Vec2 endPos = pObject->getPosition();
						float fDistance = startPos.getDistance(endPos);
						if (pWeapon->getRange() >= fDistance)
						{
							Vec2 deltaPos = endPos - startPos;
							//�õ���x�Ļ���
							float fRadians = deltaPos.getAngle();
							float fDegree = CC_RADIANS_TO_DEGREES(fRadians);
							this->setRotation(90 - fDegree);
							//�����Լ��ĳ���Ϊ���볤��
							//����Ŵ����С�ı���
							float fRate = fDistance / this->m_pBullet->getContentSize().height;
							this->setScaleY(fRate);
						}
						else  //�����ڹ�����Χ֮��
						{
							//�Լ�Ҫ��ʧ��ͬʱ������������������ѡ����Ŀ��
							unscheduleUpdate();
							pWeapon->removeAim();
							this->removeFromParent();
						}
					}
					else  //Ŀ�겻������
					{
						//�Լ�Ҫ��ʧ��ͬ�¸�����������������ѡ����Ŀ��
						pWeapon->removeAim();
						clearAim();
						unscheduleUpdate();
						this->removeFromParent();
					}
				}
			}
			else  //��������������
			{
				//�Լ�Ҫ��ʧ
				m_sAimedNode = sSignedNodeDt();
				m_nWeaponUUID = 0;
				unscheduleUpdate();
				this->removeFromParent();
			}
		}


		//���ж� Ŀ��\�������� ���ڲ��� 
		//if (nullptr != m_pAimedNode && m_pAimedNode->isRunning() && 
		//	nullptr != m_pWeapon && m_pWeapon->isRunning())
		//{
		//	//���жϵ�ǰ��Ŀ���Ƿ����Լ��Ĺ�����Χ֮��
		//	Vec2 startPos = this->getPosition();
		//	Vec2 endPos = m_pAimedNode->getPosition();
		//	float fDistance = startPos.getDistance(endPos);		
		//	if (m_pWeapon->getRange() >= fDistance)
		//	{
		//		Vec2 deltaPos = endPos - startPos;
		//		//�õ���x�Ļ���
		//		float fRadians = deltaPos.getAngle();
		//		float fDegree = CC_RADIANS_TO_DEGREES(fRadians);
		//		this->setRotation(90 - fDegree);
		//		//�����Լ��ĳ���Ϊ���볤��
		//		//����Ŵ����С�ı���
		//		float fRate = fDistance / this->m_pBullet->getContentSize().height;
		//		this->setScaleY(fRate);
		//	}
		//	else
		//	{
		//		unscheduleUpdate();
		//		m_pAimedNode = nullptr;
		//		//֪ͨ��������ǰû��Ŀ��
		//		m_pWeapon->removeAim();
		//		//����Ŀ�귶Χ�����Լ������
		//		this->removeFromParent();
		//	}
		//}
		//else
		//{
		//	if (!m_pWeapon->isRunning()) //��������������
		//	{
		//		//���Լ������
		//		this->removeFromParent();
		//	}
		//	else //Ŀ�겻����
		//	{
		//		unscheduleUpdate();
		//		//֪ͨ��������ǰû��Ŀ��
		//		m_pWeapon->removeAim();
		//		//���Լ������
		//		this->removeFromParent();
		//	}
		//}
	}
}

void CBullet::move(Vec2 endPos)
{
	//�ж��ӵ�����
	if (0 == strcmp(m_strType.c_str(), "fan")) //�ӵ��Ƴ���Ļ����ʧ
	{
		Size visibleSize = Director::getInstance()->getVisibleSize();
		//ȡ��������������
		Vec2 deltaPos = endPos - this->getPosition();
		//����
		float  fRatio = 0.f;
		//ͨ����������������ȷ���ӵ�������ǰ������������x��ļн��Ǵ���45�Ȼ���С��45
		if (abs(deltaPos.x) > abs(deltaPos.y))//С��45��
		{
			//�����ÿ��������
			fRatio = abs(deltaPos.x) / visibleSize.width;
		}
		else
		{
			//�����ø߶�������
			fRatio = abs(deltaPos.y) / visibleSize.height;
		}

		//ƫ��
		Vec2 vPos = deltaPos / fRatio;
		//�����˶�
		float fDistance = vPos.length();
		float fTime = fDistance / m_fSpeed;
		MoveBy* pMoveBy = MoveBy::create(fTime, vPos);
		Sequence* pSeq = Sequence::createWithTwoActions(pMoveBy, RemoveSelf::create());
		this->runAction(pSeq);
	}
	else
	{
		float fDistance = endPos.getDistance(this->getPosition());
		float fTime = fDistance / m_fSpeed;
		MoveTo* pMoveTo = MoveTo::create(fTime, endPos);
		/*CallFunc* pMoveOver = CallFunc::create([=](){
			playBombAnimate();
			});*/
		this->runAction(Sequence::createWithTwoActions(pMoveTo,RemoveSelf::create()));
	}
}

void CBullet::playBombAnimate()
{
	m_pBullet->stopAllActions();
	CallFunc* pAnimateOver = CallFunc::create([=](){
		this->removeFromParent();//�Ƴ��ӵ�
	});
	m_pBullet->runAction(Sequence::create(m_pBombAnimate, pAnimateOver, nullptr));
}

void CBullet::damageAroundEnemy(float fRange)
{
	int nCurNum = 0; //��ǰ�ҵ��ĵ��˸���
	//�������е���
	Vector<Node*> Vec_Nodes = CGameScene::getInstance()->getEnemyMgr()->getChildren();
	for (int i = 0; i < Vec_Nodes.size(); ++i)
	{
		//ȡ�þ���
		float fDistance = Vec_Nodes.at(i)->getPosition().getDistance(this->getPosition());
		if (fDistance <= fRange)
		{
			CEnemy* pEnemy = (CEnemy*)Vec_Nodes.at(i);
			//�Ե�������˺�
			if (pEnemy->damage(this))
			{
				//���ŵ��˱�ը����
				CGameScene::getInstance()->getAnimateMgr()->playEnemyBombAnimate(pEnemy->getPosition(), pEnemy->getCoinPng());
				//��Ǯ
				CGameScene::getInstance()->addCoin(pEnemy->getCoin());
				//��������
				pEnemy->removeSelf();
			}
			nCurNum++;
			if (3 == nCurNum)
			{
				break;
			}
		}
	}
}


void CBullet::startTimingDamage(CWeapon* pWeapon, Node* pAimedNode,int nUUID)
{
	//��������UUID
	m_nWeaponUUID = pWeapon->getUUID();
	//����Ŀ��
	m_sAimedNode.nodeName = pAimedNode->getName();
	if (m_sAimedNode.nodeName == "enemy")
	{
		CEnemy* pEnemy = (CEnemy*)pAimedNode;
		m_sAimedNode.nUUID = pEnemy->getUUID();
	}
	else if (m_sAimedNode.nodeName == "object")
	{
		CObject* pObject = (CObject*)pAimedNode;
		m_sAimedNode.nUUID = pObject->getUUID();
	}
	//����һ�ι���
	timingDamageCallback(0);
	//������ʱ����1s���һ���˺�
	this->schedule(CC_CALLBACK_1(CBullet::timingDamageCallback, this), 0.6f, "timingDamage");
}



void CBullet::timingDamageCallback(float fDelta)
{
	//���жϵ�ǰĿ����û��
	if (0 != m_sAimedNode.nUUID)
	{
		//�ж�Ŀ������
		if (m_sAimedNode.nodeName == "enemy")
		{
			CEnemy* pEnemy = (CEnemy*)CGameScene::getInstance()->getEnemyMgr()->findEnemyByUUID(m_sAimedNode.nUUID);
			if (nullptr == pEnemy)
			{
				return;
			}
			//���Ż��ж���
			CGameScene::getInstance()->getAnimateMgr()->playAnimateByVecPng(pEnemy->getPosition(), m_vecBombPng);
			//���˿�Ѫ
			if (pEnemy->damage(this))
			{
				//���ŵ��˱�ը����
				CGameScene::getInstance()->getAnimateMgr()->playEnemyBombAnimate(pEnemy->getPosition(), pEnemy->getCoinPng());
				//��Ǯ
				CGameScene::getInstance()->addCoin(pEnemy->getCoin());
				//��������
				pEnemy->removeSelf();
			}
		}
		else if (m_sAimedNode.nodeName == "object")
		{
			CObject* pObject = (CObject*)CGameScene::getInstance()->getObjectMgr()->findObjectByUUID(m_sAimedNode.nUUID);
			if (nullptr == pObject)
			{
				return;
			}
			//���Ż��ж���
			CGameScene::getInstance()->getAnimateMgr()->playAnimateByVecPng(pObject->getPosition(), m_vecBombPng);
			//���˿�Ѫ
			if (pObject->damage(this))
			{
				//���ŵ��˱�ը����
				CGameScene::getInstance()->getAnimateMgr()->playEnemyBombAnimate(pObject->getPosition(), pObject->getCoinPng());
				//��Ǯ
				CGameScene::getInstance()->addCoin(pObject->getCoin());
				//��ȥ����������ľ��κ�
				vector<Rect>& vecObjectRects = CGameScene::getInstance()->getVecObjectRect();
				for (int i = 0; i < vecObjectRects.size(); ++i)
				{

					if (vecObjectRects[i].containsPoint(pObject->getPosition()))
					{
						vecObjectRects.erase(vecObjectRects.begin() + i);
						break;
					}
				}
				//������ʧ
				pObject->removeSelf();
			}
		}
	}
}

CWeapon* CBullet::getOwner()
{
	return CGameScene::getInstance()->getWeaponMgr()->findWeaponByUUID(m_nWeaponUUID);
}
