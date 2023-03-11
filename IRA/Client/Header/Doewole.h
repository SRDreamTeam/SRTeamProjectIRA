#pragma once
#include "Boss.h"
#include "Base.h"

class CDoewole : public CBoss
{
protected:
	explicit CDoewole(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDoewole(const CDoewole& rhs);
	virtual ~CDoewole();

public:
	enum DOEWOLE_STATE { IDLE, MOVE, STANDARD_ATTACK, OUTSTRECTH_ATTACK , SMASH_ATTACK , SCRATCH_ATTACK, AREA_ATTACK
						,UPGRADE_SMASH_ATTACK, STATE_END };
public:
	void			Set_State(DOEWOLE_STATE _eState) { m_eCurState = _eState; }
	DOEWOLE_STATE	Get_State() { return m_eCurState; }
	_bool			Get_AttackToIdle() { return m_bAttackToIdle; }
	_bool			Get_Disappear() { return m_bDisappear; }
	void			Set_Disappear(_bool _b) { m_bDisappear = _b; }
	_bool			Get_BulletGo() { return m_bBulletGo; }
	void			Set_BulletGo() { m_bBulletGo = true; }

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject() {};

public:
	static CDoewole*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free(void) override;

protected:
	DOEWOLE_STATE		m_eCurState = IDLE;
	DOEWOLE_STATE		m_ePreState = STATE_END;
	_float				m_fAccTime = 0.f;
	_bool				m_bRender = true;
	_float				m_fMoveSpeed = 25.f;

private:
	void			Idle(const _float& fTimeDelta);
	void			Move(const _float& fTimeDelta);
	void			Standard_Attack(const _float& fTimeDelta);
	void			OutStretch_Attack(const _float& fTimeDelta);
	void			Smash_Attack(const _float& fTimeDelta);
	void			Scratch_Attack(const _float& fTimeDelta);
	void			Area_Attack(const _float& fTimeDelta);
	void			Upgrade_Smash_Attack(const _float& fTimeDelta);

	void			Create_StandardBullet();
	void			Create_CircleBullet();

	void			AreaAtaackPattern(const _float& fTimeDelta);

private:
	HRESULT			Add_Component(void);
	void			State_Update(const _float& fTimeDelta);

	_float			CalculateZ(_float fX); 

private:
	_uint					m_iPattern = 2;
	const _uint				m_iMaxPattern = 8;

	_uint					m_iStandardAttackCnt = 0;

	_bool					m_bAttackToIdle = false;
	_bool					m_bEffect = false;

	_float					m_fAccTime2 = 0.f;

	_bool					m_bDisappear = false;
	_bool					m_bChasePlayer = false;
	
	_bool					m_bRight = false;

	_bool					m_bAreaAttack = false;

	_bool					m_bTest = false;
	_float					m_fThornZ = 0.f;
	_int					m_iThornCnt = 0;

	_bool					m_bCrossTron = false;
	_float					m_fThronX = 128.f;

	_bool					m_bBulletGo = false;
};

