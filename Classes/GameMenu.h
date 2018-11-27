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
	 * @brief 按钮回调函数  
	 * @param 
	 * @date 2018/09/26 11:06
	 * @return 
	 */
	void menuCallBack(Ref* pSender);

	/**
	 * @brief 弹出层OK回调函数  
	 * @param 
	 * @date 2018/09/26 14:54
	 * @return 
	 */
	void okCallBack(Ref* pSender);

	/**
	 * @brief 创建背景  
	 * @param 
	 * @date 2018/09/26 13:48
	 * @return 
	 */
	void createBg();

	/**
	 * @brief 中间叶子移动回调函数  
	 * @param 
	 * @date 2018/09/26 16:06
	 * @return 
	 */
	void midLeafMove(float fDelta);

	/**
	 * @brief 中间叶子移动完成，右边叶子移动的回调函数  
	 * @param 
	 * @date 2018/09/26 16:01
	 * @return 
	 */
	void rightLeafMove();

	/**
	 * @brief 小云移动  
	 * @param 
	 * @date 2018/09/26 16:25
	 * @return 
	 */
	void smallCloudMove();

	/**
	 * @brief 小云移动  
	 * @param 
	 * @date 2018/09/26 16:36
	 * @return 
	 */
	void bigCloudMove();

	/**
	 * @brief 创建弹出层  
	 * @param 
	 * @date 2018/09/28 16:50
	 * @return 
	 */
	void createPopUpLayer();
private:
	Size m_visibleSize;
	MenuItemImage* m_pBtnAdventure; //冒险模式按钮
	MenuItemImage* m_pBtnBoss; //boss模式按钮
	MenuItemImage* m_pBtnMonsterHome; //怪物窝按钮
	Sprite* m_pBossLooked;	//boss锁的图片
	Layer* m_pPopUpLayer;  //弹出的层（boss模式锁住了）

	Sprite* m_pMidLeaf;  //中间叶子
	Sprite* m_pRightLeaf;  //右边叶子

	Sprite* m_pSmallCloud; //小云
	Sprite* m_pBigCloud; //大云
};

#endif