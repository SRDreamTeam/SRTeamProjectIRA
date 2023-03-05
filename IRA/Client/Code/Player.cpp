#include "stdafx.h"
#include "..\Header\Player.h"
#include "Export_Function.h"
#include "Ghost.h"
#include "SylphArrow.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pDeathTextureCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pColliderCom(nullptr)
	, m_pSphereBufferCom(nullptr)
	, m_pCalculatorCom(nullptr)
{
}

CPlayer::CPlayer(const CPlayer & rhs)
	: Engine::CGameObject(rhs)
	, m_pBufferCom(rhs.m_pBufferCom)
	, m_pDeathTextureCom(rhs.m_pDeathTextureCom)
	, m_pTransformCom(rhs.m_pTransformCom)
	, m_pCalculatorCom(rhs.m_pCalculatorCom)
	, m_pColliderCom(rhs.m_pColliderCom)
	, m_pSphereBufferCom(rhs.m_pSphereBufferCom)
{
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale({ 5.f, 5.f, 1.f });

	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{

	if (m_bDead)
		return OBJ_DEAD;

	if (m_iState == STAND) {
		m_fStandFrame += 7.f * fTimeDelta;
		if (7.f < m_fStandFrame)
			m_fStandFrame = 0.f;
	}

	if (m_iState == RUN) {
		m_fRunFrame += 7.f * fTimeDelta * 1.5f;
		if (7.f < m_fRunFrame)
			m_fRunFrame = 0.f;
	}

	if (m_iState == MOVE_ATTACK || m_iState == STAND_ATTACK) {
		m_fAttackFrame += 9.f * fTimeDelta;
		if (9.f < m_fAttackFrame)
			m_fAttackFrame = 0.f;
	}

	if (m_iState == DASH) {
		m_fDashFrame += 5.f * fTimeDelta * 2.0;
		if (5.f < m_fDashFrame) {
			m_fDashFrame = 0.f;
		}
	}

	if (m_iState == DEATH) {
		m_fDeathFrame += 11.f * fTimeDelta;
		if (11.f < m_fDeathFrame) {
			m_fDeathFrame = 11.f;
		}
	}

	SetUp_OnTerrain();

	Key_Input(fTimeDelta);

	Update_State();


	if (m_Is_Dash == true) {
		Dash(fTimeDelta);
	}



	/*if (m_Is_Fire_Arrow == true) {
		m_Fire_Interver += m_Fire_Speed * fTimeDelta;
		if (m_Fire_Interver > m_Fire_Speed) {
			Fire_Arrow();
			m_Fire_Interver = 0.f;
		}
	}*/



	__super::Update_GameObject(fTimeDelta);


    Engine::Add_RenderGroup(RENDER_ALPHATEST, this);
	

	_vec3 v = m_pColliderCom->Get_SpherePos();

	return 0;
}

void CPlayer::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CPlayer::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	

	if (m_iState == STAND) {

		if (m_iAngleState == ANGLE_000) {
			m_pStandTextureCom[STAND_000]->Set_Texture((_uint)m_fStandFrame);
		}
		else if (m_iAngleState == ANGLE_045) {
			m_pStandTextureCom[STAND_045]->Set_Texture((_uint)m_fStandFrame);
		}
		else if (m_iAngleState == ANGLE_090) {
			m_pStandTextureCom[STAND_090]->Set_Texture((_uint)m_fStandFrame);
		}
		else if (m_iAngleState == ANGLE_135) {
			m_pStandTextureCom[STAND_135]->Set_Texture((_uint)m_fStandFrame);
		}
		else if (m_iAngleState == ANGLE_180) {
			m_pStandTextureCom[STAND_180]->Set_Texture((_uint)m_fStandFrame);
		}
		else if (m_iAngleState == ANGLE_225) {
			m_pStandTextureCom[STAND_135]->Set_Texture((_uint)m_fStandFrame);
		}
		else if (m_iAngleState == ANGLE_270) {
			m_pStandTextureCom[STAND_090]->Set_Texture((_uint)m_fStandFrame);
		}
		else if (m_iAngleState == ANGLE_315) {
			m_pStandTextureCom[STAND_045]->Set_Texture((_uint)m_fStandFrame);
		}
	}
	else if (m_iState == RUN) {

		if (m_iAngleState == ANGLE_000)
			m_pRunTextureCom[RUN_000]->Set_Texture((_uint)m_fRunFrame);

		else if (m_iAngleState == ANGLE_045)
			m_pRunTextureCom[RUN_045]->Set_Texture((_uint)m_fRunFrame);

		else if (m_iAngleState == ANGLE_090)
			m_pRunTextureCom[RUN_090]->Set_Texture((_uint)m_fRunFrame);

		else if (m_iAngleState == ANGLE_135)
			m_pRunTextureCom[RUN_135]->Set_Texture((_uint)m_fRunFrame);

		else if (m_iAngleState == ANGLE_180)
			m_pRunTextureCom[RUN_180]->Set_Texture((_uint)m_fRunFrame);

		else if (m_iAngleState == ANGLE_225)
			m_pRunTextureCom[RUN_135]->Set_Texture((_uint)m_fRunFrame);

		else if (m_iAngleState == ANGLE_270)
			m_pRunTextureCom[RUN_090]->Set_Texture((_uint)m_fRunFrame);

		else if (m_iAngleState == ANGLE_315)
			m_pRunTextureCom[RUN_045]->Set_Texture((_uint)m_fRunFrame);

	}
	else if (m_iState == MOVE_ATTACK) {
		if (m_iAttackAngleState == ATTACK_ANGLE_045) {
			m_pMoveAttackTextureCom[MOVE_ATTACK_045]->Set_Texture((_uint)m_fAttackFrame);
		}
		else if (m_iAttackAngleState == ATTACK_ANGLE_135) {
			m_pMoveAttackTextureCom[MOVE_ATTACK_135]->Set_Texture((_uint)m_fAttackFrame);
		}
		else if (m_iAttackAngleState == ATTACK_ANGLE_225) {
			m_pMoveAttackTextureCom[MOVE_ATTACK_135]->Set_Texture((_uint)m_fAttackFrame);
		}
		else if (m_iAttackAngleState == ATTACK_ANGLE_315) {
			m_pMoveAttackTextureCom[MOVE_ATTACK_045]->Set_Texture((_uint)m_fAttackFrame);
		}
	}
	else if (m_iState == STAND_ATTACK) {
		if (m_iAttackAngleState == ATTACK_ANGLE_045) {
			m_pStandAttackTextureCom[STAND_ATTACK_045]->Set_Texture((_uint)m_fAttackFrame);
		}
		else if (m_iAttackAngleState == ATTACK_ANGLE_135) {
			m_pStandAttackTextureCom[STAND_ATTACK_135]->Set_Texture((_uint)m_fAttackFrame);
		}
		else if (m_iAttackAngleState == ATTACK_ANGLE_225) {
			m_pStandAttackTextureCom[STAND_ATTACK_135]->Set_Texture((_uint)m_fAttackFrame);
		}
		else if (m_iAttackAngleState == ATTACK_ANGLE_315) {
			m_pStandAttackTextureCom[STAND_ATTACK_045]->Set_Texture((_uint)m_fAttackFrame);
		}

	}
	else if (m_iState == DASH) {

		if (m_iAngleState == ANGLE_000) {
			m_pDashTextureCom[DASH_045]->Set_Texture((_uint)m_fDashFrame);
		}
		else if (m_iAngleState == ANGLE_045) {
			m_pDashTextureCom[DASH_045]->Set_Texture((_uint)m_fDashFrame);
		}
		else if (m_iAngleState == ANGLE_090) {
			m_pDashTextureCom[DASH_045]->Set_Texture((_uint)m_fDashFrame);
		}
		else if (m_iAngleState == ANGLE_135) {
			m_pDashTextureCom[DASH_045]->Set_Texture((_uint)m_fDashFrame);
		}
		else if (m_iAngleState == ANGLE_180) {
			m_pDashTextureCom[DASH_135]->Set_Texture((_uint)m_fDashFrame);
		}
		else if (m_iAngleState == ANGLE_225) {
			m_pDashTextureCom[DASH_135]->Set_Texture((_uint)m_fDashFrame);
		}
		else if (m_iAngleState == ANGLE_270) {
			m_pDashTextureCom[DASH_135]->Set_Texture((_uint)m_fDashFrame);
		}
		else if (m_iAngleState == ANGLE_315) {
			m_pDashTextureCom[DASH_135]->Set_Texture((_uint)m_fDashFrame);
		}


	}
	else if (m_iState == DEATH) {
		m_pDeathTextureCom->Set_Texture((_uint)m_fDeathFrame);
	}


	m_pBufferCom->Render_Buffer();
	m_pSphereBufferCom->Render_Buffer();

	

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CPlayer::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

#pragma region Player_Texture

	pComponent = m_pStandTextureCom[STAND_000] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Stand_000"));
	NULL_CHECK_RETURN(m_pStandTextureCom[STAND_000], E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Stand_000", pComponent });

	pComponent = m_pStandTextureCom[STAND_045] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Stand_045"));
	NULL_CHECK_RETURN(m_pStandTextureCom[STAND_045], E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Stand_045", pComponent });

	pComponent = m_pStandTextureCom[STAND_090] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Stand_090"));
	NULL_CHECK_RETURN(m_pStandTextureCom[STAND_090], E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Stand_090", pComponent });

	pComponent = m_pStandTextureCom[STAND_135] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Stand_135"));
	NULL_CHECK_RETURN(m_pStandTextureCom[STAND_135], E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Stand_135", pComponent });

	pComponent = m_pStandTextureCom[STAND_180] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Stand_180"));
	NULL_CHECK_RETURN(m_pStandTextureCom[STAND_180], E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Stand_180", pComponent });


	pComponent = m_pRunTextureCom[RUN_000] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Run_000"));
	NULL_CHECK_RETURN(m_pRunTextureCom[RUN_000], E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Run_000", pComponent });

	pComponent = m_pRunTextureCom[RUN_045] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Run_045"));
	NULL_CHECK_RETURN(m_pRunTextureCom[RUN_045], E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Run_045", pComponent });

	pComponent = m_pRunTextureCom[RUN_090] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Run_090"));
	NULL_CHECK_RETURN(m_pRunTextureCom[RUN_090], E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Run_090", pComponent });

	pComponent = m_pRunTextureCom[RUN_135] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Run_135"));
	NULL_CHECK_RETURN(m_pRunTextureCom[RUN_135], E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Run_135", pComponent });

	pComponent = m_pRunTextureCom[RUN_180] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Run_180"));
	NULL_CHECK_RETURN(m_pRunTextureCom[RUN_180], E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Run_180", pComponent });


	pComponent = m_pMoveAttackTextureCom[MOVE_ATTACK_045] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Move_Attack_045"));
	NULL_CHECK_RETURN(m_pMoveAttackTextureCom[MOVE_ATTACK_045], E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Move_Attack_045", pComponent });


	pComponent = m_pMoveAttackTextureCom[MOVE_ATTACK_135] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Move_Attack_135"));
	NULL_CHECK_RETURN(m_pMoveAttackTextureCom[MOVE_ATTACK_135], E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Move_Attack_135", pComponent });

	pComponent = m_pStandAttackTextureCom[STAND_ATTACK_045] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Stand_Attack_045"));
	NULL_CHECK_RETURN(m_pStandAttackTextureCom[STAND_ATTACK_045], E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Stand_Attack_045", pComponent });

	pComponent = m_pStandAttackTextureCom[STAND_ATTACK_135] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Stand_Attack_135"));
	NULL_CHECK_RETURN(m_pStandAttackTextureCom[STAND_ATTACK_135], E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Stand_Attack_135", pComponent });


	pComponent = m_pDashTextureCom[DASH_045] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Dash_045"));
	NULL_CHECK_RETURN(m_pDashTextureCom[DASH_045], E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Dash_045", pComponent });

	pComponent = m_pDashTextureCom[DASH_135] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Dash_135"));
	NULL_CHECK_RETURN(m_pDashTextureCom[DASH_135], E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Dash_135", pComponent });

	pComponent = m_pDeathTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Death"));
	NULL_CHECK_RETURN(m_pDeathTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Death", pComponent });

#pragma endregion


	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_ProtoComponent(L"Proto_Calculator"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	NULL_CHECK_RETURN(m_pColliderCom, E_FAIL);
	m_pColliderCom->Set_Radius(1.f);
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });

	pComponent = m_pSphereBufferCom = dynamic_cast<CSphereTex*>(Engine::Clone_ProtoComponent(L"Proto_SphereTex"));
	NULL_CHECK_RETURN(m_pSphereBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_SphereTex", pComponent });

	return S_OK;
}

void CPlayer::Key_Input(const _float & fTimeDelta)
{
	if (m_iState == DEATH)
		return;

	if (m_Is_Dash == true)
		return;


	/*if (GetAsyncKeyState('Q'))
		m_pTransformCom->Rotation(ROT_X, D3DXToRadian(180.f * fTimeDelta));

	if (GetAsyncKeyState('E'))
		m_pTransformCom->Rotation(ROT_X, D3DXToRadian(-180.f * fTimeDelta));*/


	CGhost* pGhost = dynamic_cast<CGhost*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Ghost"));



	_vec3	vDir;
	m_pTransformCom->Get_Info(INFO_LOOK, &vDir);
	D3DXVec3Normalize(&vDir, &vDir);

	_vec3	vRight;
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
	D3DXVec3Normalize(&vRight, &vRight);


	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	
	if (vRight.x < 0)
		vRight.x *= -1.f;

	_vec3	vLU = vDir - vRight;
	D3DXVec3Normalize(&vLU, &vLU);

	_vec3	vRU = vDir + vRight;
	D3DXVec3Normalize(&vRU, &vRU);

	_vec3	vLD = -vDir - vRight;
	D3DXVec3Normalize(&vLD, &vLD);

	_vec3	vRD = -vDir + vRight;
	D3DXVec3Normalize(&vRD, &vRD);


	if (GetAsyncKeyState(VK_LBUTTON)) {

		m_Is_Fire_Arrow = true;


		POINT ptCursor;

		GetCursorPos(&ptCursor);
		ScreenToClient(g_hWnd, &ptCursor);

		

		if (ptCursor.x>=400 && ptCursor.y <= 300) {
			m_iAttackAngleState = ATTACK_ANGLE_225;
			m_iAngleState = ANGLE_225;
		}
		else if (ptCursor.x < 400 && ptCursor.y <= 300) {
			m_iAttackAngleState = ATTACK_ANGLE_135;
			m_iAngleState = ANGLE_135;
		}
		else if (ptCursor.x < 400 && ptCursor.y > 300) {
			m_iAttackAngleState = ATTACK_ANGLE_045;
			m_iAngleState = ANGLE_045;
		}
		else if (ptCursor.x >= 400 && ptCursor.y > 300) {
			m_iAttackAngleState = ATTACK_ANGLE_315;
			m_iAngleState = ANGLE_315;
		}

		if (GetAsyncKeyState('W')) {
			if (GetAsyncKeyState('A')) {
				m_iState = MOVE_ATTACK;
				m_pTransformCom->Move_Pos(&(vLU * fTimeDelta * m_fSpeed));
			}
			else if (GetAsyncKeyState('D')) {
				m_iState = MOVE_ATTACK;
				m_pTransformCom->Move_Pos(&(vRU * fTimeDelta * m_fSpeed));
			}
			else {
				m_iState = MOVE_ATTACK;
				m_pTransformCom->Move_Pos(&(vDir * fTimeDelta * m_fSpeed));
			}
		}
		else if (GetAsyncKeyState('S')) {

			if (GetAsyncKeyState('A')) {
				m_iState = MOVE_ATTACK;
				m_pTransformCom->Move_Pos(&(vLD * fTimeDelta * m_fSpeed));
			}
			else if (GetAsyncKeyState('D')) {
				m_iState = MOVE_ATTACK;
				m_pTransformCom->Move_Pos(&(vRD * fTimeDelta * m_fSpeed));
			}
			else {
				m_iState = MOVE_ATTACK;
				m_pTransformCom->Move_Pos(&(vDir * fTimeDelta * -m_fSpeed));
			}

		}
		else if (GetAsyncKeyState('A')) {
			m_iState = MOVE_ATTACK;
			m_pTransformCom->Move_Pos(&(-vRight * fTimeDelta * m_fSpeed));

		}
		else if (GetAsyncKeyState('D')) {
			m_iState = MOVE_ATTACK;
			m_pTransformCom->Move_Pos(&(vRight * fTimeDelta * m_fSpeed));

		}
		else {
			m_iState = STAND_ATTACK;
		}

	}
	else if (GetAsyncKeyState('W')) {

		if (GetAsyncKeyState('A')) {
			if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
				pGhost->Is_Dash = true;
				m_Is_Dash = true;
				m_iState = DASH;
				m_iAngleState = ANGLE_135;
				m_pTransformCom->Move_Pos(&(vLU * fTimeDelta * m_fSpeed));
			}
			else {
				m_iState = RUN;
				m_iAngleState = ANGLE_135;
				m_pTransformCom->Move_Pos(&(vLU * fTimeDelta * m_fSpeed));
			}
		}
		else if (GetAsyncKeyState('D')) {

			if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
				pGhost->Is_Dash = true;
				m_Is_Dash = true;
				m_iState = DASH;
				m_iAngleState = ANGLE_225;
				m_pTransformCom->Move_Pos(&(vRU * fTimeDelta * m_fSpeed));
			}
			else {
				m_iState = RUN;
				m_iAngleState = ANGLE_225;
				m_pTransformCom->Move_Pos(&(vRU * fTimeDelta * m_fSpeed));
			}
		}
		else {
			if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
				pGhost->Is_Dash = true;
				m_Is_Dash = true;
				m_iState = DASH;
				m_iAngleState = ANGLE_180;
				m_pTransformCom->Move_Pos(&(vDir* fTimeDelta* m_fSpeed));
			}
			else {
				m_iState = RUN;
				m_iAngleState = ANGLE_180;
				m_pTransformCom->Move_Pos(&(vDir * fTimeDelta * m_fSpeed));
			}
		}
	}
	else if (GetAsyncKeyState('S')) {

		if (GetAsyncKeyState('A')) {
			if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
				pGhost->Is_Dash = true;
				m_Is_Dash = true;
				m_iState = DASH;
				m_iAngleState = ANGLE_045;
				m_pTransformCom->Move_Pos(&(vLD * fTimeDelta * m_fSpeed));
			}
			else {
				m_iState = RUN;
				m_iAngleState = ANGLE_045;
				m_pTransformCom->Move_Pos(&(vLD * fTimeDelta * m_fSpeed));
			}
		}
		else if (GetAsyncKeyState('D')) {
			if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
				pGhost->Is_Dash = true;
				m_Is_Dash = true;
				m_iState = DASH;
				m_iAngleState = ANGLE_315;
				m_pTransformCom->Move_Pos(&(vRD* fTimeDelta* m_fSpeed));
			}
			else {
				m_iState = RUN;
				m_iAngleState = ANGLE_315;
				m_pTransformCom->Move_Pos(&(vRD * fTimeDelta * m_fSpeed));
			}
		}
		else {
			if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
				pGhost->Is_Dash = true;
				m_Is_Dash = true;
				m_iState = DASH;
				m_iAngleState = ANGLE_000;
				m_pTransformCom->Move_Pos(&(vDir* fTimeDelta * -m_fSpeed));
			}
			else {

				m_iState = RUN;
				m_iAngleState = ANGLE_000;
				m_pTransformCom->Move_Pos(&(vDir * fTimeDelta * -m_fSpeed));
			}
		}

	}
	else if (GetAsyncKeyState('A')) {
		if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
			pGhost->Is_Dash = true;
			m_Is_Dash = true;
			m_iState = DASH;
			m_iAngleState = ANGLE_090;
			m_pTransformCom->Move_Pos(&(-vRight * fTimeDelta * m_fSpeed));
		}
		else {
			m_iState = RUN;
			m_iAngleState = ANGLE_090;
			m_pTransformCom->Move_Pos(&(-vRight * fTimeDelta * m_fSpeed));
		}
	}
	else if (GetAsyncKeyState('D')) {

	    if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
			pGhost->Is_Dash = true;
			m_Is_Dash = true;
			m_iState = DASH;
			m_iAngleState = ANGLE_270;
			m_pTransformCom->Move_Pos(&(vRight * fTimeDelta * m_fSpeed));
		}
		else {
			m_iState = RUN;
			m_iAngleState = ANGLE_270;
			m_pTransformCom->Move_Pos(&(vRight * fTimeDelta * m_fSpeed));
		}
	}
	else if (GetAsyncKeyState('P')) {
	    m_iState = DEATH;
    }
	else {
		m_iState = STAND;
	}



}



void CPlayer::Fire_Arrow(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");


	CGameObject* pGameObject = nullptr;



	m_World_Mouse = Get_World_Mouse();

	_vec3 vPos = m_pTransformCom->m_vInfo[INFO_POS];
	

	pGameObject = CSylphArrow::Create(m_pGraphicDev, vPos, m_World_Mouse);

	if (pGameObject == nullptr)
		return;


	pGameLogicLayer->Add_BulletObject(OBJ_ARROW, pGameObject);

}


void CPlayer::SetUp_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	CTerrainTex*		pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTex", ID_STATIC));
	NULL_CHECK(pTerrainBufferCom);

	_float	fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pTransformCom->Set_Pos(vPos.x, fHeight + 6.4f, vPos.z);
}

_vec3 CPlayer::Picking_OnTerrain(void)
{
	CTerrainTex*		pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTex", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, _vec3());

	CTransform*		pTerrainTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransCom, _vec3());

	return m_pCalculatorCom->Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransCom);
}

void CPlayer::Update_State()
{
	if (m_iState == m_iPreState && m_iAngleState == m_iPreAngleState && m_iAttackAngleState == m_iPreAttackAngleState)
		return;


	if (m_iState == STAND) {
		
		if (m_iAngleState == ANGLE_000) {
			
		}
		else if (m_iAngleState == ANGLE_045) {
			if (m_bReverse == true) {
				m_pTransformCom->Set_Scale_X(-1.f);
				m_bReverse = false;
			}
		}
		else if (m_iAngleState == ANGLE_090) {
			
			if (m_bReverse == true) {
				m_pTransformCom->Set_Scale_X(-1.f);
				m_bReverse = false;
			}
		}
		else if (m_iAngleState == ANGLE_135) {
			
			if (m_bReverse == true) {
				m_pTransformCom->Set_Scale_X(-1.f);
				m_bReverse = false;
			}
		}
		else if (m_iAngleState == ANGLE_180) {
			
		}
		else if (m_iAngleState == ANGLE_225) {
			
			if (m_bReverse == false) {
				m_pTransformCom->Set_Scale_X(-1.f);
				m_bReverse = true;
			}
		}
		else if (m_iAngleState == ANGLE_270) {
			
			if (m_bReverse == false) {
				m_pTransformCom->Set_Scale_X(-1.f);
				m_bReverse = true;
			}
		}
		else if (m_iAngleState == ANGLE_315) {
			if (m_bReverse == false) {
				m_pTransformCom->Set_Scale_X(-1.f);
				m_bReverse = true;
			}
		}
	}
	else if (m_iState == RUN) {
		if (m_iAngleState == ANGLE_000) {
			
		}
		else if (m_iAngleState == ANGLE_045) {
			if (m_bReverse == true) {
				m_pTransformCom->Set_Scale_X(-1.f);
				m_bReverse = false;
			}
		}
		else if (m_iAngleState == ANGLE_090) {
			
			if (m_bReverse == true) {
				m_pTransformCom->Set_Scale_X(-1.f);
				m_bReverse = false;
			}
		}
		else if (m_iAngleState == ANGLE_135) {
			
			if (m_bReverse == true) {
				m_pTransformCom->Set_Scale_X(-1.f);
				m_bReverse = false;
			}
		}
		else if (m_iAngleState == ANGLE_180) {
			
		}
		else if (m_iAngleState == ANGLE_225) {
			
			if (m_bReverse == false) {
				m_pTransformCom->Set_Scale_X(-1.f);
				m_bReverse = true;
			}
		}
		else if (m_iAngleState == ANGLE_270) {
			
			if (m_bReverse == false) {
				m_pTransformCom->Set_Scale_X(-1.f);
				m_bReverse = true;
			}
		}
		else if (m_iAngleState == ANGLE_315) {
			
			if (m_bReverse == false) {
				m_pTransformCom->Set_Scale_X(-1.f);
				m_bReverse = true;
			}
		}
	}
	else if (m_iState == MOVE_ATTACK) {
	    if (m_iAttackAngleState == ATTACK_ANGLE_045) {

			if (m_bReverse == true) {
				m_pTransformCom->Set_Scale_X(-1.f);
				m_bReverse = false;
			}
		}
		else if (m_iAttackAngleState == ATTACK_ANGLE_135) {

			if (m_bReverse == true) {
				m_pTransformCom->Set_Scale_X(-1.f);
				m_bReverse = false;
			}
		}
		else if (m_iAttackAngleState == ATTACK_ANGLE_225) {

			if (m_bReverse == false) {
				m_pTransformCom->Set_Scale_X(-1.f);
				m_bReverse = true;
			}
		}
		else if (m_iAttackAngleState == ATTACK_ANGLE_315) {

			if (m_bReverse == false) {
				m_pTransformCom->Set_Scale_X(-1.f);
				m_bReverse = true;
			}
		}
	}
	else if (m_iState == STAND_ATTACK) {
		if (m_iAttackAngleState == ATTACK_ANGLE_045) {

			if (m_bReverse == true) {
				m_pTransformCom->Set_Scale_X(-1.f);
				m_bReverse = false;
			}
		}
		else if (m_iAttackAngleState == ATTACK_ANGLE_135) {

			if (m_bReverse == true) {
				m_pTransformCom->Set_Scale_X(-1.f);
				m_bReverse = false;
			}
		}
		else if (m_iAttackAngleState == ATTACK_ANGLE_225) {

			if (m_bReverse == false) {
				m_pTransformCom->Set_Scale_X(-1.f);
				m_bReverse = true;
			}
		}
		else if (m_iAttackAngleState == ATTACK_ANGLE_315) {

			if (m_bReverse == false) {
				m_pTransformCom->Set_Scale_X(-1.f);
				m_bReverse = true;
			}
		}
	}
	else if (m_iState == DASH) {

	}






	m_iPreState = m_iState;
	m_iPreAngleState = m_iAngleState;
	m_iPreAttackAngleState = m_iAttackAngleState;

}

_vec3 CPlayer::Get_World_Mouse()
{
	POINT		ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	_vec3	vMousePos;

	// 윈도우 -> 투영
	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	// 투영 -> 뷰 스페이스
	_matrix		matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, 0, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	// 뷰 스페이스 -> 월드

	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);

	
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matView);
	
	

	return vMousePos;
}



void CPlayer::Dash(const _float& fTimeDelta)
{

	m_Dash_Time += 5.f * fTimeDelta * 2.0f;

	if (m_Dash_Time > 5.f) {
		m_Dash_Time = 0.f;
		m_Is_Dash = false;
		CGhost* pGhost = dynamic_cast<CGhost*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Ghost"));
		pGhost->Is_Dash = false;
	}


	_vec3	vDir;
	m_pTransformCom->Get_Info(INFO_LOOK, &vDir);
	D3DXVec3Normalize(&vDir, &vDir);

	_vec3	vRight;
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
	D3DXVec3Normalize(&vRight, &vRight);

	_vec3	vLU = vDir - vRight;
	D3DXVec3Normalize(&vLU, &vLU);

	_vec3	vRU = vDir - vRight;
	D3DXVec3Normalize(&vRU, &vRU);

	_vec3	vLD = -vDir - vRight;
	D3DXVec3Normalize(&vLD, &vLD);

	_vec3	vRD = -vDir - vRight;
	D3DXVec3Normalize(&vRD, &vRD);


	if (m_iAngleState == ANGLE_000) {
		m_pTransformCom->Move_Pos(&(vDir * fTimeDelta * -m_fSpeed * m_fDashSpeed));
	}
	else if (m_iAngleState == ANGLE_045) {
		m_pTransformCom->Move_Pos(&(vLD * fTimeDelta * m_fSpeed * m_fDashSpeed));
	}
	else if (m_iAngleState == ANGLE_090) {
		m_pTransformCom->Move_Pos(&(vRight * fTimeDelta * -m_fSpeed * m_fDashSpeed));
	}
	else if (m_iAngleState == ANGLE_135) {
		m_pTransformCom->Move_Pos(&(vLU * fTimeDelta * m_fSpeed * m_fDashSpeed));
	}
	else if (m_iAngleState == ANGLE_180) {
		m_pTransformCom->Move_Pos(&(vDir * fTimeDelta * m_fSpeed * m_fDashSpeed));
	}
	else if (m_iAngleState == ANGLE_225) {
		m_pTransformCom->Move_Pos(&(vRU * fTimeDelta * m_fSpeed * m_fDashSpeed));
	}
	else if (m_iAngleState == ANGLE_270) {
		m_pTransformCom->Move_Pos(&(-vRight * fTimeDelta * m_fSpeed * m_fDashSpeed));
	}
	else if (m_iAngleState == ANGLE_315) {
		m_pTransformCom->Move_Pos(&(vRD * fTimeDelta * m_fSpeed * m_fDashSpeed));
	}

}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer * pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPlayer::Free(void)
{
	__super::Free();
}


