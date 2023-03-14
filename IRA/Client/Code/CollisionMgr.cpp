#include "stdafx.h"
#include "CollisionMgr.h"
#include "Collider.h"
#include "Player.h"
#include "SylphArrow.h"
#include "Doewole_Body.h"
#include "DoewoleBullet_Circle.h"
#include "DoewoleBullet_Standard.h"
#include "DoewoleBullet_Circle.h"
#include "DestructibleObj.h"

#include "Monster.h"
#include "MutationEvilSoul.h"


#include "Bullet.h"


IMPLEMENT_SINGLETON(CCollisionMgr)

CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}


void CCollisionMgr::Collision_Update()
{
	Collision_Sphere_Player_Bullet(m_CollisionGroup[OBJ_PLAYER], m_CollisionGroup[OBJ_BULLET]);
	Collision_Sphere_Boss_Arrow(m_CollisionGroup[OBJ_BOSS], m_CollisionGroup[OBJ_ARROW]);
	Collision_Sphere_Monster_Arrow(m_CollisionGroup[OBJ_MONSTER], m_CollisionGroup[OBJ_ARROW]);
	Collision_Sphere_Landscape_Arrow(m_CollisionGroup[OBJ_LANDSCAPE], m_CollisionGroup[OBJ_ARROW]);


	Clear_CollisionGroup();
}

void CCollisionMgr::Collision_Sphere_Player_Bullet(list<CGameObject*> _Dest, list<CGameObject*> _Src)
{
	for (auto& Dest : _Dest)
	{
		for (auto& Src : _Src)
		{
			if (Check_Sphere(Dest, Src))
			{
				if (dynamic_cast<CPlayer*>(Dest) && dynamic_cast<CBullet*>(Src))
				{
					if (dynamic_cast<CPlayer*>(Dest)->m_bImmuned == false) {
						dynamic_cast<CPlayer*>(Dest)->m_bHit = true;
						dynamic_cast<CBullet*>(Src)->m_bHit = true;
					}
						
				}
			}

		}
	}
}

void CCollisionMgr::Collision_Sphere_Boss_Arrow(list<CGameObject*> _Dest, list<CGameObject*> _Src)
{
	for (auto& Dest : _Dest)
	{
		for (auto& Src : _Src)
		{
			if (dynamic_cast<CDoewole*>(Dest))
			{
				if (!dynamic_cast<CBoss*>(Dest)->m_bRender || dynamic_cast<CDoewole*>(Dest)->Get_State() == CDoewole::BOSS_DEAD)
					continue;
			}

			if (Check_Sphere(Dest, Src))
			{	
				if (dynamic_cast<CBoss*>(Dest) && dynamic_cast<CArrow*>(Src))
				{
					dynamic_cast<CArrow*>(Src)->m_bHit = true;

					dynamic_cast<CBoss*>(Dest)->m_bHit = true;
					dynamic_cast<CBoss*>(Dest)->m_Damage_List = dynamic_cast<CArrow*>(Src)->m_Damage_List;
				}
				
			}
		}

	}

}



void CCollisionMgr::Collision_Sphere_Monster_Arrow(list<CGameObject*> _Dest, list<CGameObject*> _Src)
{
	for (auto& Dest : _Dest)
	{
		for (auto& Src : _Src)
		{
			if (Check_Sphere(Dest, Src))
			{
				
				if (dynamic_cast<CMonster*>(Dest) && dynamic_cast<CArrow*>(Src))
				{
					dynamic_cast<CArrow*>(Src)->m_bHit = true;
					dynamic_cast<CMonster*>(Dest)->m_bHit = true;
				}
			}
		}

	}

}

void CCollisionMgr::Collision_Sphere_Landscape_Arrow(list<CGameObject*> _Dest, list<CGameObject*> _Src)
{
	for (auto& Dest : _Dest)
	{
		for (auto& Src : _Src)
		{
			if (Check_Sphere_Static(Dest, Src))
			{
				if (dynamic_cast<CDestructibleObj*>(Dest) && dynamic_cast<CArrow*>(Src))
				{
					if (dynamic_cast<CDestructibleObj*>(Dest)->Get_HitCnt() <= dynamic_cast<CDestructibleObj*>(Dest)->Get_MaxFrameCnt())
					{
						dynamic_cast<CDestructibleObj*>(Dest)->Set_IsHit(true);
						dynamic_cast<CArrow*>(Src)->m_bHit = true;
					}
				}
			}
		}
	}
}




bool CCollisionMgr::Check_Sphere(CGameObject* pDest, CGameObject* pSrc)
{
	CCollider* pColliderDest = dynamic_cast<CCollider*>((pDest)->Get_Component(L"Proto_Collider", ID_DYNAMIC));
	CCollider* pColliderSrc = dynamic_cast<CCollider*>((pSrc)->Get_Component(L"Proto_Collider", ID_DYNAMIC));

	_vec3 v1 = pColliderDest->Get_SpherePos();
	_vec3 v2 = pColliderSrc->Get_SpherePos();

	_float r1 = pColliderDest->Get_Radius();
	_float r2 = pColliderSrc->Get_Radius();

	return (r1 + r2) > D3DXVec3Length(&(v1 - v2));
}

bool CCollisionMgr::Check_Sphere_Static(CGameObject* pDest, CGameObject* pSrc)
{
	CCollider* pColliderDest = dynamic_cast<CCollider*>((pDest)->Get_Component(L"Proto_Collider", ID_STATIC));
	CCollider* pColliderSrc = dynamic_cast<CCollider*>((pSrc)->Get_Component(L"Proto_Collider", ID_DYNAMIC));

	_vec3 v1 = pColliderDest->Get_SpherePos();
	_vec3 v2 = pColliderSrc->Get_SpherePos();

	_float r1 = pColliderDest->Get_Radius();
	_float r2 = pColliderSrc->Get_Radius();

	return (r1 + r2) > D3DXVec3Length(&(v1 - v2));
}

void CCollisionMgr::Free(void)
{
}

void CCollisionMgr::Clear_CollisionGroup()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		m_CollisionGroup[i].clear();
	}
}

void CCollisionMgr::Add_CollisionObject(OBJ_TYPE _eID, CGameObject* _pGameObject)
{
	if (_pGameObject == nullptr || OBJ_END <= _eID)
		return;

	m_CollisionGroup[_eID].push_back(_pGameObject);
}

