#include "stdafx.h"
#include "..\Header\Boss_Bar_Hp_Back.h"
#include "Export_Function.h"

CBoss_Bar_Hp_Back::CBoss_Bar_Hp_Back(LPDIRECT3DDEVICE9 pGraphicDev)
	: CFrontUi(pGraphicDev)
{
	//m_tINFO[0]->iMax = 5;
}

CBoss_Bar_Hp_Back::CBoss_Bar_Hp_Back(const CBoss_Bar_Hp_Back& rhs)
	: CFrontUi(rhs)
{
}

CBoss_Bar_Hp_Back::~CBoss_Bar_Hp_Back()
{
}

HRESULT CBoss_Bar_Hp_Back::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_eCategory = UI_FRONT;
	return S_OK;
}

_int CBoss_Bar_Hp_Back::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CBoss_Bar_Hp_Back::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CBoss_Bar_Hp_Back::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	m_pTransformCom->Ui_BossBar_Print(1); //���� ����

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	Reset_Proj_Matrix();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBoss_Bar_Hp_Back::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_BossBar_Back"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_BossBar_Back", pComponent });

	return S_OK;
}

void CBoss_Bar_Hp_Back::Change_State(void)
{

}

void CBoss_Bar_Hp_Back::Frame_Check(const _float& fTimeDelta)
{

}

void CBoss_Bar_Hp_Back::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CBoss_Bar_Hp_Back* CBoss_Bar_Hp_Back::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber)
{
	CBoss_Bar_Hp_Back* pInstance = new CBoss_Bar_Hp_Back(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	pInstance->m_tINFO.iNum = _iNumber;

	return pInstance;
}

void CBoss_Bar_Hp_Back::Free(void)
{
	__super::Free();
}
