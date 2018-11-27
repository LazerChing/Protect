#ifndef __WEAPON_H__
#define __WEAPON_H__
#include "cocos2d.h"
#include "StructData.h"
#include "Enemy.h"
USING_NS_CC;
enum 
{
	E_ANIMATE_NONE, E_ANIMATE_FIRE
};

struct sSignedNodeDt //����ǵĽڵ���Ϣ
{
	sSignedNodeDt()
	{
		nodeName = "";
		nUUID = 0;
	}
	string nodeName; //�ڵ����֣�ͨ�������������
	int nUUID; //�ڵ�Ψһ��ʶ��ͨ����ʶ�ҵ��ڵ�
};

class CWeapon :public Node
{
public:
	CWeapon();
	virtual ~CWeapon();
	virtual bool initWithData(sWeaponDt* pData);
	static CWeapon* createWithData(sWeaponDt* pData);
	virtual void update(float fDelta);
	/**
	 * @brief �������ͼƬ  
	 * @param bCanUpGrade ����ֵ �����Ƿ�������
	 * @date 2018/09/30 14:50
	 * @return png
	 */
	string getUpGradePng(bool& bCanUpGrade);

	/**
	 * @brief ����Ƿ�����������ʾ����������
	 * @param 
	 * @date 2018/10/04 11:48
	 * @return 
	 */
	void checkUpgrade();

	/**
	 * @brief �Ƴ�����Ŀ��  
	 * @param 
	 * @date 2018/10/08 16:21
	 * @return 
	 */
	void removeAim()
	{
		m_sAimedNode = sSignedNodeDt();
	}
private:
	/**
	 * @brief ��������  
	 * @param 
	 * @date 2018/09/28 16:32
	 * @return 
	 */
	void fire(float fDelta);

	/**
	 * @brief ���ҹ�����Χ�ڵ�һ�����ˣ���ͨ������
	 * @param 
	 * @date 2018/10/04 18:24
	 * @return 
	 */
	CEnemy* findOneEnemy();

	/**
	 * @brief ���ҹ�����Χ�ڵ�3�����ˣ�����������
	 * @param 
	 * @date 2018/10/07 21:20
	 * @return 
	 */
	vector<CEnemy*>& findThreeEnemy();
private:
	CC_SYNTHESIZE(int, m_nID, ID);
	CC_SYNTHESIZE(int, m_nNextID, NextID);
	CC_SYNTHESIZE(int, m_nUpGradeCoin, UpGradeCoin);
	CC_SYNTHESIZE(int, m_nSellCoin, SellCoin);
	CC_SYNTHESIZE(string, m_strSellPng, StrSellPng);//��������ͼ
	CC_SYNTHESIZE(int, m_nBulletID, BulletID); //�ӵ�ID
	CC_SYNTHESIZE(float, m_fFireInterval, FireInterval); //������
	CC_SYNTHESIZE(int, m_nRange, Range);//������Χ
	CC_SYNTHESIZE(string, m_strType, Type); //��������
	CC_SYNTHESIZE(string, m_strFireMusic, FireMusic); //������Ч
	CC_SYNTHESIZE(int, m_nUUID, UUID); //������Ψһ��ʶ
	vector<string> m_vecUpGradePng; //���������Ҳ��� �� �㹻 �� ���� ��ͼƬ
	//string bgPng;	  //����ͼ
	string m_strNormalPng; //������̬��ͼ
	//vector<string> vecFirePng; //���𶯻�ͼ

	Sprite* m_pShowUpgradeSprite; //��������ͼƬ�ľ���
	Animate* m_pShowUpgradeAnimate; //��ʾ�����Ķ���
	Sprite* m_pBg;
	Sprite* m_pWeapon;
	Animate* m_pFireAnimate; //���𶯻�
	CallFunc* m_pStopFunc;	//ֹͣ����
	bool m_bIsMax; //�Ƿ�������

	//���Ball������
	sSignedNodeDt m_sAimedNode; //��ǰ��׼�ĵ��ˡ���Ʒ������
};

#endif