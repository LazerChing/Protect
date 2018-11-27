#include "BulletMgr.h"
#include "DataMgr.h"

CBulletMgr::CBulletMgr()
{
	m_nCurBulUUID = 0;
}

CBulletMgr::~CBulletMgr()
{
}

bool CBulletMgr::init()
{
	if (!Node::init())
	{
		return false;
	}

	return true;
}

void CBulletMgr::addBullet(const int& nBulID, Vec2 bulStartPos, Vec2 bulEndPos, string aim)
{
	Vec2 deltaPos = bulEndPos - bulStartPos;
	float fRadians = deltaPos.getAngle();
	float fDegree = CC_RADIANS_TO_DEGREES(fRadians);
	//旋转角度
	float fRotation = 90 - fDegree;

	sBulletDt* pData = CDataMgr::getInstance()->getBulletDtMgr()->getDataByID(nBulID);
	CBullet* pBullet = CBullet::createWithData(pData);
	pBullet->setUUID(++m_nCurBulUUID);
	pBullet->setTag(pBullet->getID());
	pBullet->setAim(aim);
	this->addChild(pBullet);
	pBullet->setPosition(bulStartPos);
	pBullet->setRotation(fRotation);
	pBullet->move(bulEndPos);
}


void CBulletMgr::addAimedBullet(CWeapon* pWeapon, Node* pAimNode, int nUUID)
{
	if (nullptr == pWeapon || nullptr == pAimNode)
	{
		return;
	}

	Vec2 deltaPos = pAimNode->getPosition() - pWeapon->getPosition();
	float fRadians = deltaPos.getAngle();
	float fDegree = CC_RADIANS_TO_DEGREES(fRadians);
	//旋转角度
	float fRotation = 90 - fDegree;

	sBulletDt* pData = CDataMgr::getInstance()->getBulletDtMgr()->getDataByID(pWeapon->getBulletID());
	CBullet* pBullet = CBullet::createWithData(pData);
	pBullet->setSpriteAnchorPoint(Vec2(0.5, 0));
	pBullet->setUUID(++m_nCurBulUUID);
	pBullet->setTag(pBullet->getID());
	pBullet->setAim(pAimNode->getName());
	this->addChild(pBullet);
	pBullet->setPosition(pWeapon->getPosition());
	pBullet->setRotation(fRotation);
	pBullet->startTimingDamage(pWeapon, pAimNode, nUUID);
}
