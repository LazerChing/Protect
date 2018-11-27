#ifndef __BULLET_H__
#define __BULLET_H__
#include "cocos2d.h"
#include "StructData.h"
#include "Weapon.h"
USING_NS_CC;
class CBullet :public Node
{
public:
	CBullet();
	~CBullet();
	bool initWithData(sBulletDt* pData);
	static CBullet* createWithData(sBulletDt* pData);
	virtual void update(float fDelta);

	/**
	 * @brief ȡ������ͼƬ
	 * @param
	 * @date 2018/10/05 23:19
	 * @return
	 */
	vector<string> getHitPng()
	{
		return m_vecHitPng;
	}

	/**
	 * @brief ȡ�ñ�ըͼƬ
	 * @param
	 * @date 2018/10/05 18:46
	 * @return
	 */
	vector<string> getBombPngs()
	{
		return m_vecBombPng;
	}

	/**
	 * @brief ���ű�ը����(���Ƴ��ӵ���
	 * @param
	 * @date 2018/10/05 17:03
	 * @return
	 */
	void playBombAnimate();

	/**
	 * @brief �õ������Χ��
	 * @param
	 * @date 2018/10/04 17:36
	 * @return
	 */
	Rect getSpriteBox()
	{
		return m_pBullet->getBoundingBox();
	}

	/**
	* @brief �ӵ��ƶ�
	* @param
	* @date 2018/10/04 17:34
	* @return
	*/
	void move(Vec2 endPos);

	/**
	 * @brief �Ը����ĵ�������˺�
	 * @param fRange ������˺��ķ�Χ
	 * @date 2018/10/07 21:49
	 * @return
	 */
	void damageAroundEnemy(float fRange);

	/**
	 * @brief ��ʼ��ʱ����˺������Ball�������ӵ����ӵ���һ���ߣ���ÿ��һ��ʱ������˺���
	 * @param pWeapon �ӵ����ĸ������������
	 * @param pAimedNode Ŀ��
	 * @param nUUID Ŀ��UUID����Ʒ����ˣ�
	 * @date 2018/10/08 16:31
	 * @return
	 */
	void startTimingDamage(CWeapon* pWeapon, Node* pAimedNode, int nUUID);

	/**
	 * @brief ���þ����ê��
	 * @param ê��
	 * @date 2018/10/08 16:04
	 * @return
	 */
	void setSpriteAnchorPoint(Vec2 anchorPoint)
	{
		m_pBullet->setAnchorPoint(anchorPoint);
	}

	/**
	 * @brief ȡ���ӵ��Ĺ����ߣ�������
	 * @param
	 * @date 2018/10/08 20:39
	 * @return
	 */
	CWeapon* getOwner();

	/**
	 * @brief �õ��ӵ���Ŀ��
	 * @param
	 * @date 2018/10/08 20:40
	 * @return
	 */
	sSignedNodeDt* getAimedNode()
	{
		return &m_sAimedNode;
	}

	/**
	 * @brief �����ǰĿ��
	 * @param
	 * @date 2018/10/09 13:45
	 * @return
	 */
	void clearAim()
	{
		m_sAimedNode = sSignedNodeDt();
		m_nWeaponUUID = 0;
	}

private:
	/**
	 * @brief ÿ��һ��ʱ������˺��ص����������Ball�������ӵ���
	 * @param 
	 * @date 2018/10/08 14:52
	 * @return 
	 */
	void timingDamageCallback(float fDelta);
private:
	CC_SYNTHESIZE(int, m_nID, ID);
	CC_SYNTHESIZE(string, m_strType, Type); //�ӵ�����
	CC_SYNTHESIZE(int, m_nAck, Ack);
	CC_SYNTHESIZE(float, m_fSpeed, Speed); //�ӵ��ƶ��ٶ�
	CC_SYNTHESIZE(float, m_fReduceSpeedRate, ReduceSpeedRate); //���ٱ���
	CC_SYNTHESIZE(float, m_fContinueTime, ContinueTime);//���ٳ���ʱ��
	CC_SYNTHESIZE(int, m_nUUID, UUID); //�ӵ���Ψһ��ʶ
	CC_SYNTHESIZE(string, m_strAim, Aim); //�ӵ�����ʱ���Ŀ��
	vector<string> m_vecBombPng; //��ըͼƬ
	vector<string> m_vecAnimatePng; //�ӵ�����ͼƬ
	vector<string> m_vecHitPng; //�������͵��ӵ����к��ͼƬ

	Sprite* m_pBullet; //�ӵ�����
	Animate* m_pBulAnimate; //�ӵ�����
	Animate* m_pBombAnimate;; //�ӵ���ը����
	float m_fRotation; //��ת�Ƕ�

	//���Ball�������ӵ�
	int m_nWeaponUUID; //�ӵ��Ǵ��ĸ���������� �� ����UUID
	sSignedNodeDt m_sAimedNode;//�ӵ���Ŀ�����ݣ����˻���Ʒ)
};

#endif