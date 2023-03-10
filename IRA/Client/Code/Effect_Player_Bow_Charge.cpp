#include "stdafx.h"
#include "..\Header\Effect_Player_Bow_Charge.h"
#include "Export_Function.h"
#include "Player.h"
#include "SylphBow.h"

CEffect_Player_Bow_Charge::CEffect_Player_Bow_Charge(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Player_Bow_Charge::CEffect_Player_Bow_Charge(const CEffect_Player_Bow_Charge & rhs)
	: CEffect(rhs)
{
}

CEffect_Player_Bow_Charge::~CEffect_Player_Bow_Charge()
{
	Free();
}

HRESULT CEffect_Player_Bow_Charge::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	_vec3 Scale = { 6.f , 6.f, 1.f };

	m_pTransformCom->m_vScale = Scale;

	m_fFrame = 0.f;
	m_fMaxFrame = 5.f;

	

	return S_OK;
}

_int CEffect_Player_Bow_Charge::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;


	POINT ptCursor;

	GetCursorPos(&ptCursor);
	ScreenToClient(g_hWnd, &ptCursor);

	_vec3 Axis = { -1.f,0.f,0.f };
	D3DXVec3Normalize(&Axis, &Axis);

	_vec3 Dir = { (float)(ptCursor.x - WINCX * 0.5),float(ptCursor.y - WINCY * 0.5),0 };
	D3DXVec3Normalize(&Dir, &Dir);


	_vec3 EffectDir = { Dir.x,0.f,-Dir.y };
	
	CSylphBow* pSylphBow = dynamic_cast<CSylphBow*>(Engine::Get_GameObject(L"Layer_GameLogic", L"SylphBow"));

	_vec3 vPos = pSylphBow->m_vPos;

	vPos += EffectDir * 5.f;

	m_pTransformCom->Set_Pos(vPos.x, vPos.y - 2.f, vPos.z);




	m_fFrame += m_fMaxFrame * fTimeDelta * 1.5f;

	if (m_fFrame > m_fMaxFrame)
	{
		m_fFrame = 0.f;
	}


	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	
	__super::Update_GameObject(fTimeDelta);

	return OBJ_NONE;
}

void CEffect_Player_Bow_Charge::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();


}

void CEffect_Player_Bow_Charge::Render_GameObject()
{

	__super::Render_GameObject();
}

HRESULT CEffect_Player_Bow_Charge::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Effect_BowCharge_Sylph"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Effect_BowCharge_Sylph", pComponent });

	return S_OK;
}

CEffect_Player_Bow_Charge* CEffect_Player_Bow_Charge::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect_Player_Bow_Charge* pInstance = new CEffect_Player_Bow_Charge(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Player_Bow_Charge::Free(void)
{
	__super::Free();
}


