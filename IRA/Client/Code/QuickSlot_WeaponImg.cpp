#include "stdafx.h"
#include "..\Header\QuickSlot_WeaponImg.h"
#include "Export_Function.h"

CQuickSlot_WeaponImg::CQuickSlot_WeaponImg(LPDIRECT3DDEVICE9 pGraphicDev)
	: CFrontUi(pGraphicDev)
{
	//m_tINFO[0]->iMax = 5;
}

CQuickSlot_WeaponImg::CQuickSlot_WeaponImg(const CQuickSlot_WeaponImg& rhs)
	: CFrontUi(rhs)
{
}

CQuickSlot_WeaponImg::~CQuickSlot_WeaponImg()
{
}

HRESULT CQuickSlot_WeaponImg::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_eCategory = UI_FRONT;
	return S_OK;
}

_int CQuickSlot_WeaponImg::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CQuickSlot_WeaponImg::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CQuickSlot_WeaponImg::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	m_pTransformCom->Ui_QuickSlot_Print(2); //���� ����

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	Reset_Proj_Matrix();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CQuickSlot_WeaponImg::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_QuickSlot_WeaponImg_1"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_QuickSlot_WeaponImg_1", pComponent });

	return S_OK;
}

void CQuickSlot_WeaponImg::Change_State(void)
{

}

void CQuickSlot_WeaponImg::Frame_Check(const _float& fTimeDelta)
{

}

void CQuickSlot_WeaponImg::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CQuickSlot_WeaponImg* CQuickSlot_WeaponImg::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber)
{
	CQuickSlot_WeaponImg* pInstance = new CQuickSlot_WeaponImg(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	pInstance->m_tINFO.iNum = _iNumber;

	return pInstance;
}

void CQuickSlot_WeaponImg::Free(void)
{
	__super::Free();
}
