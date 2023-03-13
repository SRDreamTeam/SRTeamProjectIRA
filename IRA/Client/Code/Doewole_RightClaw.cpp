#include "stdafx.h"
#include "..\Header\Doewole_RightClaw.h"
#include "Export_Function.h"
#include "Effect_Doewole_Hurt.h"
#include "Effect_Doewole_ChargeCircle.h"
#include "Effect_Doewole_Slam.h"
#include "DoewoleBullet_Standard.h"
#include <DoewoleBullet_SwordShot.h>
#include <DoewoleBullet_SwordShot2.h>
#include <Effect_AlertCircle.h>
#include <Effect_Doewole_Circle.h>
#include <DoewoleBullet_StopGo.h>

CDoewole_RightClaw::CDoewole_RightClaw(LPDIRECT3DDEVICE9 pGraphicDev)
	: CDoewole(pGraphicDev)

{
}

CDoewole_RightClaw::CDoewole_RightClaw(const CDoewole_RightClaw & rhs)
	: CDoewole(rhs.m_pGraphicDev)
{
}

CDoewole_RightClaw::~CDoewole_RightClaw()
{

}

HRESULT CDoewole_RightClaw::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->m_vScale = { 20.f , 20.f , 1.f };

	return S_OK;
}

_int CDoewole_RightClaw::Update_GameObject(const _float& fTimeDelta)
{
	State_Update(fTimeDelta);

	CBoss::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

	return 0;
}

void CDoewole_RightClaw::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CDoewole_RightClaw::Render_GameObject()
{
	if (!m_bRender)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	switch (m_eCurState)
	{
	case CDoewole::IDLE:
		m_pTextureCom[STAND]->Set_Texture((_uint)m_fFrame);
		break;
	case CDoewole::MOVE:
		break;
	case CDoewole::STANDARD_ATTACK:
		m_pTextureCom[STANDARD_ATTACK]->Set_Texture((_uint)m_fFrame);
		break;
	case CDoewole::OUTSTRECTH_ATTACK:
		if(!m_bUp || m_bDown)
			m_pTextureCom[UP]->Set_Texture((_uint)m_fFrame);
		else
			m_pTextureCom[OUTSTRECTH_ATTACK]->Set_Texture((_uint)m_fFrame);
		break;
	case CDoewole::SMASH_ATTACK:
		if (!m_bUp)
			m_pTextureCom[UP]->Set_Texture((_uint)m_fFrame);
		else
			m_pTextureCom[SMASH]->Set_Texture((_uint)m_fFrame);
		break;
	case CDoewole::SCRATCH_ATTACK:
		if(!m_bWait)
			m_pTextureCom[SCRATCH]->Set_Texture((_uint)m_fFrame);
		else
			m_pTextureCom[STAND]->Set_Texture((_uint)m_fFrame);
		break; 
	case CDoewole::AREA_ATTACK:
		if (!m_bUp)
			m_pTextureCom[UP]->Set_Texture((_uint)m_fFrame);
		else
			m_pTextureCom[SMASH]->Set_Texture((_uint)m_fFrame);
		break;
	case CDoewole::UPGRADE_SMASH_ATTACK:
		if (!m_bRSmashWait)
		{
			if (!m_bSmash)
				m_pTextureCom[UP]->Set_Texture((_uint)m_fFrame);
			else
				m_pTextureCom[SMASH]->Set_Texture((_uint)m_fFrame);
		}
		else
			m_pTextureCom[STAND]->Set_Texture((_uint)m_fFrame);
		break;
	case CDoewole::UPGRADE_SCRATCH_ATTACK:
		if (!m_bCrossScratch)
		{
			if (!m_bSmash)
			{
				if (!m_bWait)
					m_pTextureCom[SCRATCH]->Set_Texture((_uint)m_fFrame);
				else
					m_pTextureCom[STAND]->Set_Texture((_uint)m_fFrame);
			}
			else if (m_bSmash)
				m_pTextureCom[SMASH]->Set_Texture((_uint)m_fFrame);
		}
		else
			m_pTextureCom[CROSS_SCRATCH]->Set_Texture((_uint)m_fFrame);
		break;
	case CDoewole::STATE_END:
		break;
	default:
		break;
	}

	m_pBufferCom->Render_Buffer();

	//m_pSphereBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CDoewole_RightClaw::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom[STAND] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Doewole_Claw_Stand"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Doewole_Claw_Stand", pComponent });

	pComponent = m_pTextureCom[STANDARD_ATTACK] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Doewole_Claw_StandardAttack"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Doewole_Claw_StandardAttack", pComponent });

	pComponent = m_pTextureCom[OUTSTRECTH_ATTACK] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Doewole_Claw_OutStretchAttack"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Doewole_Claw_OutStretchAttack", pComponent });

	pComponent = m_pTextureCom[UP] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Doewole_Claw_Up"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Doewole_Claw_Up", pComponent });

	pComponent = m_pTextureCom[SMASH] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Doewole_Claw_Smash"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Doewole_Claw_Smash", pComponent });

	pComponent = m_pTextureCom[SCRATCH] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Doewole_Claw_Scratch"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Doewole_Claw_Scratch", pComponent });

	pComponent = m_pTextureCom[CROSS_SCRATCH] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Doewole_Claw_CrossScratch"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Doewole_Claw_CrossScratch", pComponent });

	return S_OK;

}

