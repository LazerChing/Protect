#ifndef __BULLET_H__
#define __BULLET_H__
#include "cocos2d.h"
#include "StructData.h"
#include "Weapon.h"
USING_NS_CC;
class CBullet :public Node
{
public:
	CBullet();
	~CBullet();
	bool initWithData(sBulletDt* pData);
	static CBullet* createWithData(sBulletDt* pData);
	virtual void update(float fDelta);

	/**
	 * @brief 取得受阻图片
	 * @param
	 * @date 2018/10/05 23:19
	 * @return
	 */
	vector<string> getHitPng()
	{
		return m_vecHitPng;
	}

	/**
	 * @brief 取得爆炸图片
	 * @param
	 * @date 2018/10/05 18:46
	 * @return
	 */
	vector<string> getBombPngs()
	{
		return m_vecBombPng;
	}

	/**
	 * @brief 播放爆炸动画(并移除子弹）
	 * @param
	 * @date 2018/10/05 17:03
	 * @return
	 */
	void playBombAnimate();

	/**
	 * @brief 得到精灵包围盒
	 * @param
	 * @date 2018/10/04 17:36
	 * @return
	 */
	Rect getSpriteBox()
	{
		return m_pBullet->getBoundingBox();
	}

	/**
	* @brief 子弹移动
	* @param
	* @date 2018/10/04 17:34
	* @return
	*/
	void move(Vec2 endPos);

	/**
	 * @brief 对附近的敌人造成伤害
	 * @param fRange 可造成伤害的范围
	 * @date 2018/10/07 21:49
	 * @return
	 */
	void damageAroundEnemy(float fRange);

	/**
	 * @brief 开始定时造成伤害（针对Ball炮塔的子弹，子弹是一条线，并每隔一定时间造成伤害）
	 * @param pWeapon 子弹从哪个炮塔发射出来
	 * @param pAimedNode 目标
	 * @param nUUID 目标UUID（物品或敌人）
	 * @date 2018/10/08 16:31
	 * @return
	 */
	void startTimingDamage(CWeapon* pWeapon, Node* pAimedNode, int nUUID);

	/**
	 * @brief 设置精灵的锚点
	 * @param 锚点
	 * @date 2018/10/08 16:04
	 * @return
	 */
	void setSpriteAnchorPoint(Vec2 anchorPoint)
	{
		m_pBullet->setAnchorPoint(anchorPoint);
	}

	/**
	 * @brief 取得子弹的归属者（炮塔）
	 * @param
	 * @date 2018/10/08 20:39
	 * @return
	 */
	CWeapon* getOwner();

	/**
	 * @brief 得到子弹的目标
	 * @param
	 * @date 2018/10/08 20:40
	 * @return
	 */
	sSignedNodeDt* getAimedNode()
	{
		return &m_sAimedNode;
	}

	/**
	 * @brief 清除当前目标
	 * @param
	 * @date 2018/10/09 13:45
	 * @return
	 */
	void clearAim()
	{
		m_sAimedNode = sSignedNodeDt();
		m_nWeaponUUID = 0;
	}

private:
	/**
	 * @brief 每隔一定时间造成伤害回调函数（针对Ball炮塔的子弹）
	 * @param 
	 * @date 2018/10/08 14:52
	 * @return 
	 */
	void timingDamageCallback(float fDelta);
private:
	CC_SYNTHESIZE(int, m_nID, ID);
	CC_SYNTHESIZE(string, m_strType, Type); //子弹类型
	CC_SYNTHESIZE(int, m_nAck, Ack);
	CC_SYNTHESIZE(float, m_fSpeed, Speed); //子弹移动速度
	CC_SYNTHESIZE(float, m_fReduceSpeedRate, ReduceSpeedRate); //减速倍率
	CC_SYNTHESIZE(float, m_fContinueTime, ContinueTime);//减速持续时间
	CC_SYNTHESIZE(int, m_nUUID, UUID); //子弹的唯一标识
	CC_SYNTHESIZE(string, m_strAim, Aim); //子弹发射时候的目标
	vector<string> m_vecBombPng; //爆炸图片
	vector<string> m_vecAnimatePng; //子弹动画图片
	vector<string> m_vecHitPng; //减速类型的子弹击中后的图片

	Sprite* m_pBullet; //子弹精灵
	Animate* m_pBulAnimate; //子弹动画
	Animate* m_pBombAnimate;; //子弹爆炸动画
	float m_fRotation; //旋转角度

	//针对Ball炮塔的子弹
	int m_nWeaponUUID; //子弹是从哪个炮塔发射的 ， 炮塔UUID
	sSignedNodeDt m_sAimedNode;//子弹的目标数据（敌人或物品)
};

#endif