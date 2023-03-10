#pragma once
#include "Bow.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;


END

class CSylphBowPair : public CBow
{

	enum { ARROW_IDLE, ARROW_DEATH, ARROW_END };

private:
	explicit CSylphBowPair(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSylphBowPair(const CSylphBowPair& rhs);
	virtual ~CSylphBowPair();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	void    Update_Bow_State();

	void    Set_Render(bool render) {
		 m_bRender = render;
	}

private:
	virtual HRESULT	Add_Component(void) override;					
	virtual void	SetUp_OnTerrain(void) override;					
	virtual void	Change_State(void) override;					
	virtual void	Frame_Check(const _float& fTimeDelta) override;

public:
	static CSylphBowPair* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _float Angle);

private:
	virtual void Free(void) override;

	bool   m_bRender = false;
	bool   m_bRealRender = false;
	float  m_Time = 3.f;
	float  m_fFrame = 0.f;
	

public:
	_matrix m_MatWorld;
	_vec3   m_vPos;
	bool   m_bCharge;
};

