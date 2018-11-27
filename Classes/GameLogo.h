#ifndef __GAMELOGO_H__
#define __GAMELOGO_H__
#include "cocos2d.h"
#include <vector>
USING_NS_CC;
using namespace std;
class CGameLogo:public Scene
{
public:
	CGameLogo();
	virtual ~CGameLogo();
	static Scene* createScene();
	virtual bool init();
	virtual void update(float fDelta);
	CREATE_FUNC(CGameLogo);
private:
	Sprite* m_pBarBg;//����������ͼ
	ProgressTimer* m_pProgressTimer;//������
	int m_nTotalNum; //����Դ��
	int m_nCurIndex; //��ǰ������Դ����
	int m_nCurNum; //��ǰ�Ѷ�ȡ��
	vector<string> m_vecPngs;
	vector<string> m_vecPlists;
	vector<string> m_vecBgMusic;
	vector<string> m_vecEffect;
};

#endif