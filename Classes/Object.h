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
	CC_SYNTHESIZE(int, m_nUUID, UUID); //物品的唯一标识

	/**
	 * @brief 受伤接口
	 * @param pBullet 子弹
	 * @date 2018/10/06 11:08
	 * @return 死亡返回 true
	 */
	bool damage(CBullet* pBullet);

	/**
	* @brief 判断该子弹是否攻击过
	* @param pBullet 子弹
	* @date 2018/10/06 11:13
	* @return 攻击过返回true
	*/
	bool isAttacked(CBullet* pBullet);

	/**
	 * @brief 获取是否被标记
	 * @param 
	 * @date 2018/10/06 12:02
	 * @return 
	 */
	bool getIsSigned()
	{
		return m_bIsSigned;
	}

	/**
	 * @brief 设置标记状态
	 * @param 
	 * @date 2018/10/06 12:02
	 * @return 
	 */
	void setIsSigned(bool bIsSign);

	/**
	* @brief 把自己从父节点清除，并执行一些其他需要改变的操作
	* @param
	* @date 2018/10/06 14:02
	* @return
	*/
	void removeSelf();

private:
	string m_strName;
	string m_strPng;
	Sprite* m_pHpBg; //血条背景
	ProgressTimer* m_pHpBar; //血条
	
	vector<int> m_vecHitBulUUID; //存放已碰撞过的子弹UUID
	clock_t m_nBeginTime; //用来计时是否显示血条的
	bool m_bIsSigned; //设置是否被标记
	Sprite* m_pSignSprite; //显示标记的精灵
};

#endif