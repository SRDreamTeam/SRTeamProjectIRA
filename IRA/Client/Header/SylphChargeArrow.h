#pragma once
#include "Arrow.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CSylphChargeArrow : public CArrow
{

	enum { ARROW_IDLE, ARROW_DEATH, ARROW_HIT, ARROW_END };

private:
	explicit CSylphChargeArrow(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSylphChargeArrow(const CSylphChargeArrow& rhs);
	virtual ~CSylphChargeArrow();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	virtual HRESULT	Add_Component(void) override;					
	virtual void	SetUp_OnTerrain(void) override;					
	virtual void	Change_State(void) override;					
	virtual void	Frame_Check(const _float& fTimeDelta) override;

public:
	static CSylphChargeArrow* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vDir,_float Angle);

public:
	bool  m_bHit = false;
	void Create_Hit_Effect(void);

private:
	virtual void Free(void) override;

	bool  m_bDisappear = false;
	float m_AccTime = 0.f;
	float m_AccMaxTime = 1.f;



};

