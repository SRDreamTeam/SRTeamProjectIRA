#pragma once
#include "Base.h"
#include "Light.h"

BEGIN(Engine)

class ENGINE_DLL CLightMgr : public CBase
{
	DECLARE_SINGLETON(CLightMgr)
private:
	explicit CLightMgr();
	virtual ~CLightMgr();

public:
	HRESULT			Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex);

public:
	list<CLight*>			m_LightList;

private:
	virtual void	Free(void);
};

END