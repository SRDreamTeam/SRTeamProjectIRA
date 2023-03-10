#include "stdafx.h"
#include "..\Header\SylphBow.h"
#include "Export_Function.h"

CSylphBow::CSylphBow(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBow(pGraphicDev)
{
}

CSylphBow::CSylphBow(const CSylphBow& rhs)
	: CBow(rhs)
{
}

CSylphBow::~CSylphBow()
{
}

HRESULT CSylphBow::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	

	__super::Ready_GameObject();

	return S_OK;
}

_int CSylphBow::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	if(m_bRender == true)
		m_bRealRender = true;

	if (m_bRender == false) {
		m_Time += 3.f * fTimeDelta * 0.55f;
		if (m_Time > 3.f) {
			m_Time = 0.f;
			m_bRealRender = false;
		}
	}


	if (m_bRender == true) {
		if (m_bCharge == true) {
			m_fFrame += 2.f * fTimeDelta * 1.f;
			if (m_fFrame > 2.f) {
				m_fFrame = 2.f;
			}
		}
		else {
			m_fFrame += 2.f * fTimeDelta * 4.f;
			if (m_fFrame > 2.f) {
				m_fFrame = 0.f;
			}
		}
	}
	else {
		m_fFrame = 0.f;
	}

	Update_Bow_State();


	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);


	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CSylphBow::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CSylphBow::Render_GameObject()
{
	if (m_bRealRender == false) {
		return;
	}
		
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_MatWorld);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture((_int)m_fFrame);

	m_pTransformCom->m_vInfo[INFO_POS];

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CSylphBow::Update_Bow_State()
{
	CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));

	pPlayerTransformCom->Get_Info(INFO_POS, &m_Bow_Pos);

	_vec3       Player_Axis = { 0.f,1.f,0.f };


	POINT ptCursor;

	GetCursorPos(&ptCursor);
	ScreenToClient(g_hWnd, &ptCursor);

	_vec3 Axis = { -1.f,0.f,0.f };

	_vec3 Dir = { (float)(ptCursor.x - WINCX * 0.5),float(ptCursor.y - WINCY * 0.5),0 };
	D3DXVec3Normalize(&Dir, &Dir);


	m_Bow_Angle = acos(D3DXVec3Dot(&Axis, &Dir));

	if (WINCY * 0.5 < ptCursor.y)
		m_Bow_Angle = 2.f * D3DX_PI - m_Bow_Angle;


	_matrix matScale, matTrans_Pre, matRot, matTrans, matRev;

	D3DXMatrixIdentity(&m_MatWorld);

	D3DXMatrixScaling(&matScale, -2.6f, 2.6f, 1.f);
	D3DXMatrixRotationY(&matRot, 220);
	D3DXMatrixTranslation(&matTrans_Pre, 3.f, 0.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_Bow_Pos.x - 0.1f, m_Bow_Pos.y + 2.4f, m_Bow_Pos.z);


	m_vPos = { m_Bow_Pos.x , m_Bow_Pos.y + 2.0f, m_Bow_Pos.z };


	Axis = { 1.f,0.f,0.f };

	m_Bow_Angle = acos(D3DXVec3Dot(&Axis, &Dir));

	if (WINCY * 0.5 < ptCursor.y)
		m_Bow_Angle = 2.f * D3DX_PI - m_Bow_Angle;

	D3DXMatrixRotationAxis(&matRev, &Player_Axis, -m_Bow_Angle);
	

	m_MatWorld = matScale * matRot * matTrans_Pre * matRev * matTrans;

	m_Pre_Bow_Angle = m_Bow_Angle;
}

HRESULT CSylphBow::Add_Component(void)
{

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Bow_Sylph"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Bow_Sylph", pComponent });


	return S_OK;
}


void CSylphBow::SetUp_OnTerrain(void)
{
	

}

void CSylphBow::Change_State(void)
{
	



}

void CSylphBow::Frame_Check(const _float& fTimeDelta)
{
	
}

CSylphBow* CSylphBow::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _float Angle)
{
	CSylphBow* pInstance = new CSylphBow(pGraphicDev);

	if (pInstance != nullptr) {
		
		pInstance->m_Bow_Pos = vPos;
		pInstance->m_Bow_Angle = Angle;
	}

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}


	return pInstance;
}

void CSylphBow::Free(void)
{
	__super::Free();
}