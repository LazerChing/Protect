#ifndef __ENEMY_H__
#define __ENEMY_H__
#include "cocos2d.h"
#include "StructData.h"
#include <string>
#include <ctime>
class CBullet;
USING_NS_CC;
enum 
{
	E_TAG_MOVE = 1
};
class CEnemy :public Sprite
{
public:
	CEnemy();
	virtual ~CEnemy();
	static CEnemy* createWithData(sEnemyDt* pData);
	virtual bool initWithData(sEnemyDt* pData);
	virtual void update(float fDelta);

	/**
	 * @brief 敌人持续受伤  
	 * @param 
	 * @date 2018/10/08 14:58
	 * @return 
	 */
	void damageContinue();

	/**
	 * @brief 怪物移动  
	 * @param 
	 * @date 2018/09/20 15:41
	 * @return 
	 */
	void move();

	/**
	 * @brief 敌人受伤接口
	 * @param pBullet子弹
	 * @date 2018/10/05 16:44
	 * @return 死亡返回true
	 */
	bool damage(CBullet* pBullet);

	/**
	 * @brief 判断该子弹是否攻击过
	 * @param pBullet 子弹
	 * @date 2018/10/05 17:39
	 * @return 攻击过返回true
	 */
	bool isAttacked(CBullet* pBullet);

	/**
	 * @brief 移动受阻
	 * @param fRate 倍率
	 * @param fTime 受阻持续时间
	 * @param hitPng 受阻图片
	 * @date 2018/10/05 20:32
	 * @return 
	 */
	void moveHindered(float fRate, float fTime, vector<string>& hitPng);

	/**
	* @brief 获取是否被标记
	* @param
	* @date 2018/10/06 12:14
	* @return
	*/
	bool getIsSigned()
	{
		return m_bIsSigned;
	}

	/**
	* @brief 设置标记状态
	* @param
	* @date 2018/10/06 12:14
	* @return
	*/
	void setIsSigned(bool bIsSign);

	/**
	 * @brief 把自己从父节点清除，并执行一些其他需要改变的操作
	 * @param 
	 * @date 2018/10/06 14:00
	 * @return 
	 */
	void removeSelf();

	/**
	 * @brief 增加血量(生成敌人的时候调用）
	 * @param nHp 增加的血量
	 * @date 2018/10/06 21:10
	 * @return 
	 */
	void addHp(int nHp)
	{
		m_nMaxHp += nHp;
		m_nHp += nHp;
	}

	/**
	 * @brief 增加移速(生成敌人的时候调用)
	 * @param fSpeed 增加的移速
	 * @date 2018/10/06 21:10
	 * @return 
	 */
	void addSpeed(float fSpeed)
	{
		m_fNormalSpeed += fSpeed;
	}
private:

	/**
	 * @brief 创建动作
	 * @param 
	 * @date 2018/09/20 17:59
	 * @return 
	 */
	void initAnimate();
private:
	CC_SYNTHESIZE(int, m_nID, ID);
	CC_SYNTHESIZE(int, m_nHp, Hp); //当前血量
	CC_SYNTHESIZE(int, m_nMaxHp, MaxHp); //总血量
	CC_SYNTHESIZE(float, m_fNormalSpeed, Speed); //正常移动速度
	CC_SYNTHESIZE(int, m_nCoin, Coin);
	CC_SYNTHESIZE(string, m_strCoin, CoinPng);
	CC_SYNTHESIZE(int, m_nUUID, UUID); //敌人的唯一标识
	vector<string> m_vecPngs; //图片
	vector<Vec2> m_vecPathPos; //移动拐点（路径）
	int m_nPosIndex; //当前拐点位置索引
	Animate* m_pAnimate; //行走动画
	Sprite* m_pShowSprite; //为了播放出场时候的动画

	vector<int> m_vecHitBulUUID; //存放已碰撞过的子弹UUID
	Sprite* m_pHpBg; //血条背景
	ProgressTimer* m_pHpBar;//血条
	float m_fCurSpeedRate;

	clock_t m_nBeginTime; //用来计时是否显示血条的

	bool m_bIsSigned; //设置是否被标记
	Sprite* m_pSignSprite; //显示标记的精灵
};

#endif