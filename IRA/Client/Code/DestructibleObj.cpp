#include "stdafx.h"
#include "..\Header\DestructibleObj.h"
#include "Export_Function.h"
#include "Effect_Thorn_Hit.h"
#include "Effect_Thorn_Destruction.h"
#include "KeyMgr.h"
#include "CollisionSphere.h"
#include "CollisionMgr.h"

CDestructibleObj::CDestructibleObj(LPDIRECT3DDEVICE9 pGraphicDev)
	: CDynamicObject(pGraphicDev)
	, m_pColliderCom(nullptr)
{
}

CDestructibleObj::CDestructibleObj(const CDestructibleObj& rhs)
	: CDynamicObject(rhs)
	, m_pColliderCom(rhs.m_pColliderCom)
	, m_iHitCnt(rhs.m_iHitCnt)
	, m_bHit(rhs.m_bHit)
	, m_fFrame(rhs.m_fFrame)
{
}

CDestructibleObj::~CDestructibleObj()
{
}

HRESULT CDestructibleObj::Ready_GameObject(void)
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);


	return S_OK;
}

_int CDestructibleObj::Update_GameObject(const _float& fTimeDelta)
{	
	CKeyMgr::Get_Instance()->Update();

	if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_1)) {
		m_bHit = true;
	}


	if (m_bSetCollider)
	{
		m_pColliderCom->Set_Radius(m_fColliderRadius);
		m_pColliderCom->Set_SpherePos(m_vColliderPos);

		m_bSetCollider = false;
	}


	if (g_bSphereMake)
	{
		if (!m_bSphereMake)
		{
			CGameObject* pCollisionSphere = CCollisionSphere::Create(m_pGraphicDev, this,COLLIDER_STATIC);
			NULL_CHECK(pCollisionSphere);
			CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
			pLayer->Add_BulletObject(pCollisionSphere);
			m_bSphereMake = true;
		}
	}

	if (m_bHit)
	{
		++m_iHitCnt;
		++m_fFrame;
		Create_Hit_Effect();
		m_bHit = false;

		if(m_pTextureCom[m_eID]->Get_MaxFrameCnt() - 1 < m_fFrame)		// 서순!!! ++하고 검사하기
			m_fFrame = m_pTextureCom[m_eID]->Get_MaxFrameCnt() - 1;
	}



	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_LANDSCAPE, this);

	return 0;
}

void CDestructibleObj::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	__super::Compute_ViewZ(&vPos);
}

void CDestructibleObj::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// ***
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// 나중에 지워라 

	m_pTextureCom[m_eID]->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();

	// ***
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		// 나중에 지워라


	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CDestructibleObj* CDestructibleObj::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDestructibleObj* pInstance = new CDestructibleObj(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDestructibleObj::Free(void)
{
	__super::Free();
}

HRESULT CDestructibleObj::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_ProtoComponent(L"Proto_Calculator"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	NULL_CHECK_RETURN(m_pColliderCom, E_FAIL);
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Collider", pComponent });

	pComponent = m_pTextureCom[THORN1] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Spr_DestructibleObject_Infectionthorn01_"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Spr_DestructibleObject_Infectionthorn01_", pComponent });

	pComponent = m_pTextureCom[THORN2] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Spr_DestructibleObject_Infectionthorn02_"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Spr_DestructibleObject_Infectionthorn02_", pComponent });

	pComponent = m_pTextureCom[THORN3] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Spr_DestructibleObject_Infectionthorn03_"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Spr_DestructibleObject_Infectionthorn03_", pComponent });

	pComponent = m_pTextureCom[THORN4] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Spr_DestructibleObject_Infectionthorn04_"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Spr_DestructibleObject_Infectionthorn04_", pComponent });

	pComponent = m_pTextureCom[THORN5] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Spr_DestructibleObject_Infectionthorn05_"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Spr_DestructibleObject_Infectionthorn05_", pComponent });

	pComponent = m_pTextureCom[THORN6] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Spr_DestructibleObject_Infectionthorn06_"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Spr_DestructibleObject_Infectionthorn06_", pComponent });

	return S_OK;
}

void CDestructibleObj::Create_Hit_Effect()
{
	switch (m_eID)
	{
	case THORN1: case THORN2: case THORN3: case THORN4: case THORN5: case THORN6:
	{
		CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
		_vec3 vPos = m_pTransformCom->m_vInfo[INFO_POS];
		_vec3 vLocal;

		if (1 == m_iHitCnt)
		{
			_vec3 vTemp = { 0.f, 0.3f, 0.f };
			vLocal = vTemp;
		}
		else if( 2 == m_iHitCnt)
		{
			_vec3 vTemp = { 0.f, -0.2f, 0.f };
			vLocal = vTemp;
		}
		else
		{
			_vec3 vTemp = { 0.f, -0.7f, 0.f };
			vLocal = vTemp;
		}

		_matrix matWorld = *(m_pTransformCom->Get_WorldMatrixPointer());

		D3DXVec3TransformCoord(&vLocal, &vLocal, &matWorld);
		vPos.y = vLocal.y;
		CGameObject* pGameObject = CEffect_Thorn_Hit::Create(m_pGraphicDev, vPos);
		NULL_CHECK(pGameObject);
		pGameLogicLayer->Add_BulletObject(pGameObject);

	}
	break;
	}

}