#include "stdafx.h"
#include "..\Header\Doewole_Body.h"
#include "Export_Function.h"

CDoewole_Body::CDoewole_Body(LPDIRECT3DDEVICE9 pGraphicDev)
	: CDoewole(pGraphicDev)
{
}

CDoewole_Body::CDoewole_Body(const CDoewole_Body & rhs)
	: CDoewole(rhs.m_pGraphicDev)
{

}

CDoewole_Body::~CDoewole_Body()
{
	Free();
}

HRESULT CDoewole_Body::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->m_vScale = { 12.f , 12.f * 1.4f  , 1.f };

	//m_pTransformCom->Rotation(ROT_X, D3DXToRadian(45.f));

	return S_OK;
}

_int CDoewole_Body::Update_GameObject(const _float& fTimeDelta)
{
	State_Update(fTimeDelta);

	CBoss::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CDoewole_Body::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CDoewole_Body::Render_GameObject()
{
	if (!m_bRender)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	/*m_fAlpha -= 0.01f;

	if (m_fAlpha < 0.f)
		m_fAlpha = 0.f;*/

	if (m_eCurState == CDoewole::IDLE)
		m_pTextureCom[STAND]->Set_Texture((_uint)m_fFrame);

	else if (m_eCurState == CDoewole::STANDARD_ATTACK || m_eCurState == CDoewole::OUTSTRECTH_ATTACK)
		m_pTextureCom[STAND_FACEON]->Set_Texture((_uint)m_fFrame);

	else if (m_eCurState == CDoewole::SMASH_ATTACK)
	{
		if (!m_bPowerSlam)
			m_pTextureCom[POWERSLAM]->Set_Texture((_uint)m_fFrame);
		if (m_bPowerSlamAfter)
			m_pTextureCom[POWERSLAM_AFTER]->Set_Texture((_uint)m_fFrame);
		if (m_bPowerSlamFaceOff)
			m_pTextureCom[POWERSLAM_FACEOFF]->Set_Texture((_uint)m_fFrame);
	}

	m_pBufferCom->Render_Buffer();
	//m_pSphereBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CDoewole_Body::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom[STAND] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Doewole_Body_Stand"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Doewole_Body_Stand", pComponent });

	pComponent = m_pTextureCom[STAND_FACEON] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Doewole_Body_Stand_FaceOn"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Doewole_Body_Stand_FaceOn", pComponent });

	pComponent = m_pTextureCom[POWERSLAM] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Doewole_Body_PowerSlam"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Doewole_Body_PowerSlam", pComponent });

	pComponent = m_pTextureCom[POWERSLAM_AFTER] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Doewole_Body_PowerSlamAfter"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Doewole_Body_PowerSlamAfter", pComponent });

	pComponent = m_pTextureCom[POWERSLAM_FACEOFF] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Doewole_Body_PowerSlamFaceOff"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Doewole_Body_PowerSlamFaceOff", pComponent });


	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	NULL_CHECK_RETURN(m_pColliderCom, E_FAIL);
	m_pColliderCom->Set_Radius(10.f);
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });

	pComponent = m_pSphereBufferCom = dynamic_cast<CSphereTex*>(Engine::Clone_ProtoComponent(L"Proto_SphereTex"));
	NULL_CHECK_RETURN(m_pSphereBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_SphereTex", pComponent });

	return S_OK;
}

void CDoewole_Body::State_Update(const _float& fTimeDelta)
{
	m_eCurState = dynamic_cast<CDoewole*>(m_pOwner)->Get_State();

	if (m_eCurState != m_ePreState)
		m_fFrame = 0.f;

	switch (m_eCurState)
	{
	case CDoewole::IDLE:
		Idle(fTimeDelta);
		break;
	case CDoewole::MOVE:
		Move(fTimeDelta);
		break;
	case CDoewole::STANDARD_ATTACK:
	case CDoewole::OUTSTRECTH_ATTACK:
		Standard_Attack(fTimeDelta);
		break;
	case CDoewole::SMASH_ATTACK:
		Smash_Attack(fTimeDelta);
		break;
	case CDoewole::STATE_END:
		break;
	default:
		break;
	}

	m_ePreState = m_eCurState;
}

void CDoewole_Body::Idle(const _float& fTimeDelta)
{
	m_bRender = true;

	m_fFrame += m_fMaxFrame * fTimeDelta;

	if (m_fMaxFrame < m_fFrame)
		m_fFrame = 0.f;

	// ================Doewole의 위치에 맞게 조정============
	CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
	NULL_CHECK(pDoewoleTransformCom);

	m_pTransformCom->m_vInfo[INFO_POS] = {  pDoewoleTransformCom->m_vInfo[INFO_POS].x,
											pDoewoleTransformCom->m_vInfo[INFO_POS].y + 13.f,
											pDoewoleTransformCom->m_vInfo[INFO_POS].z + 0.1f};
	
	// =====================================================

	m_fMaxFrame = 5.f;
}

void CDoewole_Body::Move(const _float& fTimeDelta)
{
	m_bRender = false;
}

void CDoewole_Body::Standard_Attack(const _float& fTimeDelta)
{
	m_bRender = true;

	m_fFrame += m_fMaxFrame * fTimeDelta;

	if (m_fMaxFrame < m_fFrame)
		m_fFrame = 0.f;

	// ================Doewole의 위치에 맞게 조정============
	CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
	NULL_CHECK(pDoewoleTransformCom); 

	m_pTransformCom->m_vInfo[INFO_POS] = { pDoewoleTransformCom->m_vInfo[INFO_POS].x,
											pDoewoleTransformCom->m_vInfo[INFO_POS].y + 13.f,
											pDoewoleTransformCom->m_vInfo[INFO_POS].z + 0.1f };

	// =====================================================

	m_fMaxFrame = 9.f;
}

void CDoewole_Body::Smash_Attack(const _float& fTimeDelta)
{
	m_bRender = true;

	m_fFrame += m_fMaxFrame * fTimeDelta ;

	if (!m_bPowerSlam)
	{
		m_fMaxFrame = 9.f;

		if (m_fMaxFrame < m_fFrame)
		{
			m_fFrame = 0.f;
			m_bPowerSlamAfter = true;
			m_bPowerSlam = true;
		}
	}

	if (m_bPowerSlamAfter)
	{
		m_fMaxFrame = 8.f;
		if (m_fMaxFrame < m_fFrame)
		{
			m_fFrame = 0.f;
			m_bPowerSlamFaceOff = true;
			m_bPowerSlamAfter = false;
		}
	}

	if (m_bPowerSlamFaceOff)
	{
		m_fMaxFrame = 7.f;
		if (m_fMaxFrame < m_fFrame)
		{
			m_fFrame = 0.f;
			m_bPowerSlamFaceOff = false;
			m_bPowerSlam = false;
		}
	}

	// ================Doewole의 위치에 맞게 조정============
	CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
	NULL_CHECK(pDoewoleTransformCom);

	m_pTransformCom->m_vInfo[INFO_POS] = { pDoewoleTransformCom->m_vInfo[INFO_POS].x,
											pDoewoleTransformCom->m_vInfo[INFO_POS].y + 13.f,
											pDoewoleTransformCom->m_vInfo[INFO_POS].z + 0.1f };

	// =====================================================
}

CDoewole_Body * CDoewole_Body::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDoewole_Body * pInstance = new CDoewole_Body(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDoewole_Body::Free(void)
{
	__super::Free();
}


