#include "stdafx.h"
#include "..\Header\Boss_Bar_Hp.h"
#include "Export_Function.h"

#include "Doewole_Body.h"

CBoss_Bar_Hp::CBoss_Bar_Hp(LPDIRECT3DDEVICE9 pGraphicDev)
	: CFrontUi(pGraphicDev), m_iMaxHp(0), m_iCurHp(0)
{
	//m_tINFO[0]->iMax = 5;
}

CBoss_Bar_Hp::CBoss_Bar_Hp(const CBoss_Bar_Hp& rhs)
	: CFrontUi(rhs), m_iMaxHp(0), m_iCurHp(0)
{
}

CBoss_Bar_Hp::~CBoss_Bar_Hp()
{
}

HRESULT CBoss_Bar_Hp::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_eCategory = UI_FRONT;
	return S_OK;
}

_int CBoss_Bar_Hp::Update_GameObject(const _float& fTimeDelta)
{	
	CDoewole_Body* pMonster = dynamic_cast<CDoewole_Body*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Doewole_Body"));
	m_iMaxHp = DOEWOLE_MAXHP;

	if (nullptr != pMonster)
	{
		m_iCurHp = pMonster->Get_CurHp();

	}
	//if (m_iMaxHp)
	//{
	//	m_iCurHp;
	//}

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CBoss_Bar_Hp::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CBoss_Bar_Hp::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	m_pTransformCom->Ui_BossBar_Print(4, m_iCurHp); //���� ����

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	Reset_Proj_Matrix();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBoss_Bar_Hp::Add_Component(void)
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

void CBoss_Bar_Hp::Change_State(void)
{

}

void CBoss_Bar_Hp::Frame_Check(const _float& fTimeDelta)
{
}

void CBoss_Bar_Hp::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CBoss_Bar_Hp* CBoss_Bar_Hp::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber)
{
	CBoss_Bar_Hp* pInstance = new CBoss_Bar_Hp(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	pInstance->m_tINFO.iNum = _iNumber;

	return pInstance;
}

void CBoss_Bar_Hp::Free(void)
{
	__super::Free();
}
