#include "StructData.h"

//敌人数据管理者
CEnemyDtMgr::CEnemyDtMgr()
{
}

CEnemyDtMgr::~CEnemyDtMgr()
{
}

void CEnemyDtMgr::loadFile(const char* pFileName)
{
	//获得全路径
	string fullPath = FileUtils::getInstance()->fullPathForFilename(pFileName);
	//取得内容
	string strJson = FileUtils::getInstance()->getStringFromFile(fullPath);
	//获得doc
	rapidjson::Document doc;
	//解析内容
	doc.Parse<0>(strJson.c_str());
	for (int i = 0; i < doc.Size();++i)
	{
		sEnemyDt* pData = new sEnemyDt();
		rapidjson::Value& vValue = doc[i];
		pData->nID = vValue["id"].GetInt();
		pData->nHp = vValue["hp"].GetInt();
		pData->fSpeed = vValue["speed"].GetFloat();
		rapidjson::Value& vPngs = vValue["pngs"];
		for (int j = 0; j<vPngs.Size();++j)
		{
			pData->vecPngs.push_back(vPngs[j].GetString());
		}
		pData->nCoin = vValue["coin"].GetInt();
		pData->strCoin = vValue["coinPng"].GetString();
		m_vecDatas.push_back(pData);
	}
	doc.Clear();
}

sEnemyDt* CEnemyDtMgr::getDataByID(int nID)
{
	for (sEnemyDt* pData : m_vecDatas)
	{
		if (pData->nID == nID)
		{
			return pData;
		}
	}
	return nullptr;
}


//卡片数据管理者
CCardDtMgr::CCardDtMgr()
{
}

CCardDtMgr::~CCardDtMgr()
{
}

void CCardDtMgr::loadFile(const char* pFileName)
{
	//获得全路径
	string fullPath = FileUtils::getInstance()->fullPathForFilename(pFileName);
	//取得内容
	string strJson = FileUtils::getInstance()->getStringFromFile(fullPath);
	//获得doc
	rapidjson::Document doc;
	//解析内容
	doc.Parse<0>(strJson.c_str());
	for (int i = 0; i < doc.Size(); ++i)
	{
		sCardDt* pData = new sCardDt();
		rapidjson::Value& vValue = doc[i];
		pData->nID = vValue["id"].GetInt();
		pData->nNeedCoin = vValue["needCoin"].GetInt();
		rapidjson::Value& vPngs = vValue["pngs"];
		for (int j = 0; j < vPngs.Size(); ++j)
		{
			pData->vecPngs.push_back(vPngs[j].GetString());
		}
		pData->nWeaponID = vValue["weaponID"].GetInt();
		m_vecDatas.push_back(pData);
	}
}

sCardDt* CCardDtMgr::getDataByID(int nID)
{
	for (sCardDt* pData : m_vecDatas)
	{
		if (pData->nID == nID)
		{
			return pData;
		}
	}
	return nullptr;
}


//主题

CThemeDtMgr::CThemeDtMgr()
:m_nCurThemeIndex(0)
{
}

CThemeDtMgr::~CThemeDtMgr()
{
}

void CThemeDtMgr::loadFile(const char* pFileName)
{
	//获得全路径
	string fullPath = FileUtils::getInstance()->fullPathForFilename(pFileName);
	//取得内容
	string strJson = FileUtils::getInstance()->getStringFromFile(fullPath);
	//获得doc
	rapidjson::Document doc;
	//解析内容
	doc.Parse<0>(strJson.c_str());
	for (int i = 0; i < doc.Size(); ++i)
	{
		sThemeDt* pData = new sThemeDt();
		rapidjson::Value& vValue = doc[i];
		pData->nID = vValue["id"].GetInt();
		pData->bgPng = vValue["bgPng"].GetString();
		pData->bookMark = vValue["bookMark"].GetString();
		pData->themeTxt = vValue["themeTxt"].GetString();
		rapidjson::Value& vIDs = vValue["levelID"];
		for (int j = 0; j < vIDs.Size(); ++j)
		{
			pData->vecLevelIDs.push_back(vIDs[j].GetInt());
		}
		pData->cloudPng = vValue["cloudPng"].GetString();
		m_vecDatas.push_back(pData);
	}
}

sThemeDt* CThemeDtMgr::getDataByID(const int& nID)
{
	for (sThemeDt* pData : m_vecDatas)
	{
		if (pData->nID == nID)
		{
			return pData;
		}
	}
	return nullptr;
}


//关卡
CLevelDtMgr::CLevelDtMgr()
:m_nCurLevelIndex(0)
{
}

CLevelDtMgr::~CLevelDtMgr()
{
}

