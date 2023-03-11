#include "stdafx.h"
#include "CollisionMgr.h"
#include "Collider.h"

IMPLEMENT_SINGLETON(CCollisionMgr)

CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}


void CCollisionMgr::Collision_Update()
{
	Collision_Sphere(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_MONSTER]);
	//Collision_Sphere(m_ObjList[OBJ_ARROW], m_ObjList[OBJ_MONSTER]);
}

void CCollisionMgr::Collision_Sphere(list<CGameObject*> _Dest, list<CGameObject*> _Src)
{
	for (auto& Dest : _Dest)
	{
		for (auto& Src : _Src)
		{
			if (Check_Sphere(Dest, Src))
			{
				//플레이어 힛트했다

			}
		}
	}
}

bool CCollisionMgr::Check_Sphere(CGameObject* pDest, CGameObject* pSrc)
{
	CCollider* pColliderDest = dynamic_cast<CCollider*> (pDest->Get_Component(L"Proto_Collider", ID_DYNAMIC));
	CCollider* pColliderSrc = dynamic_cast<CCollider*> (pSrc->Get_Component(L"Proto_Collider", ID_DYNAMIC));

	_vec3 v1 = pColliderDest->Get_SpherePos();
	_vec3 v2 = pColliderSrc->Get_SpherePos();

	_float r1 = pColliderDest->Get_Radius();
	_float r2 = pColliderSrc->Get_Radius();

	return (r1 + r2) > D3DXVec3Length(&(v1 - v2));
}

void CCollisionMgr::Clear_ObjectList()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		if (OBJ_PLAYER == i)
			continue;

		m_ObjList[i].clear();
	}
}

void CCollisionMgr::Add_CollisionObject(OBJ_TYPE _eID, CGameObject* _pGameObject)
{
	if (_pGameObject == nullptr || OBJ_END <= _eID)
		return;

	m_ObjList[_eID].push_back(_pGameObject);
}

