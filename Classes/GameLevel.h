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
	 * @brief ��������ҳ��  
	 * @param 
	 * @date 2018/09/19 19:00
	 * @return 
	 */
	void createPage();

	/**
	 * @brief �����  
	 * @param 
	 * @date 2018/09/19 19:01
	 * @return 
	 */
	void clickLayer();

	/**
	 * @brief �л�����  
	 * @param 
	 * @date 2018/09/19 19:32
	 * @return 
	 */
	void changeScene();

	/**
	 * @brief ��ʼ��Ϸ�ص�����  
	 * @param 
	 * @date 2018/09/27 10:33
	 * @return 
	 */
	void gameStart(Ref* pSender);

private:
	PageView* m_pPageView;
	Layer* m_pClickLayer;//����㣬�����жϵ��
	Vec2 m_vBeginClickPos;//��ʼ�����λ��
	bool m_bIsScale;//�Ƿ�Ŵ�
	Sprite * m_pWave;	//��ǰ�ؿ�����
	Sprite* m_pWeapon;	//��ǰ�ؿ�����
};

#endif