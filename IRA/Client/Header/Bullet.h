#pragma once
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;
class CCollider;

END

class CBullet : public Engine::CGameObject
{	
protected:
	explicit CBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBullet(const CBullet& rhs);
	virtual ~CBullet();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual HRESULT Ready_GameObject(_vec3 _Monster_Pos, _bool _Target_Check) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

protected:
	virtual HRESULT	Add_Component(void)PURE;
	virtual void SetUp_OnTerrain(void)PURE;
	virtual void Change_State(void)PURE;
	virtual void Frame_Check(const _float& fTimeDelta)PURE;

protected:

	CRcTex*			m_pBufferCom;
	CTransform*		m_pTransformCom;
	CTexture*		m_pTextureCom;

	CCollider*		m_pColliderCom;
	CCalculator*	m_pCalculatorCom;

	_float			m_fSpeed;
	_float			m_fFrame;
	_bool			m_bCheck;

	_vec3			m_vTargetPos;

	BULLET_STATE	m_eState;

protected:
	virtual void Free(void) override;

};

