#include "..\..\Header\Transform.h"


CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_vScale(1.f, 1.f, 1.f)
	, m_vAngle(0.f, 0.f, 0.f)
{
	ZeroMemory(m_vInfo, sizeof(m_vInfo));
	D3DXMatrixIdentity(&m_matWorld);
}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_vScale(rhs.m_vScale)
	, m_vAngle(rhs.m_vAngle)
	, m_matWorld(rhs.m_matWorld)
{
	for (size_t i = 0; i < INFO_END; ++i)
		m_vInfo[i] = rhs.m_vInfo[i];
}

CTransform::~CTransform()
{
}

void CTransform::Chase_Target(const _vec3 * pTargetPos, const _float & fSpeed, const _float & fTimeDelta, MONSTER_NAME m_eName)
{
	_vec3		vDir = *pTargetPos - m_vInfo[INFO_POS];

	switch (m_eName)
	{
	case NAME_SLIME:
		if (!(((vDir.x > -2.0f) && (vDir.x < 2.0f)) && ((vDir.z > -2.0f) && (vDir.z < 2.0f))))
			m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;
		break;

	case NAME_SOUL:
		if (!(((vDir.x > -15.0f) && (vDir.x < 15.0f)) && ((vDir.z > -15.0f) && (vDir.z < 15.0f))))
			m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;
		break;
	case SOUL_BULLET:
		if (!(((vDir.x > -1.0f) && (vDir.x < 1.0f)) && ((vDir.z > -1.0f) && (vDir.z < 1.0f))))
			m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;
		break;

	default:
		break;
	}
	_matrix		matScale, matRot, matTrans;

	//준석 수정 (23.03.02) : 타겟 추적간 임의의 축 회전 필요없기에 주석처리함
	//matRot = *Compute_LookAtTarget(pTargetPos);
	if (m_eName != SOUL_BULLET)
	{
		D3DXMatrixScaling(&matScale, 5.f, 5.f, 1.f);
	}
	if (m_eName == SOUL_BULLET)
	{
		D3DXMatrixScaling(&matScale, 2.f, 2.f, 1.f);
	}
	D3DXMatrixTranslation(&matTrans,
		m_vInfo[INFO_POS].x,
		m_vInfo[INFO_POS].y + 4.f,
		m_vInfo[INFO_POS].z);

	//준석 수정 (23.03.02) : 회전행렬 필요 없기에 회전 제외하고, 이동행렬만 적용함
	//m_matWorld = matScale * matRot * matTrans;
	m_matWorld = matScale * matTrans;

}

void CTransform::Bullet_Move(const _vec3 _Dir, const _float& fSpeed, const _float& fTimeDelta, _int iDirCount = 0)
{	
	_vec3		vNull(0, 0, 0);

	if (vNull == m_vBulletTarget && 0 == iDirCount)
	{
		m_vBulletTarget = _Dir;
	}
	if (vNull != m_vBulletTarget && 0 == iDirCount)
	{
		m_vInfo[INFO_POS] += *D3DXVec3Normalize(&m_vBulletTarget, &m_vBulletTarget) * fSpeed * fTimeDelta;
	}

	if (vNull == m_vBulletTarget && 0 != iDirCount)
	{	
		_vec3 Temp;
		Temp.x = float(cos(45 * iDirCount));
		Temp.y = 0.f;
		Temp.z = float(sin(45 * iDirCount));

		if (8 == iDirCount)
		{
			Temp.x = 0.f;;
			Temp.y = 0.f;
			Temp.z = float(sin(45 * iDirCount));
		}
		if (8 == iDirCount)
		{
			Temp.x = 0.f;;
			Temp.y = 0.f;
			Temp.z = float(sin(45 * iDirCount));
		}

		if (7 == iDirCount)
		{
			Temp.x = 0.f;;
			Temp.y = 0.f;
			Temp.z = float(sin(180));
		}

		m_vBulletTarget = Temp;
	}
	if (vNull != m_vBulletTarget && 0 != iDirCount)
	{
		m_vInfo[INFO_POS] += *D3DXVec3Normalize(&m_vBulletTarget, &m_vBulletTarget) * fSpeed * fTimeDelta;
	}

	_matrix		matScale, matRot, matTrans;

	D3DXMatrixScaling(&matScale, 2.f, 2.f, 1.f);

	if (0 == iDirCount)
	{
		D3DXMatrixTranslation(&matTrans,
			m_vInfo[INFO_POS].x,
			m_vInfo[INFO_POS].y + 3.f,
			m_vInfo[INFO_POS].z);

		m_matWorld = matScale * matTrans;
	}

	if (0 != iDirCount)
	{
		_float Temp_Degree = 0;

		switch (iDirCount)
		{
		case 1:
			Temp_Degree = 135;
			break;
		case 2:
			Temp_Degree = 45;
			break;
		case 3:
			Temp_Degree = 0;
			break;
		case 4:
			Temp_Degree = 315;
			break;
		case 5:
			Temp_Degree = 225;
			break;
		case 6:
			Temp_Degree = 180;
			break;
		case 7:
			Temp_Degree = 270;
			break;
		case 8:
			Temp_Degree = 90;
			break;

		default:
			break;
		}

		_float	fRadian_Bullet = D3DXToRadian(Temp_Degree);
		//D3DXMatrixRotationY(&matRot, fRadian_Bullet);
		D3DXMatrixRotationYawPitchRoll(&matRot, fRadian_Bullet, D3DXToRadian(90), 0.f);

		D3DXMatrixTranslation(&matTrans,
			m_vInfo[INFO_POS].x,
			m_vInfo[INFO_POS].y + 3.f,
			m_vInfo[INFO_POS].z);

		m_matWorld = matScale * matRot * matTrans;
	}
}

