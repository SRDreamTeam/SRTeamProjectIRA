#pragma once
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CGhostChild;

class CGhost : public Engine::CGameObject
{
	enum {PLAYER_HEAD,PLAYER_FACE,PLAYER_END};

	enum { DASH_045, DASH_135, DASH_END };
	
protected:
	explicit CGhost(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGhost(const CGhost& rhs);
	virtual ~CGhost();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);
	

private:
	CRcTex*			m_pBufferCom;
	CTransform*		m_pTransformCom;

	CTexture*       m_pDashTextureCom[DASH_END] = {};

	CCalculator*	m_pCalculatorCom;

private:	
	_float			m_fSpeed = 20.f;

	CGhostChild*    m_Child_List[7];

public:
	static CGhost*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free(void) override;

};



