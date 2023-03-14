#include "stdafx.h"
#include "..\Header\DynamicObject.h"
#include "Export_Function.h"

CDynamicObject::CDynamicObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: CLandscape(pGraphicDev)
{
}

CDynamicObject::CDynamicObject(const CDynamicObject& rhs)
	: CLandscape(rhs)
{
}

CDynamicObject::~CDynamicObject()
{
}

HRESULT CDynamicObject::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CDynamicObject::Update_GameObject(const _float& fTimeDelta)
{	
	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

	return 0;
}

void CDynamicObject::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	/*_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);*/
}

void CDynamicObject::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (-1 != m_byDrawID)
		m_pTextureCom[m_eID]->Set_Texture(m_byDrawID);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

DYNAMIC_OBJECT_ID CDynamicObject::CompareID(wstring strObjKey)
{
	if (strObjKey == L"spr_bush_01_")
		return BUSH1;
	else if (strObjKey == L"spr_bush_02_")
		return BUSH2;
	else if (strObjKey == L"spr_bush_03_")
		return BUSH3;
	else if (strObjKey == L"spr_bush_04_")
		return BUSH4;
	else if (strObjKey == L"spr_bush_05_")
		return BUSH5;
	else if (strObjKey == L"Spr_Deco_BushFlower01_")
		return BUSH_FLOWER1;
	else if (strObjKey == L"Spr_Deco_BushFlower02_")
		return BUSH_FLOWER2;
	else if (strObjKey == L"Spr_DestructibleObject_Infectionthorn01_")
		return THORN1;
	else if (strObjKey == L"Spr_DestructibleObject_Infectionthorn02_")
		return THORN2;
	else if (strObjKey == L"Spr_DestructibleObject_Infectionthorn03_")
		return THORN3;
	else if (strObjKey == L"Spr_DestructibleObject_Infectionthorn04_")
		return THORN4;
	else if (strObjKey == L"Spr_DestructibleObject_Infectionthorn05_")
		return THORN5;
	else if (strObjKey == L"Spr_DestructibleObject_Infectionthorn06_")
		return THORN6;
	else if (strObjKey == L"Spr_HistoricSites_DecoObject_")
		return JAR;

	return DYNAMIC_OBJ_END;
}

_int CDynamicObject::Get_MaxFrameCnt(void)
{
	return m_pTextureCom[m_eID]->Get_MaxFrameCnt();
}

void CDynamicObject::Free(void)
{
	__super::Free();
}

HRESULT CDynamicObject::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_ProtoComponent(L"Proto_Calculator"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

	pComponent = m_pTextureCom[BUSH1] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"spr_bush_01_"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"spr_bush_01_", pComponent });

	pComponent = m_pTextureCom[BUSH2] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"spr_bush_02_"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"spr_bush_02_", pComponent });

	pComponent = m_pTextureCom[BUSH3] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"spr_bush_03_"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"spr_bush_03_", pComponent });

	pComponent = m_pTextureCom[BUSH4] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"spr_bush_04_"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"spr_bush_04_", pComponent });

	pComponent = m_pTextureCom[BUSH5] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"spr_bush_05_"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"spr_bush_05_", pComponent });

	pComponent = m_pTextureCom[BUSH_FLOWER1] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Spr_Deco_BushFlower01_"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Spr_Deco_BushFlower01_", pComponent });

	pComponent = m_pTextureCom[BUSH_FLOWER2] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Spr_Deco_BushFlower02_"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Spr_Deco_BushFlower02_", pComponent });

	
	return S_OK;
}