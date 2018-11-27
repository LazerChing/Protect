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
	 * @brief ���������ӿ�  
	 * @param nWeaponID ����ID
	 * @param centerPixelPos ���������������������
	 * @date 2018/09/28 15:50
	 * @return 
	 */
	void addWeapon(int nWeaponID, Vec2 centerPixelPos);

	/**
	 * @brief ��������
	 * @param pWeapon Ҫ����������
	 * @param centerPixelPos ��������λ��
	 * @date 2018/10/01 13:32
	 * @return 
	 */
	void upgradeWeapon(CWeapon* pWeapon, Vec2 centerPixelPos);

	/**
	 * @brief ��������
	 * @param pWeapon Ҫ���۵�����
	 * @date 2018/10/04 11:24
	 * @return 
	 */
	void sellWeapon(CWeapon* pWeapon);

	/**
	 * @brief ����Ƿ�����������,��ʾ����ͼ��
	 * @param 
	 * @date 2018/10/04 11:33
	 * @return 
	 */
	void checkUpgrade();

	/**
	 * @brief ͨ��UUID��������  
	 * @param nUUID
	 * @date 2018/10/09 10:49
	 * @return ���û�ҵ�������null
	 */
	CWeapon* findWeaponByUUID(int nUUID);
private:
	int m_nCurWeaponUUID; //��ǰ����uuid
};


#endif