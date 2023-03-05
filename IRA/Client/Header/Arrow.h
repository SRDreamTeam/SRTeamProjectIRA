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

class CArrow : public Engine::CGameObject
{

	enum {ARROW_IDLE,ARROW_DEATH,ARROW_END};


protected:
	explicit CArrow(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CArrow(const CArrow& rhs);
	virtual ~CArrow();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	void Set_Target_Pos(_vec3 vPos) {
		m_target_Pos = vPos;
	}

protected:

	virtual HRESULT	Add_Component(void)PURE;				
	virtual void SetUp_OnTerrain(void)PURE;					
	virtual void Change_State(void)PURE;					
	virtual void Frame_Check(const _float& fTimeDelta)PURE;

protected:
	CRcTex*			m_pBufferCom;

	CTransform*		m_pTransformCom;

	CTexture*       m_pTextureCom[ARROW_END] = {};
	
	CCollider*		m_pColliderCom;

	CCalculator*	m_pCalculatorCom;
	
	_float			m_fSpeed;

	_float			m_fIdleFrame = 0.f;
	_float			m_fDeathFrame = 0.f;

	_vec3           m_vDir;
	
	_vec3           m_target_Pos;
	_vec3           m_Fire_Pos;

	int             m_iState = ARROW_IDLE;
	int             m_iPreState = ARROW_IDLE;

protected:
	virtual void Free(void) override;

};