void CDoewole_RightClaw::State_Update(const _float& fTimeDelta)
{
	m_eCurState = dynamic_cast<CDoewole*>(m_pOwner)->Get_State();

	if (m_eCurState != m_ePreState)
	{
		m_fFrame = 0.f;
		m_fAccTime = 0.f;

		m_bUp = false;
		m_bDown = false;
		m_iReplayCnt = 0;
		m_bWait = false;
		m_bHurt = false;
		m_bSmash = false;
		m_bAlertEffect = false;
		m_bRSmashWait = true;
		m_bCircleEffect = false;
		m_bSmashEffect = false;
		m_bAlert = false;
		m_bCrossScratch = false;
	}

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
	case CDoewole::UPGRADE_SCRATCH_ATTACK:
		Upgrade_Scratch_Attack(fTimeDelta);
		break;
	case CDoewole::STATE_END:
		break;
	default:
		break;
	}

	m_ePreState = m_eCurState;
}

void CDoewole_RightClaw::Idle(const _float& fTimeDelta)
{
	m_bRender = true;

	m_fFrame += m_fMaxFrame * fTimeDelta;

	if (m_fMaxFrame < m_fFrame)
		m_fFrame = 0.f;

	// ================Doewole의 위치에 맞게 조정============
	CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
	NULL_CHECK(pDoewoleTransformCom);

	m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];
	m_pTransformCom->m_vInfo[INFO_POS].y += 20.f;
	m_pTransformCom->m_vInfo[INFO_POS].x +=	15.f;
	m_pTransformCom->m_vInfo[INFO_POS].z -= 0.1f;
	// =====================================================

	m_fMaxFrame = 10.f;
}

void CDoewole_RightClaw::Move(const _float& fTimeDelta)
{
	m_bRender = false;
}

void CDoewole_RightClaw::Standard_Attack(const _float& fTimeDelta)
{
	m_bRender = true;
	m_fMaxFrame = 7.f;

	_bool bGoToIdle = dynamic_cast<CDoewole*> (m_pOwner)->Get_AttackToIdle();


	if (!bGoToIdle)
	{
		m_fFrame += m_fMaxFrame * fTimeDelta * 2;

		if (m_fMaxFrame < m_fFrame)
			m_fFrame = m_fMaxFrame;
	}
	else
	{
		m_fFrame -= m_fMaxFrame * fTimeDelta * 2;

		if (m_fFrame < 0.f)
			m_fFrame = 0.f;
	}

	// ================Doewole의 위치에 맞게 조정============
	CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
	NULL_CHECK(pDoewoleTransformCom);

	m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];
	m_pTransformCom->m_vInfo[INFO_POS].y += 20.f + m_fFrame;
	m_pTransformCom->m_vInfo[INFO_POS].x += 15.f + m_fFrame * (-2.5f);
	m_pTransformCom->m_vInfo[INFO_POS].z -= 0.1f;
	// =====================================================
	
}

