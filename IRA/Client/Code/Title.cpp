#include "stdafx.h"
#include "..\Header\Logo.h"

#include "Export_Function.h"
#include "Stage.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
	, m_pLoading(nullptr)
{
}


CLogo::~CLogo()
{
}

HRESULT CLogo::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	
	m_pLoading = CLoading::Create(m_pGraphicDev, LOADING_STAGE);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	return S_OK;
}

_int CLogo::Update_Scene(const _float & fTimeDelta)
{
	_int iExit = __super::Update_Scene(fTimeDelta);

	if (true == m_pLoading->Get_Finish())
	{
		if (GetAsyncKeyState(VK_RETURN))
		{
			CScene*		pScene = CStage::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pScene, -1);

			FAILED_CHECK_RETURN(Engine::Set_Scene(pScene), E_FAIL);
		}
	}

	return iExit;
}

void CLogo::LateUpdate_Scene(void)
{
	__super::LateUpdate_Scene();
}

void CLogo::Render_Scene(void)
{
	// _DEBUG용
	Engine::Render_Font(L"Font_Default", m_pLoading->Get_String(), &_vec2(20.f, WINCY - 50.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

HRESULT CLogo::Ready_Layer_Environment(const _tchar* pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	// 오브젝트

	CGameObject*	pGameObject = nullptr;

	// 오브젝트 
	pGameObject = CBackGround::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BackGround", pGameObject  ), E_FAIL);

	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CLogo::Ready_Prototype(void)
{
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_RcTex", CRcTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_RcTex2", CRcTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_Texture_Logo", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Logo/Spr_Tilte_Illustration.png")), E_FAIL);

	return S_OK;
}

CLogo * CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogo *	pInstance = new CLogo(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{ 
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLogo::Free(void)
{
	Safe_Release(m_pLoading);

	__super::Free();
}
