#pragma once
#include "FrontUi.h"

class CBoss_Bar_Hp_Delay : public CFrontUi
{
private:
	explicit CBoss_Bar_Hp_Delay(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBoss_Bar_Hp_Delay(const CBoss_Bar_Hp_Delay& rhs);
	virtual ~CBoss_Bar_Hp_Delay();

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
	static CBoss_Bar_Hp_Delay* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber);

private:
	_float	m_fHp;
	_float	m_fReceiveHp;
	_bool	m_bCount;
	_float	m_fDelay;



private:
	virtual void Free(void) override;
};

