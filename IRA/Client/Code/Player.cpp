#include "stdafx.h"
#include "..\Header\Player.h"
#include "Export_Function.h"
#include "Ghost.h"
#include "SylphArrow.h"
#include "SylphChargeArrow.h"
#include "SylphBow.h"
#include "SylphBowPair.h"
#include "Effect_Player_Bow_Pulse.h"
#include "Effect_Player_Bow_Charge.h"
#include "Effect_Player_Foot.h"
#include "KeyMgr.h"
#include "CollisionMgr.h"

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
	CKeyMgr::Get_Instance()->Destroy_Instance();
}

HRESULT CPlayer::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	m_fSpeed = 25.f;

	m_pTransformCom->Set_Scale_Ratio({ 5.f, 5.f, 1.f });

	m_tInfo.Maxhp = 5;
	m_tInfo.Nowhp = m_tInfo.Maxhp;
	m_tInfo.MaxEnergy = 5;
	m_tInfo.NowEnergy = m_tInfo.MaxEnergy;
	m_tInfo.Gem = 0;
	m_tInfo.Key = 0;
	m_tInfo.Money = 1000;


	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_PLAYER, this);

	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{

	if (m_bHit == true && m_bImmuned == false) {
		Hp_Down();
		m_HitBlend = true;
		m_bImmuned = true;
	}

	if (m_bDead)
		return OBJ_DEAD;

	Frame_Manage(fTimeDelta);

	SetUp_OnTerrain();

	CKeyMgr::Get_Instance()->Update();

	if (CKeyMgr::Get_Instance()->Key_Down(KEY_H)) {
		m_bHit = true;
	}

	Key_Input(fTimeDelta);


	Update_State();


	if (m_Is_Dash == true) {
		m_bImmuned = true;
		Dash(fTimeDelta);
	}

	if (m_Is_Fire_Arrow == true && m_Is_Effect_Charge_Arrow == false) {
		m_Fire_Frame += m_Fire_Init * fTimeDelta * m_Fire_Speed;
		if (m_Fire_Frame > m_Fire_Init){
			Fire_Arrow();
			m_Fire_Frame = 0.f;
		}
	}

	if (m_Is_Effect_Charge_Arrow == true) {
		Effect_Charge_Arrow();
	}

	if (m_Is_Run == true) {
		m_FootFrame += 1.f * fTimeDelta * 2.f;
		if (m_FootFrame > 1.f) {
			m_FootFrame = 0.f;
			Effect_Foot_Step_Smoke();
		}
	}
	


	__super::Update_GameObject(fTimeDelta);


    Engine::Add_RenderGroup(RENDER_ALPHATEST, this);
	

	_vec3 v = m_pColliderCom->Get_SpherePos();

	return 0;
}

void CPlayer::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	/*_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);*/
}

void CPlayer::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	

	if (m_HitBlendRender) {
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(AlphaValue, R, G, B));
		m_pGraphicDev->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		m_pGraphicDev->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TFACTOR);
		
	}


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

	
	if (m_HitBlendRender) {
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		m_pGraphicDev->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	}


	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

