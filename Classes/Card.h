#ifndef __CARD_H__
#define __CARD_H__
#include "cocos2d.h"
#include "StructData.h"
USING_NS_CC;
class CCard :public Sprite
{
public:
	CCard();
	virtual ~CCard();
	bool initWithData(sCardDt* pData);
	static CCard* createWithData(sCardDt* pData);

	/**
	 * @brief 根据当前金币来显示卡片的样式
	 * @param nCoin 当前金币
	 * @date 2018/10/07 23:07
	 * @return 
	 */
	void updateView(const int& nCoin);

	/**
	 * @brief 是否能购买
	 * @param nCoin 当前金币
	 * @date 2018/10/07 23:11
	 * @return 
	 */
	bool canBuy(const int& nCoin);
private:
	CC_SYNTHESIZE(int, m_nID, ID);
	CC_SYNTHESIZE(int, m_nNeedCoin,NeedCoin);  //所需金币
	CC_SYNTHESIZE(int, m_nWeaponID, WeaponID); //武器ID（塔ID）
	vector<string> m_vecPngs;
};

#endif