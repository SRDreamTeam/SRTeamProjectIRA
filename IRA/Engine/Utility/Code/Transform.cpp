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
		if (!(((vDir.x > -1.0f) && (vDir.x < 1.0f)) && ((vDir.z > -1.0f) && (vDir.z < 1.0f))))
			m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;
		break;

	case NAME_SOUL:
		if (!(((vDir.x > -3.0f) && (vDir.x < 3.0f)) && ((vDir.z > -3.0f) && (vDir.z < 3.0f))))
			m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;
		break;

	default:
		break;
	}
	_matrix		matScale, matRot, matTrans;

	//준석 수정 (23.03.02) : 타겟 추적간 임의의 축 회전 필요없기에 주석처리함
	//matRot = *Compute_LookAtTarget(pTargetPos);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixTranslation(&matTrans,
		m_vInfo[INFO_POS].x,
		m_vInfo[INFO_POS].y,
		m_vInfo[INFO_POS].z);

	//준석 수정 (23.03.02) : 회전행렬 필요 없기에 회전 제외하고, 이동행렬만 적용함
	//m_matWorld = matScale * matRot * matTrans;
	m_matWorld = matScale * matTrans;

}

_vec3 CTransform::Patrol_Map(const _float& fSpeed, const _float& fTimeDelta)
{	
	_vec3	vTemp;
	_vec3	vNull(0, 0, 0);

	if (vNull == m_vPatrolTarget)
	{	
		vTemp.x = (rand() % 15);
		vTemp.y = m_vInfo[INFO_POS].y;
		vTemp.z = (rand() % 15);

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

			D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
			D3DXMatrixTranslation(&matTrans,
				m_vInfo[INFO_POS].x,
				m_vInfo[INFO_POS].y,
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

void CTransform::Reverse_Scale_x(void)
{
	_matrix		matScale, matRot, matTrans;

	D3DXMatrixScaling(&matScale, -1.f, 1.f, 1.f);
	D3DXMatrixTranslation(&matTrans,
		m_vInfo[INFO_POS].x,
		m_vInfo[INFO_POS].y,
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
