#include "WeaponMgr.h"
#include "DataMgr.h"

CWeaponMgr::CWeaponMgr()
{
	m_nCurWeaponUUID = 0;
}

CWeaponMgr::~CWeaponMgr()
{	
}

bool CWeaponMgr::init()
{
	if (!Layer::init())
	{
		return false;
	}

	return true;
}

void CWeaponMgr::addWeapon(int nWeaponID, Vec2 centerPixelPos)
{
	sWeaponDt* pWeaponData = CDataMgr::getInstance()->getWeaponDtMgr()->getDataByID(nWeaponID);
	CWeapon* pWeapon = CWeapon::createWithData(pWeaponData);
	this->addChild(pWeapon);
	pWeapon->setPosition(centerPixelPos);
	pWeapon->setUUID(++m_nCurWeaponUUID);
}

void CWeaponMgr::upgradeWeapon(CWeapon* pWeapon, Vec2 centerPixelPos)
{
	//记下下一等级武器ID
	int nNextID = pWeapon->getNextID();

	//判断当前炮塔是不是ball类型的
	if (713 == pWeapon->getTag() ||
		714 == pWeapon->getTag() ||
		715 == pWeapon->getTag()) //ball
	{
		//清除目标
		pWeapon->removeAim();
	}

	//先去除之前的		
	pWeapon->removeFromParent();
	//再新加一个
	addWeapon(nNextID, centerPixelPos);
}

void CWeaponMgr::sellWeapon(CWeapon* pWeapon)
{
	pWeapon->removeFromParent();
}

void CWeaponMgr::checkUpgrade()
{
	//遍历所有武器，判断有没有可以升级的
	Vector<Node*> VecWeaponNodes = this->getChildren();
	for (int i = 0; i < VecWeaponNodes.size(); i++)
	{
		CWeapon* pWeapon = (CWeapon*)VecWeaponNodes.at(i);
		pWeapon->checkUpgrade();
	}
}

CWeapon* CWeaponMgr::findWeaponByUUID(int nUUID)
{
	Vector<Node*> VecNodes = this->getChildren();
	for (Node* pNode : VecNodes)
	{
		CWeapon* pWeapon = (CWeapon*)pNode;
		if (pWeapon->getUUID() == nUUID)
		{
			return pWeapon;
		}
	}
	return nullptr;
}
