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
	enum DOEWOLE_STATE { IDLE, MOVE, STATE_END };
public:
	void			Set_State(DOEWOLE_STATE _eState) { m_eCurState = _eState; }
	DOEWOLE_STATE	Get_State() { return m_eCurState; }

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
	_float				m_fMoveSpeed = 10.f;

private:
	void			Idle(const _float& fTimeDelta);
	void			Move(const _float& fTimeDelta);

private:
	HRESULT			Add_Component(void);
	void			State_Update(const _float& fTimeDelta);

	bool			m_bTest = false;
};

