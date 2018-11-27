#ifndef __ANIMATEMGR_H__
#define __ANIMATEMGR_H__
#include "cocos2d.h"
#include <vector>
#include <string>
USING_NS_CC;
using namespace std;
class CAnimateMgr : public Node
{
public:
	CAnimateMgr();
	~CAnimateMgr();
	virtual bool init();
	CREATE_FUNC(CAnimateMgr);

	/**
	 * @brief 创建箭头并播放动画
	 * @param pos 坐标
	 * @param fRotate 旋转角度
	 * @date 2018/10/07 16:17
	 * @return 
	 */
	void createArrow(Vec2 pos, float fRotate);

	/**
	 * @brief 通过数组播放动画
	 * @param 
	 * @date 2018/10/05 19:14
	 * @return 
	 */
	void playAnimateByVecPng(Vec2 pos, vector<string>& vecPngs);

	/**
	 * @brief 播放敌人爆炸动画
	 * @param pos 敌人位置
	 * @param strCoin 所加金币图片
	 * @date 2018/10/05 19:19
	 * @return 
	 */
	void playEnemyBombAnimate(Vec2 pos, string strCoin);

	/**
	 * @brief 播放最后一波特效
	 * @param 
	 * @date 2018/10/06 21:18
	 * @return 
	 */
	void playFinalWaveAnimate();
private:
	Animate* m_pEnemyBombAnimate;
};

#endif