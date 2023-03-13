#include "stdafx.h"
#include "..\Header\Effect_Player_Charge_Gauge.h"
#include "Export_Function.h"
#include "Player.h"
#include "SylphBow.h"

CEffect_Player_Charge_Gauge::CEffect_Player_Charge_Gauge(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Player_Charge_Gauge::CEffect_Player_Charge_Gauge(const CEffect_Player_Charge_Gauge& rhs)
	: CEffect(rhs)
{
}

CEffect_Player_Charge_Gauge::~CEffect_Player_Charge_Gauge()
{
	Free();
}

HRESULT CEffect_Player_Charge_Gauge::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	_vec3 Scale = { 1.82f , 1.86f , 1.f };

	m_pTransformCom->m_vScale = Scale * PUBLIC_SCALE;

	m_fFrame = 0.f;
	m_fMaxFrame = 22.f;

	

	return S_OK;
}

_int CEffect_Player_Charge_Gauge::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;




	CTransform* pPlayerCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));

	_vec3 vPos = pPlayerCom->m_vInfo[INFO_POS];


	m_pTransformCom->Set_Pos(vPos.x + 4.f, vPos.y + 1.f, vPos.z - 1.f);


	m_fFrame += m_fMaxFrame * fTimeDelta * 1.f * 1.5f;
	

	if (m_fFrame > m_fMaxFrame)
	{
		m_fFrame = m_fMaxFrame;
		m_iAlpha = 255;
	}

	

	


	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	
	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CEffect_Player_Charge_Gauge::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);
}

void CEffect_Player_Charge_Gauge::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB((int)m_iAlpha, 50, 229, 255));

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CEffect_Player_Charge_Gauge::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Effect_BowCharge_Gauge_Sylph"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Effect_BowCharge_Gauge_Sylph", pComponent });

	return S_OK;
}

CEffect_Player_Charge_Gauge* CEffect_Player_Charge_Gauge::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect_Player_Charge_Gauge* pInstance = new CEffect_Player_Charge_Gauge(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Player_Charge_Gauge::Free(void)
{
	__super::Free();
}


