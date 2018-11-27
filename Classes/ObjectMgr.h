#ifndef __OBJECTMGR_H__
#define __OBJECTMGR_H__
#include "cocos2d.h"
#include "Object.h"
USING_NS_CC;
class CObjectMgr :public Layer
{
public:
	CObjectMgr();
	virtual ~CObjectMgr();
	virtual bool init();
	CREATE_FUNC(CObjectMgr);

	/**
	 * @brief 增加Object的接口  
	 * @param strID  Object的ID
	 * @param pos 要设置的位置
	 * @date 2018/09/28 19:58
	 * @return 
	 */
	void addObject(const string& strID, Vec2 pos);
	
	/**
	 * @brief 通过坐标查找物品
	 * @param pos 要查找的坐标
	 * @date 2018/10/06 13:40
	 * @return 
	 */
	CObject* findObjectByPos(Vec2 pos);

	/**
	 * @brief 通过UUID找物品
	 * @param nUUID 
	 * @date 2018/10/06 16:11
	 * @return 
	 */
	CObject* findObjectByUUID(int nUUID);

private:
	int m_nCurObjectUUID; //当前物品uuid
};


#endif