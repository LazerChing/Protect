#ifndef __ENEMYMGR_H__
#define __ENEMYMGR_H__
#include "cocos2d.h"
#include "Enemy.h"
USING_NS_CC;

class CEnmeyMgr :public Layer
{
public:
	CEnmeyMgr();
	virtual ~CEnmeyMgr();
	virtual bool init();
	CREATE_FUNC(CEnmeyMgr);

	/**
	 * @brief 通过UUID查找敌人
	 * @param nUUID
	 * @date 2018/10/06 16:13
	 * @return 
	 */
	CEnemy* findEnemyByUUID(int nUUID);

	/**
	 * @brief 通知游戏开始了
	 * @param 
	 * @date 2018/10/07 15:36
	 * @return 
	 */
	void gameStart();
private:
	/**
	 * @brief 生成敌人  
	 * @param 
	 * @date 2018/09/20 15:20
	 * @return 
	 */
	void generateEnemy(float fDelta);
private:
	CC_SYNTHESIZE(int, m_nCurWaveNum, CurWaveNum); //当前波数
	int m_nCurEnemyUUID; //当前生成敌人uuid

	int m_nCurEnemyNum; //当前敌人数量
	sWaveData* m_pWaveData; //波数数据

	bool m_bIsPlayedFinalWaveAnimate; //是否播放了最后一波动画
	bool m_bIsRunNextWave; //是否执行了“下一波”逻辑
};

#endif