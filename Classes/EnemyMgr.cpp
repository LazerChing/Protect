#include "EnemyMgr.h"
#include "DataMgr.h"
#include "GameScene.h"
#include <ctime>
CEnmeyMgr::CEnmeyMgr()
{
	m_nCurEnemyUUID = 0;
	m_nCurEnemyNum = 0;
	m_nCurWaveNum = 1;
	m_pWaveData = nullptr;
	m_bIsPlayedFinalWaveAnimate = false;
	m_bIsRunNextWave = false;
}

CEnmeyMgr::~CEnmeyMgr()
{
}

bool CEnmeyMgr::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//得到当前关卡敌人波数数据
	m_pWaveData = CDataMgr::getInstance()->getCurLevelWaveDt();	
	
	return true;
}

void CEnmeyMgr::generateEnemy(float fDelta)
{	
	//判断当前波数小于等于总波数的时候生成敌人
	if (m_nCurWaveNum <= m_pWaveData->nWave)
	{
		//当前生成的敌人数量小于当前波的总敌人数量
		if (m_nCurEnemyNum < m_pWaveData->vec_inWaveData[m_nCurWaveNum - 1]->nEnemyNum)
		{
			//生成敌人
			//获得该波数据里面的随机一个敌人数据
			int nEnemySize = m_pWaveData->vec_inWaveData[m_nCurWaveNum - 1]->vec_enemyData.size();
			srand(clock());
			int nIndex = rand() % nEnemySize;
			//得到敌人buff数据
			sInEnemyData* pInEnemyData = m_pWaveData->vec_inWaveData[m_nCurWaveNum - 1]->vec_enemyData[nIndex];

			//敌人id
			int nEnemyID = pInEnemyData->nEnemyID;
			//获得敌人原始数据
			sEnemyDt* pData = CDataMgr::getInstance()->getEnemyMgr()->getDataByID(nEnemyID);
			CEnemy* pEnemy = CEnemy::createWithData(pData);
			pEnemy->setUUID(++m_nCurEnemyUUID);
			pEnemy->addHp(pInEnemyData->nAddHp);
			pEnemy->addSpeed(pInEnemyData->fAddSpeed);

			this->addChild(pEnemy);
			m_nCurEnemyNum++;
		}
		else
		{
			if (!m_bIsRunNextWave) //如果还没执行这段逻辑
			{
				//如果所有的敌人都没了
				if (0 == this->getChildrenCount())
				{
					m_bIsRunNextWave = true;
					//判断是不是最后一波 不是最后一波
					if (m_nCurWaveNum != m_pWaveData->nWave)
					{
						//开启计时器 3秒后进入下一波
						this->scheduleOnce([=](float fDelta){
							//进入下一波
							m_nCurWaveNum++;
							m_nCurEnemyNum = 0;
							if (m_nCurWaveNum == m_pWaveData->nWave)
							{
								if (!m_bIsPlayedFinalWaveAnimate)
								{
									//播放最后一波特效
									CGameScene::getInstance()->getAnimateMgr()->playFinalWaveAnimate();
									m_bIsPlayedFinalWaveAnimate = true;
								}
							}
							//更新波数显示
							CGameScene::getInstance()->updataWaveView(m_nCurWaveNum);
							m_bIsRunNextWave = false;
						}, 4.f, "nextWave");
					}
					else
					{
						CGameScene::getInstance()->gameWin();
					}
				}
			}					
		}
	}
}

CEnemy* CEnmeyMgr::findEnemyByUUID(int nUUID)
{
	Vector<Node*> VecNodes = this->getChildren();
	for (Node* pNode : VecNodes)
	{
		CEnemy* pEnemy = (CEnemy*)pNode;
		if (pEnemy->getUUID() == nUUID)
		{
			return pEnemy;
		}
	}
	return nullptr;
}

void CEnmeyMgr::gameStart()
{
	this->schedule(CC_CALLBACK_1(CEnmeyMgr::generateEnemy, this), 1.f, "generateEnemy");
}

