#ifndef __STRUCTDATA_H__
#define __STRUCTDATA_H__
#include "cocos2d.h"
#include "json/document.h"
#include "json/rapidjson.h"
#include <vector>
#include <string>
USING_NS_CC;
using namespace std;

//����
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


//��Ƭ
struct sCardDt
{
	sCardDt()
	{
		nID = 0;
		vecPngs.clear();
		nWeaponID = 0;
	}
	int nID;
	int nNeedCoin;//������
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


//����
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
	CC_SYNTHESIZE(int, m_nCurThemeIndex, CurThemeIndex); //��ǰ����
};


//�ؿ�
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
	vector<int> vecCardID; //��ǰ��ͼ�Ŀ�Ƭid
	string weaponPng; //��ǰ��ͼ����ͼƬ ����ѡ�س����õ�
	int nWave; //����
	string wavePng; //��ǰ��ͼ�Ĺ��ﲨ��ͼƬ
	string bgPng; //��ǰ��ͼ����ͼ
	int nCoin; //��ҳ�ʼֵ
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
	CC_SYNTHESIZE(int, m_nCurLevelIndex, CurLevelIndex); //��ǰ�ؿ�
};


//��Դ
struct sResData
{
	vector<string> vec_Pngs;
	vector<string> vec_Plists;
	vector<string> vec_bgMusic; //��������
	vector<string> vec_effect;  //��Ч
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


//����������
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
	int nNextID;      //��һ�ȼ�����ID
	int nUpGradeCoin; //����������
	vector<string> vecUpGradePng; //���������Ҳ��� �� �㹻 ��ͼƬ
	int nSellCoin;	  //���ۿɵý��
	string sellPng;   //��������ͼ
	string bgPng;	  //����ͼ
	string normalPng; //������̬��ͼ
	vector<string> vecFirePng; //���𶯻�ͼ
	int nBulletID; //�ӵ�id
	float fFireInterval; //������
	int nRange; //������Χ
	string type; //��������
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


//��Ʒ
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


//�ӵ�����
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
	float fSpeed; //�ƶ��ٶ�
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


//���sInWaveDataʹ�õ�
struct sInEnemyData
{
	sInEnemyData()
	{
		nEnemyID = 0;
		nAddHp = 0;
		fAddSpeed = 0.f;
	}
	int nEnemyID; //����ID
	int nAddHp; //���˶���ӵ�Ѫ��
	float fAddSpeed; //���˶���ӵ��ٶ�
};

//���sWaveDataʹ�õ�
struct sInWaveData
{
	sInWaveData()
	{
		nWaveNum = 0;
		nEnemyNum = 0;
	}
	int nWaveNum; //�ڼ���
	int nEnemyNum; //��������
	vector<sInEnemyData*> vec_enemyData;
};

//��������
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