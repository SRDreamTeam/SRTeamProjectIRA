#pragma once
#include "Effect.h"
#include "Base.h"

class CEffect_Player_Damage_Font : public CEffect
{
	
private:
	explicit CEffect_Player_Damage_Font(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Player_Damage_Font(const CEffect_Player_Damage_Font& rhs);
	virtual ~CEffect_Player_Damage_Font();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);
	void            Movement(const _float& fTimeDelta);

public:
	static CEffect_Player_Damage_Font*		Create(LPDIRECT3DDEVICE9 pGraphicDev , _vec3 vPos , int num , bool Critical);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;
	_vec3               m_vPos;
	_vec3               m_vOriginPos;
	bool                m_Is_Critical = false;
	int                 m_iAlpha = 255;

};

