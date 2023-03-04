#include "stdafx.h"
#include "..\Header\Bullet.h"
#include "Export_Function.h"

CBullet::CBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CBullet::CBullet(const CBullet& rhs)
	: Engine::CGameObject(rhs)
{
}

CBullet::~CBullet()
{
}

HRESULT CBullet::Ready_GameObject(void)
{
	return E_NOTIMPL;
}

_int CBullet::Update_GameObject(const _float& fTimeDelta)
{
	return _int();
}

void CBullet::LateUpdate_GameObject()
{
}

void CBullet::Render_GameObject()
{
}
