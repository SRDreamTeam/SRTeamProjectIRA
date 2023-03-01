#include "..\..\Header\Layer.h"

CLayer::CLayer()
{
}

CLayer::~CLayer()
{
}

CComponent * CLayer::Get_Component(const _tchar * pObjTag, const _tchar * pComponentTag, COMPONENTID eID)
{
	auto	iter = find_if(m_uMapObject.begin(), m_uMapObject.end(), CTag_Finder(pObjTag));

	if (iter == m_uMapObject.end())
		return nullptr;

	return iter->second->Get_Component(pComponentTag, eID);
}

CGameObject * CLayer::Get_GameObject(const _tchar * pObjTag)
{
	auto	iter = find_if(m_uMapObject.begin(), m_uMapObject.end(), CTag_Finder(pObjTag));

	if (iter == m_uMapObject.end())
		return nullptr;

	return iter->second;
}


HRESULT CLayer::Add_GameObject(const _tchar * pObjTag, CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_uMapObject.insert({ pObjTag, pGameObject });

	return S_OK;
}

HRESULT CLayer::Ready_Layer(void)
{
	return S_OK;
}

_int CLayer::Update_Layer(const _float & fTimeDelta)
{
	_int iResult = 0;

	for (auto& iter : m_uMapObject)
	{
		iResult = iter.second->Update_GameObject(fTimeDelta);

		if (iResult & 0x80000000)
			return iResult;
	}

	return iResult;
}

void CLayer::LateUpdate_Layer(void)
{
	for (auto& iter : m_uMapObject)
		iter.second->LateUpdate_GameObject();
}


CLayer * CLayer::Create(void)
{
	CLayer *	pInstance = new CLayer;

	if (FAILED(pInstance->Ready_Layer()))
		Safe_Release(pInstance);

	return pInstance;
}

void CLayer::Free(void)
{
	for_each(m_uMapObject.begin(), m_uMapObject.end(), CDeleteMap());
	m_uMapObject.clear();
}
