#pragma once
#include "Effect.h"

class CEffect_Monster_Dead_1 : public CEffect
{
private:
	explicit CEffect_Monster_Dead_1(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Monster_Dead_1(const CEffect_Monster_Dead_1& rhs);
	virtual ~CEffect_Monster_Dead_1();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);
	
public:
	static CEffect_Monster_Dead_1* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	void	Create_Next_Effect(void);

private:
	_vec3	m_vPos;

private:
	virtual void Free(void) override;
};

