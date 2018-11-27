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
	 * @brief �������ʾ�����ؿ�Ƭ
	 * @param pos ���λ��
	 * @param nType �ɲ���������
	 * @param pSelectedNode ѡ�еĽڵ㣬����Ϊ��
	 * @date 2018/09/22 11:01
	 * @return 
	 */
	void clickEvent(const Vec2& pos, const int& nType, Node*& pSelectedNode);

private:
	CC_SYNTHESIZE(bool, m_bIsFirstClick, IsFirstClick);//�Ƿ��ǵ�һ�ε��
	vector<CCard*> m_vecCards; //��Ƭ
	Node* m_pCardsNode;  //������ʾ��Ƭ�Ľڵ�
	CCard* m_pPlaneCard;
	Sprite* m_pSelectTower; //ѡ�еĵط��ļӺ�
	Animate* m_pSelectAnimate; //ѡ�еط��Ķ���
	Sprite* m_pDisablePng; //������ʾ���ɲ���ͼƬ
	Animate* m_pBuildTowerAnimate; //��������

	//����Ϊ��Ƭ����
	CWeapon* m_pSelectedWeapon;	//�򿪿�Ƭ����ʱ����ǰѡ�е�����
	Node* m_pUpGradeNode; //������ʾ��Ƭ�����Ľڵ�
	Sprite* m_pRangeSprite; //������Χ��ִ�ж����ľ���
	Sprite* m_pUpgradeCoinSprite; //�����Ҿ���
	Sprite* m_pSellSprite; //���۾���
};

#endif