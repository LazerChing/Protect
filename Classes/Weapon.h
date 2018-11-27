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

struct sSignedNodeDt //被标记的节点信息
{
	sSignedNodeDt()
	{
		nodeName = "";
		nUUID = 0;
	}
	string nodeName; //节点名字（通过名字区分类别）
	int nUUID; //节点唯一标识，通过标识找到节点
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
	 * @brief 获得升级图片  
	 * @param bCanUpGrade 返回值 返回是否能升级
	 * @date 2018/09/30 14:50
	 * @return png
	 */
	string getUpGradePng(bool& bCanUpGrade);

	/**
	 * @brief 检查是否能升级，显示可升级动画
	 * @param 
	 * @date 2018/10/04 11:48
	 * @return 
	 */
	void checkUpgrade();

	/**
	 * @brief 移除攻击目标  
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
	 * @brief 武器发射  
	 * @param 
	 * @date 2018/09/28 16:32
	 * @return 
	 */
	void fire(float fDelta);

	/**
	 * @brief 查找攻击范围内的一个敌人（普通炮塔）
	 * @param 
	 * @date 2018/10/04 18:24
	 * @return 
	 */
	CEnemy* findOneEnemy();

	/**
	 * @brief 查找攻击范围内的3个敌人（星星炮塔）
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
	CC_SYNTHESIZE(string, m_strSellPng, StrSellPng);//销售武器图
	CC_SYNTHESIZE(int, m_nBulletID, BulletID); //子弹ID
	CC_SYNTHESIZE(float, m_fFireInterval, FireInterval); //发射间隔
	CC_SYNTHESIZE(int, m_nRange, Range);//攻击范围
	CC_SYNTHESIZE(string, m_strType, Type); //武器类型
	CC_SYNTHESIZE(string, m_strFireMusic, FireMusic); //发射音效
	CC_SYNTHESIZE(int, m_nUUID, UUID); //武器的唯一标识
	vector<string> m_vecUpGradePng; //升级所需金币不足 或 足够 或 满级 的图片
	//string bgPng;	  //背景图
	string m_strNormalPng; //正常形态的图
	//vector<string> vecFirePng; //开火动画图

	Sprite* m_pShowUpgradeSprite; //承载升级图片的精灵
	Animate* m_pShowUpgradeAnimate; //显示升级的动画
	Sprite* m_pBg;
	Sprite* m_pWeapon;
	Animate* m_pFireAnimate; //开火动画
	CallFunc* m_pStopFunc;	//停止动画
	bool m_bIsMax; //是否是满级

	//针对Ball类炮塔
	sSignedNodeDt m_sAimedNode; //当前瞄准的敌人、物品的数据
};

#endif