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
	if (0 == m_nNextID) //下一等级ID为0表示当前等级为满级
	{
		m_bIsMax = true;
	}
	this->m_nUpGradeCoin = pData->nUpGradeCoin;
	this->m_vecUpGradePng = pData->vecUpGradePng;
	this->m_nSellCoin = pData->nSellCoin;
	this->m_strSellPng = pData->sellPng;
	if (0 != strcmp("",pData->bgPng.c_str())) //如果有图片才创建
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


	//添加动画
	Animation* pFireAnimation = Animation::create();
	for (int i = 0; i < pData->vecFirePng.size();++i)
	{
		pFireAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(pData->vecFirePng[i]));
	}
	pFireAnimation->setDelayPerUnit(0.1f);
	m_pFireAnimate = Animate::create(pFireAnimation);
	m_pFireAnimate->setTag(E_ANIMATE_FIRE);
	//手动保留
	m_pFireAnimate->retain();

	m_pStopFunc = CallFunc::create([=](){
		//停止前面的动作
		m_pWeapon->stopAllActionsByTag(E_ANIMATE_FIRE);
		//设回原样
		m_pWeapon->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(m_strNormalPng));
	});
	m_pStopFunc->retain();

	//升级图片的精灵
	m_pShowUpgradeSprite = Sprite::create();
	m_pShowUpgradeSprite->setPosition(0, 50);
	//初始化为不可见
	m_pShowUpgradeSprite->setVisible(false);
	this->addChild(m_pShowUpgradeSprite);

	//可升级动画
	Animation* pShowUpgradeAni = Animation::create();
	pShowUpgradeAni->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("showupgrade01.png"));
	pShowUpgradeAni->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("showupgrade02.png"));
	pShowUpgradeAni->setDelayPerUnit(0.2f);
	//一直循环
	pShowUpgradeAni->setLoops(-1);
	m_pShowUpgradeAnimate = Animate::create(pShowUpgradeAni);
	
	m_pShowUpgradeSprite->runAction(m_pShowUpgradeAnimate);
	//手动保存
	m_pShowUpgradeAnimate->retain();

	this->scheduleUpdate();
	//定时发射
	this->schedule(CC_CALLBACK_1(CWeapon::fire, this), this->m_fFireInterval, "fire");	

	return true;
}

void CWeapon::fire(float fDelta)
{
	//先查找有没有被标记的节点
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
			//如果有，判断距离够不够
			//得到包围盒的四个点坐标，分别判断和炮塔的距离
			Rect boundBox = pNode->getBoundingBox();
			//左下角坐标
			Vec2 LBPos = boundBox.origin;
			//左上角的坐标
			Vec2 LTPos = Vec2(boundBox.origin.x, boundBox.origin.y + boundBox.size.height);
			//右下角的坐标
			Vec2 RBPos = Vec2(boundBox.origin.x + boundBox.size.width, boundBox.origin.y);
			//右上角的坐标
			Vec2 RTPos = Vec2(boundBox.origin.x + boundBox.size.width,
				boundBox.origin.y + boundBox.size.height);
			//矩形中心坐标
			Vec2 MidPos = Vec2(boundBox.getMidX(), boundBox.getMidY());

			if (/*m_nRange >= this->getPosition().getDistance(LBPos) ||
				m_nRange >= this->getPosition().getDistance(LTPos) ||
				m_nRange >= this->getPosition().getDistance(RBPos) ||
				m_nRange >= this->getPosition().getDistance(RTPos)*/
				m_nRange>=this->getPosition().getDistance(MidPos)
				)
			{
				//距离够				
				//炮塔发射动画
				m_pWeapon->runAction(Sequence::create(m_pFireAnimate, m_pStopFunc, nullptr));
				//根据炮塔类型来添加子弹
				if (0 == strcmp(m_strType.c_str(), "ball")) //子弹是一条线的
				{
					//当前的目标节点为空才发射，否则不发射
					if (0 == m_sAimedNode.nUUID)
					{
						//设置目标
						m_sAimedNode = pSignedNode;
						CGameScene::getInstance()->getBulletMgr()->
							addAimedBullet(this, pNode, nUUID);
					}
				}
				else
				{
					//添加子弹
					CGameScene::getInstance()->getBulletMgr()->addBullet(this->m_nBulletID, this->getPosition(), MidPos, pNode->getName());
				}
				//播放音效
				SimpleAudioEngine::getInstance()->playEffect(m_strFireMusic.c_str());
			}
			else
			{
				//距离不够，查找敌人 朝敌人发射
				if (nullptr != this->findOneEnemy()) //找到敌人
				{
					//炮塔发射动画
					m_pWeapon->runAction(Sequence::create(m_pFireAnimate, m_pStopFunc, nullptr));
					//根据炮塔类型来添加子弹
					if (0 == strcmp(m_strType.c_str(), "ball")) //子弹是一条线的
					{
						//当前目标节点为空才发射
						if (0 == m_sAimedNode.nUUID)
						{
							//设置目标
							m_sAimedNode.nodeName = this->findOneEnemy()->getName();
							m_sAimedNode.nUUID = this->findOneEnemy()->getUUID();
							CGameScene::getInstance()->getBulletMgr()->
								addAimedBullet(this, this->findOneEnemy(), this->findOneEnemy()->getUUID());
						}
					}
					else
					{
						//添加子弹
						CGameScene::getInstance()->getBulletMgr()->
							addBullet(this->m_nBulletID, this->getPosition(), this->findOneEnemy()->getPosition(),"enemy");
					}
					//播放音效
					SimpleAudioEngine::getInstance()->playEffect(m_strFireMusic.c_str());
				}
			}
		}		
	}
	else
	{
		//发射的时候判断攻击范围里面的敌人
		if (nullptr != this->findOneEnemy()) //找到敌人
		{
			//炮塔发射动画
			m_pWeapon->runAction(Sequence::create(m_pFireAnimate, m_pStopFunc, nullptr));
			//根据炮塔类型来添加子弹
			if (0 == strcmp(m_strType.c_str(), "ball")) //子弹是一条线的
			{
				//当前目标节点为空才发射
				if (0 == m_sAimedNode.nUUID)
				{
					//设置目标
					m_sAimedNode.nodeName = this->findOneEnemy()->getName();
					m_sAimedNode.nUUID = this->findOneEnemy()->getUUID();
					CGameScene::getInstance()->getBulletMgr()->
						addAimedBullet(this, this->findOneEnemy(), this->findOneEnemy()->getUUID());
				}			
			}
			else
			{
				//添加子弹
				CGameScene::getInstance()->getBulletMgr()->
					addBullet(this->m_nBulletID, this->getPosition(),this->findOneEnemy()->getPosition(),"enemy");
			}
			//播放音效
			SimpleAudioEngine::getInstance()->playEffect(m_strFireMusic.c_str());
		}
	}
}