void CPlayer::Create_Basic_Bow(void)
{
	POINT ptCursor;

	GetCursorPos(&ptCursor);
	ScreenToClient(g_hWnd, &ptCursor);

	_vec3 Axis = { -1.f,0.f,0.f };

	_vec3 Dir = { ptCursor.x - 400.f,ptCursor.y - 300.f,0 };
	D3DXVec3Normalize(&Dir, &Dir);


	// Angle 계산
	m_fAngle = acos(D3DXVec3Dot(&Axis, &Dir));

	if (300.f < ptCursor.y)
		m_fAngle = 2.f * D3DX_PI - m_fAngle;


	m_Mouse_Dir = { Dir.x,0.f,-Dir.y };

	_vec3 vPos = m_pTransformCom->m_vInfo[INFO_POS];

	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");


	CGameObject* pGameObject = nullptr;


	pGameObject = CSylphBow::Create(m_pGraphicDev, vPos, m_fAngle);

	if (pGameObject == nullptr)
		return;

	m_Bow_List[BOW_SYLPH] = pGameObject;
	m_Bow_Active[BOW_SYLPH] = true;
	pGameLogicLayer->Add_GameObject(L"SylphBow", pGameObject);    /// ?? 왜 터짐

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
	m_pColliderCom->Set_Radius(5.f);
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


	
	if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_LBUTTON)){

		m_Is_Fire_Arrow = true;

		CSylphBow* pObject1 = dynamic_cast<CSylphBow*>(Engine::Get_GameObject(L"Layer_GameLogic", L"SylphBow"));
		CSylphBowPair* pObject2 = dynamic_cast<CSylphBowPair*>(Engine::Get_GameObject(L"Layer_GameLogic", L"SylphBowPair"));

		if (m_Is_Effect_Charge_Arrow == false) {
			pObject1->m_bCharge = false;
			pObject2->m_bCharge = false;
		}
		

		POINT ptCursor;

		GetCursorPos(&ptCursor);
		ScreenToClient(g_hWnd, &ptCursor);


		if (ptCursor.x >= WINCX*0.5 && ptCursor.y <= WINCY*0.5) {
			m_iAttackAngleState = ATTACK_ANGLE_225;
			m_iAngleState = ANGLE_225;
		}
		else if (ptCursor.x < WINCX * 0.5 && ptCursor.y <= WINCY * 0.5) {
			m_iAttackAngleState = ATTACK_ANGLE_135;
			m_iAngleState = ANGLE_135;
		}
		else if (ptCursor.x < WINCX * 0.5 && ptCursor.y > WINCY * 0.5) {
			m_iAttackAngleState = ATTACK_ANGLE_045;
			m_iAngleState = ANGLE_045;
		}
		else if (ptCursor.x >= WINCX * 0.5 && ptCursor.y > WINCY * 0.5) {
			m_iAttackAngleState = ATTACK_ANGLE_315;
			m_iAngleState = ANGLE_315;
		}

		if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_W)) {
			

			if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_A)) {
				m_iState = MOVE_ATTACK;
				m_pTransformCom->Move_Pos(&(vLU * fTimeDelta * m_fSpeed));
			}
			else if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_D)) {
				m_iState = MOVE_ATTACK;
				m_pTransformCom->Move_Pos(&(vRU * fTimeDelta * m_fSpeed));
			}
			else {
				m_iState = MOVE_ATTACK;
				m_pTransformCom->Move_Pos(&(vDir * fTimeDelta * m_fSpeed));
			}
		}
		else if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_S)) {
			

			if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_A)) {
				m_iState = MOVE_ATTACK;
				m_pTransformCom->Move_Pos(&(vLD * fTimeDelta * m_fSpeed));
			}
			else if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_D)) {
				m_iState = MOVE_ATTACK;
				m_pTransformCom->Move_Pos(&(vRD * fTimeDelta * m_fSpeed));
			}
			else {
				m_iState = MOVE_ATTACK;
				m_pTransformCom->Move_Pos(&(vDir * fTimeDelta * -m_fSpeed));
			}

		}
		else if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_A)) {
			
			m_iState = MOVE_ATTACK;
			m_pTransformCom->Move_Pos(&(-vRight * fTimeDelta * m_fSpeed));

		}
		else if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_D)) {
			
			m_iState = MOVE_ATTACK;
			m_pTransformCom->Move_Pos(&(vRight * fTimeDelta * m_fSpeed));

		}
		else {
			m_iState = STAND_ATTACK;
		}

	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_RBUTTON)) {
		
		CSylphBow* pObject1 = dynamic_cast<CSylphBow*>(Engine::Get_GameObject(L"Layer_GameLogic", L"SylphBow"));
		CSylphBowPair* pObject2 = dynamic_cast<CSylphBowPair*>(Engine::Get_GameObject(L"Layer_GameLogic", L"SylphBowPair"));

		pObject1->m_bCharge = true;
		pObject2->m_bCharge = true;

		m_Is_Effect_Charge_Arrow = true;

		m_fChargeFrame += 1.f * fTimeDelta * 0.8f;
		if (m_fChargeFrame > 1.f) {
			m_Is_Charge_Arrow = true;
		}
		
		POINT ptCursor;

		GetCursorPos(&ptCursor);
		ScreenToClient(g_hWnd, &ptCursor);


		if (ptCursor.x >= WINCX * 0.5 && ptCursor.y <= WINCY * 0.5) {
			m_iAttackAngleState = ATTACK_ANGLE_225;
			m_iAngleState = ANGLE_225;
		}
		else if (ptCursor.x < WINCX * 0.5 && ptCursor.y <= WINCY * 0.5) {
			m_iAttackAngleState = ATTACK_ANGLE_135;
			m_iAngleState = ANGLE_135;
		}
		else if (ptCursor.x < WINCX * 0.5 && ptCursor.y > WINCY * 0.5) {
			m_iAttackAngleState = ATTACK_ANGLE_045;
			m_iAngleState = ANGLE_045;
		}
		else if (ptCursor.x >= WINCX * 0.5 && ptCursor.y > WINCY * 0.5) {
			m_iAttackAngleState = ATTACK_ANGLE_315;
			m_iAngleState = ANGLE_315;
		}

		
		if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_W)) {
			
			if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_A)) {
				m_iState = MOVE_ATTACK;
				m_pTransformCom->Move_Pos(&(vLU * fTimeDelta * m_fSpeed));
			}
			else if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_D)) {
				m_iState = MOVE_ATTACK;
				m_pTransformCom->Move_Pos(&(vRU * fTimeDelta * m_fSpeed));
			}
			else {
				m_iState = MOVE_ATTACK;
				m_pTransformCom->Move_Pos(&(vDir * fTimeDelta * m_fSpeed));
			}
		}
		else if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_S)) {
			
			if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_A)) {
				m_iState = MOVE_ATTACK;
				m_pTransformCom->Move_Pos(&(vLD * fTimeDelta * m_fSpeed));
			}
			else if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_D)) {
				m_iState = MOVE_ATTACK;
				m_pTransformCom->Move_Pos(&(vRD * fTimeDelta * m_fSpeed));
			}
			else {
				m_iState = MOVE_ATTACK;
				m_pTransformCom->Move_Pos(&(vDir * fTimeDelta * -m_fSpeed));
			}

		}
		else if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_A)) {
			
			m_iState = MOVE_ATTACK;
			m_pTransformCom->Move_Pos(&(-vRight * fTimeDelta * m_fSpeed));

		}
		else if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_D)) {
			
			m_iState = MOVE_ATTACK;
			m_pTransformCom->Move_Pos(&(vRight * fTimeDelta * m_fSpeed));

		}
		else {
			m_iState = STAND_ATTACK;
		}

	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_W)) {
	    
		m_Is_Fire_Arrow = false;
		m_Fire_Frame = m_Fire_Init;

		if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_A)) {
			if (CKeyMgr::Get_Instance()->Key_Down(KEY_SPACE)) {
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
		else if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_D)) {
			

			if (CKeyMgr::Get_Instance()->Key_Down(KEY_SPACE)) {
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
			if (CKeyMgr::Get_Instance()->Key_Down(KEY_SPACE)) {
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
	else if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_S)) {
	    m_Is_Fire_Arrow = false;
		m_Fire_Frame = m_Fire_Init;

		if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_A)) {
			if (CKeyMgr::Get_Instance()->Key_Down(KEY_SPACE)) {
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
		else if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_D)) {
			if (CKeyMgr::Get_Instance()->Key_Down(KEY_SPACE)) {
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
			if (CKeyMgr::Get_Instance()->Key_Down(KEY_SPACE)) {
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
	else if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_A)) {
	    m_Is_Fire_Arrow = false; 
		m_Fire_Frame = m_Fire_Init;
	    
		if (CKeyMgr::Get_Instance()->Key_Down(KEY_SPACE)) {
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
	else if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_D)) {
	    m_Is_Fire_Arrow = false;
		m_Fire_Frame = m_Fire_Init;

	    if (CKeyMgr::Get_Instance()->Key_Down(KEY_SPACE)) {
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
	else if (CKeyMgr::Get_Instance()->Key_Down(KEY_L)) {
	    m_Is_Fire_Arrow = false;
		m_Fire_Frame = m_Fire_Init;

	    m_iState = DEATH;
    }
	else {
		m_Is_Fire_Arrow = false;
		m_Fire_Frame = m_Fire_Init;
		m_iState = STAND;
	}


	if (CKeyMgr::Get_Instance()->Key_Up(KEY_RBUTTON)) {

		CEffect_Player_Bow_Charge* pObject = dynamic_cast<CEffect_Player_Bow_Charge*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Bow_Charge_Effect"));

		if (pObject != nullptr) {
			pObject->Set_Dead();
			m_Charge_Effect_Cnt = 0;
		}
			

		m_Is_Fire_Arrow = false;
		m_Fire_Frame = m_Fire_Init;

		if (m_Is_Charge_Arrow == true) {
			Fire_Charge_Arrow();
		}

		m_Is_Effect_Charge_Arrow = false;
		m_fChargeFrame = 0.f;
		m_Is_Charge_Arrow = false;
		
	}


}



void CPlayer::Fire_Arrow(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject = nullptr;

	POINT ptCursor;

	GetCursorPos(&ptCursor);
	ScreenToClient(g_hWnd, &ptCursor);

	_vec3 Axis = { -1.f,0.f,0.f };

	_vec3 Dir = { (float)(ptCursor.x - WINCX * 0.5),float(ptCursor.y - WINCY * 0.5),0 };
	D3DXVec3Normalize(&Dir, &Dir);


	// Angle 계산
	m_fAngle = acos(D3DXVec3Dot(&Axis, &Dir));

	if (WINCY * 0.5 < ptCursor.y)
		m_fAngle = 2.f * D3DX_PI - m_fAngle;

	
	m_Mouse_Dir = { Dir.x,0.f,-Dir.y };


	CSylphBow* pSylphBow = dynamic_cast<CSylphBow*>(Engine::Get_GameObject(L"Layer_GameLogic", L"SylphBow"));


	_vec3 vPos = pSylphBow->m_vPos;
	
	pGameObject = CSylphArrow::Create(m_pGraphicDev, vPos, m_Mouse_Dir, m_fAngle);

	if (pGameObject == nullptr)
		return;


	pGameLogicLayer->Add_BulletObject(pGameObject);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_ARROW, pGameObject);


	pGameObject = CEffect_Player_Bow_Pulse::Create(m_pGraphicDev);

	if (pGameObject == nullptr)
		return;

	pGameLogicLayer->Add_BulletObject( pGameObject);



}

void CPlayer::Fire_Charge_Arrow(void)
{

	//m_Charge_Effect_Cnt--;


	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject = nullptr;

	POINT ptCursor;

	GetCursorPos(&ptCursor);
	ScreenToClient(g_hWnd, &ptCursor);

	_vec3 Axis = { -1.f,0.f,0.f };
	D3DXVec3Normalize(&Axis, &Axis);

	_vec3 Dir = { (float)(ptCursor.x - WINCX * 0.5),float(ptCursor.y - WINCY * 0.5),0 };
	D3DXVec3Normalize(&Dir, &Dir);


	// Angle 계산
	m_fAngle = acos(D3DXVec3Dot(&Axis, &Dir));

	if (WINCY * 0.5 < ptCursor.y)
		m_fAngle = 2.f * D3DX_PI - m_fAngle;


	m_Mouse_Dir = { Dir.x,0.f,-Dir.y };


	CSylphBow* pSylphBow = dynamic_cast<CSylphBow*>(Engine::Get_GameObject(L"Layer_GameLogic", L"SylphBow"));


	_vec3 vPos = pSylphBow->m_vPos;

	pGameObject = CSylphChargeArrow::Create(m_pGraphicDev, vPos, m_Mouse_Dir, m_fAngle);

	if (pGameObject == nullptr)
		return;


	pGameLogicLayer->Add_BulletObject(  pGameObject);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_ARROW, pGameObject);

}

