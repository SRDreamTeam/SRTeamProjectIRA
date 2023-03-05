#include "stdafx.h"
#include "..\Header\GhostChild.h"
#include "Export_Function.h"
#include "Player.h"


CGhostChild::CGhostChild(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGhost(pGraphicDev),m_iAngleState(0)
{
}

CGhostChild::CGhostChild(const CGhostChild& rhs)
	:CGhost(rhs), m_iAngleState(rhs.m_iAngleState)
{
}

CGhostChild::~CGhostChild()
{
	Free();
}

HRESULT CGhostChild::Ready_GameObject(void)
{
	
	__super::Ready_GameObject();

	m_pTransformCom->Set_Scale({ 5.f, 5.f, 1.f });


	CTransform* pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));

	if (pPlayer != nullptr) {
		m_Sprite = pPlayer->Get_Dash_Frame();
		m_iAngleState = pPlayer->Get_Anlge_State();
	}

	_vec3 vPos;

	if (pPlayerTransform != nullptr) {
		pPlayerTransform->Get_Info(INFO_POS, &vPos);
		m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	}


	
	return S_OK;
}

_int CGhostChild::Update_GameObject(const _float& fTimeDelta)
{

	if (m_bDead)
		return OBJ_DEAD;

	

	m_Alive_Time += 500.f * fTimeDelta;

	if (m_Alive_Time > 500.f) {
		//m_bDead = true;
	}

	Engine::Add_RenderGroup(RENDER_ALPHA, this);


	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CGhostChild::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CGhostChild::Render_GameObject()
{
	
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (m_bRender == true) {
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());

		if (m_iAngleState == ANGLE_000) {
			m_pDashTextureCom[DASH_045]->Set_Texture((_uint)m_Sprite);
		}
		else if (m_iAngleState == ANGLE_045) {
			m_pDashTextureCom[DASH_045]->Set_Texture((_uint)m_Sprite);
		}
		else if (m_iAngleState == ANGLE_090) {
			m_pDashTextureCom[DASH_045]->Set_Texture((_uint)m_Sprite);
		}
		else if (m_iAngleState == ANGLE_135) {
			m_pDashTextureCom[DASH_045]->Set_Texture((_uint)m_Sprite);
		}
		else if (m_iAngleState == ANGLE_180) {
			m_pDashTextureCom[DASH_135]->Set_Texture((_uint)m_Sprite);
		}
		else if (m_iAngleState == ANGLE_225) {
			m_pDashTextureCom[DASH_135]->Set_Texture((_uint)m_Sprite);
		}
		else if (m_iAngleState == ANGLE_270) {
			m_pDashTextureCom[DASH_135]->Set_Texture((_uint)m_Sprite);
		}
		else if (m_iAngleState == ANGLE_315) {
			m_pDashTextureCom[DASH_135]->Set_Texture((_uint)m_Sprite);
		}

		m_pBufferCom->Render_Buffer();
	}
	

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CGhostChild* CGhostChild::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGhostChild* pInstance = new CGhostChild(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CGhostChild::Free(void)
{
	__super::Free();
}
