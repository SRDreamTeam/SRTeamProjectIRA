#include "stdafx.h"
#include "..\Header\Doewole.h"
#include "Export_Function.h"
#include "Effect_Doewole_Vanish.h"
#include "Effect_Doewole_StandardAttack.h"
#include "Effect_Doewole_ChargeExplosion.h"
#include "Effect_Doewole_Loof.h"
#include "DoewoleBullet_SwordShot.h"
#include "DoewoleBullet_Circle.h"
#include "Effect_AlertRect.h"
#include "DoewoleBullet_Thorn.h"
#include <Effect_AlertCircle.h>

CDoewole::CDoewole(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBoss(pGraphicDev)
{
}

CDoewole::CDoewole(const CDoewole & rhs)
	: CBoss(rhs)
{
}

CDoewole::~CDoewole()
{
	
}

HRESULT CDoewole::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->m_vInfo[INFO_POS] = { 40.f , 0.f , 40.f };

	return S_OK;
}

_int CDoewole::Update_GameObject(const _float& fTimeDelta)
{
	State_Update(fTimeDelta);

	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CDoewole::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}


void CDoewole::Idle(const _float& fTimeDelta)
{
	m_fAccTime += fTimeDelta;

	if(1 == m_iPattern)
	{ 
		if (m_fAccTime > 1.f)
		{
			m_fAccTime = 0.f;
			m_eCurState = MOVE;
			++m_iPattern;
		
			// ������� ����Ʈ ����
			CGameObject* pEffect = CEffect_Doewole_Vanish::Create(m_pGraphicDev);
			NULL_CHECK(pEffect);
			CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
			pLayer->Add_BulletObject(OBJ_NONE, pEffect);
		}
		
	}

	else if (2 == m_iPattern)
	{
		if (m_fAccTime > 1.f)
		{
			m_fAccTime = 0.f;
			m_eCurState = STANDARD_ATTACK;
			++m_iPattern;
		}
	}

	else if (3 == m_iPattern)
	{
		if (m_fAccTime > 1.f)
		{
			m_fAccTime = 0.f;
			m_eCurState = OUTSTRECTH_ATTACK;
			++m_iPattern;
		}
	}

	else if (4 == m_iPattern)
	{
		if (m_fAccTime > 1.f)
		{
			m_fAccTime = 0.f;
			m_eCurState = SMASH_ATTACK;
			++m_iPattern;
		}
	}

	else if (5 == m_iPattern)
	{
		if (m_fAccTime > 1.f)
		{
			m_fAccTime = 0.f;
			m_eCurState = SCRATCH_ATTACK;
			++m_iPattern;
		}
	}

	else if (6 == m_iPattern)
	{
		if (m_fAccTime > 1.f)
		{
			m_fAccTime = 0.f;
			m_eCurState = AREA_ATTACK;
			++m_iPattern;
		}
	}

}

void CDoewole::Move(const _float& fTimeDelta)
{
	m_fAccTime += fTimeDelta;

	if(m_fAccTime > 1.f)
		m_pTransformCom->m_vInfo[INFO_POS].x -= m_fMoveSpeed * fTimeDelta;

	if (m_fAccTime > 3.f)
	{
		m_fAccTime = 0.f;
		m_eCurState = IDLE;
		m_fMoveSpeed *= -1.f;
	}
}

void CDoewole::Standard_Attack(const _float& fTimeDelta)
{
	m_fAccTime += fTimeDelta;
	
	if (m_iStandardAttackCnt < 2)
	{
		if (m_fAccTime > 1.f) // 1�ʰ������� ���
		{
			m_fAccTime = 0.f;

			// Standard Bullet ����
			Create_StandardBullet();

			// Standard_Attack ����Ʈ ����
			CGameObject* pEffect = CEffect_Doewole_StandardAttack::Create(m_pGraphicDev);
			NULL_CHECK(pEffect);
			CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
			pLayer->Add_BulletObject(OBJ_NONE, pEffect);

			++m_iStandardAttackCnt;
			
		}
	}
	else if(m_iStandardAttackCnt >=2 && m_fAccTime > 1.f)
	{
		m_bAttackToIdle = true;

		if (m_fAccTime > 2.f)
		{
			m_eCurState = IDLE;
			m_fAccTime = 0.f;
			m_bAttackToIdle = false;
		}
	}
}

