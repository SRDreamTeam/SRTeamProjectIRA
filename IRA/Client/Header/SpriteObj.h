#pragma once
#include "DynamicObject.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;
//class CCollider;

END

class CSpriteObj : public CDynamicObject
{
private:
	explicit CSpriteObj(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSpriteObj(const CSpriteObj& rhs);
	virtual ~CSpriteObj();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	void	Set_IsSprite(_bool bSprite) { m_bSprite = bSprite; }
	_bool	Get_IsSprite(void) { return m_bSprite; }

private:
	//_bool	m_bSprite = false;
	_bool	m_bSprite = true;
	_float	m_fFrame = 0.f;

public:
	static CSpriteObj* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

};

