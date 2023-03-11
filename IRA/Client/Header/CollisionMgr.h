#pragma once

#include "Base.h"
#include "GameObject.h"
#include "Include.h"

class CCollisionMgr
{
	DECLARE_SINGLETON(CCollisionMgr)
private:
	explicit CCollisionMgr();
	virtual ~CCollisionMgr();

public:
	void				Add_CollisionObject(OBJ_TYPE _eID, CGameObject* _pGameObject);
	void				Collision_Update();
	void				Clear_CollisionGroup();

private:
	static void	Collision_Sphere(list<CGameObject*> _Dest, list<CGameObject*> _Src);
	static bool Check_Sphere(CGameObject* pDest, CGameObject* pSrc);

private:
	list<CGameObject*>			m_CollisionGroup[OBJ_END];

private:
	virtual void		Free(void);

};

