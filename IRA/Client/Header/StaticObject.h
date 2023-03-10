#pragma once
#include "Landscape.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;
//class CCollider;

END

class CStaticObject : public CLandscape
{
private:
	explicit CStaticObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStaticObject(const CStaticObject& rhs);
	virtual ~CStaticObject();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	STATIC_OBJECT_ID	CompareID(wstring strObjKey);
	STATIC_OBJECT_ID	Get_StaticObj_ID(void) { return m_eID; }
	void				Set_StaticObj_ID(STATIC_OBJECT_ID id) { m_eID = id; }

private:
	virtual HRESULT	Add_Component(void) override;
	virtual void SetUp_OnTerrain(void) override;
	//virtual void Change_State(void)PURE;					
	//virtual void Frame_Check(const _float& fTimeDelta)PURE;

private:
	CTexture*			m_pTextureCom[STATIC_OBJ_END];
	STATIC_OBJECT_ID	m_eID;

public:
	static CStaticObject* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

};

