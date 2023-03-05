#include "stdafx.h"
#include "..\Header\Ghost.h"
#include "Export_Function.h"
#include "GhostChild.h"

CGhost::CGhost(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pCalculatorCom(nullptr)
	, m_trail_num(6)
{
}

CGhost::CGhost(const CGhost& rhs)
	: Engine::CGameObject(rhs)
	, m_pBufferCom(rhs.m_pBufferCom)
	, m_pTransformCom(rhs.m_pTransformCom)
	, m_pCalculatorCom(rhs.m_pCalculatorCom)
	, m_trail_num(rhs.m_trail_num)
{
}

CGhost::~CGhost()
{
	Free();
}

HRESULT CGhost::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	

	return S_OK;
}

_int CGhost::Update_GameObject(const _float& fTimeDelta)
{
	
	if (Is_Dash == true) {
		m_interver += 2.f * fTimeDelta;
		if (m_interver > 2.f) {
			//Show_Ghost();
			m_interver = 0.f;
		}
		
	}

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CGhost::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CGhost::Render_GameObject()
{
	
}

void CGhost::Show_Ghost(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");


	CGameObject* pGameObject = nullptr;

	pGameObject = CGhostChild::Create(m_pGraphicDev);

	if (pGameObject == nullptr)
		return;


	if (i > 5)
		return;

	if (i == 0)
		pGameLogicLayer->Add_GameObject(L"Ghost_Child_0", pGameObject, OBJ_GHOST);
	if (i == 1)
		pGameLogicLayer->Add_GameObject(L"Ghost_Child_1", pGameObject, OBJ_GHOST);
	if (i == 2)
		pGameLogicLayer->Add_GameObject(L"Ghost_Child_2", pGameObject, OBJ_GHOST);
	if (i == 3)
		pGameLogicLayer->Add_GameObject(L"Ghost_Child_3", pGameObject, OBJ_GHOST);
	if (i == 4)
		pGameLogicLayer->Add_GameObject(L"Ghost_Child_4", pGameObject, OBJ_GHOST);
	if (i == 5)
		pGameLogicLayer->Add_GameObject(L"Ghost_Child_5", pGameObject, OBJ_GHOST);

	i++;


}

HRESULT CGhost::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pDashTextureCom[DASH_045] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Ghost_Dash_045"));
	NULL_CHECK_RETURN(m_pDashTextureCom[DASH_045], E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Ghost_Dash_045", pComponent });

	pComponent = m_pDashTextureCom[DASH_135] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Ghost_Dash_135"));
	NULL_CHECK_RETURN(m_pDashTextureCom[DASH_135], E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Ghost_Dash_135", pComponent });


	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_ProtoComponent(L"Proto_Calculator"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

	return S_OK;
}


CGhost * CGhost::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGhost * pInstance = new CGhost(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CGhost::Free(void)
{
	__super::Free();
}


