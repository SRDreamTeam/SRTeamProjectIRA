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
#include "CollisionMgr.h"

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

	m_pTransformCom->m_vInfo[INFO_POS] = { 100.f , 0.f , 100.f };

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
		
			// 사라지는 이펙트 생성
			CGameObject* pEffect = CEffect_Doewole_Vanish::Create(m_pGraphicDev);
			NULL_CHECK(pEffect);
			CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
			pLayer->Add_BulletObject(pEffect);
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

	else if (7 == m_iPattern)
	{
		if (m_fAccTime > 3.f)
		{
			m_fAccTime = 0.f;
			m_eCurState = MOVE;
			++m_iPattern;
		}
	}

	else if (8 == m_iPattern)
	{
		if (m_fAccTime > 3.f)
		{
			m_fAccTime = 0.f;
			m_eCurState = UPGRADE_SMASH_ATTACK;
			++m_iPattern;
		}
	}
}

void CDoewole::Move(const _float& fTimeDelta)
{
	m_fAccTime += fTimeDelta;

	// 플레이어 근처로 이동한다
	CTransform* pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransform);

	_vec3 vDiff = pPlayerTransform->m_vInfo[INFO_POS] - m_pTransformCom->m_vInfo[INFO_POS];
	_vec3 vDir;
	D3DXVec3Normalize(&vDir, &vDiff);

	if (D3DXVec3Length(&vDiff) > 20.f && !m_bChasePlayer)
		m_pTransformCom->m_vInfo[INFO_POS] += vDir * m_fMoveSpeed * fTimeDelta;
	else
	{
		if (!m_bChasePlayer)
		{
			m_bChasePlayer = true;
			m_fAccTime = 0.f;
		}

		if (m_fAccTime > 1.f)
		{
			m_eCurState = IDLE;
			m_fAccTime = 0.f;
			m_bChasePlayer = false;
		}
	}
}