void CDoewole::OutStretch_Attack(const _float& fTimeDelta)
{
	m_fAccTime += fTimeDelta;
	m_fAccTime2 += fTimeDelta;

	if (!m_bEffect)
	{
		// ChargeExplosion ����Ʈ ����
		CGameObject* pEffect = CEffect_Doewole_ChargeExplosion::Create(m_pGraphicDev);
		NULL_CHECK(pEffect);
		CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
		pLayer->Add_BulletObject(OBJ_NONE, pEffect);

		// Loof ����Ʈ ����
		pEffect = CEffect_Doewole_Loof::Create(m_pGraphicDev);
		NULL_CHECK(pEffect);
		pLayer->Add_BulletObject(OBJ_NONE, pEffect);

		m_bEffect = true;
	}

	if (m_fAccTime > 4.f)
	{
		m_bAttackToIdle = true;

		if (m_fAccTime > 5.f)
		{
			m_eCurState = IDLE;
			m_fAccTime = 0.f;
			m_bAttackToIdle = false;
		}
	}

	if (m_fAccTime2 > 0.5f)
	{
		Create_CircleBullet();
		m_fAccTime2 = 0.f;
	}
}

void CDoewole::Smash_Attack(const _float& fTimeDelta)
{

}

void CDoewole::Scratch_Attack(const _float& fTimeDelta)
{
	m_fAccTime += fTimeDelta;

	if (!m_bDisappear)
	{
		if (m_fAccTime > 2.f)
		{
			if (!m_bChasePlayer)
			{
				// �÷��̾� ���� �̵�
				CTransform* pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
				NULL_CHECK(pPlayerTransform);
				m_pTransformCom->m_vInfo[INFO_POS] = { pPlayerTransform->m_vInfo[INFO_POS].x , 0.f ,pPlayerTransform->m_vInfo[INFO_POS].z + 15.f };
				m_bChasePlayer = true;

				// AlertRect ����Ʈ ����
				_vec3 vScale;
				if (!m_bRight) vScale = { -30.f , 30.f,30.f };
				else vScale = { 30.f , 30.f,30.f };

				CGameObject* pEffect = CEffect_AlertRect::Create(m_pGraphicDev , vScale);
				NULL_CHECK(pEffect);
				CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
				if(!m_bRight) pLayer->Add_GameObject(L"AlertRect_Right", pEffect, OBJ_NONE);
				else pLayer->Add_GameObject(L"AlertRect_Left", pEffect, OBJ_NONE);
							
				m_bRight = !m_bRight;
			}
		}

		if (m_fAccTime > 3.f)
		{
			m_fAccTime = 0.f;
			m_bDisappear = true;
			m_bChasePlayer = false;
		}
	}
}

void CDoewole::Area_Attack(const _float& fTimeDelta)
{
	m_fAccTime += fTimeDelta;
	m_bDisappear = true;

	_vec3	vDestPos = { 128.f , 0.f, 128.f };
	_vec3 vDir;

	if (!m_bAreaAttack)
	{
		if (m_fAccTime > 1.f)
		{
			// �� �߾����� �̵�
			vDir = vDestPos - m_pTransformCom->m_vInfo[INFO_POS];
			D3DXVec3Normalize(&vDir, &vDir);
			m_pTransformCom->m_vInfo[INFO_POS] += vDir * 30.f * fTimeDelta;

			if (D3DXVec3Length(&(vDestPos - m_pTransformCom->m_vInfo[INFO_POS])) < 0.3f)
			{
				m_pTransformCom->m_vInfo[INFO_POS] = vDestPos;
				m_bAreaAttack = true;
			}
		}
	}
	else
	{
		AreaAtaackPattern(fTimeDelta);
	}
}

