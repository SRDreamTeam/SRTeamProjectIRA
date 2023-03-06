#include "stdafx.h"
#include "..\Header\FrontUi_Hp.h"
#include "Export_Function.h"

CFrontUi_Hp::CFrontUi_Hp(LPDIRECT3DDEVICE9 pGraphicDev)
	: CPui(pGraphicDev), iMax_Hp(5), iNow_Hp(4)
{
}

CFrontUi_Hp::CFrontUi_Hp(const CFrontUi_Hp& rhs)
	: CPui(rhs), iMax_Hp(5), iNow_Hp(4)
{
}

CFrontUi_Hp::~CFrontUi_Hp()
{
}

HRESULT CFrontUi_Hp::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_eCategory = UI_FRONT;

	m_pTransformCom->Set_Pos(1.f, 1.f, 0.5f);
	m_pTransformCom->UpdatePos_OnWorld();

	return S_OK;
}

_int CFrontUi_Hp::Update_GameObject(const _float& fTimeDelta)
{	
	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CFrontUi_Hp::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CFrontUi_Hp::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	//D3DXMATRIXA16 matWorld;
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

	//D3DXVECTOR3 vEyePt(1.0f, 1.0f, -5.0f); /// 1. 눈의 위치 (0, 3.0, -5)
	//D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f); /// 2. 눈이 바라보는 위치 (0,0,0)
	//D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);   /// 3. 천정 방향의 상방벡터 (0,1,0)
	//D3DXMATRIXA16 matView;
	//D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);

	//D3DXMATRIX		matWorld, matView, matProj; //직교투영 행렬
	//D3DXMatrixIdentity(&matWorld); //월드행렬을 항등행렬로
	//D3DXMatrixIdentity(&matView);
	//D3DXMatrixOrthoLH(&matProj, 0.f, 0.f, 0.f, 0.f);

	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	//m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	//m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

	m_pTransformCom->Ui_Print();

	m_pBufferCom->Render_Buffer();

	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CFrontUi_Hp::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_FieldHp"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_FieldHp", pComponent });

	return S_OK;
}

void CFrontUi_Hp::SetUp_OnTerrain(void)
{

}

void CFrontUi_Hp::Change_State(void)
{

}

void CFrontUi_Hp::Frame_Check(const _float& fTimeDelta)
{

}

CFrontUi_Hp* CFrontUi_Hp::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFrontUi_Hp* pInstance = new CFrontUi_Hp(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFrontUi_Hp::Free(void)
{
	__super::Free();
}
