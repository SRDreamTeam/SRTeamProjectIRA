#include "stdafx.h"
#include "..\Header\StaticCamera.h"

#include "Export_Function.h"

CStaticCamera::CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{

}


CStaticCamera::~CStaticCamera()
{
	Free();
}


HRESULT CStaticCamera::Ready_GameObject(const _vec3* pEye,
	const _vec3* pAt,
	const _vec3* pUp,
	const _float& fFov,
	const _float& fAspect,
	const _float& fNear,
	const _float& fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;


	m_fAngle = D3DXToRadian(320.f);
	m_fDistance = 15.f;

	FAILED_CHECK_RETURN(__super::Ready_GameObject(), E_FAIL);


	return S_OK;
}


_int CStaticCamera::Update_GameObject(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);

	Target_Renewal();

	_int iExit = __super::Update_GameObject(fTimeDelta);

	return iExit;
}
void CStaticCamera::LateUpdate_GameObject()
{
}

void CStaticCamera::Key_Input(const _float& fTimeDelta)
{
	if (Get_DIKeyState(DIK_W) & 0x80)
		m_fDistance -= 10.f * fTimeDelta;

	if (Get_DIKeyState(DIK_S) & 0x80)
		m_fDistance += 10.f * fTimeDelta;

	if (Get_DIKeyState(DIK_A) & 0x80)
		m_fAngle += D3DXToRadian(180.f) * fTimeDelta;

	if (Get_DIKeyState(DIK_D) & 0x80)
		m_fAngle -= D3DXToRadian(180.f) * fTimeDelta;
}

void CStaticCamera::Target_Renewal(void)
{
	CTransform*		pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
	NULL_CHECK(pPlayerTransform);

	_vec3	vLook;
	pPlayerTransform->Get_Info(INFO_LOOK, &vLook);

	m_vEye = vLook * -1.f;
	D3DXVec3Normalize(&m_vEye, &m_vEye);

	m_vEye.y += 2.f;
	m_vEye *= m_fDistance;

	_vec3	vRight;
	memcpy(vRight, &pPlayerTransform->m_matWorld.m[0][0], sizeof(_vec3));

	if (vRight.x < 0)
		vRight.x *= -1.f;

	_matrix		matRot;
	D3DXMatrixRotationAxis(&matRot, &vRight, m_fAngle);
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRot);

	m_vEye += pPlayerTransform->m_vInfo[INFO_POS];
	m_vAt = pPlayerTransform->m_vInfo[INFO_POS];
}


void CStaticCamera::Mouse_Move(void)
{
	_long	dwMouseMove = 0;

	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, 0, &m_matView);

	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_Y))
	{
		_vec3		vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_matrix		matRot;
		D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove / 10.f));

		memcpy(&matRot._41, &m_vAt, sizeof(_vec3));

		_vec3	vLook = m_vEye - m_vAt;

		D3DXVec3TransformCoord(&m_vEye, &vLook, &matRot);

		//m_vAt = m_vEye + vLook;
	}

	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_X))
	{
		_vec3		vUp = { 0.f, 1.f, 0.f };


		_matrix		matRot;
		D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian(dwMouseMove / 10.f));

		memcpy(&matRot._41, &m_vAt, sizeof(_vec3));

		_vec3	vLook = m_vEye - m_vAt;

		D3DXVec3TransformCoord(&m_vEye, &vLook, &matRot);

		//m_vAt = m_vEye + vLook;
	}

}







CStaticCamera* CStaticCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CStaticCamera*		pInstance = new CStaticCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CStaticCamera::Free(void)
{
	__super::Free();
}
