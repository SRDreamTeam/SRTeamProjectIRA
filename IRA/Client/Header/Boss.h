#pragma once
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;
class CCollider;
class CSphereTex;

END

class CBoss : public Engine::CGameObject
{
protected:
	explicit CBoss(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBoss(const CBoss& rhs);
	virtual ~CBoss();

public:
	virtual HRESULT Ready_GameObject(void) PURE;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() {}

protected:
	CRcTex*			m_pBufferCom;
	CTransform*		m_pTransformCom;
	CCalculator*	m_pCalculatorCom;
	CCollider*		m_pColliderCom;
	CSphereTex*		m_pSphereBufferCom;

protected:
	_float			m_fFrame = 0.f;
	_float			m_fBossHP = 0.f;
	_float			m_fMaxFrame = 0.f;
	
protected:
	virtual void Free(void) override;

};