void CLevelDtMgr::loadFile(const char* pFileName)
{
	//获取全路径
	string fullPath = FileUtils::getInstance()->fullPathForFilename(pFileName);
	//取得内容
	string strJson = FileUtils::getInstance()->getStringFromFile(fullPath);
	//获得doc，解析内容
	rapidjson::Document doc;
	doc.Parse<0>(strJson.c_str());
	for (int i = 0; i < doc.Size(); ++i)
	{
		sLevelDt* pData = new sLevelDt();
		rapidjson::Value& vValue = doc[i];
		pData->nID = vValue["id"].GetInt();
		pData->mapName = vValue["mapName"].GetString();
		rapidjson::Value& vCardID = vValue["cardID"];
		for (int j = 0; j < vCardID.Size(); ++j)
		{
			pData->vecCardID.push_back(vCardID[j].GetInt());
		}
		pData->nWave = vValue["wave"].GetInt();
		pData->wavePng = vValue["wavePng"].GetString();
		pData->weaponPng = vValue["weaponPng"].GetString();
		pData->bgPng = vValue["bgPng"].GetString();
		pData->nCoin = vValue["coin"].GetInt();
		m_vecDatas.push_back(pData);
	}
}

sLevelDt* CLevelDtMgr::getDataByID(int nID)
{
	for (sLevelDt* pData : m_vecDatas)
	{
		if (pData->nID == nID)
		{
			return pData;
		}
	}
	return nullptr;
}


//资源
CResDtMgr::CResDtMgr()
{

}

CResDtMgr::~CResDtMgr()
{

}

void CResDtMgr::loadFile(const char* pFileName)
{
	//获取全路径
	string fullPath = FileUtils::getInstance()->fullPathForFilename(pFileName);
	//取得文件
	string strJson = FileUtils::getInstance()->getStringFromFile(fullPath);
	//获得document对象
	rapidjson::Document doc;
	//解析doc
	doc.Parse<0>(strJson.c_str());

	rapidjson::Value& vPngData = doc["png"];
	for (int i = 0; i < vPngData.Size(); ++i)
	{
		m_vecPngs.push_back(vPngData[i].GetString());
	}

	rapidjson::Value& vPlistData = doc["plist"];
	for (int i = 0; i < vPlistData.Size(); ++i)
	{
		m_vecPlists.push_back(vPlistData[i].GetString());
	}

	rapidjson::Value& vBgMusicDt = doc["backgroundMusic"];
	for (int i = 0; i < vBgMusicDt.Size(); ++i)
	{
		m_vecBgMusic.push_back(vBgMusicDt[i].GetString());
	}

	rapidjson::Value& vEffectDt = doc["effect"];
	for (int i = 0; i < vEffectDt.Size(); ++i)
	{
		m_vecEffect.push_back(vEffectDt[i].GetString());
	}
}



//武器

CWeaponDtMgr::CWeaponDtMgr()
{
}

CWeaponDtMgr::~CWeaponDtMgr()
{
}

void CWeaponDtMgr::loadFile(const char* pFileName)
{
	//获得全路径
	string fullPath = FileUtils::getInstance()->fullPathForFilename(pFileName);
	//取得内容
	string strJson = FileUtils::getInstance()->getStringFromFile(fullPath);
	//获得doc
	rapidjson::Document doc;
	//解析内容
	doc.Parse<0>(strJson.c_str());
	for (int i = 0; i < doc.Size(); ++i)
	{
		sWeaponDt* pData = new sWeaponDt();
		rapidjson::Value& vValue = doc[i];
		pData->nID = vValue["id"].GetInt();
		pData->nNextID = vValue["nextID"].GetInt();
		pData->nUpGradeCoin = vValue["upgradeCoin"].GetInt();

		rapidjson::Value& vUpGradePng = vValue["upgradePng"];
		for (int j = 0; j < vUpGradePng.Size();++j)
		{
			pData->vecUpGradePng.push_back(vUpGradePng[j].GetString());
		}
		pData->nSellCoin = vValue["sellCoin"].GetInt();
		pData->sellPng = vValue["sellPng"].GetString();
		pData->bgPng = vValue["bgPng"].GetString();
		pData->normalPng = vValue["normalPng"].GetString();
		
		rapidjson::Value& vFirePng = vValue["firePng"];
		for (int j = 0; j < vFirePng.Size(); ++j)
		{
			pData->vecFirePng.push_back(vFirePng[j].GetString());
		}
		pData->nBulletID = vValue["bulletID"].GetInt();
		pData->fFireInterval = vValue["fireInterval"].GetFloat();
		pData->nRange = vValue["range"].GetInt();
		pData->type = vValue["type"].GetString();
		pData->fireMusic = vValue["fireMusic"].GetString();
		m_vecDatas.push_back(pData);
	}
}

sWeaponDt* CWeaponDtMgr::getDataByID(const int& nID)
{
	for (sWeaponDt* pData : m_vecDatas)
	{
		if (pData->nID == nID)
		{
			return pData;
		}
	}
	return nullptr;
}


//物品
CObjectDtMgr::CObjectDtMgr()
{
}

CObjectDtMgr::~CObjectDtMgr()
{
}

