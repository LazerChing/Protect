#ifndef __STRUCTDATA_H__
#define __STRUCTDATA_H__
#include "cocos2d.h"
#include "json/document.h"
#include "json/rapidjson.h"
#include <vector>
#include <string>
USING_NS_CC;
using namespace std;

//敌人
struct sEnemyDt
{
	sEnemyDt()
	{
		nID = 0;
		nHp = 0;
		fSpeed = 0.0f;
		nCoin = 0;
		vecPngs.clear();
	}
	int nID;
	int nHp;
	float fSpeed;
	vector<string> vecPngs;
	int nCoin;
	string strCoin;
};

class CEnemyDtMgr
{
public:
	CEnemyDtMgr();
	~CEnemyDtMgr();
	void loadFile(const char* pFileName);
	sEnemyDt* getDataByID(int nID);
private:
	vector<sEnemyDt*> m_vecDatas;
};


//卡片
struct sCardDt
{
	sCardDt()
	{
		nID = 0;
		vecPngs.clear();
		nWeaponID = 0;
	}
	int nID;
	int nNeedCoin;//所需金币
	vector<string> vecPngs;
	int nWeaponID;
};

class CCardDtMgr
{
public:
	CCardDtMgr();
	~CCardDtMgr();
	void loadFile(const char* pFileName);
	sCardDt* getDataByID(int nID);
private:
	vector<sCardDt*> m_vecDatas;
};


//主题
struct sThemeDt
{
	sThemeDt()
	{
		nID = 0;
		vecLevelIDs.resize(0);
	}
	int nID;
	string bgPng;
	string bookMark;
	string themeTxt;
	string cloudPng;
	vector<int> vecLevelIDs;
};

class CThemeDtMgr
{
public:
	CThemeDtMgr();
	~CThemeDtMgr();
	void loadFile(const char* pFileName);
	sThemeDt* getDataByID(const int& nID);
	const vector<sThemeDt*>& getVecDatas()
	{
		return m_vecDatas;
	}
	const sThemeDt* getCurThemeData() const
	{
		return m_vecDatas[m_nCurThemeIndex];
	}
private:
	vector<sThemeDt*> m_vecDatas;
	CC_SYNTHESIZE(int, m_nCurThemeIndex, CurThemeIndex); //当前主题
};


//关卡
struct sLevelDt
{
	sLevelDt()
	{
		nID = 0;
		mapName = "";
		nCoin = 0;
	}
	int nID;
	string mapName;
	vector<int> vecCardID; //当前地图的卡片id
	string weaponPng; //当前地图武器图片 用来选关场景用的
	int nWave; //波数
	string wavePng; //当前地图的怪物波数图片
	string bgPng; //当前地图缩略图
	int nCoin; //金币初始值
};

class CLevelDtMgr
{
public:
	CLevelDtMgr();
	~CLevelDtMgr();
	void loadFile(const char* pFileName);
	sLevelDt* getDataByID(int nID);		

	const vector<sLevelDt*>& getVecDatas() const
	{
		return m_vecDatas;
	}
	void addCurLevelIndex()
	{
		m_nCurLevelIndex++;
	}
private:
	vector<sLevelDt*> m_vecDatas;
	CC_SYNTHESIZE(int, m_nCurLevelIndex, CurLevelIndex); //当前关卡
};


//资源
struct sResData
{
	vector<string> vec_Pngs;
	vector<string> vec_Plists;
	vector<string> vec_bgMusic; //背景音乐
	vector<string> vec_effect;  //音效
};

class CResDtMgr
{
public:
	CResDtMgr();
	~CResDtMgr();
	void loadFile(const char* pFileName);
	const vector<string>& getVecPngs() const
	{
		return m_vecPngs;
	}
	const vector<string>& getVecPlists() const
	{
		return m_vecPlists;
	}
	const vector<string>& getVecBgMusic() const
	{
		return m_vecBgMusic;
	}
	const vector<string>& getVecEffect() const
	{
		return m_vecEffect;
	}
private:
	vector<string> m_vecPngs;
	vector<string> m_vecPlists;
	vector<string> m_vecBgMusic;
	vector<string> m_vecEffect;
};


//武器（塔）
struct sWeaponDt
{
	sWeaponDt()
	{
		nID = 0;
		nNextID = 0;
		nUpGradeCoin = 0;
		nSellCoin = 0;
		nBulletID = 0;
		nRange = 0;
	}
	int nID;
	int nNextID;      //下一等级武器ID
	int nUpGradeCoin; //升级所需金币
	vector<string> vecUpGradePng; //升级所需金币不足 或 足够 的图片
	int nSellCoin;	  //销售可得金币
	string sellPng;   //销售武器图
	string bgPng;	  //背景图
	string normalPng; //正常形态的图
	vector<string> vecFirePng; //开火动画图
	int nBulletID; //子弹id
	float fFireInterval; //发射间隔
	int nRange; //攻击范围
	string type; //武器类型
	string fireMusic;
};


class CWeaponDtMgr
{
public:
	CWeaponDtMgr();
	~CWeaponDtMgr();
	void loadFile(const char* pFileName);
	sWeaponDt* getDataByID(const int& nID);
private:
	vector<sWeaponDt*> m_vecDatas;
};


//物品
struct sObjectDt
{
	sObjectDt()
	{
		nHp = 0;
		nCoin = 0;
	}
	string strID;
	string strName;
	string strPng;
	int nHp;
	int nCoin;
	string strCoinPng;
};

class CObjectDtMgr
{
public:
	CObjectDtMgr();
	~CObjectDtMgr();
	void loadFile(const char* pFileName);
	sObjectDt* getDataByID(const string& strID);
private:
	vector<sObjectDt*> m_vecDatas;
};


//子弹数据
struct sBulletDt
{
	sBulletDt()
	{
		nID = 0;
		nAck = 0;
		fReduceSpeedRate = 0.0f;
		fContinueTime = 0.0f;
	}
	int nID;
	string strType;
	int nAck;
	float fSpeed; //移动速度
	vector<string> vec_bombPng;
	vector<string> vec_animatePng;
	vector<string> vec_hitPng;
	float fReduceSpeedRate;
	float fContinueTime;
};

class CBulletDtMgr
{
public:
	CBulletDtMgr();
	~CBulletDtMgr();
	void loadFile(const char* pFileName);
	sBulletDt* getDataByID(const int& nID);
private:
	vector<sBulletDt*> m_vecDatas;
};


//结合sInWaveData使用的
struct sInEnemyData
{
	sInEnemyData()
	{
		nEnemyID = 0;
		nAddHp = 0;
		fAddSpeed = 0.f;
	}
	int nEnemyID; //敌人ID
	int nAddHp; //敌人额外加的血量
	float fAddSpeed; //敌人额外加的速度
};

//结合sWaveData使用的
struct sInWaveData
{
	sInWaveData()
	{
		nWaveNum = 0;
		nEnemyNum = 0;
	}
	int nWaveNum; //第几波
	int nEnemyNum; //敌人数量
	vector<sInEnemyData*> vec_enemyData;
};

//波数数据
struct sWaveData
{
	sWaveData()
	{
		nID = 0;
		nLevelID = 0;
		nWave = 0;
	}
	int nID;
	int nLevelID;
	int nWave;
	vector<sInWaveData*> vec_inWaveData;
};


class CWaveDtMgr
{
public:
	CWaveDtMgr();
	~CWaveDtMgr();
	void loadFile(const char* pFileName);
	sWaveData* getDataByLevelID(const int& nLevelID);
private:
	vector<sWaveData*> m_vecDatas;
};

#endif