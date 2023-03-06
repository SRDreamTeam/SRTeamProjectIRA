#pragma once
#include "Pui.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CFrontUi_Hp : public CPui
{
private:
	explicit CFrontUi_Hp(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFrontUi_Hp(const CFrontUi_Hp& rhs);
	virtual ~CFrontUi_Hp();

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
	static CFrontUi_Hp* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	//View
	//_vec3	m_vAxis[3];

	_int	iMax_Hp;
	_int	iNow_Hp;

private:
	virtual void Free(void) override;
};

