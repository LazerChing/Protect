#ifndef __DATAMGR_H__
#define __DATAMGR_H__
#include "StructData.h"
class CDataMgr
{
public:
	CDataMgr();
	~CDataMgr();
	/**
	* @brief 得到当前关卡数据
	* @param
	* @date 2018/09/27 19:11
	* @return
	*/
	sLevelDt* getCurLevelDt();

	/**
	 * @brief 得到当前关卡敌人波数数据
	 * @param 
	 * @date 2018/10/06 20:49
	 * @return 
	 */
	sWaveData* getCurLevelWaveDt();

	/**
	 * @brief 下一关
	 * @param 
	 * @date 2018/10/09 21:44
	 * @return 
	 */
	void nextLevel();

	static CDataMgr* getInstance();
	CEnemyDtMgr* getEnemyMgr()
	{
		return m_pEnemyDtMgr;
	}
	CCardDtMgr* getCardDtMgr()
	{
		return m_pCardDtMgr;
	}
	CLevelDtMgr* getLevelDtMgr()
	{
		return m_pLevelDtMgr;
	}
	CResDtMgr* getResDtMgr()
	{
		return m_pResDtMgr;
	}
	CThemeDtMgr* getThemeDtMgr()
	{
		return m_pThemeDtMgr;
	}
	CWeaponDtMgr* getWeaponDtMgr()
	{
		return m_pWeaponDtMgr;
	}
	CObjectDtMgr* getObjectDtMgr()
	{
		return m_pObjectDtMgr;
	}
	CBulletDtMgr* getBulletDtMgr()
	{
		return m_pBulletDtMgr;
	}
	CWaveDtMgr* getWaveDtMgr()
	{
		return m_pWaveDtMgr;
	}
private:
	static CDataMgr* m_spInstance;
	CEnemyDtMgr* m_pEnemyDtMgr;
	CCardDtMgr* m_pCardDtMgr;
	CLevelDtMgr* m_pLevelDtMgr;
	CResDtMgr* m_pResDtMgr;
	CThemeDtMgr* m_pThemeDtMgr;
	CWeaponDtMgr* m_pWeaponDtMgr;
	CObjectDtMgr* m_pObjectDtMgr;
	CBulletDtMgr* m_pBulletDtMgr;
	CWaveDtMgr* m_pWaveDtMgr;
};

#endif