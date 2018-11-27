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
	 * @brief ���ݵ�ǰ�������ʾ��Ƭ����ʽ
	 * @param nCoin ��ǰ���
	 * @date 2018/10/07 23:07
	 * @return 
	 */
	void updateView(const int& nCoin);

	/**
	 * @brief �Ƿ��ܹ���
	 * @param nCoin ��ǰ���
	 * @date 2018/10/07 23:11
	 * @return 
	 */
	bool canBuy(const int& nCoin);
private:
	CC_SYNTHESIZE(int, m_nID, ID);
	CC_SYNTHESIZE(int, m_nNeedCoin,NeedCoin);  //������
	CC_SYNTHESIZE(int, m_nWeaponID, WeaponID); //����ID����ID��
	vector<string> m_vecPngs;
};

#endif