void CPlayer::Effect_Charge_Arrow(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject = nullptr;


	if (m_Charge_Effect_Cnt == 0) {

		pGameObject = CEffect_Player_Bow_Charge::Create(m_pGraphicDev);

		if (pGameObject == nullptr)
			return;

		pGameLogicLayer->Add_GameObject(L"Bow_Charge_Effect", pGameObject);

		m_Charge_Effect_Cnt++;
	}


}

void CPlayer::Effect_Foot_Step_Smoke(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject = nullptr;

	if (m_bReverse)
		m_FootRev = false;
	else
		m_FootRev = true;
	
	pGameObject = CEffect_Player_Foot::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS],FOOT_TYPE_SMOKE, m_FootRev);

	if (pGameObject == nullptr)
		return;

	pGameLogicLayer->Add_BulletObject(pGameObject);


}

void CPlayer::Effect_Foot_Step_Water(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject = nullptr;

	if (m_bReverse)
		m_FootRev = false;
	else
		m_FootRev = true;

	pGameObject = CEffect_Player_Foot::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS], FOOT_TYPE_WATER, m_FootRev);

	if (pGameObject == nullptr)
		return;

	pGameLogicLayer->Add_BulletObject(pGameObject);

}

void CPlayer::Frame_Manage(const _float& fTimeDelta)
{

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

		m_fDashFrame += 5.f * fTimeDelta * 2.0f;
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

	if (m_HitBlend) {

		m_HitBlendFrame += m_HitMaxFrame * fTimeDelta * 6.f;
		if (m_HitBlendFrame > m_HitMaxFrame) {
			m_HitBlendFrame = 0.f;
			m_HitBlendCnt++;
			if (m_HitBlendRender)
				m_HitBlendRender = false;
			else {
				m_HitBlendRender = true;
			}
		}
	}

	if (m_HitBlendCnt > m_HitBlendMaxCnt) {
		m_HitBlend = false;
		m_HitBlendRender = false;
		m_bImmuned = false;
		m_HitBlendCnt = 0;
	}

}


