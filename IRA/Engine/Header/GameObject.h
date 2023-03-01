#pragma once

#include "Base.h"
#include "Include.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject :
	public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject();

public:
	CComponent*				Get_Component(const _tchar* pComponentTag, COMPONENTID eID);

public:
	virtual		HRESULT		Ready_GameObject(void)PURE;
	virtual		_int		Update_GameObject(const _float& fTimeDelta);
	virtual		void		LateUpdate_GameObject();
	virtual		void		Render_GameObject()PURE;

protected:
	unordered_map<const _tchar*, CComponent*>		m_uMapComponent[ID_END];
	LPDIRECT3DDEVICE9 m_pGraphicDev;

private:
	CComponent*			Find_Component(const _tchar* pComponentTag, COMPONENTID eID);

protected:
	virtual void		Free(void);
};

END