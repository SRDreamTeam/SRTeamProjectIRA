#include "stdafx.h"
#include "..\Header\MainApp.h"
#include "Logo.h"
#include "CollisionMgr.h"


CMainApp::CMainApp()
{
}


CMainApp::~CMainApp()
{
}

HRESULT CMainApp::Ready_MainApp(void)
{
	FAILED_CHECK_RETURN(Ready_DefaultSetting(&m_pGraphicDev), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_Scene(m_pGraphicDev, &m_pManagementClass), E_FAIL);

	return S_OK;
}

int CMainApp::Update_MainApp(const _float & fTimeDelta)
{
	Engine::Update_DInput();

	m_pManagementClass->Update_Management(fTimeDelta);

	return 0;
}

void CMainApp::LateUpdate_MainApp(void)
{
	m_pManagementClass->LateUpdate_Management();

	CCollisionMgr::GetInstance()->Collision_Update();
}

void CMainApp::Render_MainApp(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	Engine::Render_Begin(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f));

	m_pManagementClass->Render_Management(m_pGraphicDev);

	Engine::Render_End();
}

HRESULT CMainApp::Ready_DefaultSetting(LPDIRECT3DDEVICE9 * ppGraphicDev)
{
	FAILED_CHECK_RETURN(Engine::Ready_GraphicDev(g_hWnd, MODE_WIN, WINCX, WINCY, &m_pDeviceClass), E_FAIL);
	m_pDeviceClass->AddRef();		// 디바이스 클래스의 참조 카운트

	(*ppGraphicDev) = m_pDeviceClass->Get_GraphicDev();
	(*ppGraphicDev)->AddRef();		// 그래픽 디바이스 컴객체 참조 카운트

	FAILED_CHECK_RETURN(Engine::Ready_Font((*ppGraphicDev), L"Font_Default", L"바탕", 15, 20, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font((*ppGraphicDev), L"Font_Jinji", L"궁서", 15, 20, FW_THIN), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_DInput(g_hInst, g_hWnd), E_FAIL);


	(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	

	return S_OK;
}

HRESULT CMainApp::SetUp_Scene(LPDIRECT3DDEVICE9 pGraphicDev, CManagement ** ppManagement)
{
	CScene*		pScene = nullptr;
	pScene = CLogo::Create(pGraphicDev);
	NULL_CHECK_RETURN(pScene, E_FAIL);

	FAILED_CHECK_RETURN(Engine::Create_Management(ppManagement), E_FAIL);
	(*ppManagement)->AddRef();

	FAILED_CHECK_RETURN((*ppManagement)->Set_Scene(pScene), E_FAIL);

	return S_OK;
}

CMainApp * CMainApp::Create(void)
{
	CMainApp *		pInstance = new CMainApp;

	if (FAILED(pInstance->Ready_MainApp()))
	{
		Safe_Release<CMainApp*>(pInstance);
		return pInstance;
	}

	//FAILED_CHECK_RETURN(pInstance->Ready_MainApp(), pInstance);


	return pInstance;
}

void CMainApp::Free(void)
{
	CCollisionMgr::GetInstance()->DestroyInstance();

	Safe_Release(m_pDeviceClass);
	Safe_Release(m_pManagementClass);
	Safe_Release(m_pGraphicDev);

	Engine::Release_Utility();
	Engine::Release_System();

	//ax + by + cz + d
	//D3DXPLANE	Plane;
	//D3DXPlaneFromPoints();

	//D3DXIntersectTri( );

}
