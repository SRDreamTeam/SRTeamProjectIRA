#pragma once

#include "Scene.h"
#include "Base.h"

#include "BackGround.h"

#include "Loading.h"

class CLogo : public Engine::CScene
{
private:
	explicit CLogo(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLogo();

public:
	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene(void) override;
	virtual			_int		Update_Scene(const _float& fTimeDelta);
	virtual			void		LateUpdate_Scene(void);
	virtual			void		Render_Scene(void);

private:
	HRESULT			Ready_Layer_Environment(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_GameLogic(const _tchar* pLayerTag) { return S_OK; }
	HRESULT			Ready_Layer_UI(const _tchar* pLayerTag) { return S_OK; }
	HRESULT			Ready_Prototype(void);

private:
	CLoading*			m_pLoading;

public:
	static	CLogo*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};

