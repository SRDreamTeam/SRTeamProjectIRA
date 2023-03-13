#pragma once

#include "Camera.h"
#include "Player.h"

class CStaticCamera : public CCamera
{
private:
	explicit CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStaticCamera();

public:
	HRESULT Ready_GameObject(const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFov,
		const _float& fAspect,
		const _float& fNear,
		const _float& fFar);

	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;

private:
	void		Key_Input(const _float& fTimeDelta);
	void		Target_Renewal(void);
	void		Mouse_Move(void);

private:
	_float			m_fDistance = 10.f;
	_float			m_fAngle = 0.f;
	_float          m_fHeight = 3.f;
	_float          m_fDisZ = 6.f;
	_float          m_fDisY = 6.f;
	_float          m_AtY = 3.f;
    _float          m_AtZ = 7.f;

public:
	static CStaticCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFov,
		const _float& fAspect,
		const _float& fNear,
		const _float& fFar);

private:
	virtual void Free(void) override;

};

