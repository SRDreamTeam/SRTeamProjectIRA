#include "stdafx.h"
#include "..\Header\SylphArrow.h"
#include "Export_Function.h"

CSylphArrow::CSylphArrow(LPDIRECT3DDEVICE9 pGraphicDev)
	: CArrow(pGraphicDev)
{
}

CSylphArrow::CSylphArrow(const CSylphArrow& rhs)
	: CArrow(rhs)
{
}

CSylphArrow::~CSylphArrow()
{
}

HRESULT CSylphArrow::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale({ 1.f, 1.f, 1.f });

	m_pTransformCom->Rotation(ROT_Y, m_Arrow_Angle);

	m_pTransformCom->Arrow_Move(); // 왜 안먹지? 나중에 다시 해결

	
	m_pTransformCom->Set_Pos(m_Fire_Pos.x, m_Fire_Pos.y - 2.f, m_Fire_Pos.z);

	m_vDir = m_target_Dir;
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	
	
	__super::Ready_GameObject();

	return S_OK;
}

_int CSylphArrow::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_pTransformCom->Move_Pos(&(m_vDir * fTimeDelta * m_fSpeed));

	if (m_iState == ARROW_IDLE) {
		m_fIdleFrame += 5.f * fTimeDelta;
		if (5.f < m_fIdleFrame)
			m_fIdleFrame = 0.f;
	}

	if (m_iState == ARROW_DEATH) {
		m_fDeathFrame += 3.f * fTimeDelta;
		if (3.f < m_fDeathFrame) {
			m_fDeathFrame = 3.f;
			m_bDead = true;
		}
			
	}

	Engine::Add_RenderGroup(RENDER_ALPHA, this);



	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CSylphArrow::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CSylphArrow::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	if (m_iState == ARROW_IDLE) {
		m_pTextureCom[ARROW_IDLE]->Set_Texture((_uint)m_fIdleFrame);
	}
	else if (m_iState == ARROW_DEATH) {
		m_pTextureCom[ARROW_DEATH]->Set_Texture((_uint)m_fDeathFrame);
	}


	m_pTransformCom->m_vInfo[INFO_POS];

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CSylphArrow::Add_Component(void)
{
	
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	

	pComponent = m_pTextureCom[ARROW_IDLE] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Arrow_Sylph_Idle"));
	NULL_CHECK_RETURN(m_pTextureCom[ARROW_IDLE], E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Arrow_Sylph_Idle", pComponent });

	pComponent = m_pTextureCom[ARROW_DEATH] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Arrow_Sylph_Death"));
	NULL_CHECK_RETURN(m_pTextureCom[ARROW_DEATH], E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Arrow_Sylph_Death", pComponent });


	return S_OK;
}


void CSylphArrow::SetUp_OnTerrain(void)
{
	

}

void CSylphArrow::Change_State(void)
{
	



}

void CSylphArrow::Frame_Check(const _float& fTimeDelta)
{
	
}

CSylphArrow* CSylphArrow::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vDir,_float Angle)
{
	CSylphArrow* pInstance = new CSylphArrow(pGraphicDev);

	if (pInstance != nullptr) {
		pInstance->m_target_Dir = vDir;
		pInstance->m_Fire_Pos = vPos;
		pInstance->m_Arrow_Angle = Angle;
	}

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}


	return pInstance;
}

void CSylphArrow::Free(void)
{
	__super::Free();
}