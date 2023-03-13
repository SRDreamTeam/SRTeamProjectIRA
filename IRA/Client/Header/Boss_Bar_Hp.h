#pragma once
#include "FrontUi.h"

class CBoss_Bar_Hp : public CFrontUi
{
private:
	explicit CBoss_Bar_Hp(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBoss_Bar_Hp(const CBoss_Bar_Hp& rhs);
	virtual ~CBoss_Bar_Hp();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

	_int	Get_MaxHp(void) { return m_iMaxHp; }
	_int	Get_CurHp(void) { return m_iCurHp; }

private:
	virtual HRESULT	Add_Component(void) override;
	virtual void	Change_State(void) override;
	virtual void	Frame_Check(const _float& fTimeDelta) override;
	virtual void	Reset_Proj_Matrix(void) override;

public:
	static CBoss_Bar_Hp* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber);

private:
	_int	m_iMaxHp;
	_int	m_iCurHp;


private:
	virtual void Free(void) override;
};

