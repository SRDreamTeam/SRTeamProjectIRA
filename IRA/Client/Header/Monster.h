#pragma once
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)

class CTriCol;
class CTransform;
class CCollider;

END

class CMonster : public Engine::CGameObject
{
private:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMonster(const CMonster& rhs);
	virtual ~CMonster();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

private:
	CTriCol*		m_pBufferCom;
	CTransform*		m_pTransformCom;
	CCollider*		m_pColliderCom;
	
	_float			m_fSpeed = 5.f;

public:
	static CMonster*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

};

