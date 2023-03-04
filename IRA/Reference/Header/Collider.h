#pragma once
#include "Component.h"
#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CCollider : public CComponent
{
private:
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCollider();

public:
	void				Set_SpherePos(const _vec3 vSpherePos) { m_vSpherePos = vSpherePos; }
	void				Set_Radius(const _float fRadius) {m_fRadius = fRadius;}
	void				Set_TransformCom(CTransform* pTransformCom) { m_pTransformCom = pTransformCom; }
	const _vec3&		Get_SpherePos() { return m_vSpherePos; }
	const _float&		Get_Radius() { return m_fRadius; }


public:
	HRESULT				Ready_CCollider(void);
	virtual _int		Update_Component(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Component() override;

public:
	static CCollider*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*		Clone(void);
private:
	virtual void Free(void);

private:
	_vec3					m_vSpherePos = {};
	_float					m_fRadius = 0.f;

	CTransform*				m_pTransformCom;
};

END