void CPlayer::SetUp_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	CTerrainTex*		pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTex", ID_STATIC));
	NULL_CHECK(pTerrainBufferCom);

	_float	fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pTransformCom->Set_Pos(vPos.x, fHeight + 5.f + jump, vPos.z);
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

	CSylphBow* pObject1 = dynamic_cast<CSylphBow*>(Engine::Get_GameObject(L"Layer_GameLogic", L"SylphBow"));
	CSylphBowPair* pObject2 = dynamic_cast<CSylphBowPair*>(Engine::Get_GameObject(L"Layer_GameLogic", L"SylphBowPair"));

	if (m_iState == STAND) {
		m_Is_Run = false;
		if (pObject1 != nullptr && pObject2 != nullptr) {
			pObject1->Set_Render(false);
			pObject2->Set_Render(false);
		}
			
		if (m_iAngleState == ANGLE_000) {
			
		}
		else if (m_iAngleState == ANGLE_045) {
			if (m_bReverse == true) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = false;
			}
		}
		else if (m_iAngleState == ANGLE_090) {
			
			if (m_bReverse == true) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = false;
			}
		}
		else if (m_iAngleState == ANGLE_135) {
			
			if (m_bReverse == true) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = false;
			}
		}
		else if (m_iAngleState == ANGLE_180) {
			
		}
		else if (m_iAngleState == ANGLE_225) {
			
			if (m_bReverse == false) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = true;
			}
		}
		else if (m_iAngleState == ANGLE_270) {
			
			if (m_bReverse == false) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = true;
			}
		}
		else if (m_iAngleState == ANGLE_315) {
			if (m_bReverse == false) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = true;
			}
		}
	}
	else if (m_iState == RUN) {

		m_Is_Run = true;

		if (pObject1 != nullptr && pObject2 != nullptr) {
			pObject1->Set_Render(false);
			pObject2->Set_Render(false);
		}

		if (m_iAngleState == ANGLE_000) {
			
		}
		else if (m_iAngleState == ANGLE_045) {
			if (m_bReverse == true) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = false;
			}
		}
		else if (m_iAngleState == ANGLE_090) {
			
			if (m_bReverse == true) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = false;
			}
		}
		else if (m_iAngleState == ANGLE_135) {
			
			if (m_bReverse == true) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = false;
			}
		}
		else if (m_iAngleState == ANGLE_180) {
			
		}
		else if (m_iAngleState == ANGLE_225) {
			
			if (m_bReverse == false) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = true;
			}
		}
		else if (m_iAngleState == ANGLE_270) {
			
			if (m_bReverse == false) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = true;
			}
		}
		else if (m_iAngleState == ANGLE_315) {
			
			if (m_bReverse == false) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = true;
			}
		}
	}
	else if (m_iState == MOVE_ATTACK) {
		m_Is_Run = false;

		if (pObject1 != nullptr && pObject2 != nullptr) {
			pObject1->Set_Render(true);
			pObject2->Set_Render(true);
		}
		if (m_iAttackAngleState == ATTACK_ANGLE_045) {

			if (m_bReverse == true) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = false;
			}
		}
		else if (m_iAttackAngleState == ATTACK_ANGLE_135) {

			if (m_bReverse == true) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = false;
			}
		}
		else if (m_iAttackAngleState == ATTACK_ANGLE_225) {

			if (m_bReverse == false) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = true;
			}
		}
		else if (m_iAttackAngleState == ATTACK_ANGLE_315) {

			if (m_bReverse == false) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = true;
			}
		}
	}
	else if (m_iState == STAND_ATTACK) {
		m_Is_Run = false;
		if (pObject1 != nullptr && pObject2 != nullptr) {
			pObject1->Set_Render(true);
			pObject2->Set_Render(true);
		}


		if (m_iAttackAngleState == ATTACK_ANGLE_045) {

			if (m_bReverse == true) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = false;
			}
		}
		else if (m_iAttackAngleState == ATTACK_ANGLE_135) {

			if (m_bReverse == true) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = false;
			}
		}
		else if (m_iAttackAngleState == ATTACK_ANGLE_225) {

			if (m_bReverse == false) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = true;
			}
		}
		else if (m_iAttackAngleState == ATTACK_ANGLE_315) {

			if (m_bReverse == false) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = true;
			}
		}
	}
	else if (m_iState == DASH) {
		m_Is_Run = false;
		if (pObject1 != nullptr && pObject2 != nullptr) {
			pObject1->Set_Render(false);
			pObject2->Set_Render(false);
		}
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


	jump += 2.f * fTimeDelta * jumpDir * 4.0f;

	if (jump >= 2.f) {
		jump = 2.f;
		jumpDir = -1.f;
	}

	if (jump <= 0.f) {
		jump = 0.f;
		jumpDir = 1.f;
	}

	if (m_Dash_Time > 5.f) {
		m_Dash_Time = 0.f;
		m_Is_Dash = false;
		m_bImmuned = false;
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