void CTransform::Bullet_Move_Boss(const _vec3 _Dir, const _float& fSpeed, const _float& fTimeDelta, _int iDirCount)
{
	_vec3		vNull(0, 0, 0);

	if (vNull == m_vBulletTarget && 0 != iDirCount)
	{
		_vec3 Temp;
		Temp.x = float(cos(45 * iDirCount));
		Temp.y = 0.f;
		Temp.z = float(sin(45 * iDirCount));

		if (8 == iDirCount)
		{
			Temp.x = 0.f;;
			Temp.y = 0.f;
			Temp.z = float(sin(45 * iDirCount));
		}
		if (8 == iDirCount)
		{
			Temp.x = 0.f;;
			Temp.y = 0.f;
			Temp.z = float(sin(45 * iDirCount));
		}

		if (7 == iDirCount)
		{
			Temp.x = 0.f;;
			Temp.y = 0.f;
			Temp.z = float(sin(180));
		}

		m_vBulletTarget = Temp;
	}
	if (vNull != m_vBulletTarget && 0 != iDirCount)
	{
		m_vInfo[INFO_POS] += *D3DXVec3Normalize(&m_vBulletTarget, &m_vBulletTarget) * fSpeed * fTimeDelta;
	}

	_matrix		matScale, matRot, matTrans;

	D3DXMatrixScaling(&matScale, 2.f, 2.f, 1.f);

	if (0 == iDirCount)
	{
		D3DXMatrixTranslation(&matTrans,
			m_vInfo[INFO_POS].x,
			m_vInfo[INFO_POS].y + 3.f,
			m_vInfo[INFO_POS].z);

		m_matWorld = matScale * matTrans;
	}

	if (0 != iDirCount)
	{
		_float Temp_Degree = 0;

		switch (iDirCount)
		{
		case 1:
			Temp_Degree = 135;
			break;
		case 2:
			Temp_Degree = 45;
			break;
		case 3:
			Temp_Degree = 0;
			break;
		case 4:
			Temp_Degree = 315;
			break;
		case 5:
			Temp_Degree = 225;
			break;
		case 6:
			Temp_Degree = 180;
			break;
		case 7:
			Temp_Degree = 270;
			break;
		case 8:
			Temp_Degree = 90;
			break;

		default:
			break;
		}

		_float	fRadian_Bullet = D3DXToRadian(Temp_Degree);
		//D3DXMatrixRotationY(&matRot, fRadian_Bullet);
		//D3DXMatrixRotationYawPitchRoll(&matRot, fRadian_Bullet, 0.f, 0.f);

		D3DXMatrixTranslation(&matTrans,
			m_vInfo[INFO_POS].x,
			m_vInfo[INFO_POS].y + 3.f,
			m_vInfo[INFO_POS].z);

		m_matWorld = matScale * matTrans;
	}
}

_vec3 CTransform::Patrol_Map(const _float& fSpeed, const _float& fTimeDelta)
{	
	_vec3	vTemp;
	_vec3	vNull(0, 0, 0);

	if (vNull == m_vPatrolTarget)
	{	
		vTemp.x = (rand() % 50);
		vTemp.y = m_vInfo[INFO_POS].y;
		vTemp.z = (rand() % 50);

		_vec3		vDir_Temp = vTemp - m_vInfo[INFO_POS];

		if (((3 <= vDir_Temp.x) || (-3 >= vDir_Temp.x)) || ((3 <= vDir_Temp.z) || (-3 >= vDir_Temp.z)))
		{
			m_vPatrolTarget = vTemp;
		}
	}

	if (vNull != m_vPatrolTarget)
	{
		_vec3		vDir = m_vPatrolTarget - m_vInfo[INFO_POS];

		if (((0.5f >= vDir.x) && (-0.5f <= vDir.x)) && ((0.5f >= vDir.z) && (-0.5f <= vDir.z)))
		{
			m_vPatrolTarget = vNull;
		}
		else
		{
			m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;

			_matrix		matScale, matRot, matTrans;

			D3DXMatrixScaling(&matScale, 5.f, 5.f, 1.f);
			D3DXMatrixTranslation(&matTrans,
				m_vInfo[INFO_POS].x,
				m_vInfo[INFO_POS].y + 4.f,
				m_vInfo[INFO_POS].z);

			m_matWorld = matScale * matTrans;

			return vDir;
		}
	}
	
	return vNull;
}

