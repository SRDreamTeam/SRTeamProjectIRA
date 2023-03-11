#include "..\..\Header\GameObject.h"


CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	if (m_pGraphicDev)
		m_pGraphicDev->AddRef();
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)	
{
	m_pGraphicDev->AddRef();
}

CGameObject::~CGameObject()
{
}

CComponent * CGameObject::Get_Component(const _tchar * pComponentTag, COMPONENTID eID)
{
	CComponent*		pComponent = Find_Component(pComponentTag, eID);
	NULL_CHECK_RETURN(pComponent, nullptr);
	
	return pComponent;
}

_int CGameObject::Update_GameObject(const _float & fTimeDelta)
{
	for (auto& iter : m_uMapComponent[ID_DYNAMIC])
		iter.second->Update_Component(fTimeDelta);

	return _int();
}

void CGameObject::LateUpdate_GameObject()
{
	for (auto& iter : m_uMapComponent[ID_DYNAMIC])
		iter.second->LateUpdate_Component ();
}

Engine::CComponent* Engine::CGameObject::Find_Component(const _tchar* pComponentTag, COMPONENTID eID)
{
	auto	iter = find_if(m_uMapComponent[eID].begin(), m_uMapComponent[eID].end(), CTag_Finder(pComponentTag));

	if (iter == m_uMapComponent[eID].end())
		return nullptr;

	return iter->second;
}

void CGameObject::Free(void)
{
	Safe_Release(m_pGraphicDev);

	for (size_t i = 0; i < ID_END; ++i)
	{
		for_each(m_uMapComponent[i].begin(), m_uMapComponent[i].end(), CDeleteMap());
		m_uMapComponent[i].clear();
	}

}
