#include "stdafx.h"
#include "..\Header\EvilSoul.h"
#include "Export_Function.h"

CEvilSoul::CEvilSoul(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev), m_pTextureCom_135_1(nullptr), m_pTextureCom_135_2(nullptr), m_eHead(HEAD_FRONT)
{
}

CEvilSoul::CEvilSoul(const CEvilSoul& rhs)
	: CMonster(rhs), m_pTextureCom_135_1(nullptr), m_pTextureCom_135_2(nullptr), m_eHead(HEAD_FRONT)
{
}

CEvilSoul::~CEvilSoul()
{
}


HRESULT CEvilSoul::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_eName = NAME_SOUL;

	m_pTransformCom->Set_Pos(rand() % 50, 1.f, rand() % 50);
	m_pTransformCom->UpdatePos_OnWorld();

	return S_OK;
}

_int CEvilSoul::Update_GameObject(const _float& fTimeDelta)
{
	// 준석 수정 (23.03.02)
	Frame_Check(fTimeDelta);
	SetUp_OnTerrain();
	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	// 준석 수정 (23.03.02) : Layer_Environment 에서 Layer_GameLogic 으로 정정
	CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTransformCom, -1);
	_vec3	vPlayerPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

	//if (MONSTER_IDLE == m_eState)
		m_pTransformCom->Chase_Target(&vPlayerPos, m_fSpeed, fTimeDelta, m_eName);

	_vec3	vDir = vPlayerPos - m_pTransformCom->m_vInfo[INFO_POS];

	if (((vDir.x > -3.0f) && (vDir.x < 3.0f)) && ((vDir.z > -3.0f) && (vDir.z < 3.0f)) && !m_bCheck)
		Change_State();

	Head_Check(vDir);

	

	return 0;
}

void CEvilSoul::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CEvilSoul::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (HEAD_FRONT == m_eHead)
	{
		switch (m_eState)
		{
		case MONSTER_IDLE:
			m_pTextureCom->Set_Texture((_uint)m_fFrame);
			break;
		case MONSTER_MOVE:
			m_pTextureCom_2->Set_Texture((_uint)m_fFrame);
			break;
		case MONSTER_ATTACK:
			m_pTextureCom->Set_Texture((_uint)m_fFrame);
			break;
		case MONSTER_END:
			break;
		default:
			break;
		}
	}
	else if (HEAD_BACK == m_eHead)
	{
		switch (m_eState)
		{
		case MONSTER_IDLE:
			m_pTextureCom_135_1->Set_Texture((_uint)m_fFrame);
			break;
		case MONSTER_MOVE:
			m_pTextureCom_135_2->Set_Texture((_uint)m_fFrame);
			break;
		case MONSTER_ATTACK:
			m_pTextureCom_135_1->Set_Texture((_uint)m_fFrame);
			break;
		case MONSTER_END:
			break;
		default:
			break;
		}
	}

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CEvilSoul::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	//45 IDLE, ATTACK
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_EvilSoul_45_Idle"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_EvilSoul_45_Idle", pComponent });
	//45 MOVE
	pComponent = m_pTextureCom_2 = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_EvilSoul_45_Move"));
	NULL_CHECK_RETURN(m_pTextureCom_2, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_EvilSoul_45_Move", pComponent });

	//135 IDLE, ATTACK
	pComponent = m_pTextureCom_135_1 = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_EvilSoul_135_Idle"));
	NULL_CHECK_RETURN(m_pTextureCom_135_1, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_EvilSoul_135_Idle", pComponent });
	//135 MOVE
	pComponent = m_pTextureCom_135_2 = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_EvilSoul_135_Move"));
	NULL_CHECK_RETURN(m_pTextureCom_135_2, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_EvilSoul_135_Move", pComponent });

	return S_OK;
}

// 준석 수정 (23.03.02) : Player에서 복사해와서 그대로 사용함
void CEvilSoul::SetUp_OnTerrain(void)
{
	_vec3		vPos;

	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTex", ID_STATIC));
	NULL_CHECK(pTerrainBufferCom);

	_float	fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	// 준석 수정 (23.03.02) : 테스트용 추가 
	fHeight += 1.f;
	m_pTransformCom->Set_Pos(vPos.x, fHeight, vPos.z);

}

void CEvilSoul::Change_State(void)
{
	if (0.f == m_fFrame)
		m_eState = MONSTER_ATTACK;
}

void CEvilSoul::Frame_Check(const _float& fTimeDelta)
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

	if (m_eState == MONSTER_MOVE)
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
		m_fFrame += 7.f * fTimeDelta;

		if (7.f < m_fFrame)
		{
			m_fFrame = 0.f;
			m_eState = MONSTER_IDLE;
			m_bCheck = true;
		}
	}
}

void CEvilSoul::Head_Check(const _vec3 vDir)
{
	if (0.f < vDir.z)	// 플레이어가 몬스터의 위(뒤)쪽에 위치
		m_eHead = HEAD_BACK;
	else if (0.f > vDir.z)	// 플레이어가 몬스터의 아래(앞)쪽에 위치
		m_eHead = HEAD_FRONT;
	
	if (0.f < vDir.x)
	{
		m_pTransformCom->Reverse_Scale_x();
	}
}

CEvilSoul* CEvilSoul::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEvilSoul* pInstance = new CEvilSoul(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEvilSoul::Free(void)
{
	__super::Free();
}