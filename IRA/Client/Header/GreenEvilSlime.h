#pragma once
#include "Monster.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CGreenEvilSlime : public CMonster
{
private:
	explicit CGreenEvilSlime(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGreenEvilSlime(const CGreenEvilSlime& rhs);
	virtual ~CGreenEvilSlime();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	virtual HRESULT	Add_Component(void) override;					
	virtual void	SetUp_OnTerrain(void) override;					
	virtual void	Change_State(void) override;					
	virtual void	Frame_Check(const _float& fTimeDelta) override;

public:
	static CGreenEvilSlime* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	void	Create_Dead_Effect(void);
	virtual void Free(void) override;
};

