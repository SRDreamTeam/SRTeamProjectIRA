#include "..\..\Header\CollisionMgr.h"
#include "Collider.h"

CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::Collision_Sphere(unordered_map<const _tchar*, CGameObject*> _Dest, unordered_map<const _tchar*, CGameObject*> _Src)
{
	for (auto& Dest : _Dest)
	{
		for (auto& Src : _Src)
		{
			if (Check_Sphere(Dest.second, Src.second))
			{
				int a = 0;
			}
		}
	}
}

bool CCollisionMgr::Check_Sphere(CGameObject* pDest, CGameObject* pSrc)
{
	CCollider* pColliderDest = dynamic_cast<CCollider*> (pDest->Get_Component(L"Proto_Collider", ID_DYNAMIC));
	CCollider* pColliderSrc=  dynamic_cast<CCollider*> (pSrc->Get_Component(L"Proto_Collider", ID_DYNAMIC));

	_vec3 v1 = pColliderDest->Get_SpherePos();
	_vec3 v2 = pColliderSrc->Get_SpherePos();

	_float r1 = pColliderDest->Get_Radius();
	_float r2 = pColliderSrc->Get_Radius();

	return (r1+r2) > D3DXVec3Length(&(v1 - v2));
}


