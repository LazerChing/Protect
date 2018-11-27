#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"
#include "EnemyMgr.h"
#include "CardMgr.h"
#include "WeaponMgr.h"
#include "ObjectMgr.h"
#include "Radish.h"
#include "BulletMgr.h"
#include "AnimateMgr.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace std;
using namespace CocosDenshion;

//build ���Խ����� forbid ��ֹ���� Object ѡ�е�����Ʒ  towerѡ�е������� radishѡ�е����ܲ�
//top ����������Ϸ���״̬�� enemy ��ѡ�е���
enum eAimType  
{
	E_AIM_NONE, E_AIM_BUILD, E_AIM_FORBID, E_AIM_OBJECT, E_AIM_TOWER, E_AIM_RADISH,
	E_AIM_TOP, E_AIM_ENEMY
};


class CGameScene: public Scene
{
public:
	CGameScene();
	virtual ~CGameScene();
	static Scene* createScene();
	static CGameScene* getInstance();
	static void destroy();
	virtual bool init();
	CREATE_FUNC(CGameScene);
	virtual void update(float fDelta);

	/**
	 * @brief ��Ϸ����
	 * @param 
	 * @date 2018/10/07 11:47
	 * @return 
	 */
	void gameOver();

	/**
	 * @brief ��Ϸ��ʤ
	 * @param 
	 * @date 2018/10/06 23:26
	 * @return 
	 */
	void gameWin();

	/**
	 * @brief ��������������������Ϊ�㼶��ϵ���ŵ���������ʾ��
	 * @param pos Ҫ��ʾ��λ��
	 * @date 2018/10/05 15:15
	 * @return 
	 */
	void playUpgradeWeaponAnimate(Vec2 pos);

	/**
	 * @brief ���ӽ��
	 * @param nCoin ���
	 * @date 2018/10/04 11:23
	 * @return 
	 */
	void addCoin(int nCoin);

	/**
	 * @brief ���ٽ��
	 * @param nCoin ���
	 * @date 2018/10/03 17:01
	 * @return 
	 */
	void reduceCoin(int nCoin);

	/**
	* @brief ����ת�� ͨ�����������ø������꣨������������ԭ�����Ͻǣ�
	* @param pos��������
	* @date 2018/09/19 14:28
	* @return ��������
	*/
	Vec2 getTiledPosByPixel(Vec2 pos);

	/**
	 * @brief �õ����ӵ�ͼ�Ĵ�С  
	 * @param 
	 * @date 2018/09/25 11:01
	 * @return MapSize
	 */
	const Size getTiledMapSize() const
	{
		return m_pTmxTiledMap->getMapSize();
	}

	/**
	 * @brief �õ����ӵĴ�С  
	 * @param 
	 * @date 2018/09/25 11:02
	 * @return TiledSize
	 */
	const Size getTiledSize() const
	{
		return m_pTmxTiledMap->getTileSize();
	}
	
	/**
	 * @brief ͨ�����������øø��ӵ��е���������
	 * @param pos ��������
	 * @date 2018/09/20 11:18
	 * @return ��������
	 */
	Vec2 getCenterPixelPosByTiled(Vec2 pos);

	/**
	 * @brief ͨ����������ϵ����������ת��Ϊ��������ϵ����������  
	 * @param pos ��������ϵ����������
	 * @date 2018/09/28 20:07
	 * @return ��������ϵ����������
	 */
	Vec2 convertGLToWorld(Vec2 pos);

	/**
	 * @brief �õ�Ŀ������Ŀ�ִ�в�������
	 * @param pixelPos ��������
	 * @param pNode ���������õģ�����ֵ��
	 * @date 2018/09/29 15:47
	 * @return Ŀ�������
	 */
	int getTypeOfAimPos(Vec2 pixelPos, Node*& pNode);

	/**
	* @brief ȡ�ùյ�����(����·��)
	* @param
	* @date 2018/09/20 15:35
	* @return vector<Vec2>���յ㣩
	*/
	vector<Vec2> getVecPathPos()
	{
		return m_vecPos;
	}

	/**
	 * @brief �Ƴ�һ���ܽ���������  
	 * @param �����Ƴ�������
	 * @date 2018/09/29 16:14
	 * @return true ��ʾ�Ƴ��ɹ�
	 */
	bool removeCanBuildPos(Vec2 centerPixelPos);

	/**
	 * @brief �����ѡ�нڵ㣨���ܵ��˻���Ʒ�ѱ����𣬶�����ָ��һ��δ֪�Ĳ�Ϊnull�ĵ�ַ�������
	 * @param 
	 * @date 2018/10/06 13:56
	 * @return 
	 */
	void clearSignedNode();