void CDoewole::Standard_Attack(const _float& fTimeDelta)
{
	m_fAccTime += fTimeDelta;
	
	if (m_iStandardAttackCnt < 2)
	{
		if (m_fAccTime > 1.f) // 1초간격으로 쏜다
		{
			m_fAccTime = 0.f;

			// Standard Bullet 생성
			Create_StandardBullet();

			// Standard_Attack 이펙트 생성
			CGameObject* pEffect = CEffect_Doewole_StandardAttack::Create(m_pGraphicDev);
			NULL_CHECK(pEffect);
			CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
			pLayer->Add_BulletObject(pEffect);

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
		// ChargeExplosion 이펙트 생성
		CGameObject* pEffect = CEffect_Doewole_ChargeExplosion::Create(m_pGraphicDev);
		NULL_CHECK(pEffect);
		CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
		pLayer->Add_BulletObject(pEffect);

		// Loof 이펙트 생성
		pEffect = CEffect_Doewole_Loof::Create(m_pGraphicDev);
		NULL_CHECK(pEffect);
		pLayer->Add_BulletObject(pEffect);

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
			m_fAccTime2 = 0.f;
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
				// 플레이어 한테 이동
				CTransform* pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
				NULL_CHECK(pPlayerTransform);
				m_pTransformCom->m_vInfo[INFO_POS] = { pPlayerTransform->m_vInfo[INFO_POS].x , 0.f ,pPlayerTransform->m_vInfo[INFO_POS].z + 15.f };
				m_bChasePlayer = true;

				// AlertRect 이펙트 생성
				_vec3 vScale;
				if (!m_bRight) vScale = { -30.f , 30.f,30.f };
				else vScale = { 30.f , 30.f,30.f };

				CGameObject* pEffect = CEffect_AlertRect::Create(m_pGraphicDev , vScale);
				NULL_CHECK(pEffect);
				CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
				if (!m_bRight)
					pLayer->Add_GameObject(L"AlertRect_Right", pEffect);
				else 
					pLayer->Add_GameObject(L"AlertRect_Left", pEffect);
							
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
	m_fAccTime2 += fTimeDelta;
	m_bDisappear = false;

	_vec3	vDestPos = { 128.f , 0.f, 128.f };
	_vec3 vDir;

	if (!m_bAreaAttack)
	{
		if (m_fAccTime > 1.f)
		{
			// 맵 중앙으로 이동
			vDir = vDestPos - m_pTransformCom->m_vInfo[INFO_POS];
			D3DXVec3Normalize(&vDir, &vDir);
			m_pTransformCom->m_vInfo[INFO_POS] += vDir * 30.f * fTimeDelta;

			if (D3DXVec3Length(&(vDestPos - m_pTransformCom->m_vInfo[INFO_POS])) < 0.3f)
			{
				m_pTransformCom->m_vInfo[INFO_POS] = vDestPos;
				m_bAreaAttack = true;
				m_fAccTime = 0.f;
			}
		}
	}
	else
	{
		AreaAtaackPattern(fTimeDelta);
	}
}

void CDoewole::Upgrade_Smash_Attack(const _float& fTimeDelta)
{
	m_fAccTime += fTimeDelta;

	m_bDisappear = true;
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
		pLayer->Add_BulletObject(  pBulletObject);
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
		pLayer->Add_BulletObject(  pBulletObject);
	}
}

void CDoewole::AreaAtaackPattern(const _float& fTimeDelta)
{
	_vec3 vPos;
	_vec3 vPos2;
	_vec3 vPos3;
	
	if (m_iThornCnt <= 4)
	{
		if (m_fAccTime > 0.03f)
		{
			if (!m_bTest)
			{
				for (int i = 0; i < 256; i += 12)
				{
					vPos = { (_float)i , 5.f, m_fThornZ };
					vPos2 = { vPos.x - 1.f , 5.f , vPos.z };
					vPos3 = { vPos.x , 0.5f , vPos.z };

					CGameObject* pBulletObject = nullptr;
					CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

					// 오른쪽 가시
					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos, _vec3(-5.f, 6.f, 5.f));
					NULL_CHECK(pBulletObject);
					pLayer->Add_BulletObject(  pBulletObject);

					// 왼쪽 가시
					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos2, _vec3(5.f, 5.f, 5.f));
					NULL_CHECK(pBulletObject);
					pLayer->Add_BulletObject(  pBulletObject);

					// AlertCircle 이펙트 생성
					CGameObject* pEffect = CEffect_AlertCircle::Create(m_pGraphicDev, vPos3, _vec3(5.f, 5.f, 5.f), 1.5f, FALSE);
					NULL_CHECK(pEffect);
					pLayer->Add_BulletObject(  pEffect);

					m_fAccTime = 0.f;
				}

				m_fThornZ += 6.f;

				if (m_fThornZ >= 256.f)
				{
					m_bTest = true;
					m_fThornZ = 3.f;
					m_iThornCnt++;
				}
			}
			else
			{
				for (int i = 0; i < 256; i += 12)
				{
					vPos = { (_float)i + 6.f , 5, 256.f - m_fThornZ };
					vPos2 = { vPos.x - 1.f , 5 , vPos.z };
					vPos3 = { vPos.x , 0.2f , vPos.z };

					CGameObject* pBulletObject = nullptr;
					CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

					// 오른쪽 가시
					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos, _vec3(-5.f, 6.f, 5.f));
					NULL_CHECK(pBulletObject);
					pLayer->Add_BulletObject(  pBulletObject);

					// 왼쪽 가시
					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos2, _vec3(5.f, 5.f, 5.f));
					NULL_CHECK(pBulletObject);
					pLayer->Add_BulletObject(  pBulletObject);

					// AlertCircle 이펙트 생성
					CGameObject* pEffect = CEffect_AlertCircle::Create(m_pGraphicDev, vPos3, _vec3(5.f, 5.f, 5.f), 1.5f, FALSE);
					NULL_CHECK(pEffect);
					pLayer->Add_BulletObject(  pEffect);

					m_fAccTime = 0.f;
					m_fAccTime2 = 0.f;
				}
				m_fThornZ += 6.f;

				if (m_fThornZ >= 256.f)
				{
					m_bTest = false;
					m_fThornZ = 0.f;
					m_iThornCnt++;
				}
			}
		}
	}

	if (m_iThornCnt >= 5)
	{
		m_bDisappear = true;

		if (m_fAccTime > 2.f)
		{
			m_fAccTime2 += fTimeDelta;

			if (m_fAccTime2 > 0.04f)
			{
				if (!m_bCrossTron)
				{
					// 좌
					vPos = { m_fThronX , 5.f, 128.f };
					vPos2 = { vPos.x - 1.f , 5.f , vPos.z };
					vPos3 = { vPos.x , 0.2f , vPos.z };

					CGameObject* pBulletObject = nullptr;
					CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

					// 오른쪽 가시
					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos, _vec3(-5.f, 6.f, 5.f)); pLayer->Add_BulletObject(  pBulletObject);
					// 왼쪽 가시
					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos2, _vec3(5.f, 5.f, 5.f));NULL_CHECK(pBulletObject); pLayer->Add_BulletObject(  pBulletObject);
					// AlertCircle 이펙트 생성
					CGameObject* pEffect = CEffect_AlertCircle::Create(m_pGraphicDev, vPos3, _vec3(5.f, 5.f, 5.f), 1.5f, FALSE); pLayer->Add_BulletObject(  pEffect);

					// 우
					vPos = { 256.f - m_fThronX , 5.f, 128.f }; vPos2 = { vPos.x - 1.f , 5.f , vPos.z }; vPos3 = { vPos.x , 0.2f , vPos.z };

					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos, _vec3(-5.f, 6.f, 5.f));
					pLayer->Add_BulletObject(  pBulletObject);

					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos2, _vec3(5.f, 5.f, 5.f));
					pLayer->Add_BulletObject(  pBulletObject);

				
					pEffect = CEffect_AlertCircle::Create(m_pGraphicDev, vPos3, _vec3(5.f, 5.f, 5.f), 1.5f, FALSE);
					pLayer->Add_BulletObject(  pEffect);
					// 아래
					vPos = { 128.f, 5.f, m_fThronX };vPos2 = { vPos.x - 1.f , 5.f , vPos.z };vPos3 = { vPos.x , 0.2f , vPos.z };

					// 오른쪽 가시
					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos, _vec3(-5.f, 6.f, 5.f));pLayer->Add_BulletObject(  pBulletObject);
					// 왼쪽 가시
					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos2, _vec3(5.f, 5.f, 5.f));pLayer->Add_BulletObject(  pBulletObject);
					// AlertCircle 이펙트 생성
					pEffect = CEffect_AlertCircle::Create(m_pGraphicDev, vPos3, _vec3(5.f, 5.f, 5.f), 1.5f, FALSE);
					pLayer->Add_BulletObject(  pEffect);

					// 위
					vPos = { 128.f, 5.f, (256.f-m_fThronX) };
					vPos2 = { vPos.x - 1.f , 5.f , vPos.z };
					vPos3 = { vPos.x , 0.2f , vPos.z };

					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos, _vec3(-5.f, 6.f, 5.f));
					pLayer->Add_BulletObject(  pBulletObject);

					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos2, _vec3(5.f, 5.f, 5.f));
					pLayer->Add_BulletObject(  pBulletObject);

			
					pEffect = CEffect_AlertCircle::Create(m_pGraphicDev, vPos3, _vec3(5.f, 5.f, 5.f), 1.5f, FALSE);
					pLayer->Add_BulletObject(  pEffect);

					// 우상
					vPos = { (256.f - m_fThronX), 5.f, (256.f - m_fThronX) }; vPos2 = { vPos.x - 1.f , 5.f , vPos.z }; vPos3 = { vPos.x , 0.2f , vPos.z };
					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos, _vec3(-5.f, 6.f, 5.f));pLayer->Add_BulletObject(  pBulletObject);
					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos2, _vec3(5.f, 5.f, 5.f));pLayer->Add_BulletObject(  pBulletObject);
					pEffect = CEffect_AlertCircle::Create(m_pGraphicDev, vPos3, _vec3(5.f, 5.f, 5.f), 1.5f, FALSE); pLayer->Add_BulletObject(  pEffect);

					// 우하
					vPos = { (256.f - m_fThronX), 5.f, m_fThronX}; vPos2 = { vPos.x - 1.f , 5.f , vPos.z }; vPos3 = { vPos.x , 0.2f , vPos.z };
					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos, _vec3(-5.f, 6.f, 5.f)); pLayer->Add_BulletObject(  pBulletObject);
					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos2, _vec3(5.f, 5.f, 5.f)); pLayer->Add_BulletObject(  pBulletObject);
					pEffect = CEffect_AlertCircle::Create(m_pGraphicDev, vPos3, _vec3(5.f, 5.f, 5.f), 1.5f, FALSE); pLayer->Add_BulletObject(  pEffect);

					// 좌하
					vPos = { m_fThronX, 5.f, m_fThronX }; vPos2 = { vPos.x - 1.f , 5.f , vPos.z }; vPos3 = { vPos.x , 0.2f , vPos.z };
					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos, _vec3(-5.f, 6.f, 5.f)); pLayer->Add_BulletObject(  pBulletObject);
					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos2, _vec3(5.f, 5.f, 5.f)); pLayer->Add_BulletObject(  pBulletObject);
					pEffect = CEffect_AlertCircle::Create(m_pGraphicDev, vPos3, _vec3(5.f, 5.f, 5.f), 1.5f, FALSE); pLayer->Add_BulletObject(  pEffect);

					// 좌상
					vPos = { m_fThronX, 5.f, (256.f - m_fThronX) }; vPos2 = { vPos.x - 1.f , 5.f , vPos.z }; vPos3 = { vPos.x , 0.2f , vPos.z };
					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos, _vec3(-5.f, 6.f, 5.f)); pLayer->Add_BulletObject(  pBulletObject);
					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos2, _vec3(5.f, 5.f, 5.f)); pLayer->Add_BulletObject(  pBulletObject);
					pEffect = CEffect_AlertCircle::Create(m_pGraphicDev, vPos3, _vec3(5.f, 5.f, 5.f), 1.5f, FALSE); pLayer->Add_BulletObject(  pEffect);
					m_fThronX -= 6.f;
					m_fAccTime2 = 0.f;

					if (m_fThronX < 0.f)
					{
						m_bCrossTron = true;
						m_fAccTime = 0.f;
						m_eCurState = IDLE;
					}
				}
			}
		}
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
	case CDoewole::UPGRADE_SMASH_ATTACK:
		Upgrade_Smash_Attack(fTimeDelta);
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
		return (-1 / 128.f) * fX * fX + 2.f * fX;
	else
		return (1.f / 128.f) * fX * fX - 2.f * fX + 256.f;

	
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


