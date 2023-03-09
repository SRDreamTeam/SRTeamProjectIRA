#pragma once
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)

class CTerrainTex;
class CTransform;
class CTexture;

END

class CTerrain : public Engine::CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrain(const CTerrain& rhs);
	virtual ~CTerrain();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);
	HRESULT			SetUp_Material(void);
	void			Key_Input(const _float& fTimeDelta);

private:
	CTerrainTex*	m_pBufferCom;
	CTransform*	m_pTransformCom;
	CTexture*	m_pTextureCom;
	_float		m_fSpeed = 5.f;

public:
	static CTerrain*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

};

