#include "stdafx.h"
#include "..\Header\GreenEvilSlime.h"
#include "Export_Function.h"

CGreenEvilSlime::CGreenEvilSlime(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev)
{
}

CGreenEvilSlime::CGreenEvilSlime(const CGreenEvilSlime& rhs)
	: CMonster(rhs)
{
}

CGreenEvilSlime::~CGreenEvilSlime()
{
}

HRESULT CGreenEvilSlime::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_eName = NAME_SLIME;

	m_pTransformCom->Set_Pos(rand() % 100, 1.f, rand() % 100);
	m_pTransformCom->UpdatePos_OnWorld();
	m_fSpeed = 3.f;

	return S_OK;
}

_int CGreenEvilSlime::Update_GameObject(const _float& fTimeDelta)
{
	Frame_Check(fTimeDelta);
	SetUp_OnTerrain();
	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTransformCom, -1);
	_vec3	vPlayerPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

	if (MONSTER_IDLE == m_eState)
		m_pTransformCom->Chase_Target(&vPlayerPos, m_fSpeed, fTimeDelta, m_eName);

	_vec3	vDir = vPlayerPos - m_pTransformCom->m_vInfo[INFO_POS];

	if (((vDir.x > -2.0f) && (vDir.x < 2.0f)) && ((vDir.z > -2.0f) && (vDir.z < 2.0f)) && !m_bCheck)
		Change_State();

	return 0;
}

void CGreenEvilSlime::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CGreenEvilSlime::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	switch (m_eState)
	{
	case MONSTER_IDLE:
		m_pTextureCom->Set_Texture((_uint)m_fFrame);
		break;
	case MONSTER_MOVE:
		break;
	case MONSTER_ATTACK:
		m_pTextureCom_2->Set_Texture((_uint)m_fFrame);
		break;
	case MONSTER_END:
		break;
	default:
		break;
	}
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CGreenEvilSlime::Add_Component(void)
{
	// 준석 수정 (23.03.02)
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_GreenEvilSlime_Move"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_GreenEvilSlime_Move", pComponent });

	pComponent = m_pTextureCom_2 = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_GreenEvilSlime_Attack"));
	NULL_CHECK_RETURN(m_pTextureCom_2, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_GreenEvilSlime_Attack", pComponent });

	return S_OK;
}

// 준석 수정 (23.03.02) : Player에서 복사해와서 그대로 사용함
void CGreenEvilSlime::SetUp_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTex", ID_STATIC));
	NULL_CHECK(pTerrainBufferCom);
	m_pTransformCom->Set_Pos(vPos.x, 1.f, vPos.z);
}

void CGreenEvilSlime::Change_State(void)
{
	if (0.f == m_fFrame)
		m_eState = MONSTER_ATTACK;
}

void CGreenEvilSlime::Frame_Check(const _float& fTimeDelta)
{
	if (m_eState == MONSTER_IDLE)
	{
		m_fFrame += 7.f * fTimeDelta;

		if (7.f < m_fFrame)
		{
			m_fFrame = 0.f;
			m_bCheck = false;
		}
	}

	if (m_eState == MONSTER_ATTACK)
	{
		m_fFrame += 9.f * fTimeDelta;

		if (9.f < m_fFrame)
		{
			m_fFrame = 0.f;
			m_eState = MONSTER_IDLE;
			m_bCheck = true;
		}
	}
}

CGreenEvilSlime* CGreenEvilSlime::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGreenEvilSlime* pInstance = new CGreenEvilSlime(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CGreenEvilSlime::Free(void)
{
	__super::Free();
}