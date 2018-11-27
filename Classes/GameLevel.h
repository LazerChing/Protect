#ifndef __GAMELEVEL_H__
#define __GAMELEVEL_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "CardMgr.h"
USING_NS_CC;
using namespace ui;
class CGameLevel :public Scene
{
public:
	CGameLevel();
	virtual ~CGameLevel();
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(CGameLevel);
private:
	/**
	 * @brief 创建滑动页面  
	 * @param 
	 * @date 2018/09/19 19:00
	 * @return 
	 */
	void createPage();

	/**
	 * @brief 检测点击  
	 * @param 
	 * @date 2018/09/19 19:01
	 * @return 
	 */
	void clickLayer();

	/**
	 * @brief 切换场景  
	 * @param 
	 * @date 2018/09/19 19:32
	 * @return 
	 */
	void changeScene();

	/**
	 * @brief 开始游戏回调函数  
	 * @param 
	 * @date 2018/09/27 10:33
	 * @return 
	 */
	void gameStart(Ref* pSender);

private:
	PageView* m_pPageView;
	Layer* m_pClickLayer;//点击层，用来判断点击
	Vec2 m_vBeginClickPos;//开始点击的位置
	bool m_bIsScale;//是否放大
	Sprite * m_pWave;	//当前关卡波数
	Sprite* m_pWeapon;	//当前关卡武器
};

#endif