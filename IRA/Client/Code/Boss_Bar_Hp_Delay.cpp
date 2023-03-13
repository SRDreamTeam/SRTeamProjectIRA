#include "stdafx.h"
#include "..\Header\Boss_Bar_Hp_Delay.h"
#include "Export_Function.h"

#include "GreenEvilSlime.h"


CBoss_Bar_Hp_Delay::CBoss_Bar_Hp_Delay(LPDIRECT3DDEVICE9 pGraphicDev)
	: CFrontUi(pGraphicDev), m_fHp(100), m_fReceiveHp(0), m_bCount(false), m_fDelay(0)
{
	//m_tINFO[0]->iMax = 5;
}

CBoss_Bar_Hp_Delay::CBoss_Bar_Hp_Delay(const CBoss_Bar_Hp_Delay& rhs)
	: CFrontUi(rhs), m_fHp(100), m_fReceiveHp(0), m_bCount(false), m_fDelay(0)
{
}

CBoss_Bar_Hp_Delay::~CBoss_Bar_Hp_Delay()
{
}

HRESULT CBoss_Bar_Hp_Delay::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_eCategory = UI_FRONT;

	return S_OK;
}

_int CBoss_Bar_Hp_Delay::Update_GameObject(const _float& fTimeDelta)
{	
	Frame_Check(fTimeDelta);

	CGreenEvilSlime* pMonster = dynamic_cast<CGreenEvilSlime*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Monster"));
	m_fReceiveHp = pMonster->Get_Hp();

	if (0 == (_uint)m_fDelay && (false == m_bCount))
		m_bCount = true;

	if (m_fHp > m_fReceiveHp && m_bCount)
	{
		m_fHp -= 1;
		m_bCount = false;
	}

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CBoss_Bar_Hp_Delay::LateUpdate_GameObject()
{	
	__super::LateUpdate_GameObject();
}

void CBoss_Bar_Hp_Delay::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 120, 120, 120));
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	m_pTransformCom->Ui_BossBar_Print(4, m_fHp); //서순 주의

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	Reset_Proj_Matrix();

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBoss_Bar_Hp_Delay::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_BossBar_Hp"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_BossBar_Hp", pComponent });

	return S_OK;
}

void CBoss_Bar_Hp_Delay::Change_State(void)
{

}

void CBoss_Bar_Hp_Delay::Frame_Check(const _float& fTimeDelta)
{
	m_fDelay += 8.f * fTimeDelta;

	if (8.f < m_fDelay)
	{
		m_fDelay = 0.f;
	}
}

void CBoss_Bar_Hp_Delay::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CBoss_Bar_Hp_Delay* CBoss_Bar_Hp_Delay::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber)
{
	CBoss_Bar_Hp_Delay* pInstance = new CBoss_Bar_Hp_Delay(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	pInstance->m_tINFO.iNum = _iNumber;

	return pInstance;
}

void CBoss_Bar_Hp_Delay::Free(void)
{
	__super::Free();
}
