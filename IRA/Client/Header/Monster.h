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

class CMonster : public Engine::CGameObject
{
protected:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMonster(const CMonster& rhs);
	virtual ~CMonster();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

	_float	Get_Hp(void) { return m_fHp; }

protected:
	// 준석 수정 (23.03.02)
	virtual HRESULT	Add_Component(void)PURE;				
	virtual void SetUp_OnTerrain(void)PURE;					
	virtual void Change_State(void)PURE;					
	virtual void Frame_Check(const _float& fTimeDelta)PURE;

protected:
	CRcTex*			m_pBufferCom;
	CTransform*		m_pTransformCom;
	CTexture*		m_pTextureCom;
	CTexture*		m_pTextureCom_2;

	CCollider*		m_pColliderCom;
	CCalculator*	m_pCalculatorCom;
	
	_float			m_fSpeed;
	_float			m_fFrame;
	_bool			m_bCheck;
	_float			m_fHp;

	MONSTER_STATE	m_eState;
	MONSTER_NAME	m_eName;

protected:
	virtual void Free(void) override;

};

