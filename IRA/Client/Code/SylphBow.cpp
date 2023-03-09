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

	m_pTransformCom->Set_Scale_Ratio({ 1.5f, 1.5f, 1.5f });

	m_pTransformCom->Rotation(ROT_Y, m_Bow_Angle);

	m_pTransformCom->Set_Pos(m_Bow_Pos.x, m_Bow_Pos.y, m_Bow_Pos.z);

	
	
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
		
	



	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pTextureCom->Set_Texture(0);

	m_pTransformCom->m_vInfo[INFO_POS];

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CSylphBow::Update_Bow_State()
{
	POINT ptCursor;

	GetCursorPos(&ptCursor);
	ScreenToClient(g_hWnd, &ptCursor);

	_vec3 Axis = { -1.f,0.f,0.f };

	_vec3 Dir = { (float)(ptCursor.x - WINCX * 0.5),float(ptCursor.y - WINCY * 0.5),0 };
	D3DXVec3Normalize(&Dir, &Dir);


	// Angle °è»ê
	m_Bow_Angle = acos(D3DXVec3Dot(&Axis, &Dir));

	if (WINCY * 0.5 < ptCursor.y)
		m_Bow_Angle = 2.f * D3DX_PI - m_Bow_Angle;


	CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
	pPlayerTransformCom->Get_Info(INFO_POS, &m_Bow_Pos);

	m_pTransformCom->Set_Pos(m_Bow_Pos.x + 4.f, m_Bow_Pos.y + 2.2f + 1.f, m_Bow_Pos.z - 2.f);



	if (m_Bow_Angle == m_Pre_Bow_Angle)
		return;

	m_pTransformCom->Rotation(ROT_Y, m_Bow_Angle - m_Pre_Bow_Angle);

	
	_matrix		matRevol;
	_vec3       Player_Axis;
	pPlayerTransformCom->Get_Info(INFO_UP, &Player_Axis);
	D3DXMatrixRotationAxis(&matRevol, &Player_Axis, m_Bow_Angle - m_Pre_Bow_Angle);

	m_pTransformCom->Set_Revolution(matRevol);

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