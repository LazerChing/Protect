#ifndef __CARDMGR_H__
#define __CARDMGR_H__
#include "cocos2d.h"
#include "Card.h"
#include "DataMgr.h"
#include "Weapon.h"
USING_NS_CC;
class CCardMgr :public Node
{
public:
	CCardMgr();
	virtual ~CCardMgr();
	virtual bool init();
	CREATE_FUNC(CCardMgr);

	/**
	 * @brief 点击后显示或隐藏卡片
	 * @param pos 点击位置
	 * @param nType 可操作性类型
	 * @param pSelectedNode 选中的节点，可能为空
	 * @date 2018/09/22 11:01
	 * @return 
	 */
	void clickEvent(const Vec2& pos, const int& nType, Node*& pSelectedNode);

private:
	CC_SYNTHESIZE(bool, m_bIsFirstClick, IsFirstClick);//是否是第一次点击
	vector<CCard*> m_vecCards; //卡片
	Node* m_pCardsNode;  //负责显示卡片的节点
	CCard* m_pPlaneCard;
	Sprite* m_pSelectTower; //选中的地方的加号
	Animate* m_pSelectAnimate; //选中地方的动画
	Sprite* m_pDisablePng; //负责显示不可操作图片
	Animate* m_pBuildTowerAnimate; //建塔动画

	//以下为卡片升级
	CWeapon* m_pSelectedWeapon;	//打开卡片升级时，当前选中的武器
	Node* m_pUpGradeNode; //负责显示卡片升级的节点
	Sprite* m_pRangeSprite; //升级范围的执行动画的精灵
	Sprite* m_pUpgradeCoinSprite; //所需金币精灵
	Sprite* m_pSellSprite; //出售精灵
};

#endif