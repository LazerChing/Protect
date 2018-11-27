#ifndef __OBJECT_H__
#define __OBJECT_H__
#include "cocos2d.h"
#include "StructData.h"
#include "Bullet.h"
#include <ctime>
USING_NS_CC;
class CObject : public Sprite
{
public:
	CObject();
	virtual ~CObject();
	virtual void update(float fDelta);
	virtual bool initWithData(sObjectDt* pData);
	static CObject* createWithData(sObjectDt* pData);
	CC_SYNTHESIZE(string, m_strID, StrID);
	CC_SYNTHESIZE(int, m_nHp, Hp);
	CC_SYNTHESIZE(int, m_nMaxHp, MaxHp);
	CC_SYNTHESIZE(int, m_nCoin, Coin);
	CC_SYNTHESIZE(string, m_strCoinPng, CoinPng);
	CC_SYNTHESIZE(int, m_nUUID, UUID); //��Ʒ��Ψһ��ʶ

	/**
	 * @brief ���˽ӿ�
	 * @param pBullet �ӵ�
	 * @date 2018/10/06 11:08
	 * @return �������� true
	 */
	bool damage(CBullet* pBullet);

	/**
	* @brief �жϸ��ӵ��Ƿ񹥻���
	* @param pBullet �ӵ�
	* @date 2018/10/06 11:13
	* @return ����������true
	*/
	bool isAttacked(CBullet* pBullet);

	/**
	 * @brief ��ȡ�Ƿ񱻱��
	 * @param 
	 * @date 2018/10/06 12:02
	 * @return 
	 */
	bool getIsSigned()
	{
		return m_bIsSigned;
	}

	/**
	 * @brief ���ñ��״̬
	 * @param 
	 * @date 2018/10/06 12:02
	 * @return 
	 */
	void setIsSigned(bool bIsSign);

	/**
	* @brief ���Լ��Ӹ��ڵ��������ִ��һЩ������Ҫ�ı�Ĳ���
	* @param
	* @date 2018/10/06 14:02
	* @return
	*/
	void removeSelf();

private:
	string m_strName;
	string m_strPng;
	Sprite* m_pHpBg; //Ѫ������
	ProgressTimer* m_pHpBar; //Ѫ��
	
	vector<int> m_vecHitBulUUID; //�������ײ�����ӵ�UUID
	clock_t m_nBeginTime; //������ʱ�Ƿ���ʾѪ����
	bool m_bIsSigned; //�����Ƿ񱻱��
	Sprite* m_pSignSprite; //��ʾ��ǵľ���
};

#endif