void CDoewole_RightClaw::OutStretch_Attack(const _float& fTimeDelta)
{
	m_bRender = true;

	if (!m_bUp && !m_bDown)
	{
		m_fMaxFrame = 6.f;

		m_fFrame += m_fMaxFrame * fTimeDelta * 2;

		// ================Doewole의 위치에 맞게 조정============
		CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
		NULL_CHECK(pDoewoleTransformCom);

		m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];
		m_pTransformCom->m_vInfo[INFO_POS].y += 20.f + m_fFrame * 1.f;
		m_pTransformCom->m_vInfo[INFO_POS].x += 15.f;
		m_pTransformCom->m_vInfo[INFO_POS].z -= 0.1f;
		// =====================================================

		if (m_fMaxFrame < m_fFrame)
		{
			m_fFrame = 0.f;
			m_bUp = true;
		}
	}
	else if(m_bUp && !m_bDown)
	{
		m_fMaxFrame = 9.f;

		m_fFrame += m_fMaxFrame * fTimeDelta;

		// ================Doewole의 위치에 맞게 조정============
		CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
		NULL_CHECK(pDoewoleTransformCom);

		m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];
		m_pTransformCom->m_vInfo[INFO_POS].y += 20.f + 6 * 2.f ;
		m_pTransformCom->m_vInfo[INFO_POS].x += 15.f + 5.f;
		m_pTransformCom->m_vInfo[INFO_POS].z -= 0.1f;
		// =====================================================

		if (m_fMaxFrame < m_fFrame)
		{
			m_fFrame = 0.f;
			++m_iReplayCnt;
		}

		if (m_iReplayCnt == 3)
		{
			m_bDown = true;
			m_bUp = false;
			m_fFrame = 6.f;
		}
	}

	if (m_bDown)
	{
		m_fMaxFrame = 6.f;

		m_fFrame -= m_fMaxFrame * fTimeDelta * 2;

		// ================Doewole의 위치에 맞게 조정============
		CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
		NULL_CHECK(pDoewoleTransformCom);

		m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];
		m_pTransformCom->m_vInfo[INFO_POS].y += 20.f + m_fFrame * 1.f;
		m_pTransformCom->m_vInfo[INFO_POS].x += 15.f;
		m_pTransformCom->m_vInfo[INFO_POS].z -= 0.1f;
		// =====================================================

		if (m_fFrame < 0.f)
		{
			m_fFrame = 0.f;
			//m_bDown = false;
		}
	}
}

void CDoewole_RightClaw::Smash_Attack(const _float& fTimeDelta)
{
	m_bRender = true;
	m_fAccTime += fTimeDelta;

	if (!m_bUp)
	{
		m_fMaxFrame = 6.f;

		m_fFrame += m_fMaxFrame * fTimeDelta * 2;

		// ================Doewole의 위치에 맞게 조정============
		CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
		NULL_CHECK(pDoewoleTransformCom);

		m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];
		m_pTransformCom->m_vInfo[INFO_POS].y += 20.f + m_fFrame * 1.7f;
		m_pTransformCom->m_vInfo[INFO_POS].x += 15.f - m_fFrame * 1.7f;
		// =====================================================

		if (m_fFrame == 5.f)
			m_fAccTime = 0.f;

		if (m_fAccTime < 2.f)
		{
			if (m_fFrame > 6.f)
				m_fFrame = 6.f;
		}

		if (m_fMaxFrame < m_fFrame)
		{
			m_fFrame = 0.f;
			m_bUp = true;
			m_fAccTime = 0.f;
		}
	}
	else
	{
		m_fMaxFrame = 13.f;

		m_fFrame += m_fMaxFrame * fTimeDelta * 2;
		
		if (m_fFrame == 5.f)
		{
			m_fAccTime = 0.f;
		}

		if (m_fAccTime < 2.f)
		{
			if (m_fFrame > 6.f)
				m_fFrame = 6.f;
		}


		// ================Doewole의 위치에 맞게 조정============
		CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
		NULL_CHECK(pDoewoleTransformCom);

		m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];

		if (m_fFrame <= 6)
		{
			m_pTransformCom->m_vInfo[INFO_POS].y += 20.f - m_fFrame * 0.7f;
			m_pTransformCom->m_vInfo[INFO_POS].x += 15.f - 6 * 2.f;
		}
		else
		{
			m_pTransformCom->m_vInfo[INFO_POS].y += 20.f - (6 * 0.7f) + ((m_fFrame - 1.f) - 6.f) * 0.7f ;
			m_pTransformCom->m_vInfo[INFO_POS].x += 15.f - 6 * 2.f + ((m_fFrame - 1.f) - 6.f) * 2.f;
		}

		if (m_fMaxFrame < m_fFrame)
		{
			m_fFrame = m_fMaxFrame;
			m_bUp = false;
			m_fAccTime = 0.f;
		}
	}
}