void CWeapon::update(float fDelta)
{
	//判断武器类型 可以旋转的武器
	if (0 == strcmp(m_strType.c_str(), "bottle") ||
		0 == strcmp(m_strType.c_str(), "fBottle"))
	{
		bool bCanShootSign = false; //是否能攻击标记节点

		Node* pNode = nullptr;
		//先查找有没有被标记的节点
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
				//如果有，判断距离够不够
				//得到包围盒的四个点坐标，分别判断和炮塔的距离
				Rect boundBox = pNode->getBoundingBox();
				//左下角坐标
				Vec2 LBPos = boundBox.origin;
				//左上角的坐标
				Vec2 LTPos = Vec2(boundBox.origin.x, boundBox.origin.y + boundBox.size.height);
				//右下角的坐标
				Vec2 RBPos = Vec2(boundBox.origin.x + boundBox.size.width, boundBox.origin.y);
				//右上角的坐标
				Vec2 RTPos = Vec2(boundBox.origin.x + boundBox.size.width,
					boundBox.origin.y + boundBox.size.height);
				//矩形中心坐标
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
			//得到与x的弧度
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
			//得到与x的弧度
			float fRadians = deltaPos.getAngle();
			float fDegree = CC_RADIANS_TO_DEGREES(fRadians);
			m_pWeapon->setRotation(90 - fDegree);
		}
	}
}


std::string CWeapon::getUpGradePng(bool& bCanUpGrade)
{
	//判断当前等级是否是最大等级
	if (m_bIsMax)
	{
		bCanUpGrade = false; //不能升级
		return m_vecUpGradePng[0];
	}
	//否则不是满级，判断当前金币是否能够升级
	int nCurCoin = CGameScene::getInstance()->getCurCoin();
    if (nCurCoin >= m_nUpGradeCoin)
	{
		bCanUpGrade = true; //能升级
		return  m_vecUpGradePng[1];
    }
	bCanUpGrade = false;
	return m_vecUpGradePng[0];
}

void CWeapon::checkUpgrade()
{
	bool bCanUpGrade = false;
	//判断当前等级是否是最大等级
	if (m_bIsMax)
	{
		bCanUpGrade = false; //不能升级
	}
	else
	{
		//否则不是满级，判断当前金币是否能够升级
		int nCurCoin = CGameScene::getInstance()->getCurCoin();
		if (nCurCoin >= m_nUpGradeCoin)
		{
			bCanUpGrade = true; //能升级
		}
		else
		{
			bCanUpGrade = false;
		}
	}

	if (bCanUpGrade)//如果能升级
	{
		//显示升级动画
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
		//取得距离
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
	int nCurNum = 0; //当前找到的敌人个数
	Vector<Node*> Vec_Nodes = CGameScene::getInstance()->getEnemyMgr()->getChildren();
	for (int i = 0; i < Vec_Nodes.size(); ++i)
	{
		//取得距离
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
