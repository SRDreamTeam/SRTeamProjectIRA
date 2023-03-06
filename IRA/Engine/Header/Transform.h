#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
private:
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform();

public:
	const _matrix*		Get_WorldMatrixPointer() { return &m_matWorld; }
	void				Get_Info(INFO eType, _vec3* pInfo)
	{
		memcpy(pInfo, &m_matWorld.m[eType][0], sizeof(_vec3));
	}
	void				Move_Pos(const _vec3* const pDir)
	{
		m_vInfo[INFO_POS] += *pDir;
	}
	void				Move_Pos(const _vec3* pPos, const _float& fSpeed, const _float& fTimeDelta)
	{
		_vec3	vDir = *pPos - m_vInfo[INFO_POS];
		m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fTimeDelta * fSpeed;
	} 

	void	Rotation(ROTATION eRot, const _float& fAngle)
	{
		*(((_float*)&m_vAngle) + eRot) += fAngle;
	}
	void				Set_Pos(const 	_float& fX, const _float& fY, const _float& fZ)
	{
		m_vInfo[INFO_POS].x = fX;
		m_vInfo[INFO_POS].y = fY;
		m_vInfo[INFO_POS].z = fZ;
	}

	void				Set_Right_Reverse()
	{
		m_vInfo[INFO_RIGHT] *= -1;
		
	}

	void                Set_Scale(_vec3 scale) 
	{
		m_vScale.x /= fabs(m_vScale.x);
		m_vScale.x *= scale.x;

		m_vScale.y /= fabs(m_vScale.y);
		m_vScale.y *= scale.y;
	}

	void                Set_Scale_X(_float x)
	{
		m_vScale.x *= x;
	}



	void					Chase_Target(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta, const MONSTER_NAME m_eName);
	void					Bullet_Move(const _vec3 _Dir, const _float& fSpeed, const _float& fTimeDelta, _int iDirCount);
	_vec3					Patrol_Map(const _float& fSpeed, const _float& fTimeDelta);
	void					Reverse_Scale_x(void);
	void					UpdatePos_OnWorld(void);
	const	_matrix*		Compute_LookAtTarget(const _vec3* pTargetPos);

	void					Ui_Print(void);

public:
	HRESULT		Ready_Transform(void);
	virtual _int Update_Component(const _float& fTimeDelta) override;
	virtual void LateUpdate_Component() override;

public:
	_vec3			m_vInfo[INFO_END];
	_vec3			m_vScale;
	_vec3			m_vAngle;
	_matrix			m_matWorld;

	
	
public:
	static CTransform*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void) override;

private:
	_vec3		m_vPatrolTarget;
	_vec3		m_vBulletTarget = {0, 0, 0};

private:
	virtual void Free(void) override;

};

END
