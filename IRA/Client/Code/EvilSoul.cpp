#include "stdafx.h"
#include "..\Header\EvilSoul.h"
#include "Export_Function.h"
#include "MonsterBullet.h"

#include "Effect_Monster_Dead_1.h"

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

	m_fSpeed = 3.f;
	m_pTransformCom->Set_Pos(_float(rand() % 100), 1.f, _float(rand() % 100));
	m_pTransformCom->UpdatePos_OnWorld();

	return S_OK;
}

_int CEvilSoul::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
	{
		Create_Dead_Effect();
		return OBJ_DEAD;
	}

	Frame_Check(fTimeDelta);
	SetUp_OnTerrain();
	__super::Update_GameObject(fTimeDelta);
	
	// 준석 수정 (23.03.02) : Layer_Environment 에서 Layer_GameLogic 으로 정정
	CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTransformCom, -1);
	_vec3	vPlayerPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

	//if (MONSTER_IDLE == m_eState)
		m_pTransformCom->Chase_Target(&vPlayerPos, m_fSpeed, fTimeDelta, m_eName);

	_vec3	vDir = vPlayerPos - m_pTransformCom->m_vInfo[INFO_POS];

	if (((vDir.x > -15.0f) && (vDir.x < 15.0f)) && ((vDir.z > -15.0f) && (vDir.z < 15.0f)) && !m_bCheck)
		Change_State();

	Head_Check(vDir);
	Bullet_Test();	

	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

	return OBJ_NOEVENT;
}

void CEvilSoul::LateUpdate_GameObject()
{	
	if (GetAsyncKeyState('8') & 0x8000)
	{
		m_bDead = true;
	}
	if (GetAsyncKeyState('9'))
	{

	}

	__super::LateUpdate_GameObject();
}

void CEvilSoul::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

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
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
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
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

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
	m_pTransformCom->Set_Pos(vPos.x, 1.f, vPos.z);
}

void CEvilSoul::Change_State(void)
{
	if (0.f == m_fFrame)
	{
		m_eState = MONSTER_ATTACK;
		Create_Bullet();
	}
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

void CEvilSoul::Bullet_Test(void)
{	
	if ((GetAsyncKeyState('K') & 0x8000))
	{
		Create_Bullet();
	}
}

HRESULT CEvilSoul::Create_Bullet(void)
{	
	_vec3 vMonster_Pos = (m_pTransformCom->m_vInfo[INFO_POS]);
	CGameObject*	pGameObject = nullptr;
	CGameObject* pBulletObject = CMonsterBullet::Create(m_pGraphicDev, vMonster_Pos, true);
	NULL_CHECK_RETURN(pBulletObject, -1);
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
	pLayer->Add_BulletObject(OBJ_BULLET, pBulletObject);

	return S_OK;
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

void CEvilSoul::Create_Dead_Effect(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject = nullptr;

	pGameObject = CEffect_Monster_Dead_1::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);

	if (pGameObject == nullptr)
		return;

	pGameLogicLayer->Add_BulletObject(OBJ_NONE, pGameObject);
}

void CEvilSoul::Free(void)
{
	__super::Free();
}