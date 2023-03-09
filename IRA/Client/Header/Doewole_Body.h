#pragma once
#include "Doewole.h"
#include "Base.h"

class CDoewole_Body : public CDoewole
{
	enum {STAND, STAND_FACEON ,POWERSLAM ,POWERSLAM_AFTER, POWERSLAM_FACEOFF, TEX_END};


private:
	explicit CDoewole_Body(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDoewole_Body(const CDoewole_Body& rhs);
	virtual ~CDoewole_Body();

public:
	void		Set_Owner(CGameObject* pOwner) { m_pOwner = pOwner; }


public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	static CDoewole_Body*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

private:
	HRESULT			Add_Component(void);
	void			State_Update(const _float& fTimeDelta);

private:
	void			Idle(const _float& fTimeDelta);
	void			Move(const _float& fTimeDelta);
	void			Standard_Attack(const _float& fTimeDelta);
	void			Smash_Attack(const _float& fTimeDelta);

private:
	CGameObject*					m_pOwner = nullptr;
	_float							m_fAlpha = 1.f;
	CTexture*						m_pTextureCom[TEX_END];

	_bool							m_bPowerSlam = false;
	_bool							m_bPowerSlamAfter = false;
	_bool							m_bPowerSlamFaceOff = false;

};

