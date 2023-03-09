#pragma once
#include "FrontUi.h"

class CApostle_Frame_Back : public CFrontUi
{
private:
	explicit CApostle_Frame_Back(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CApostle_Frame_Back(const CApostle_Frame_Back& rhs);
	virtual ~CApostle_Frame_Back();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	virtual HRESULT	Add_Component(void) override;
	virtual void	Change_State(void) override;
	virtual void	Frame_Check(const _float& fTimeDelta) override;
	virtual void	Reset_Proj_Matrix(void) override;

public:
	static CApostle_Frame_Back* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber);

private:
	virtual void Free(void) override;
};

