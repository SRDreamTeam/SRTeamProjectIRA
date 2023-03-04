#include "..\..\Header\Layer.h"
#include "Collider.h"
#include "CollisionMgr.h"

CLayer::CLayer()
{
}

CLayer::~CLayer()
{
}

CComponent * CLayer::Get_Component(const _tchar * pObjTag, const _tchar * pComponentTag, COMPONENTID eID)
{
	for (int i = 0; i < OBJ_END; ++i)
	{
		auto	iter = find_if(m_uMapObject[i].begin(), m_uMapObject[i].end(), CTag_Finder(pObjTag));

		if (iter == m_uMapObject[i].end())
			continue;

		return iter->second->Get_Component(pComponentTag, eID);
	}

	return nullptr;
}

CGameObject * CLayer::Get_GameObject(const _tchar * pObjTag)
{
	for (int i = 0; i < OBJ_END; ++i)
	{
		auto	iter = find_if(m_uMapObject[i].begin(), m_uMapObject[i].end(), CTag_Finder(pObjTag));

		if (iter == m_uMapObject[i].end())
			continue;

		return iter->second;
	}

	return nullptr;
}


HRESULT CLayer::Add_GameObject(const _tchar * pObjTag, CGameObject * pGameObject , OBJ_TYPE eID)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_uMapObject[eID].insert({pObjTag, pGameObject});

	return S_OK;
}

HRESULT CLayer::Ready_Layer(void)
{
	return S_OK;
}

_int CLayer::Update_Layer(const _float & fTimeDelta)
{
	int iResult = 0;

	for (int i = 0; i < OBJ_END; ++i)
	{
		for (auto iter = m_uMapObject[i].begin(); iter != m_uMapObject[i].end();)
		{
			int iResult = iter->second->Update_GameObject(fTimeDelta);

			if (OBJ_DEAD == iResult)
			{
				Safe_Delete <CGameObject*>(iter->second);
				m_uMapObject[i].erase(iter++);
			}
			else
				++iter;
		}
	}

	return iResult;
}

void CLayer::LateUpdate_Layer(void)
{
	_int iResult = 0;

	for (int i = 0; i < OBJ_END; ++i)
	{
		auto iter = m_uMapObject[i].begin();

		for (; iter != m_uMapObject[i].end(); ++iter)
			iter->second->LateUpdate_GameObject();
	}

	//Engine::CCollisionMgr::Collision_Sphere(m_uMapObject[OBJ_PLAYER], m_uMapObject[OBJ_MONSTER]);



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
	for (int i = 0; i < OBJ_END; ++i)
	{
		for_each(m_uMapObject[i].begin(), m_uMapObject[i].end(), CDeleteMap());
		m_uMapObject[i].clear();
	}
}