void CDoewole_RightClaw::Scratch_Attack(const _float& fTimeDelta)
{
	m_bRender = dynamic_cast<CDoewole*>(m_pOwner)->Get_Disappear();

	if (!m_bRender)
		m_fFrame = 0.f;

	if (!m_bWait)
	{
		m_fMaxFrame = 7.f;

		if (m_bRender)
		{
			m_fFrame += m_fMaxFrame * fTimeDelta * 2.f;

			if (!m_bHurt)
			{
				m_bHurt = true;

				CTransform* pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"AlertRect_Right", L"Proto_Transform", ID_DYNAMIC));
				_vec3 vPos = { pTransform->m_vInfo[INFO_POS].x , 1.f , pTransform->m_vInfo[INFO_POS].z };

				// Hurt 이펙트 생성
				for (int i = 0; i < 3; ++i)
				{
					vPos.z -= i * 2.f;
					vPos.x -= i * 1.f;

					CGameObject* pEffect = CEffect_Doewole_Hurt::Create(m_pGraphicDev , vPos, _vec3(30.f,30.f,30.f));
					NULL_CHECK(pEffect);
					CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
					pLayer->Add_BulletObject(  pEffect);
				}
			}
		}

		if (m_fFrame > m_fMaxFrame)
		{
			m_fFrame = m_fMaxFrame;
		}

		// ================Doewole의 위치에 맞게 조정============
		CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
		NULL_CHECK(pDoewoleTransformCom);

		m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];

		m_pTransformCom->m_vInfo[INFO_POS].x += 15.f - m_fFrame * 4.f;

		if(m_fFrame < 5.f)
			m_pTransformCom->m_vInfo[INFO_POS].z -= m_fFrame * 3.f;

		if (m_fFrame < 7.f)
			m_pTransformCom->m_vInfo[INFO_POS].y += 20.f - m_fFrame;
		else
			m_pTransformCom->m_vInfo[INFO_POS].y += 20.f - m_fFrame + 11.f;
	}
	else
	{
		m_fMaxFrame = 10.f;

		// ================Doewole의 위치에 맞게 조정============
		CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
		NULL_CHECK(pDoewoleTransformCom);

		m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];
		m_pTransformCom->m_vInfo[INFO_POS].y += 20.f;
		m_pTransformCom->m_vInfo[INFO_POS].x += 15.f;
		m_pTransformCom->m_vInfo[INFO_POS].z -= 0.1f;
		// =====================================================
	}

}

