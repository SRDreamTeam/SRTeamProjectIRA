#pragma once

#include "Scene.h"
#include "Base.h"

#include "Player.h"
#include "Monster.h"
#include "GreenEvilSlime.h"
#include "EvilSoul.h"
#include "MutationEvilSoul.h"
#include "FrontUi_Hp.h"

#include "Terrain.h"

class CStage : public Engine::CScene
{
private:
	explicit CStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage();

public:
	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene(void) override;
	virtual			_int		Update_Scene(const _float& fTimeDelta);
	virtual			void		LateUpdate_Scene(void);
	virtual			void		Render_Scene(void);

private:
	HRESULT			Ready_Layer_Environment(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_GameLogic(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_UI(const _tchar* pLayerTag);
	HRESULT			Ready_LightInfo();

public:
	static	CStage*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};

