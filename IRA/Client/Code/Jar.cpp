#include "stdafx.h"
#include "..\Header\Jar.h"
#include "Export_Function.h"

CJar::CJar(LPDIRECT3DDEVICE9 pGraphicDev)
	: CDynamicObject(pGraphicDev)
{
}

CJar::CJar(const CJar& rhs)
	: CDynamicObject(rhs)
{
}

CJar::~CJar()
{
	Free();
}

HRESULT CJar::Ready_GameObject(void)
{
	__super::Ready_GameObject();

	//dynamic_cast<CJar*>(pGameObject)->Set_ObjKey(L"Spr_HistoricSites_DecoObject_0", sizeof(L"Spr_HistoricSites_DecoObject_0") / sizeof(_tchar));
	//dynamic_cast<CJar*>(pGameObject)->Set_DynamicObj_ID(dynamic_cast<CJar*>(pGameObject)->CompareID(L"Spr_HistoricSites_DecoObject_"));

	//pTransformCom->Set_Scale(5.f, 5.f, 0.f);
	//pTransformCom->Set_Angle(D3DXToRadian(15.f), 0.f, 0.f);
	//pTransformCom->Set_Pos(tStaticObjData.vPos.x, tStaticObjData.vPos.y, tStaticObjData.vPos.z);

	//dynamic_cast<CJar*>(pGameObject)->Set_DrawID(tStaticObjData.byDrawID);
	//dynamic_cast<CJar*>(pGameObject)->Set_DrawOption(tStaticObjData.byDrawOption);
	//dynamic_cast<CJar*>(pGameObject)->Set_ColliderPos(&tStaticObjData.vColliderPos);
	//dynamic_cast<v*>(pGameObject)->Set_ColliderRadius(tStaticObjData.fColliderRadius);

	Set_ObjKey(L"Spr_HistoricSites_DecoObject_0", sizeof(L"Spr_HistoricSites_DecoObject_0") / sizeof(_tchar));
	Set_DynamicObj_ID(CompareID(L"Spr_HistoricSites_DecoObject_"));

	m_pTransformCom->Set_Scale(5.f, 5.f, 0.f);
	m_pTransformCom->Set_Angle(D3DXToRadian(15.f), 0.f, 0.f);
	m_pTransformCom->Set_Pos(100.f, 0.2f, 100.f);
	Set_DrawID(0);
	Set_DrawOption(0);

	return S_OK;
}

_int CJar::Update_GameObject(const _float& fTimeDelta)
{	
	m_pTransformCom->Set_Pos(300.f, 0.2f, 300.f);

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CJar::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CJar::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (-1 != m_byDrawID)
		m_pTextureCom[JAR]->Set_Texture(m_byDrawID);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CJar* CJar::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CJar* pInstance = new CJar(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CJar::Free(void)
{
	__super::Free();
}

HRESULT CJar::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom[JAR] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Spr_HistoricSites_DecoObject_"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Spr_HistoricSites_DecoObject_", pComponent });

	return S_OK;
}
