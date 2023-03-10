#pragma once
#include "Arrow.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CSylphArrow : public CArrow
{

	enum { ARROW_IDLE, ARROW_DEATH, ARROW_HIT, ARROW_END };

private:
	explicit CSylphArrow(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSylphArrow(const CSylphArrow& rhs);
	virtual ~CSylphArrow();

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
	static CSylphArrow* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vDir,_float Angle);


public:
	bool Final_Damage(void);


public:
	void Create_Hit_Effect(void);
	void Create_Damage_Font(void);
	void Create_Font_List(int damage);

private:
	virtual void Free(void) override;

	bool  m_bDisappear = false;
	float m_AccTime = 0.f;
	float m_AccMaxTime = 10.f;

	float m_fPower = 4197.f;
	int   m_fAttack_Num = 1;
	float m_fRandom_Value = 0.f;
	float m_fDamage = 0.f;

	bool  m_Is_Cri = false;

	list<int> m_Font_List;
	

};

