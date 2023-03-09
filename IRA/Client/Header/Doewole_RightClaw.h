#pragma once
#include "Doewole.h"
#include "Base.h"

class CDoewole_RightClaw : public CDoewole
{
	enum { STAND, STANDARD_ATTACK, OUTSTRECTH_ATTACK, UP, SMASH, SCRATCH ,TEX_END };

private:
	explicit CDoewole_RightClaw(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDoewole_RightClaw(const CDoewole_RightClaw& rhs);
	virtual ~CDoewole_RightClaw();

public:
	void		Set_Owner(CGameObject* pOwner) { m_pOwner = pOwner; }
	void		Set_Wait(_bool	_b) { m_bWait = _b;  m_fAccTime = 0.f; }


public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);
	void			State_Update(const _float& fTimeDelta);

private:
	void			Idle(const _float& fTimeDelta);
	void			Move(const _float& fTimeDelta);
	void			Standard_Attack(const _float& fTimeDelta);
	void			OutStretch_Attack(const _float& fTimeDelta);
	void			Smash_Attack(const _float& fTimeDelta);
	void			Scratch_Attack(const _float& fTimeDelta);
	void			Area_Attack(const _float& fTimeDelta);

public:
	static CDoewole_RightClaw*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

private:
	CGameObject*				m_pOwner = nullptr;
	CTexture*					m_pTextureCom[TEX_END];

	_bool						m_bUp = false;
	_bool						m_bDown = false;

	_int						m_iReplayCnt = 0;

	_bool						m_bWait = false;

	_bool						m_bHurt = false;
	
	_bool						m_bSmash = false;

};

