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
	 * @brief ����Object�Ľӿ�  
	 * @param strID  Object��ID
	 * @param pos Ҫ���õ�λ��
	 * @date 2018/09/28 19:58
	 * @return 
	 */
	void addObject(const string& strID, Vec2 pos);
	
	/**
	 * @brief ͨ�����������Ʒ
	 * @param pos Ҫ���ҵ�����
	 * @date 2018/10/06 13:40
	 * @return 
	 */
	CObject* findObjectByPos(Vec2 pos);

	/**
	 * @brief ͨ��UUID����Ʒ
	 * @param nUUID 
	 * @date 2018/10/06 16:11
	 * @return 
	 */
	CObject* findObjectByUUID(int nUUID);

private:
	int m_nCurObjectUUID; //��ǰ��Ʒuuid
};


#endif