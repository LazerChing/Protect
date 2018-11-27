#include "GameLogo.h"
#include "DataMgr.h"
#include "GameMenu.h"

CGameLogo::CGameLogo()
:m_nTotalNum(0), m_nCurIndex(0), m_nCurNum(0)
{
}

CGameLogo::~CGameLogo()
{
}

Scene* CGameLogo::createScene()
{
	return CGameLogo::create();
}

bool CGameLogo::init()
{
	if (!Scene::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//���ñ���
	Sprite* pBg = Sprite::create("Default-Landscape~ipad.png");
	this->addChild(pBg);
	pBg->setPosition(visibleSize.width / 2, visibleSize.height / 2);


	//���ý���������
	m_pBarBg = Sprite::create("ui/ui_ProgressBar_01.png");
	this->addChild(m_pBarBg);
	m_pBarBg->setPosition(visibleSize.width / 2, 200);

	//���ý�����
	Sprite* pTimer = Sprite::create("ui/ui_ProgressBar_02.png");
	m_pProgressTimer = ProgressTimer::create(pTimer);
	this->addChild(m_pProgressTimer);
	m_pProgressTimer->setMidpoint(Vec2(0, 0));
	m_pProgressTimer->setPercentage(0);
	m_pProgressTimer->setType(ProgressTimer::Type::BAR);
	m_pProgressTimer->setBarChangeRate(Vec2(1, 0));
	m_pProgressTimer->setPosition(visibleSize.width / 2, 200);


	//��ȡ��Դ����
	m_vecPngs = CDataMgr::getInstance()->getResDtMgr()->getVecPngs();
	m_vecPlists = CDataMgr::getInstance()->getResDtMgr()->getVecPlists();
	m_vecBgMusic = CDataMgr::getInstance()->getResDtMgr()->getVecBgMusic();
	m_vecEffect = CDataMgr::getInstance()->getResDtMgr()->getVecEffect();

	m_nTotalNum = m_vecPngs.size() + m_vecPlists.size() + m_vecBgMusic.size() + m_vecEffect.size();

	//����ÿ֡����
	this->scheduleUpdate();

	return true;
}

void CGameLogo::update(float fDelta)
{
	if (m_nCurNum < m_nTotalNum)
	{
		if (m_nCurNum < m_vecPngs.size())
		{
			//ȡ��������
			TextureCache* pCache = Director::getInstance()->getTextureCache();
			pCache->addImage(m_vecPngs[m_nCurIndex]);
			m_nCurIndex++;
			m_nCurNum++;
			if (m_nCurIndex == m_vecPngs.size())
			{
				m_nCurIndex = 0;
			}
		}
		else if (m_nCurNum < m_vecPlists.size() + m_vecPngs.size()) //����plist
		{
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile(m_vecPlists[m_nCurIndex]);
			m_nCurIndex++;
			m_nCurNum++;
			if (m_nCurIndex == m_vecPlists.size())
			{
				m_nCurIndex = 0;
			}
		}
		else if (m_nCurNum < m_vecPlists.size() + m_vecPngs.size() + m_vecBgMusic.size()) //����bgMusic
		{
			SimpleAudioEngine::getInstance()->preloadBackgroundMusic(m_vecBgMusic[m_nCurIndex].c_str());
			m_nCurIndex++;
			m_nCurNum++;
			if (m_nCurIndex == m_vecBgMusic.size())
			{
				m_nCurIndex = 0;
			}
		}
		else if (m_nCurNum < m_vecPlists.size() + m_vecPngs.size() + m_vecBgMusic.size() + m_vecEffect.size()) //����effect
		{
			SimpleAudioEngine::getInstance()->preloadEffect(m_vecEffect[m_nCurIndex].c_str());
			m_nCurIndex++;
			m_nCurNum++;
			if (m_nCurIndex == m_vecEffect.size())
			{
				m_nCurIndex = 0;
			}
		}
		
		float fPercent = 100 * m_nCurNum / m_nTotalNum;
		m_pProgressTimer->setPercentage(fPercent);
		if (m_nCurNum == m_nTotalNum)
		{
			//ֹͣupdate ��Ȼ���ܻ����
			this->unscheduleUpdate();
			//ȫ���������,�л�����
			TransitionFade* pTrans = TransitionFade::create(1.5f, CGameMenu::createScene());
			Director::getInstance()->replaceScene(pTrans);
		}
	}
}