void CDoewole_RightClaw::Area_Attack(const _float& fTimeDelta)
{
	m_fAccTime += fTimeDelta;

	m_bRender = dynamic_cast<CDoewole*> (m_pOwner)->Get_Disappear();

	if (m_bRender)
	{
		if (!m_bUp)
		{
			m_fMaxFrame = 6.f;

			m_fFrame += m_fMaxFrame * fTimeDelta * 1.f;

			// ================Doewole의 위치에 맞게 조정============
			CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
			NULL_CHECK(pDoewoleTransformCom);

			m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];
			m_pTransformCom->m_vInfo[INFO_POS].y += 20.f + m_fFrame * 2.2f;
			m_pTransformCom->m_vInfo[INFO_POS].x += 15.f - m_fFrame * 1.7f;
			// =====================================================

			if (!m_bAlertEffect)
			{
				_vec3 vPos = { pDoewoleTransformCom->m_vInfo[INFO_POS].x , 0.001f , pDoewoleTransformCom->m_vInfo[INFO_POS].z };

				CGameObject* pEffect = CEffect_AlertCircle::Create(m_pGraphicDev, vPos, _vec3(25.f, 25.f, 25.f), 1.f, TRUE);
				NULL_CHECK(pEffect);
				CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
				pLayer->Add_BulletObject(  pEffect);
			
				m_bAlertEffect = true;
			}

			if (fabs(m_fFrame - 5.f) < 0.01f)
				m_fAccTime = 0.f;

			if (m_fAccTime < 1.f)
			{
				if (m_fFrame > 6.f)
					m_fFrame = 6.f;
			}

			if (m_fMaxFrame < m_fFrame)
			{
				m_fFrame = 0.f;
				m_bUp = true;
				m_fAccTime = 0.f;
				m_bAlertEffect = false;
			}
		}
		else
		{
			m_fMaxFrame = 13.f;

			m_fFrame += m_fMaxFrame * fTimeDelta * 2;

			if (m_fFrame == 5.f)
			{
				m_fAccTime = 0.f;
			}

			if (m_fAccTime < 2.f)
			{
				if (m_fFrame > 6.f)
				{
					m_fFrame = 6.f;

					if (!m_bSmash)
					{ // ChargeCircle 이펙트 생성

						CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
						NULL_CHECK(pDoewoleTransformCom);

						_vec3 vEffectPos = { pDoewoleTransformCom->m_vInfo[INFO_POS].x , 0.3f , pDoewoleTransformCom->m_vInfo[INFO_POS].z };

						CGameObject* pEffect = CEffect_Doewole_ChargeCircle::Create(m_pGraphicDev , vEffectPos);
						NULL_CHECK(pEffect);
						CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
						pLayer->Add_BulletObject(  pEffect);

						// Smash 이펙트 생성
						pEffect = CEffect_Doewole_Slam::Create(m_pGraphicDev);
						NULL_CHECK(pEffect);
						pLayer = Engine::Get_Layer(L"Layer_GameLogic");
						pLayer->Add_BulletObject(  pEffect);

						// SwordShot Bullet 생성
						for (int i = 0; i <360; ++i)
						{
							if (i % 15 > 10)
								continue;

							CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
							NULL_CHECK(pDoewoleTransformCom);

							_float cosTheta = cos(D3DXToRadian(360.f / 360.f) * i);
							_float sinTheta = sin(D3DXToRadian(360.f / 360.f) * i);

							_float fAngle = 360.f / 360.f * i;

							_vec3 vPos = { pDoewoleTransformCom->m_vInfo[INFO_POS].x + cosTheta , 3.f ,  pDoewoleTransformCom->m_vInfo[INFO_POS].z + sinTheta };

							CGameObject* pBullet = CDoewoleBullet_SwordShot2::Create(m_pGraphicDev, vPos, fAngle);
							NULL_CHECK(pBullet);
							pLayer->Add_BulletObject(  pBullet);
						}

						m_bSmash = true;
					}
				}
			}

			// ================Doewole의 위치에 맞게 조정============
			CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
			NULL_CHECK(pDoewoleTransformCom);

			m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];

			if (m_fFrame <= 6)
			{
				m_pTransformCom->m_vInfo[INFO_POS].y += 20.f - m_fFrame * 0.7f;
				m_pTransformCom->m_vInfo[INFO_POS].x += 15.f - 6 * 2.f;
			}
			else
			{
				m_pTransformCom->m_vInfo[INFO_POS].y += 20.f - (6 * 0.7f) + ((m_fFrame - 1.f) - 6.f) * 0.7f;
				m_pTransformCom->m_vInfo[INFO_POS].x += 15.f - 6 * 2.f + ((m_fFrame - 1.f) - 6.f) * 2.f;

				m_bSmash = false;
			}

			if (m_fMaxFrame < m_fFrame)
			{
				m_fFrame = m_fMaxFrame;
				m_bUp = false;
				m_fAccTime = 0.f;
			}
		}
	}
}