void CObjectDtMgr::loadFile(const char* pFileName)
{
	//获取全路径
	string fullPath = FileUtils::getInstance()->fullPathForFilename(pFileName);
	//取得内容
	string strJson = FileUtils::getInstance()->getStringFromFile(fullPath);
	//获得doc，解析内容
	rapidjson::Document doc;
	doc.Parse<0>(strJson.c_str());
	for (int i = 0; i < doc.Size(); ++i)
	{
		sObjectDt* pData = new sObjectDt();
		rapidjson::Value& vValue = doc[i];
		pData->strID = vValue["strID"].GetString();
		pData->strName = vValue["name"].GetString();
		pData->strPng = vValue["png"].GetString();
		pData->nHp = vValue["hp"].GetInt();
		pData->nCoin = vValue["coin"].GetInt();
		pData->strCoinPng = vValue["coinPng"].GetString();
		m_vecDatas.push_back(pData);
	}
}

sObjectDt* CObjectDtMgr::getDataByID(const string& strID)
{
	for (sObjectDt* pData : m_vecDatas)
	{
		if (0 == strcmp(strID.c_str(),pData->strID.c_str()))
		{
			return pData;
		}
	}
	return nullptr;
}


//子弹数据

CBulletDtMgr::CBulletDtMgr()
{
}

CBulletDtMgr::~CBulletDtMgr()
{
}

void CBulletDtMgr::loadFile(const char* pFileName)
{
	//获得全路径
	string fullPath = FileUtils::getInstance()->fullPathForFilename(pFileName);
	//取得内容
	string strJson = FileUtils::getInstance()->getStringFromFile(fullPath);
	//获得doc
	rapidjson::Document doc;
	//解析内容
	doc.Parse<0>(strJson.c_str());
	for (int i = 0; i < doc.Size(); ++i)
	{
		sBulletDt* pData = new sBulletDt();
		rapidjson::Value& vValue = doc[i];
		pData->nID = vValue["id"].GetInt();
		pData->strType = vValue["type"].GetString();
		pData->nAck = vValue["ack"].GetInt();
		pData->fSpeed = vValue["speed"].GetFloat();

		rapidjson::Value& vBombPng = vValue["bombPng"];
		for (int j = 0; j < vBombPng.Size(); ++j)
		{
			pData->vec_bombPng.push_back(vBombPng[j].GetString());
		}

		rapidjson::Value& vAnimatePng = vValue["animatePng"];
		for (int j = 0; j < vAnimatePng.Size(); ++j)
		{
			pData->vec_animatePng.push_back(vAnimatePng[j].GetString());
		}

		rapidjson::Value& vHitPng = vValue["hitPng"];
		for (int j = 0; j < vHitPng.Size(); ++j)
		{
			pData->vec_hitPng.push_back(vHitPng[j].GetString());
		}
		
		pData->fReduceSpeedRate = vValue["reduceSpeed"].GetFloat();
		pData->fContinueTime = vValue["continueTime"].GetFloat();
		m_vecDatas.push_back(pData);
	}
}

sBulletDt* CBulletDtMgr::getDataByID(const int& nID)
{
	for (sBulletDt* pData : m_vecDatas)
	{
		if (nID == pData->nID)
		{
			return pData;
		}
	}
	return nullptr;
}


//波数数据管理者

CWaveDtMgr::CWaveDtMgr()
{
}

CWaveDtMgr::~CWaveDtMgr()
{
}

void CWaveDtMgr::loadFile(const char* pFileName)
{
	//获得全路径
	string fullPath = FileUtils::getInstance()->fullPathForFilename(pFileName);
	//取得内容
	string strJson = FileUtils::getInstance()->getStringFromFile(fullPath);
	//获得doc
	rapidjson::Document doc;
	//解析内容
	doc.Parse<0>(strJson.c_str());
	for (int i = 0; i < doc.Size();++i)
	{
		sWaveData* pData = new sWaveData();
		rapidjson::Value& vValue = doc[i];
		pData->nID = vValue["id"].GetInt();
		pData->nLevelID = vValue["levelID"].GetInt();
		pData->nWave = vValue["wave"].GetInt();

		rapidjson::Value& vWaveData= vValue["waveData"];
		for (int j = 0; j < vWaveData.Size(); ++j)
		{
			sInWaveData* pInWaveData = new sInWaveData();
			pInWaveData->nWaveNum = vWaveData[j]["waveNum"].GetInt();
			pInWaveData->nEnemyNum = vWaveData[j]["enemyNum"].GetInt();

			rapidjson::Value& vEnemyData = vWaveData[j]["enemyData"];
			for (int k = 0; k < vEnemyData.Size();++k)
			{
				sInEnemyData* pInEnemyData = new sInEnemyData();
				pInEnemyData->nEnemyID = vEnemyData[k]["enemyID"].GetInt();
				pInEnemyData->nAddHp = vEnemyData[k]["addHp"].GetInt();
				pInEnemyData->fAddSpeed = vEnemyData[k]["addSpeed"].GetFloat();

				pInWaveData->vec_enemyData.push_back(pInEnemyData);
			}
			pData->vec_inWaveData.push_back(pInWaveData);
		}
		m_vecDatas.push_back(pData);
	}
}

sWaveData* CWaveDtMgr::getDataByLevelID(const int& nLevelID)
{
	for (sWaveData* pData : m_vecDatas)
	{
		if (nLevelID == pData->nLevelID)
		{
			return pData;
		}
	}
	return nullptr;
}
