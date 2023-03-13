#include "stdafx.h"
#include "..\Header\Boss_Bar_Icon.h"
#include "Export_Function.h"

CBoss_Bar_Icon::CBoss_Bar_Icon(LPDIRECT3DDEVICE9 pGraphicDev)
	: CFrontUi(pGraphicDev)
{
	//m_tINFO[0]->iMax = 5;
}

CBoss_Bar_Icon::CBoss_Bar_Icon(const CBoss_Bar_Icon& rhs)
	: CFrontUi(rhs)
{
}

CBoss_Bar_Icon::~CBoss_Bar_Icon()
{
}

HRESULT CBoss_Bar_Icon::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_eCategory = UI_FRONT;
	return S_OK;
}

_int CBoss_Bar_Icon::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CBoss_Bar_Icon::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CBoss_Bar_Icon::Render_GameObject()
{	
	_tchar				m_szString_Temp[128] = L"도 올";
	_tchar				m_szString_Temp_2[128] = L"[ 탐욕의 거대 사념체 ]";
	_tchar				m_szString_Temp_3[128] = L"[보스]";
	_tchar				m_szString_Temp_4[128] = L"[식물형]";
	Engine::Render_Font(L"Font_Title", m_szString_Temp, &_vec2(814.f, 26.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	Engine::Render_Font(L"Font_Title_2", m_szString_Temp_2, &_vec2(759.f, 54.f), D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.f));
	Engine::Render_Font(L"Font_Title_2", m_szString_Temp_3, &_vec2(616.f, 54.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	Engine::Render_Font(L"Font_Title_2", m_szString_Temp_4, &_vec2(616.f, 99.f), D3DXCOLOR(0.66f, 0.66f, 0.f, 1.f));

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	m_pTransformCom->Ui_BossBar_Print(3); //서순 주의

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	Reset_Proj_Matrix();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBoss_Bar_Icon::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_BossBar_Icon_1"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_BossBar_Icon_1", pComponent });

	return S_OK;
}

void CBoss_Bar_Icon::Change_State(void)
{

}

void CBoss_Bar_Icon::Frame_Check(const _float& fTimeDelta)
{

}

void CBoss_Bar_Icon::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CBoss_Bar_Icon* CBoss_Bar_Icon::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber)
{
	CBoss_Bar_Icon* pInstance = new CBoss_Bar_Icon(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	pInstance->m_tINFO.iNum = _iNumber;

	return pInstance;
}

void CBoss_Bar_Icon::Free(void)
{
	__super::Free();
}
