#pragma once
#include "Effect.h"
#include "Base.h"

class CEffect_AlertCircle : public CEffect
{
private:
	explicit CEffect_AlertCircle(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_AlertCircle(const CEffect_AlertCircle& rhs);
	virtual ~CEffect_AlertCircle();

public:
	virtual HRESULT Ready_GameObject(const _vec3& vPos , const _vec3& vScale, const _float& fAliveTime);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CEffect_AlertCircle*		Create(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3& vPos , const _vec3& vScale ,  const _float& fAliveTime);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;
	_float				m_fAliveTime = 0.f;
	_vec3				m_vMaxScale;
};

