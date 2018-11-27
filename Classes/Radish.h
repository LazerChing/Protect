#ifndef __RADISH_H__
#define __RADISH_H__
#include "cocos2d.h"
USING_NS_CC;
using namespace std;
class CRadish :public Node
{
public:
	CRadish();
	virtual ~CRadish();
	virtual bool init();
	CREATE_FUNC(CRadish);

	/**
	 * @brief 受伤接口  
	 * @param 
	 * @date 2018/09/29 17:26
	 * @return true 表示死亡
	 */
	bool damage();

	/**
	 * @brief 播放抖动动画  
	 * @param 
	 * @date 2018/09/29 20:58
	 * @return 
	 */
	void playShakeAnimate();

	/**
	 * @brief 得到萝卜精灵的世界坐标系下包围盒  
	 * @param 
	 * @date 2018/09/30 08:58
	 * @return 世界坐标系下的萝卜包围盒 
	 */
	Rect getSpriteWorldBox();
private:
	CC_SYNTHESIZE(int, m_nCurLife, CurLife); //当前生命
	Sprite* m_pRadishSprite; //萝卜
	string m_strNormalPng;  //正常图
	Animate* m_pShakeAnimate; //抖动动作
	Sprite* m_pLifeSprite; //生命图片
};

#endif