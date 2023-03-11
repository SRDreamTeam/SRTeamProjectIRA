#include "..\..\Header\Layer.h"
#include "Collider.h"

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

	m_uMapObject.insert({pObjTag, pGameObject});

	return S_OK;
}

void CLayer::Add_BulletObject(CGameObject* pGameObject)
{
	if (pGameObject == nullptr)
		return;

	m_ListBulletObject.push_back(pGameObject);
}

HRESULT CLayer::Ready_Layer(void)
{
	return S_OK;
}

_int CLayer::Update_Layer(const _float & fTimeDelta)
{
	int iResult = 0;

	for (auto iter = m_uMapObject.begin(); iter != m_uMapObject.end();)
	{
		iResult = iter->second->Update_GameObject(fTimeDelta);

		if (OBJ_DEAD == iResult)
		{
			Safe_Delete <CGameObject*>(iter->second);
			m_uMapObject.erase(iter++);
		}
		else
			++iter;
	}

	for (auto iter = m_ListBulletObject.begin(); iter != m_ListBulletObject.end();)
	{
		iResult = (*iter)->Update_GameObject(fTimeDelta);

		if (OBJ_DEAD == iResult)
		{
			Safe_Delete <CGameObject*>(*iter);
			iter = m_ListBulletObject.erase(iter);
		}
		else
			++iter;
	}

	return iResult;
}

void CLayer::LateUpdate_Layer(void)
{
	for (auto iter = m_uMapObject.begin(); iter != m_uMapObject.end(); ++iter)
		iter->second->LateUpdate_GameObject();

	for (auto iter = m_ListBulletObject.begin(); iter != m_ListBulletObject.end(); ++iter)
		(*iter)->LateUpdate_GameObject();
}

CLayer* CLayer::Create(void)
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

	for_each(m_ListBulletObject.begin(), m_ListBulletObject.end(), CDeleteObj());
	m_ListBulletObject.clear();
}
