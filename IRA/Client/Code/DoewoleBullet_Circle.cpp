#include "stdafx.h"
#include "..\Header\DoewoleBullet_Circle.h"
#include "Export_Function.h"
#include <Effect_CircleBullet_Death.h>

CDoewoleBullet_Circle::CDoewoleBullet_Circle(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev)
{
}

CDoewoleBullet_Circle::CDoewoleBullet_Circle(const CDoewoleBullet_Circle& rhs)
	: CBullet(rhs)
{
}

CDoewoleBullet_Circle::~CDoewoleBullet_Circle()
{
}


HRESULT CDoewoleBullet_Circle::Ready_GameObject(const _vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fSpeed = 40.f;
	m_fMaxFrame = 5.f;

	m_pTransformCom->m_vScale = { 2.f , 2.f , 2.f };
	m_pTransformCom->m_vInfo[INFO_POS] = vPos;

	CTransform* pTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTransformCom, -1);

	m_vDir = { vPos.x - pTransformCom->m_vInfo[INFO_POS].x , 0.f , vPos.z - pTransformCom->m_vInfo[INFO_POS].z };

	D3DXVec3Normalize(&m_vDir, &m_vDir);

	return S_OK;
}

_int CDoewoleBullet_Circle::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
	{
		Create_DeathEffect();
		return OBJ_DEAD;
	}

	Frame_Check(fTimeDelta);

	m_fAccTime += fTimeDelta;

	if (!m_bChangeDir)
	{
		if (m_fAccTime > 0.5f)
		{
			_matrix matRot;

			_float fAngle = _float(40 - rand() % 80);

			D3DXMatrixRotationY(&matRot, D3DXToRadian(fAngle));
			
			D3DXVec3TransformNormal(&m_vDir, &m_vDir, &matRot);

			m_bChangeDir = true;
		}
	}

	m_pTransformCom->m_vInfo[INFO_POS] += m_vDir * m_fSpeed * fTimeDelta;

	CGameObject::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return OBJ_NOEVENT;
}

void CDoewoleBullet_Circle::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CDoewoleBullet_Circle::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CDoewoleBullet_Circle::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Bullet_Doewole_Circle"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Bullet_Doewole_Circle", pComponent });

	return S_OK;
}

void CDoewoleBullet_Circle::Frame_Check(const _float& fTimeDelta)
{
	m_fFrame += m_fMaxFrame * fTimeDelta;

	if (m_fMaxFrame < m_fFrame)
	{
		m_fFrame = 0.f;
	}
}


CDoewoleBullet_Circle* CDoewoleBullet_Circle::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos)
{
	CDoewoleBullet_Circle* pInstance = new CDoewoleBullet_Circle(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDoewoleBullet_Circle::Create_DeathEffect()
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject;

	pGameObject = CEffect_CircleBullet_Death::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);

	if (pGameObject == nullptr)
		return;

	pGameLogicLayer->Add_BulletObject(pGameObject);
}

void CDoewoleBullet_Circle::Free(void)
{
	__super::Free();
}