const _matrix * CTransform::Compute_LookAtTarget(const _vec3 * pTargetPos)
{
	_vec3		vDir = *pTargetPos - m_vInfo[INFO_POS];

/*
	_vec3		vAxis = *D3DXVec3Cross(&vAxis, &m_vInfo[INFO_UP], &vDir);

	_vec3		vUp;

	_float		fDot = D3DXVec3Dot(D3DXVec3Normalize(&vDir, &vDir), D3DXVec3Normalize(&vUp, &m_vInfo[INFO_UP]));

	_matrix		matRot;

	D3DXMatrixRotationAxis(&matRot, &vAxis, acosf(fDot));

	return &matRot;*/


	_matrix		matRot;
	_vec3		vAxis, vUp;
	
	return D3DXMatrixRotationAxis(&matRot, 
								D3DXVec3Cross(&vAxis, &m_vInfo[INFO_UP], &vDir),
								acosf(D3DXVec3Dot(D3DXVec3Normalize(&vDir, &vDir), 
												   D3DXVec3Normalize(&vUp, &m_vInfo[INFO_UP]))));
}

void CTransform::Ui_Print(void)
{
	D3DXMATRIX		matView, matProj; //직교투영 행렬

	D3DXMatrixIdentity(&m_matWorld); //월드행렬을 항등행렬로
	D3DXMatrixIdentity(&matView);
	D3DXMatrixOrthoLH(&matProj, 20.f, 20.f, 0.f, 20.f);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CTransform::Arrow_Move(void)
{
	D3DXMATRIX		matScale, matRot, matTrans;


	D3DXMatrixRotationYawPitchRoll(&matRot, 0.f, D3DXToRadian(90), 0.f);
	D3DXMatrixTranslation(&matTrans,
		m_vInfo[INFO_POS].x,
		m_vInfo[INFO_POS].y,
		m_vInfo[INFO_POS].z);

	m_matWorld = matScale * matRot * matTrans;


}

void CTransform::Reverse_Scale_x(void)
{
	_matrix		matScale, matRot, matTrans;

	D3DXMatrixScaling(&matScale, -5.f, 5.f, 1.f);
	D3DXMatrixTranslation(&matTrans,
		m_vInfo[INFO_POS].x,
		m_vInfo[INFO_POS].y + 4.f,
		m_vInfo[INFO_POS].z);

	m_matWorld = matScale * matTrans;
}

void CTransform::UpdatePos_OnWorld(void)
{
	D3DXMatrixIdentity(&m_matWorld);
	memcpy(&m_matWorld.m[INFO_POS][0], &m_vInfo[INFO_POS], sizeof(_vec3));
}

HRESULT CTransform::Ready_Transform(void)
{
	D3DXMatrixIdentity(&m_matWorld);

	for (size_t i = 0; i < INFO_END; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));
	
	return S_OK;
}

_int Engine::CTransform::Update_Component(const _float& fTimeDelta)
{
	D3DXMatrixIdentity(&m_matWorld);

	for (size_t i = 0; i < INFO_POS; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	// 크기 변환
	for (size_t i = 0; i < INFO_POS; ++i)
	{
		D3DXVec3Normalize(&m_vInfo[i], &m_vInfo[i]);
		m_vInfo[i] *= *(((_float*)&m_vScale) + i);
	}

	// 회전 변환
	_matrix			matRot[ROT_END];

	D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
	D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y);
	D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);

	for (size_t i = 0; i < INFO_POS; ++i)
	{
		for (size_t j = 0; j < ROT_END; ++j)
		{
			D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRot[j]);
		}
	}

	for (size_t i = 0; i < INFO_END; ++i)
		memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));

	return 0;
}

void Engine::CTransform::LateUpdate_Component()
{

}

void Engine::CTransform::Free(void)
{
	__super::Free();
}

CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTransform *	pInstance = new CTransform(pGraphicDev);

	if (FAILED(pInstance->Ready_Transform()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	
	return pInstance;
}

Engine::CComponent* Engine::CTransform::Clone(void)
{
	return new CTransform(*this);
}
