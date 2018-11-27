#ifndef __RADISH_H__
#define __RADISH_H__
#include "cocos2d.h"
USING_NS_CC;
using namespace std;
class CRadish :public Node
{
public:
	CRadish();
	virtual ~CRadish();
	virtual bool init();
	CREATE_FUNC(CRadish);

	/**
	 * @brief ���˽ӿ�  
	 * @param 
	 * @date 2018/09/29 17:26
	 * @return true ��ʾ����
	 */
	bool damage();

	/**
	 * @brief ���Ŷ�������  
	 * @param 
	 * @date 2018/09/29 20:58
	 * @return 
	 */
	void playShakeAnimate();

	/**
	 * @brief �õ��ܲ��������������ϵ�°�Χ��  
	 * @param 
	 * @date 2018/09/30 08:58
	 * @return ��������ϵ�µ��ܲ���Χ�� 
	 */
	Rect getSpriteWorldBox();
private:
	CC_SYNTHESIZE(int, m_nCurLife, CurLife); //��ǰ����
	Sprite* m_pRadishSprite; //�ܲ�
	string m_strNormalPng;  //����ͼ
	Animate* m_pShakeAnimate; //��������
	Sprite* m_pLifeSprite; //����ͼƬ
};

#endif