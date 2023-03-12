#pragma once
#include "DynamicObject.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;
//class CCollider;

END

class CMovingObj : public CDynamicObject
{
private:
	explicit CMovingObj(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMovingObj(const CMovingObj& rhs);
	virtual ~CMovingObj();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:


private:
	virtual HRESULT	Add_Component(void)override;
	virtual void SetUp_OnTerrain(void)override;
	//virtual void Change_State(void)PURE;					
	//virtual void Frame_Check(const _float& fTimeDelta)PURE;


public:
	static CMovingObj* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

};

