#pragma once
#include "Effect.h"
#include "Base.h"

class CEffect_Player_Bow_Pulse : public CEffect
{

	enum { BOW_TYPE_SYLPH, BOW_TYPE_END };

private:
	explicit CEffect_Player_Bow_Pulse(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Player_Bow_Pulse(const CEffect_Player_Bow_Pulse& rhs);
	virtual ~CEffect_Player_Bow_Pulse();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CEffect_Player_Bow_Pulse*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;
	int                 m_Bow_Type = BOW_TYPE_SYLPH;

};

