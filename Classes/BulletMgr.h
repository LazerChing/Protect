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
	 * @brief ����ӵ��ӿ�
	 * @param nBulID �ӵ�ID
	 * @param bulStartPos �ӵ���ʼλ��
	 * @param bulEndPos �ӵ��յ�
	 * @param aim �ӵ�����ʱ��Ŀ��
	 * @date 2018/10/05 11:36
	 * @return 
	 */
	void addBullet(const int& nBulID, Vec2 bulStartPos, Vec2 bulEndPos, string aim);

	/**
	 * @brief �����׼���ӵ������Ball�������ӵ���һ���ߣ�
	 * @param pWeapon �������������
	 * @param pAimNode Ŀ��
	 * @param nUUID Ŀ��UUID�����˻���Ʒ��
	 * @date 2018/10/08 16:28
	 * @return 
	 */
	void addAimedBullet(CWeapon* pWeapon, Node* pAimNode, int nUUID);
private:
	int m_nCurBulUUID; //��ǰ�ӵ�uuid
};

#endif