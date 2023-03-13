#include "stdafx.h"
#include "..\Header\MiniMap_Character.h"
#include "Export_Function.h"

CMiniMap_Character::CMiniMap_Character(LPDIRECT3DDEVICE9 pGraphicDev)
	: CFrontUi(pGraphicDev)
{
	//m_tINFO[0]->iMax = 5;
}

CMiniMap_Character::CMiniMap_Character(const CMiniMap_Character& rhs)
	: CFrontUi(rhs)
{
}

CMiniMap_Character::~CMiniMap_Character()
{
}

HRESULT CMiniMap_Character::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_eCategory = UI_FRONT;
	return S_OK;
}

_int CMiniMap_Character::Update_GameObject(const _float& fTimeDelta)
{	
	_vec3 vPlayerPos;
	CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

	m_vPlayerPos = vPlayerPos;

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CMiniMap_Character::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CMiniMap_Character::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	m_pTransformCom->Ui_MiniMap_Print(m_tINFO.iNum, &m_vPlayerPos); //서순 주의

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	Reset_Proj_Matrix();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CMiniMap_Character::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_MiniMap_Character"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_MiniMap_Character", pComponent });

	return S_OK;
}

void CMiniMap_Character::Change_State(void)
{

}

void CMiniMap_Character::Frame_Check(const _float& fTimeDelta)
{

}

void CMiniMap_Character::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CMiniMap_Character* CMiniMap_Character::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber)
{
	CMiniMap_Character* pInstance = new CMiniMap_Character(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	pInstance->m_tINFO.iNum = _iNumber;

	return pInstance;
}

void CMiniMap_Character::Free(void)
{
	__super::Free();
}
