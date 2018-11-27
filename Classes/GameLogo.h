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
	Sprite* m_pBarBg;//进度条背景图
	ProgressTimer* m_pProgressTimer;//进度条
	int m_nTotalNum; //总资源数
	int m_nCurIndex; //当前加载资源索引
	int m_nCurNum; //当前已读取数
	vector<string> m_vecPngs;
	vector<string> m_vecPlists;
	vector<string> m_vecBgMusic;
	vector<string> m_vecEffect;
};

#endif