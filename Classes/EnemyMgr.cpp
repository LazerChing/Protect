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

	//�õ���ǰ�ؿ����˲�������
	m_pWaveData = CDataMgr::getInstance()->getCurLevelWaveDt();	
	
	return true;
}

void CEnmeyMgr::generateEnemy(float fDelta)
{	
	//�жϵ�ǰ����С�ڵ����ܲ�����ʱ�����ɵ���
	if (m_nCurWaveNum <= m_pWaveData->nWave)
	{
		//��ǰ���ɵĵ�������С�ڵ�ǰ�����ܵ�������
		if (m_nCurEnemyNum < m_pWaveData->vec_inWaveData[m_nCurWaveNum - 1]->nEnemyNum)
		{
			//���ɵ���
			//��øò�������������һ����������
			int nEnemySize = m_pWaveData->vec_inWaveData[m_nCurWaveNum - 1]->vec_enemyData.size();
			srand(clock());
			int nIndex = rand() % nEnemySize;
			//�õ�����buff����
			sInEnemyData* pInEnemyData = m_pWaveData->vec_inWaveData[m_nCurWaveNum - 1]->vec_enemyData[nIndex];

			//����id
			int nEnemyID = pInEnemyData->nEnemyID;
			//��õ���ԭʼ����
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
			if (!m_bIsRunNextWave) //�����ûִ������߼�
			{
				//������еĵ��˶�û��
				if (0 == this->getChildrenCount())
				{
					m_bIsRunNextWave = true;
					//�ж��ǲ������һ�� �������һ��
					if (m_nCurWaveNum != m_pWaveData->nWave)
					{
						//������ʱ�� 3��������һ��
						this->scheduleOnce([=](float fDelta){
							//������һ��
							m_nCurWaveNum++;
							m_nCurEnemyNum = 0;
							if (m_nCurWaveNum == m_pWaveData->nWave)
							{
								if (!m_bIsPlayedFinalWaveAnimate)
								{
									//�������һ����Ч
									CGameScene::getInstance()->getAnimateMgr()->playFinalWaveAnimate();
									m_bIsPlayedFinalWaveAnimate = true;
								}
							}
							//���²�����ʾ
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

