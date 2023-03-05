#pragma once
#include "Arrow.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CSylphArrow : public CArrow
{

	enum { ARROW_IDLE, ARROW_DEATH, ARROW_END };

private:
	explicit CSylphArrow(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSylphArrow(const CSylphArrow& rhs);
	virtual ~CSylphArrow();

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
	static CSylphArrow* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vDir);

private:
	virtual void Free(void) override;





};

