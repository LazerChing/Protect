#include "ObjectMgr.h"
#include "DataMgr.h"

CObjectMgr::CObjectMgr()
{
	m_nCurObjectUUID = 0;
}

CObjectMgr::~CObjectMgr()
{
}

bool CObjectMgr::init()
{
	if (!Layer::init())
	{
		return false;
	}


	return true;
}

void CObjectMgr::addObject(const string& strID, Vec2 pos)
{
	sObjectDt* pData = CDataMgr::getInstance()->getObjectDtMgr()->getDataByID(strID);
	CObject* pObject = CObject::createWithData(pData);
	m_nCurObjectUUID++;
	pObject->setUUID(m_nCurObjectUUID);
	pObject->setTag(m_nCurObjectUUID);
	pObject->setPosition(pos);
	this->addChild(pObject);
}

CObject* CObjectMgr::findObjectByPos(Vec2 pos)
{
	Vector<Node*> VecNodes = this->getChildren();
	for (Node* pNode : VecNodes)
	{
		if (pNode->getBoundingBox().containsPoint(pos))
		{
			CObject* pObject = (CObject*)pNode;
			return pObject;
		}
	}
	return nullptr;
}

CObject* CObjectMgr::findObjectByUUID(int nUUID)
{
	Vector<Node*> VecNodes = this->getChildren();
	for (Node* pNode : VecNodes)
	{
		CObject* pObject = (CObject*)pNode;		
		if (pObject->getUUID() == nUUID)
		{
			return pObject;
		}
	}
	return nullptr;
}