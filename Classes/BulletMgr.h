#ifndef __BULLETMGR_H__
#define __BULLETMGR_H__
#include "cocos2d.h"
#include "Bullet.h"
#include "Weapon.h"
USING_NS_CC;
class CBulletMgr :public Node
{
public:
	CBulletMgr();
	~CBulletMgr();
	virtual bool init();
	CREATE_FUNC(CBulletMgr);

	/**
	 * @brief 添加子弹接口
	 * @param nBulID 子弹ID
	 * @param bulStartPos 子弹初始位置
	 * @param bulEndPos 子弹终点
	 * @param aim 子弹发射时的目标
	 * @date 2018/10/05 11:36
	 * @return 
	 */
	void addBullet(const int& nBulID, Vec2 bulStartPos, Vec2 bulEndPos, string aim);

	/**
	 * @brief 添加瞄准的子弹（针对Ball炮塔，子弹是一条线）
	 * @param pWeapon 发射出来的炮塔
	 * @param pAimNode 目标
	 * @param nUUID 目标UUID（敌人或物品）
	 * @date 2018/10/08 16:28
	 * @return 
	 */
	void addAimedBullet(CWeapon* pWeapon, Node* pAimNode, int nUUID);
private:
	int m_nCurBulUUID; //当前子弹uuid
};

#endif