#include "DataMgr.h"

CDataMgr* CDataMgr::m_spInstance = nullptr;

CDataMgr::CDataMgr()
{
	m_pEnemyDtMgr = new CEnemyDtMgr();
	m_pEnemyDtMgr->loadFile("data/enemyData.json");
	m_pCardDtMgr = new CCardDtMgr();
	m_pCardDtMgr->loadFile("data/cardData.json");
	m_pLevelDtMgr = new CLevelDtMgr();
	m_pLevelDtMgr->loadFile("data/levelData.json");
	m_pResDtMgr = new CResDtMgr();
	m_pResDtMgr->loadFile("data/resData.json");
	m_pThemeDtMgr = new CThemeDtMgr();
	m_pThemeDtMgr->loadFile("data/themeData.json");
	m_pWeaponDtMgr = new CWeaponDtMgr();
	m_pWeaponDtMgr->loadFile("data/weaponData.json");
	m_pObjectDtMgr = new CObjectDtMgr();
	m_pObjectDtMgr->loadFile("data/objectData.json");
	m_pBulletDtMgr = new CBulletDtMgr();
	m_pBulletDtMgr->loadFile("data/bulletData.json");
	m_pWaveDtMgr = new CWaveDtMgr();
	m_pWaveDtMgr->loadFile("data/waveData.json");
}

CDataMgr::~CDataMgr()
{
	CC_SAFE_DELETE(m_pEnemyDtMgr);
	CC_SAFE_DELETE(m_pCardDtMgr);
	CC_SAFE_DELETE(m_pLevelDtMgr);
	CC_SAFE_DELETE(m_pResDtMgr);
	CC_SAFE_DELETE(m_pThemeDtMgr);
	CC_SAFE_DELETE(m_pWeaponDtMgr);
	CC_SAFE_DELETE(m_pObjectDtMgr);
	CC_SAFE_DELETE(m_pBulletDtMgr);
	CC_SAFE_DELETE(m_pWaveDtMgr);
}

CDataMgr* CDataMgr::getInstance()
{
	if (nullptr == m_spInstance)
	{
		m_spInstance = new CDataMgr();
	}
	return m_spInstance;
}

sLevelDt* CDataMgr::getCurLevelDt()
{
	const sThemeDt* pThemeData = m_pThemeDtMgr->getCurThemeData();
	int nCurLevelIndex = m_pLevelDtMgr->getCurLevelIndex();	
	int nCurLevelID = pThemeData->vecLevelIDs[nCurLevelIndex];
	sLevelDt* pLevelData = m_pLevelDtMgr->getDataByID(nCurLevelID);
	return pLevelData;
}

sWaveData* CDataMgr::getCurLevelWaveDt()
{
	sLevelDt* pLevelData = getCurLevelDt();
	return m_pWaveDtMgr->getDataByLevelID(pLevelData->nID);
}

void CDataMgr::nextLevel()
{
	const sThemeDt* pThemeData = m_pThemeDtMgr->getCurThemeData();
	m_pLevelDtMgr->addCurLevelIndex();
	if (m_pLevelDtMgr->getCurLevelIndex() >= pThemeData->vecLevelIDs.size())
	{
		//超过最后一关设为第一关
		m_pLevelDtMgr->setCurLevelIndex(0);
	}
}
