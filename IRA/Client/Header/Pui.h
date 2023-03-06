#pragma once
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CPui : public Engine::CGameObject
{
protected:
	explicit CPui(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPui(const CPui& rhs);
	virtual ~CPui();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

protected:
	virtual HRESULT	Add_Component(void)PURE;
	virtual void SetUp_OnTerrain(void)PURE;
	virtual void Change_State(void)PURE;
	virtual void Frame_Check(const _float& fTimeDelta)PURE;

protected:
	CRcTex* m_pBufferCom;
	CTransform* m_pTransformCom;
	CTexture* m_pTextureCom;

	_float			m_fFrame;
	_bool			m_bCheck;
	UI_CATEGORY		m_eCategory;

protected:
	virtual void Free(void) override;

};

