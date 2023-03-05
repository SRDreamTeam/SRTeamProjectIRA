#pragma once
#include "Bullet.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;
class CCollider;

END

class CMonsterBullet : public CBullet
{	
protected:
	explicit CMonsterBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMonsterBullet(const CMonsterBullet& rhs);
	virtual ~CMonsterBullet();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

protected:
	virtual HRESULT	Add_Component(void) override;
	virtual void SetUp_OnTerrain(void) override;
	virtual void Change_State(void) override;
	virtual void Frame_Check(const _float& fTimeDelta)override;

public:
	static CMonsterBullet* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _Monster_Pos, _bool _Target_Check);

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

