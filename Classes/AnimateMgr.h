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
	 * @brief ������ͷ�����Ŷ���
	 * @param pos ����
	 * @param fRotate ��ת�Ƕ�
	 * @date 2018/10/07 16:17
	 * @return 
	 */
	void createArrow(Vec2 pos, float fRotate);

	/**
	 * @brief ͨ�����鲥�Ŷ���
	 * @param 
	 * @date 2018/10/05 19:14
	 * @return 
	 */
	void playAnimateByVecPng(Vec2 pos, vector<string>& vecPngs);

	/**
	 * @brief ���ŵ��˱�ը����
	 * @param pos ����λ��
	 * @param strCoin ���ӽ��ͼƬ
	 * @date 2018/10/05 19:19
	 * @return 
	 */
	void playEnemyBombAnimate(Vec2 pos, string strCoin);

	/**
	 * @brief �������һ����Ч
	 * @param 
	 * @date 2018/10/06 21:18
	 * @return 
	 */
	void playFinalWaveAnimate();
private:
	Animate* m_pEnemyBombAnimate;
};

#endif