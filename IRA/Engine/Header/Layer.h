#pragma once
#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLayer : public CBase
{
private:
	explicit CLayer();
	virtual ~CLayer();

public:
	CComponent*				Get_Component(const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
	CGameObject*			Get_GameObject(const _tchar* pObjTag);
	
public:
	HRESULT		Add_GameObject(const _tchar* pObjTag, CGameObject* pGameObject);
	void		Add_BulletObject(CGameObject* pGameObject);

	HRESULT		Ready_Layer(void);
	_int		Update_Layer(const _float& fTimeDelta);
	void		LateUpdate_Layer(void);	

private:
	unordered_map<const _tchar*, CGameObject*>		m_uMapObject;
	list<CGameObject*>							m_ListBulletObject;

public:
	static CLayer*		Create(void);

private:
	virtual void Free(void);
};

END