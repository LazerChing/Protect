#ifndef __ENEMY_H__
#define __ENEMY_H__
#include "cocos2d.h"
#include "StructData.h"
#include <string>
#include <ctime>
class CBullet;
USING_NS_CC;
enum 
{
	E_TAG_MOVE = 1
};
class CEnemy :public Sprite
{
public:
	CEnemy();
	virtual ~CEnemy();
	static CEnemy* createWithData(sEnemyDt* pData);
	virtual bool initWithData(sEnemyDt* pData);
	virtual void update(float fDelta);

	/**
	 * @brief ���˳�������  
	 * @param 
	 * @date 2018/10/08 14:58
	 * @return 
	 */
	void damageContinue();

	/**
	 * @brief �����ƶ�  
	 * @param 
	 * @date 2018/09/20 15:41
	 * @return 
	 */
	void move();

	/**
	 * @brief �������˽ӿ�
	 * @param pBullet�ӵ�
	 * @date 2018/10/05 16:44
	 * @return ��������true
	 */
	bool damage(CBullet* pBullet);

	/**
	 * @brief �жϸ��ӵ��Ƿ񹥻���
	 * @param pBullet �ӵ�
	 * @date 2018/10/05 17:39
	 * @return ����������true
	 */
	bool isAttacked(CBullet* pBullet);

	/**
	 * @brief �ƶ�����
	 * @param fRate ����
	 * @param fTime �������ʱ��
	 * @param hitPng ����ͼƬ
	 * @date 2018/10/05 20:32
	 * @return 
	 */
	void moveHindered(float fRate, float fTime, vector<string>& hitPng);

	/**
	* @brief ��ȡ�Ƿ񱻱��
	* @param
	* @date 2018/10/06 12:14
	* @return
	*/
	bool getIsSigned()
	{
		return m_bIsSigned;
	}

	/**
	* @brief ���ñ��״̬
	* @param
	* @date 2018/10/06 12:14
	* @return
	*/
	void setIsSigned(bool bIsSign);

	/**
	 * @brief ���Լ��Ӹ��ڵ��������ִ��һЩ������Ҫ�ı�Ĳ���
	 * @param 
	 * @date 2018/10/06 14:00
	 * @return 
	 */
	void removeSelf();

	/**
	 * @brief ����Ѫ��(���ɵ��˵�ʱ����ã�
	 * @param nHp ���ӵ�Ѫ��
	 * @date 2018/10/06 21:10
	 * @return 
	 */
	void addHp(int nHp)
	{
		m_nMaxHp += nHp;
		m_nHp += nHp;
	}

	/**
	 * @brief ��������(���ɵ��˵�ʱ�����)
	 * @param fSpeed ���ӵ�����
	 * @date 2018/10/06 21:10
	 * @return 
	 */
	void addSpeed(float fSpeed)
	{
		m_fNormalSpeed += fSpeed;
	}
private:

	/**
	 * @brief ��������
	 * @param 
	 * @date 2018/09/20 17:59
	 * @return 
	 */
	void initAnimate();
private:
	CC_SYNTHESIZE(int, m_nID, ID);
	CC_SYNTHESIZE(int, m_nHp, Hp); //��ǰѪ��
	CC_SYNTHESIZE(int, m_nMaxHp, MaxHp); //��Ѫ��
	CC_SYNTHESIZE(float, m_fNormalSpeed, Speed); //�����ƶ��ٶ�
	CC_SYNTHESIZE(int, m_nCoin, Coin);
	CC_SYNTHESIZE(string, m_strCoin, CoinPng);
	CC_SYNTHESIZE(int, m_nUUID, UUID); //���˵�Ψһ��ʶ
	vector<string> m_vecPngs; //ͼƬ
	vector<Vec2> m_vecPathPos; //�ƶ��յ㣨·����
	int m_nPosIndex; //��ǰ�յ�λ������
	Animate* m_pAnimate; //���߶���
	Sprite* m_pShowSprite; //Ϊ�˲��ų���ʱ��Ķ���

	vector<int> m_vecHitBulUUID; //�������ײ�����ӵ�UUID
	Sprite* m_pHpBg; //Ѫ������
	ProgressTimer* m_pHpBar;//Ѫ��
	float m_fCurSpeedRate;

	clock_t m_nBeginTime; //������ʱ�Ƿ���ʾѪ����

	bool m_bIsSigned; //�����Ƿ񱻱��
	Sprite* m_pSignSprite; //��ʾ��ǵľ���
};

#endif