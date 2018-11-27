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
	 * @brief ����PageView  
	 * @param 
	 * @date 2018/09/27 16:50
	 * @return 
	 */
	void createPage();

	/**
	 * @brief ���������  
	 * @param 
	 * @date 2018/09/27 16:56
	 * @return 
	 */
	void clickLayer();

	/**
	 * @brief �л������ص�����  
	 * @param 
	 * @date 2018/09/27 16:59
	 * @return 
	 */
	void changeScene();
private:
	PageView* m_pPageView;
	Layer* m_pClickLayer;//����㣬�����жϵ��
	Vec2 m_vBeginClickPos;//��ʼ�����λ��
	bool m_bIsScale;//�Ƿ�Ŵ�
};

#endif