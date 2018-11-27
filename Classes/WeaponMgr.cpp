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
	//������һ�ȼ�����ID
	int nNextID = pWeapon->getNextID();

	//�жϵ�ǰ�����ǲ���ball���͵�
	if (713 == pWeapon->getTag() ||
		714 == pWeapon->getTag() ||
		715 == pWeapon->getTag()) //ball
	{
		//���Ŀ��
		pWeapon->removeAim();
	}

	//��ȥ��֮ǰ��		
	pWeapon->removeFromParent();
	//���¼�һ��
	addWeapon(nNextID, centerPixelPos);
}

void CWeaponMgr::sellWeapon(CWeapon* pWeapon)
{
	pWeapon->removeFromParent();
}

void CWeaponMgr::checkUpgrade()
{
	//���������������ж���û�п���������
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
