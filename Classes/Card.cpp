#include "Card.h"

CCard::CCard()
: m_nID(0), m_nNeedCoin(0), m_nWeaponID(0)
{
}

CCard::~CCard()
{
}

CCard* CCard::createWithData(sCardDt* pData)
{
	CCard* pCard = new CCard();
	if (nullptr != pCard && pCard->initWithData(pData))
	{
		pCard->autorelease();
		return pCard;
	}
	else
	{
		CC_SAFE_DELETE(pCard);
		return nullptr;
	}
}

bool CCard::initWithData(sCardDt* pData)
{
	if (!Sprite::initWithSpriteFrameName(pData->vecPngs[1]))
	{
		return false;
	}
	this->m_nID = pData->nID;
	this->m_nNeedCoin = pData->nNeedCoin;
	this->m_vecPngs = pData->vecPngs;
	this->m_nWeaponID = pData->nWeaponID;
	return true;
}

void CCard::updateView(const int& nCoin)
{
	if (m_nNeedCoin <= nCoin) //�ɹ���
	{
		this->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(m_vecPngs[1]));
	}
	else //��Ҳ��� ���ɹ���
	{
		this->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(m_vecPngs[0]));
	}
}

bool CCard::canBuy(const int& nCoin)
{
	if (m_nNeedCoin <= nCoin) //�ɹ���
	{
		return true;
	}
	return false;
}