void CDoewole_RightClaw::Upgrade_Smash_Attack(const _float& fTimeDelta)
{
	m_bRender = true;

	CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
	NULL_CHECK(pDoewoleTransformCom);

	if (!m_bRSmashWait)
	{
		if (!m_bSmash)
		{
			// 들어올린다
			m_fMaxFrame = 6.f;

			m_fFrame += m_fMaxFrame * fTimeDelta;

			if (!m_bAlert)
			{
				m_bAlert = true;

				_vec3 vPos = { m_pTransformCom->m_vInfo[INFO_POS].x , 0.001f , m_pTransformCom->m_vInfo[INFO_POS].z };

				CGameObject* pEffect = CEffect_AlertCircle::Create(m_pGraphicDev, vPos, _vec3(20.f, 20.f, 20.f), 3.f, TRUE);
				NULL_CHECK(pEffect);
				CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
				pLayer->Add_BulletObject(  pEffect);
			}

			if (m_fFrame > m_fMaxFrame)
			{
				m_fFrame = m_fMaxFrame;
				m_fAccTime += fTimeDelta;

				// 이펙트 한번 생성
				if (!m_bCircleEffect)
				{
					m_bCircleEffect = true;

					CGameObject* pEffect = CEffect_Doewole_Circle::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);
					NULL_CHECK(pEffect);
					CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
					pLayer->Add_BulletObject(  pEffect);
				}

				if (m_fAccTime > 2.f)
				{
					m_bSmash = true;
					m_fAccTime = 0.f;
					m_fFrame = 0.f;
					m_bCircleEffect = false;
					m_bAlert = false;
				}
			}

			m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];
			m_pTransformCom->m_vInfo[INFO_POS].y += 20.f + m_fFrame * 2.2f;
			m_pTransformCom->m_vInfo[INFO_POS].x += 15.f;
		}
		else
		{
			// 내려찍는다
			m_fMaxFrame = 13.f;

			m_fAccTime += fTimeDelta;

			m_fFrame += m_fMaxFrame * fTimeDelta;

			if (fabs(m_fFrame - 5.f) < 0.1f)
				m_fAccTime = 0.f;

			// 내려 찍힌 상태
			if (m_fAccTime < 1.f)
			{
				m_fFrame = 6.f;
				if (!m_bSmashEffect)
				{
					_vec3 vEffectPos = { m_pTransformCom->m_vInfo[INFO_POS].x , 0.3f , m_pTransformCom->m_vInfo[INFO_POS].z };

					CGameObject* pEffect = CEffect_Doewole_ChargeCircle::Create(m_pGraphicDev, vEffectPos);
					NULL_CHECK(pEffect);
					CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
					pLayer->Add_BulletObject(  pEffect);


					for (int i = 0; i < 40; ++i)
					{
						_vec3 vPos = { m_pTransformCom->m_vInfo[INFO_POS].x , 3.f , m_pTransformCom->m_vInfo[INFO_POS].z };
						CDoewoleBullet_StopGo* pBullet = CDoewoleBullet_StopGo::Create(m_pGraphicDev, vPos, CLAW_RIGHT);
						NULL_CHECK(pBullet);
						pLayer = Engine::Get_Layer(L"Layer_GameLogic");
						pLayer->Add_BulletObject(  pBullet);
					}


					m_bSmashEffect = true;
				}
			}

			if (m_fFrame > m_fMaxFrame)
			{
				m_bSmash = false;
				m_bRSmashWait = true;
				m_fFrame = 0.f;
				m_bSmashEffect = false;
				dynamic_cast<CDoewole*>(m_pOwner)->Set_State(IDLE);
			}

			m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];
			// 내려가고
			if (m_fFrame <= 6.f)
				m_pTransformCom->m_vInfo[INFO_POS].y += 20.f + m_fFrame * -0.3f;
			// 올라가고
			else
				m_pTransformCom->m_vInfo[INFO_POS].y += 20.f + (6.f - m_fFrame) * -0.3f;
			m_pTransformCom->m_vInfo[INFO_POS].x += 15.f;
		}
	}
	else
	{
		m_fAccTime += fTimeDelta;

		m_fMaxFrame = 10.f;

		m_fFrame += m_fMaxFrame * fTimeDelta;

		if (m_fFrame > m_fMaxFrame)
			m_fFrame = 0.f;
	}
}

