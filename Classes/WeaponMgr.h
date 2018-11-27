#ifndef __WEAPONMGR_H__
#define __WEAPONMGR_H__
#include "cocos2d.h"
#include "Weapon.h"
USING_NS_CC;
class CWeaponMgr : public Layer
{
public:
	CWeaponMgr();
	virtual ~CWeaponMgr();
	virtual bool init();
	CREATE_FUNC(CWeaponMgr);

	/**
	 * @brief 创建武器接口  
	 * @param nWeaponID 武器ID
	 * @param centerPixelPos 格子坐标的中心像素坐标
	 * @date 2018/09/28 15:50
	 * @return 
	 */
	void addWeapon(int nWeaponID, Vec2 centerPixelPos);

	/**
	 * @brief 升级武器
	 * @param pWeapon 要升级的武器
	 * @param centerPixelPos 武器中心位置
	 * @date 2018/10/01 13:32
	 * @return 
	 */
	void upgradeWeapon(CWeapon* pWeapon, Vec2 centerPixelPos);

	/**
	 * @brief 出售武器
	 * @param pWeapon 要出售的武器
	 * @date 2018/10/04 11:24
	 * @return 
	 */
	void sellWeapon(CWeapon* pWeapon);

	/**
	 * @brief 检测是否能升级武器,显示升级图标
	 * @param 
	 * @date 2018/10/04 11:33
	 * @return 
	 */
	void checkUpgrade();

	/**
	 * @brief 通过UUID查找武器  
	 * @param nUUID
	 * @date 2018/10/09 10:49
	 * @return 如果没找到，返回null
	 */
	CWeapon* findWeaponByUUID(int nUUID);
private:
	int m_nCurWeaponUUID; //当前炮塔uuid
};


#endif