#pragma once
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;
class CCollider;
class CSphereTex;

END

class CPlayer : public Engine::CGameObject
{
	enum {PLAYER_HEAD,PLAYER_FACE,PLAYER_END};

	enum {STAND,RUN,MOVE_ATTACK,STAND_ATTACK,DEATH,DASH,TELEPORT,LANDING,STATE_END};

	enum { STAND_000, STAND_045, STAND_090, STAND_135, STAND_180, STAND_END };
	enum { RUN_000, RUN_045, RUN_090, RUN_135, RUN_180, RUN_END };
	enum { MOVE_ATTACK_045,MOVE_ATTACK_135, MOVE_ATTACK_END };
	enum { STAND_ATTACK_045,STAND_ATTACK_135,STAND_ATTACK_END };
	enum { DASH_045, DASH_135, DASH_END };
	enum { ANGLE_000, ANGLE_045, ANGLE_090, ANGLE_135, ANGLE_180, ANGLE_225, ANGLE_270, ANGLE_315, ANGLE_STATE_END };
	enum { ATTACK_ANGLE_045, ATTACK_ANGLE_135, ATTACK_ANGLE_225, ATTACK_ANGLE_315, ATTACK_ANGLE_STATE_END };
	
	enum { BOW_SYLPH, BOW_END };

private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;


public:
	_float Get_Dash_Frame(void) {
		return m_fDashFrame;
	}
	int    Get_Anlge_State(void) {
		return m_iAngleState;
	}

	void Create_Basic_Bow(void);

	const PLAYER_INFO&  Get_Player_Info(void) {
		return m_tInfo;
	}

	void Set_Player_Info(const PLAYER_INFO& tInfo) {
		m_tInfo = tInfo;
	}

	

private:
	HRESULT			Add_Component(void);

	void			Key_Input(const _float& fTimeDelta);

	void			SetUp_OnTerrain(void);

	_vec3			Picking_OnTerrain(void);

	void            Update_State();

	_vec3           Get_World_Mouse();

	void            Dash(const _float& fTimeDelta);

	void            Fire_Arrow(void);;

	void            Fire_Charge_Arrow(void);

	void            Effect_Charge_Arrow(void);

	void            Effect_Foot_Step_Smoke(void);
	void            Effect_Foot_Step_Water(void);

	void            Frame_Manage(const _float& fTimeDelta);

	void            Hp_Down(void)
	{
		m_tInfo.Nowhp--;

		if (m_tInfo.Nowhp < 0) {
			m_tInfo.Nowhp = 0;
		}

		m_bHit = false;
	}

private:
	CRcTex*			m_pBufferCom;
	CTransform*		m_pTransformCom;

	CTexture*       m_pStandTextureCom[STAND_END] = {};
	CTexture*       m_pRunTextureCom[RUN_END] = {};

	CTexture*       m_pMoveAttackTextureCom[MOVE_ATTACK_END] = {};
	CTexture*       m_pStandAttackTextureCom[STAND_ATTACK_END] = {};

	CTexture*       m_pDashTextureCom[DASH_END] = {};

	CTexture*		m_pDeathTextureCom;

	CCollider*		m_pColliderCom;
	CCalculator*	m_pCalculatorCom;

private:	
	_float			m_fSpeed = 25.f * PUBLIC_SCALE;
	_float			m_fDashSpeed = 1.2f * PUBLIC_SCALE;

	_float			m_fStandFrame = 0.f;
	_float			m_fRunFrame = 0.f;
	_float			m_fAttackFrame = 0.f;
	_float			m_fDashFrame = 0.f;
	_float			m_fDeathFrame = 0.f;


	_int            m_iState = STAND;
	_int            m_iPreState = STAND;

	_int            m_iAngleState = ANGLE_000;
	_int            m_iPreAngleState = ANGLE_000;

	_int            m_iAttackAngleState = ATTACK_ANGLE_045;
	_int            m_iPreAttackAngleState = ATTACK_ANGLE_045;

	bool            m_bReverse = false;
	bool            m_bAttackReverse = false;

	_float          m_fAngle = 0.f;

	_float          m_Dash_Time = 0.f;
	bool            m_Is_Dash = false;
	bool            m_Is_Jump = false;
	bool            m_Is_Run = false;
	float           jump = 0.f;
	float           jumpDir = 1.f;

	bool            m_Is_Fire_Arrow = false;
	_float          m_Fire_Frame = 3.f;
	_float          m_Fire_Speed = 4.f;
	_float          m_Fire_Init = 4.f;

	bool            m_Is_Effect_Charge_Arrow = false;
	int             m_Charge_Effect_Cnt = 0;
	bool            m_Is_Charge_Arrow = false;
	_float			m_fChargeFrame = 0.f;


	CSphereTex*		m_pSphereBufferCom;

	_vec3           m_Mouse_Dir;
	

	CGameObject*    m_Bow_List[BOW_END] = {};
	bool            m_Bow_Active[BOW_END] = {};
	bool            m_Bow_Render[BOW_END] = {};


	// Foot
	_float          m_FootFrame = 0.f;
	bool            m_FootRev = false;

	
public:
	bool            m_bHit = false;
	bool            m_bImmuned = false;
	float           m_Critical_Rate = 38.f;

private:
	// UI ????
	PLAYER_INFO     m_tInfo;

	
	// Hit ????
	bool            m_HitBlend = false;
	bool            m_HitBlendRender = false;
	float           m_HitBlendFrame = 0.f;
	float           m_HitMaxFrame = 2.f;
	int             m_HitBlendCnt = 0;
	int             m_HitBlendMaxCnt = 6;

	DWORD           AlphaValue = 100;
	DWORD           R = 255;
	DWORD           G = 255;
	DWORD           B = 255;

	

public:
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

};