void CDoewole_RightClaw::Upgrade_Scratch_Attack(const _float& fTimeDelta)
{
	m_bRender = dynamic_cast<CDoewole*>(m_pOwner)->Get_Disappear();

	if (!m_bRender)
		m_fFrame = 0.f;

	CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
	NULL_CHECK(pDoewoleTransformCom);

	if (!m_bCrossScratch)
	{
		if (!m_bSmash)
		{
			if (!m_bWait)
			{
				m_fMaxFrame = 7.f;

				if (m_bRender)
				{
					m_fFrame += m_fMaxFrame * fTimeDelta * 2.f;

					if (!m_bHurt)
					{
						m_bHurt = true;

						_vec3 vPos = { pDoewoleTransformCom->m_vInfo[INFO_POS].x , 0.1f , pDoewoleTransformCom->m_vInfo[INFO_POS].z - 6.f };

						// Hurt 이펙트 생성
						for (int i = 0; i < 3; ++i)
						{
							vPos.z -= i * 2.f;
							vPos.x -= i * 1.f;

							CGameObject* pEffect = CEffect_Doewole_Hurt::Create(m_pGraphicDev, vPos, _vec3(30.f, 30.f, 30.f));
							NULL_CHECK(pEffect);
							CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
							pLayer->Add_BulletObject(pEffect);
						}
					}
				}

				if (m_fFrame > m_fMaxFrame)
				{
					m_fFrame = m_fMaxFrame;
				}

				// ================Doewole의 위치에 맞게 조정============

				m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];

				m_pTransformCom->m_vInfo[INFO_POS].x += 15.f - m_fFrame * 4.f;

				if (m_fFrame < 5.f)
					m_pTransformCom->m_vInfo[INFO_POS].z -= m_fFrame * 3.f;

				if (m_fFrame < 7.f)
					m_pTransformCom->m_vInfo[INFO_POS].y += 20.f - m_fFrame;
				else
					m_pTransformCom->m_vInfo[INFO_POS].y += 20.f - m_fFrame + 11.f;
			}
			else
			{
				m_fMaxFrame = 10.f;

				// ================Doewole의 위치에 맞게 조정============
				m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];
				m_pTransformCom->m_vInfo[INFO_POS].y += 20.f;
				m_pTransformCom->m_vInfo[INFO_POS].x += 15.f;
				m_pTransformCom->m_vInfo[INFO_POS].z -= 0.1f;
				// =====================================================

				m_fFrame = 0.f;
			}
		}
		else if (m_bSmash)
		{
			if (m_bRender)
			{
				m_fMaxFrame = 6.f;

				m_fFrame += m_fMaxFrame * fTimeDelta * 2;

				if (m_fAccTime < 2.f)
				{
					if (m_fFrame > 6.f)
						m_fFrame = 6.f;
				}


				// ================Doewole의 위치에 맞게 조정============
				m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];

				if (m_fFrame <= 6)
				{
					m_pTransformCom->m_vInfo[INFO_POS].y += 20.f - m_fFrame * 0.7f;
					m_pTransformCom->m_vInfo[INFO_POS].x += 15.f - 6 * 2.f;
				}

				m_bHurt = false;
			}
		}
	}
	else
	{
		if (m_bRender)
		{
			m_fMaxFrame = 13.f;

			if (m_fFrame < 9.f)
				m_fFrame += m_fMaxFrame * fTimeDelta * 2.f;
			else
				m_fFrame += m_fMaxFrame * fTimeDelta * 3.f;

			if (m_fFrame > m_fMaxFrame)
				m_fFrame = m_fMaxFrame;

			// ================Doewole의 위치에 맞게 조정===========
			m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];
			m_pTransformCom->m_vInfo[INFO_POS].y += 25.f;

			if (m_fFrame < 9.f)
				m_pTransformCom->m_vInfo[INFO_POS].x -= m_fFrame * 0.2f;
			else
			{
				m_pTransformCom->m_vInfo[INFO_POS].x += m_fFrame * 1.2f;
				m_pTransformCom->m_vInfo[INFO_POS].z -= m_fFrame * 0.7f;
			}

			if (m_fFrame > 11.f)
				m_pTransformCom->m_vInfo[INFO_POS].y += 3.f;
			if (m_fFrame > 12.f)
				m_pTransformCom->m_vInfo[INFO_POS].y += 3.f;

			if (!m_bHurt)
			{
				m_bHurt = true;

				_vec3 vPos = { pDoewoleTransformCom->m_vInfo[INFO_POS].x , 0.1f , pDoewoleTransformCom->m_vInfo[INFO_POS].z - 6.f };

				// Hurt 이펙트 생성
				for (int i = 0; i < 3; ++i)
				{
					vPos.z -= i * 2.f;
					vPos.x += i * 1.f;

					CGameObject* pEffect = CEffect_Doewole_Hurt::Create(m_pGraphicDev, vPos, _vec3(+30.f, 30.f, 30.f));
					NULL_CHECK(pEffect);
					CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
					pLayer->Add_BulletObject(pEffect);
				}
			}

		}
	}

}

CDoewole_RightClaw * CDoewole_RightClaw::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDoewole_RightClaw * pInstance = new CDoewole_RightClaw(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDoewole_RightClaw::Free(void)
{
	__super::Free();
}