void CDoewole::Create_StandardBullet()
{
	_vec3 vMonster_Pos = (m_pTransformCom->m_vInfo[INFO_POS]);
	vMonster_Pos.y += 1.f;

	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CGameObject* pBulletObject = nullptr;

	for (size_t i = 0; i < 8; i++)
	{
		pBulletObject = CDoewoleBullet_SwordShot::Create(m_pGraphicDev, vMonster_Pos, (i + 1));
		NULL_CHECK(pBulletObject);
		pLayer->Add_BulletObject(OBJ_NONE, pBulletObject);
	}
}

void CDoewole::Create_CircleBullet()
{
	_vec3 vMonster_Pos = (m_pTransformCom->m_vInfo[INFO_POS]);
	vMonster_Pos.y += 1.f;

	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CGameObject* pBulletObject = nullptr;

	for (size_t i = 0; i < 8; i++)
	{
		pBulletObject = CDoewoleBullet_Circle::Create(m_pGraphicDev, vMonster_Pos, (i + 1));
		NULL_CHECK(pBulletObject);
		pLayer->Add_BulletObject(OBJ_NONE, pBulletObject);
	}
}

void CDoewole::AreaAtaackPattern(const _float& fTimeDelta)
{
	_vec3 vPos;
	_vec3 vPos2;

	if (!m_bTest)
	{
		for (int i = 0; i < 250; ++i)
		{
			vPos = { (_float)i , 5, CalculateZ((_float)i) };
			vPos2 = { vPos.x , 0.2f , vPos.z };

			CGameObject* pBulletObject = nullptr;
			CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

			// ������ ����
			pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos, _vec3(-5.f, 6.f, 5.f));
			NULL_CHECK(pBulletObject);
			pLayer->Add_BulletObject(OBJ_NONE, pBulletObject);

			// ���� ����
			pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos, _vec3(5.f, 5.f, 5.f));
			NULL_CHECK(pBulletObject);
			pLayer->Add_BulletObject(OBJ_NONE, pBulletObject);

			// AlertCircle ����Ʈ ����
			CGameObject* pEffect = CEffect_AlertCircle::Create(m_pGraphicDev, vPos2, _vec3(7.f, 7.f, 7.f), 2.f, FALSE);
			NULL_CHECK(pEffect);
			pLayer->Add_BulletObject(OBJ_NONE, pEffect);
		}
		m_bTest = true;
	}
}

HRESULT CDoewole::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

void CDoewole::State_Update(const _float& fTimeDelta)
{
	switch (m_eCurState)
	{
	case CDoewole::IDLE:
		Idle(fTimeDelta);
		break;
	case CDoewole::MOVE:
		Move(fTimeDelta);
		break;
	case CDoewole::STANDARD_ATTACK:
		Standard_Attack(fTimeDelta);
		break;
	case CDoewole::OUTSTRECTH_ATTACK:
		OutStretch_Attack(fTimeDelta);
		break;
	case CDoewole::SMASH_ATTACK:
		Smash_Attack(fTimeDelta);
		break;
	case CDoewole::SCRATCH_ATTACK:
		Scratch_Attack(fTimeDelta);
		break;
	case CDoewole::AREA_ATTACK:
		Area_Attack(fTimeDelta);
		break;
	case CDoewole::STATE_END:
		break;
	default:
		break;
	}

	if (m_iPattern > m_iMaxPattern)
		m_iPattern = 0;
}
_float CDoewole::CalculateZ(_float fX)
{
	if (fX <= 128.f)
		return (- 1 / 128.f)* fX* fX + 2.f * fX;
	else
		return 0.f;

	
}
CDoewole * CDoewole::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDoewole * pInstance = new CDoewole(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDoewole::Free(void)
{
	__super::Free();
}


