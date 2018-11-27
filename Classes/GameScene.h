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

//build 可以建塔， forbid 禁止建塔 Object 选中的是物品  tower选中的是炮塔 radish选中的是萝卜
//top 点击的是最上方，状态栏 enemy 是选中敌人
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
	 * @brief 游戏结束
	 * @param 
	 * @date 2018/10/07 11:47
	 * @return 
	 */
	void gameOver();

	/**
	 * @brief 游戏获胜
	 * @param 
	 * @date 2018/10/06 23:26
	 * @return 
	 */
	void gameWin();

	/**
	 * @brief 播放武器升级动画（因为层级关系，放到这里来显示）
	 * @param pos 要显示的位置
	 * @date 2018/10/05 15:15
	 * @return 
	 */
	void playUpgradeWeaponAnimate(Vec2 pos);

	/**
	 * @brief 增加金币
	 * @param nCoin 金币
	 * @date 2018/10/04 11:23
	 * @return 
	 */
	void addCoin(int nCoin);

	/**
	 * @brief 减少金币
	 * @param nCoin 金币
	 * @date 2018/10/03 17:01
	 * @return 
	 */
	void reduceCoin(int nCoin);

	/**
	* @brief 坐标转换 通过像素坐标获得格子坐标（格子坐标坐标原点左上角）
	* @param pos像素坐标
	* @date 2018/09/19 14:28
	* @return 格子坐标
	*/
	Vec2 getTiledPosByPixel(Vec2 pos);

	/**
	 * @brief 得到格子地图的大小  
	 * @param 
	 * @date 2018/09/25 11:01
	 * @return MapSize
	 */
	const Size getTiledMapSize() const
	{
		return m_pTmxTiledMap->getMapSize();
	}

	/**
	 * @brief 得到格子的大小  
	 * @param 
	 * @date 2018/09/25 11:02
	 * @return TiledSize
	 */
	const Size getTiledSize() const
	{
		return m_pTmxTiledMap->getTileSize();
	}
	
	/**
	 * @brief 通过格子坐标获得该格子的中点像素坐标
	 * @param pos 格子坐标
	 * @date 2018/09/20 11:18
	 * @return 像素坐标
	 */
	Vec2 getCenterPixelPosByTiled(Vec2 pos);

	/**
	 * @brief 通过格子坐标系的像素坐标转化为世界坐标系的像素坐标  
	 * @param pos 格子坐标系的像素坐标
	 * @date 2018/09/28 20:07
	 * @return 世界坐标系的像素坐标
	 */
	Vec2 convertGLToWorld(Vec2 pos);

	/**
	 * @brief 得到目的坐标的可执行操作类型
	 * @param pixelPos 像素坐标
	 * @param pNode 用来接收用的（返回值）
	 * @date 2018/09/29 15:47
	 * @return 目标的类型
	 */
	int getTypeOfAimPos(Vec2 pixelPos, Node*& pNode);

	/**
	* @brief 取得拐点坐标(敌人路径)
	* @param
	* @date 2018/09/20 15:35
	* @return vector<Vec2>（拐点）
	*/
	vector<Vec2> getVecPathPos()
	{
		return m_vecPos;
	}

	/**
	 * @brief 移除一个能建塔的坐标  
	 * @param 即将移除的坐标
	 * @date 2018/09/29 16:14
	 * @return true 表示移除成功
	 */
	bool removeCanBuildPos(Vec2 centerPixelPos);

	/**
	 * @brief 清楚被选中节点（可能敌人或物品已被消灭，而这里指向一个未知的不为null的地址，会出错
	 * @param 
	 * @date 2018/10/06 13:56
	 * @return 
	 */
	void clearSignedNode();

	/**
	 * @brief 得到被标记的节点数据
	 * @param 
	 * @date 2018/10/06 14:12
	 * @return 
	 */
	sSignedNodeDt& getSignedNodeDt()
	{
		return m_sSignedNodeDt;
	}

	/**
	 * @brief 更新波数显示
	 * @param nWave 波数
	 * @date 2018/10/06 22:10
	 * @return 
	 */
	void updataWaveView(int& nWave)
	{
		m_pCurWaveLabel->setString(StringUtils::format("%02d", nWave));
	}

	/**
	 * @brief 获得物品的矩形盒 容器
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

	//点击事件回调函数
	bool onTouchBegin(Touch* pTouch, Event* pEvent);
	void onTouchEnd(Touch* pTouch, Event* pEvent);

	/**
	 * @brief 游戏开始回调函数  
	 * @param 
	 * @date 2018/09/29 14:03
	 * @return 
	 */
	void gameStart(float fDelta);

	/**
	 * @brief 添加点击检测  
	 * @param 
	 * @date 2018/09/19 14:14
	 * @return 
	 */
	void addTouchListener();	

	/**
	 * @brief 初始化路径坐标(拐点)  
	 * @param 
	 * @date 2018/09/20 10:31
	 * @return 
	 */
	void initPos();
	
	/**
	 * @brief 创建背景  
	 * @param 
	 * @date 2018/09/20 15:31
	 * @return 
	 */
	void createBg();

	/**
	 * @brief 创建各种管理者  
	 * @param 
	 * @date 2018/09/20 15:31
	 * @return 
	 */
	void createMgr();

	/**
	 * @brief 碰撞检测
	 * @param 
	 * @date 2018/10/05 16:50
	 * @return 
	 */
	void collision();

	/**
	 * @brief 子弹击中敌人
	 * @param pBullet 子弹
	 * @param pEnemy 敌人
	 * @date 2018/10/05 19:00
	 * @return 
	 */
	void bulletTouchEnemy(CBullet* pBullet, CEnemy* pEnemy);

	/**
	 * @brief 子弹击中物品
	 * @param pBullet 子弹
	 * @param pObject 物品
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
	TMXTiledMap* m_pTmxTiledMap;//Tiled地图
	TMXLayer* m_pTmxPathLayer; //路径层
	vector<Vec2> m_vecPos;  //拐点坐标(格子坐标的中点像素坐标)
	Sprite* m_pDisablePng;  //不可操作图片
	vector<Rect> m_vecObjectRect;  //物体的矩形盒子
	vector<Vec2>m_vecCanBuildPos; //能建塔的中心像素坐标
	Layer* m_pMenuBgLayer; //菜单背景层
	Layer* m_pWaveBgLayer; //波数背景层
	Sprite* m_pPauseSprite; //暂停图片
	Label* m_pCoinLabel; //金币
	Label* m_pCurWaveLabel; //当前波数

	//显示武器升级的动画和精灵（因为层级关系，所以放这里显示）
	Sprite* m_pUpgradeAnimateSprite;//承载升级动画的精灵
	Animate* m_pUpgradeAnimate; //升级动画

	sSignedNodeDt m_sSignedNodeDt; //被标记的节点数据
};

#endif