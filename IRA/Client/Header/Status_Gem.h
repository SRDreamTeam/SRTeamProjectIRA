#pragma once
#include "FrontUi.h"

class CStatus_Gem : public CFrontUi
{
private:
	explicit CStatus_Gem(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStatus_Gem(const CStatus_Gem& rhs);
	virtual ~CStatus_Gem();

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
	static CStatus_Gem* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber);

private:
	virtual void Free(void) override;
};
