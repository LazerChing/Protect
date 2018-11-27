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
	 * @brief ͨ��UUID���ҵ���
	 * @param nUUID
	 * @date 2018/10/06 16:13
	 * @return 
	 */
	CEnemy* findEnemyByUUID(int nUUID);

	/**
	 * @brief ֪ͨ��Ϸ��ʼ��
	 * @param 
	 * @date 2018/10/07 15:36
	 * @return 
	 */
	void gameStart();
private:
	/**
	 * @brief ���ɵ���  
	 * @param 
	 * @date 2018/09/20 15:20
	 * @return 
	 */
	void generateEnemy(float fDelta);
private:
	CC_SYNTHESIZE(int, m_nCurWaveNum, CurWaveNum); //��ǰ����
	int m_nCurEnemyUUID; //��ǰ���ɵ���uuid

	int m_nCurEnemyNum; //��ǰ��������
	sWaveData* m_pWaveData; //��������

	bool m_bIsPlayedFinalWaveAnimate; //�Ƿ񲥷������һ������
	bool m_bIsRunNextWave; //�Ƿ�ִ���ˡ���һ�����߼�
};

#endif