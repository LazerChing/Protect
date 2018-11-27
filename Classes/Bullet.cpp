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

	//子弹运动动画
	Animation* pAnimation = Animation::create();
	for (int i = 0; i < m_vecAnimatePng.size(); i++)
	{
		pAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(m_vecAnimatePng[i]));
	}
	pAnimation->setLoops(-1);
	pAnimation->setDelayPerUnit(0.05f);
	m_pBulAnimate = Animate::create(pAnimation);
	m_pBulAnimate->retain();

	//子弹爆炸动画
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
	//判断子弹类型
	if (0 == strcmp(m_strType.c_str(), "fan"))
	{
		m_fRotation += 10; //旋转子弹
		if (360 <= m_fRotation)
		{
			m_fRotation = 0.0f;
		}
		this->setRotation(m_fRotation);
	}
	else if (0 == strcmp(m_strType.c_str(), "ball")) //子弹是一条线的
	{
		
		//先判断当前有没有目标
		if (0 != m_sAimedNode.nUUID)
		{
			//判断当前子弹所属炮塔还在不在
			CWeapon* pWeapon = (CWeapon*)CGameScene::getInstance()->getWeaponMgr()->findWeaponByUUID(m_nWeaponUUID);
			if (nullptr != pWeapon)
			{
				//再判断目标还存不存在
				//判断目标类型
				if (m_sAimedNode.nodeName == "enemy")
				{
					CEnemy* pEnemy = (CEnemy*)CGameScene::getInstance()->getEnemyMgr()->findEnemyByUUID(m_sAimedNode.nUUID);
					//如果目标还存在
					if (nullptr != pEnemy)
					{
						//再判断当前的目标是否还在自己的攻击范围之内
						Vec2 startPos = this->getPosition();
						Vec2 endPos = pEnemy->getPosition();
						float fDistance = startPos.getDistance(endPos);
						if (pWeapon->getRange() >= fDistance)
						{
							Vec2 deltaPos = endPos - startPos;
							//得到与x的弧度
							float fRadians = deltaPos.getAngle();
							float fDegree = CC_RADIANS_TO_DEGREES(fRadians);
							this->setRotation(90 - fDegree);
							//设置自己的长度为距离长度
							//计算放大或缩小的倍数
							float fRate = fDistance / this->m_pBullet->getContentSize().height;
							this->setScaleY(fRate);
						}
						else  //否则不在攻击范围之内
						{
							//自己要消失，同时告诉所属武器，可以选择新目标
							clearAim();
							unscheduleUpdate();
							pWeapon->removeAim();
							this->removeFromParent();
						}
					}
					else  //目标不存在了，告诉所有正在攻击此目标的这种类型炮塔
					{
						//通知所有这个类型的子弹
						Vector<Node*> Vec_Nodes = CGameScene::getInstance()->getBulletMgr()->getChildren();
						for (Node* pNode : Vec_Nodes)
						{
							if (713 == pNode->getTag() ||
								714 == pNode->getTag() ||
								715 == pNode->getTag()) //ball
							{
								CBullet* pBullet = (CBullet*)pNode;
								//判断目标是否一样
								if (pBullet->getAimedNode()->nodeName == m_sAimedNode.nodeName &&
									pBullet->getAimedNode()->nUUID == m_sAimedNode.nUUID)
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
				}
				else if (m_sAimedNode.nodeName == "object")
				{
					CObject* pObject = (CObject*)CGameScene::getInstance()->getObjectMgr()->findObjectByUUID(m_sAimedNode.nUUID);
					//如果目标还存在
					if (nullptr != pObject)
					{
						//再判断当前的目标是否还在自己的攻击范围之内
						Vec2 startPos = this->getPosition();
						Vec2 endPos = pObject->getPosition();
						float fDistance = startPos.getDistance(endPos);
						if (pWeapon->getRange() >= fDistance)
						{
							Vec2 deltaPos = endPos - startPos;
							//得到与x的弧度
							float fRadians = deltaPos.getAngle();
							float fDegree = CC_RADIANS_TO_DEGREES(fRadians);
							this->setRotation(90 - fDegree);
							//设置自己的长度为距离长度
							//计算放大或缩小的倍数
							float fRate = fDistance / this->m_pBullet->getContentSize().height;
							this->setScaleY(fRate);
						}
						else  //否则不在攻击范围之内
						{
							//自己要消失，同时告诉所属武器，可以选择新目标
							unscheduleUpdate();
							pWeapon->removeAim();
							this->removeFromParent();
						}
					}
					else  //目标不存在了
					{
						//自己要消失，同事告诉所属武器，可以选择新目标
						pWeapon->removeAim();
						clearAim();
						unscheduleUpdate();
						this->removeFromParent();
					}
				}
			}
			else  //所属炮塔不在了
			{
				//自己要消失
				m_sAimedNode = sSignedNodeDt();
				m_nWeaponUUID = 0;
				unscheduleUpdate();
				this->removeFromParent();
			}
		}


		//先判断 目标\所属炮塔 还在不在 
		//if (nullptr != m_pAimedNode && m_pAimedNode->isRunning() && 
		//	nullptr != m_pWeapon && m_pWeapon->isRunning())
		//{
		//	//再判断当前的目标是否还在自己的攻击范围之内
		//	Vec2 startPos = this->getPosition();
		//	Vec2 endPos = m_pAimedNode->getPosition();
		//	float fDistance = startPos.getDistance(endPos);		
		//	if (m_pWeapon->getRange() >= fDistance)
		//	{
		//		Vec2 deltaPos = endPos - startPos;
		//		//得到与x的弧度
		//		float fRadians = deltaPos.getAngle();
		//		float fDegree = CC_RADIANS_TO_DEGREES(fRadians);
		//		this->setRotation(90 - fDegree);
		//		//设置自己的长度为距离长度
		//		//计算放大或缩小的倍数
		//		float fRate = fDistance / this->m_pBullet->getContentSize().height;
		//		this->setScaleY(fRate);
		//	}
		//	else
		//	{
		//		unscheduleUpdate();
		//		m_pAimedNode = nullptr;
		//		//通知炮塔，当前没有目标
		//		m_pWeapon->removeAim();
		//		//超过目标范围，把自己清除掉
		//		this->removeFromParent();
		//	}
		//}
		//else
		//{
		//	if (!m_pWeapon->isRunning()) //所属炮塔不在了
		//	{
		//		//把自己清除掉
		//		this->removeFromParent();
		//	}
		//	else //目标不在了
		//	{
		//		unscheduleUpdate();
		//		//通知炮塔，当前没有目标
		//		m_pWeapon->removeAim();
		//		//把自己清除掉
		//		this->removeFromParent();
		//	}
		//}
	}
}

void CBullet::move(Vec2 endPos)
{
	//判断子弹类型
	if (0 == strcmp(m_strType.c_str(), "fan")) //子弹移出屏幕才消失
	{
		Size visibleSize = Director::getInstance()->getVisibleSize();
		//取得两点间的向量差
		Vec2 deltaPos = endPos - this->getPosition();
		//比例
		float  fRatio = 0.f;
		//通过两点间的向量差来确定子弹起点跟当前怪物点的连线与x轴的夹角是大于45度还是小于45
		if (abs(deltaPos.x) > abs(deltaPos.y))//小于45度
		{
			//比例用宽度来计算
			fRatio = abs(deltaPos.x) / visibleSize.width;
		}
		else
		{
			//比例用高度来计算
			fRatio = abs(deltaPos.y) / visibleSize.height;
		}

		//偏移
		Vec2 vPos = deltaPos / fRatio;
		//匀速运动
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
		this->removeFromParent();//移除子弹
	});
	m_pBullet->runAction(Sequence::create(m_pBombAnimate, pAnimateOver, nullptr));
}

void CBullet::damageAroundEnemy(float fRange)
{
	int nCurNum = 0; //当前找到的敌人个数
	//遍历所有敌人
	Vector<Node*> Vec_Nodes = CGameScene::getInstance()->getEnemyMgr()->getChildren();
	for (int i = 0; i < Vec_Nodes.size(); ++i)
	{
		//取得距离
		float fDistance = Vec_Nodes.at(i)->getPosition().getDistance(this->getPosition());
		if (fDistance <= fRange)
		{
			CEnemy* pEnemy = (CEnemy*)Vec_Nodes.at(i);
			//对敌人造成伤害
			if (pEnemy->damage(this))
			{
				//播放敌人爆炸动画
				CGameScene::getInstance()->getAnimateMgr()->playEnemyBombAnimate(pEnemy->getPosition(), pEnemy->getCoinPng());
				//加钱
				CGameScene::getInstance()->addCoin(pEnemy->getCoin());
				//敌人死亡
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
	//记下炮塔UUID
	m_nWeaponUUID = pWeapon->getUUID();
	//记下目标
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
	//先受一次攻击
	timingDamageCallback(0);
	//开启定时器，1s造成一次伤害
	this->schedule(CC_CALLBACK_1(CBullet::timingDamageCallback, this), 0.6f, "timingDamage");
}



void CBullet::timingDamageCallback(float fDelta)
{
	//先判断当前目标有没有
	if (0 != m_sAimedNode.nUUID)
	{
		//判断目标类型
		if (m_sAimedNode.nodeName == "enemy")
		{
			CEnemy* pEnemy = (CEnemy*)CGameScene::getInstance()->getEnemyMgr()->findEnemyByUUID(m_sAimedNode.nUUID);
			if (nullptr == pEnemy)
			{
				return;
			}
			//播放击中动画
			CGameScene::getInstance()->getAnimateMgr()->playAnimateByVecPng(pEnemy->getPosition(), m_vecBombPng);
			//敌人扣血
			if (pEnemy->damage(this))
			{
				//播放敌人爆炸动画
				CGameScene::getInstance()->getAnimateMgr()->playEnemyBombAnimate(pEnemy->getPosition(), pEnemy->getCoinPng());
				//加钱
				CGameScene::getInstance()->addCoin(pEnemy->getCoin());
				//敌人死亡
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
			//播放击中动画
			CGameScene::getInstance()->getAnimateMgr()->playAnimateByVecPng(pObject->getPosition(), m_vecBombPng);
			//敌人扣血
			if (pObject->damage(this))
			{
				//播放敌人爆炸动画
				CGameScene::getInstance()->getAnimateMgr()->playEnemyBombAnimate(pObject->getPosition(), pObject->getCoinPng());
				//加钱
				CGameScene::getInstance()->addCoin(pObject->getCoin());
				//除去容器中物体的矩形盒
				vector<Rect>& vecObjectRects = CGameScene::getInstance()->getVecObjectRect();
				for (int i = 0; i < vecObjectRects.size(); ++i)
				{

					if (vecObjectRects[i].containsPoint(pObject->getPosition()))
					{
						vecObjectRects.erase(vecObjectRects.begin() + i);
						break;
					}
				}
				//物体消失
				pObject->removeSelf();
			}
		}
	}
}

CWeapon* CBullet::getOwner()
{
	return CGameScene::getInstance()->getWeaponMgr()->findWeaponByUUID(m_nWeaponUUID);
}
