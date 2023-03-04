#pragma once
#include "Monster.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CEvilSoul : public CMonster
{
private:
	explicit CEvilSoul(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEvilSoul(const CEvilSoul& rhs);
	virtual ~CEvilSoul();

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

	void	Head_Check(const _vec3	vDir);

public:
	static CEvilSoul* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	CTexture* m_pTextureCom_135_1;	//45�� ��ӹ��� Tex Ptr��, 135�� ���� Ŭ������ ������ ����
	CTexture* m_pTextureCom_135_2;
	MONSTER_HEAD	m_eHead;

private:
	virtual void Free(void) override;
};

