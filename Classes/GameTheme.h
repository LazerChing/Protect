#ifndef __GAMETHEME_H__
#define __GAMETHEME_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "DataMgr.h"
USING_NS_CC;
using namespace ui;

class CGameTheme: public Scene
{
public:
	CGameTheme();
	virtual ~CGameTheme();
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(CGameTheme);
private:
	/**
	 * @brief 创建PageView  
	 * @param 
	 * @date 2018/09/27 16:50
	 * @return 
	 */
	void createPage();

	/**
	 * @brief 创建点击层  
	 * @param 
	 * @date 2018/09/27 16:56
	 * @return 
	 */
	void clickLayer();

	/**
	 * @brief 切换场景回调函数  
	 * @param 
	 * @date 2018/09/27 16:59
	 * @return 
	 */
	void changeScene();
private:
	PageView* m_pPageView;
	Layer* m_pClickLayer;//点击层，用来判断点击
	Vec2 m_vBeginClickPos;//开始点击的位置
	bool m_bIsScale;//是否放大
};

#endif