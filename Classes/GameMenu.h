#ifndef __GAMEMENU_H__
#define __GAMEMENU_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;
class CGameMenu :public Scene
{
public:
	CGameMenu();
	virtual ~CGameMenu();
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(CGameMenu);
private:
	/**
	 * @brief ��ť�ص�����  
	 * @param 
	 * @date 2018/09/26 11:06
	 * @return 
	 */
	void menuCallBack(Ref* pSender);

	/**
	 * @brief ������OK�ص�����  
	 * @param 
	 * @date 2018/09/26 14:54
	 * @return 
	 */
	void okCallBack(Ref* pSender);

	/**
	 * @brief ��������  
	 * @param 
	 * @date 2018/09/26 13:48
	 * @return 
	 */
	void createBg();

	/**
	 * @brief �м�Ҷ���ƶ��ص�����  
	 * @param 
	 * @date 2018/09/26 16:06
	 * @return 
	 */
	void midLeafMove(float fDelta);

	/**
	 * @brief �м�Ҷ���ƶ���ɣ��ұ�Ҷ���ƶ��Ļص�����  
	 * @param 
	 * @date 2018/09/26 16:01
	 * @return 
	 */
	void rightLeafMove();

	/**
	 * @brief С���ƶ�  
	 * @param 
	 * @date 2018/09/26 16:25
	 * @return 
	 */
	void smallCloudMove();

	/**
	 * @brief С���ƶ�  
	 * @param 
	 * @date 2018/09/26 16:36
	 * @return 
	 */
	void bigCloudMove();

	/**
	 * @brief ����������  
	 * @param 
	 * @date 2018/09/28 16:50
	 * @return 
	 */
	void createPopUpLayer();
private:
	Size m_visibleSize;
	MenuItemImage* m_pBtnAdventure; //ð��ģʽ��ť
	MenuItemImage* m_pBtnBoss; //bossģʽ��ť
	MenuItemImage* m_pBtnMonsterHome; //�����Ѱ�ť
	Sprite* m_pBossLooked;	//boss����ͼƬ
	Layer* m_pPopUpLayer;  //�����Ĳ㣨bossģʽ��ס�ˣ�

	Sprite* m_pMidLeaf;  //�м�Ҷ��
	Sprite* m_pRightLeaf;  //�ұ�Ҷ��

	Sprite* m_pSmallCloud; //С��
	Sprite* m_pBigCloud; //����
};

#endif