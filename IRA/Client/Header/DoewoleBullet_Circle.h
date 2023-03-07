#pragma once
#include "Bullet.h"

class CDoewoleBullet_Circle : public CBullet
{
protected:
	explicit CDoewoleBullet_Circle(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDoewoleBullet_Circle(const CDoewoleBullet_Circle& rhs);
	virtual ~CDoewoleBullet_Circle();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

protected:
	virtual HRESULT	Add_Component(void) override;
	virtual void SetUp_OnTerrain(void) override;
	virtual void Change_State(void) override;
	virtual void Frame_Check(const _float& fTimeDelta)override;
	virtual void Distance_Dead_Check(void) override;

public:
	static CDoewoleBullet_Circle* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _Monster_Pos, _int _Dir_Count);

private:
	void Set_FireInfo(_vec3 _Monster_Pos, _int _Dir_Count);

private:
	_int	m_iDirCount;
private:
	virtual void Free(void) override;
};