	/**
	 * @brief �õ�����ǵĽڵ�����
	 * @param 
	 * @date 2018/10/06 14:12
	 * @return 
	 */
	sSignedNodeDt& getSignedNodeDt()
	{
		return m_sSignedNodeDt;
	}

	/**
	 * @brief ���²�����ʾ
	 * @param nWave ����
	 * @date 2018/10/06 22:10
	 * @return 
	 */
	void updataWaveView(int& nWave)
	{
		m_pCurWaveLabel->setString(StringUtils::format("%02d", nWave));
	}

	/**
	 * @brief �����Ʒ�ľ��κ� ����
	 * @param 
	 * @date 2018/10/09 20:37
	 * @return 
	 */
	vector<Rect>& getVecObjectRect()
	{
		return m_vecObjectRect;
	}

	CEnmeyMgr* getEnemyMgr()
	{
		return m_pEnemyMgr;
	}

	CCardMgr* getCardMgr()
	{
		return m_pCardMgr;
	}

	CWeaponMgr* getWeaponMgr()
	{
		return m_pWeaponMgr;
	}

	CObjectMgr* getObjectMgr()
	{
		return m_pObjectMgr;
	}

	CBulletMgr* getBulletMgr()
	{
		return m_pBulletMgr;
	}
	CAnimateMgr* getAnimateMgr()
	{
		return m_pAnimateMgr;
	}

	CRadish* getRadish()
	{
		return m_pRadish;
	}
private:

	//����¼��ص�����
	bool onTouchBegin(Touch* pTouch, Event* pEvent);
	void onTouchEnd(Touch* pTouch, Event* pEvent);

	/**
	 * @brief ��Ϸ��ʼ�ص�����  
	 * @param 
	 * @date 2018/09/29 14:03
	 * @return 
	 */
	void gameStart(float fDelta);

	/**
	 * @brief ��ӵ�����  
	 * @param 
	 * @date 2018/09/19 14:14
	 * @return 
	 */
	void addTouchListener();	

	/**
	 * @brief ��ʼ��·������(�յ�)  
	 * @param 
	 * @date 2018/09/20 10:31
	 * @return 
	 */
	void initPos();
	
	/**
	 * @brief ��������  
	 * @param 
	 * @date 2018/09/20 15:31
	 * @return 
	 */
	void createBg();

	/**
	 * @brief �������ֹ�����  
	 * @param 
	 * @date 2018/09/20 15:31
	 * @return 
	 */
	void createMgr();

	/**
	 * @brief ��ײ���
	 * @param 
	 * @date 2018/10/05 16:50
	 * @return 
	 */
	void collision();

	/**
	 * @brief �ӵ����е���
	 * @param pBullet �ӵ�
	 * @param pEnemy ����
	 * @date 2018/10/05 19:00
	 * @return 
	 */
	void bulletTouchEnemy(CBullet* pBullet, CEnemy* pEnemy);

	/**
	 * @brief �ӵ�������Ʒ
	 * @param pBullet �ӵ�
	 * @param pObject ��Ʒ
	 * @date 2018/10/06 11:27
	 * @return 
	 */
	void bulletTouchObject(CBullet* pBullet, CObject* pObject);
private:
	CC_SYNTHESIZE(int, m_nCurCoin, CurCoin);
	static CGameScene* m_spInstance;
	CBulletMgr* m_pBulletMgr;
	CEnmeyMgr* m_pEnemyMgr;
	CCardMgr* m_pCardMgr;
	CWeaponMgr* m_pWeaponMgr;
	CObjectMgr* m_pObjectMgr;
	CRadish* m_pRadish;
	CAnimateMgr* m_pAnimateMgr;
	TMXTiledMap* m_pTmxTiledMap;//Tiled��ͼ
	TMXLayer* m_pTmxPathLayer; //·����
	vector<Vec2> m_vecPos;  //�յ�����(����������е���������)
	Sprite* m_pDisablePng;  //���ɲ���ͼƬ
	vector<Rect> m_vecObjectRect;  //����ľ��κ���
	vector<Vec2>m_vecCanBuildPos; //�ܽ�����������������
	Layer* m_pMenuBgLayer; //�˵�������
	Layer* m_pWaveBgLayer; //����������
	Sprite* m_pPauseSprite; //��ͣͼƬ
	Label* m_pCoinLabel; //���
	Label* m_pCurWaveLabel; //��ǰ����

	//��ʾ���������Ķ����;��飨��Ϊ�㼶��ϵ�����Է�������ʾ��
	Sprite* m_pUpgradeAnimateSprite;//�������������ľ���
	Animate* m_pUpgradeAnimate; //��������

	sSignedNodeDt m_sSignedNodeDt; //����ǵĽڵ�����
};

#endif