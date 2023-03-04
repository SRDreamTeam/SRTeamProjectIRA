#pragma once
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CPlayer : public Engine::CGameObject
{
	enum {PLAYER_HEAD,PLAYER_FACE,PLAYER_END};

	enum {STAND,RUN,MOVE_ATTACK,STAND_ATTACK,DEATH,DASH,STATE_END};

	enum { STAND_000, STAND_045, STAND_090, STAND_135, STAND_180, STAND_END };
	enum { RUN_000, RUN_045, RUN_090, RUN_135, RUN_180, RUN_END };
	enum { MOVE_ATTACK_045,MOVE_ATTACK_135, MOVE_ATTACK_END };
	enum { STAND_ATTACK_045,STAND_ATTACK_135,STAND_ATTACK_END };
	enum { DASH_045, DASH_135, DASH_END };
	enum { ANGLE_000, ANGLE_045, ANGLE_090, ANGLE_135, ANGLE_180, ANGLE_225, ANGLE_270, ANGLE_315, ANGLE_STATE_END };
	enum { ATTACK_ANGLE_045, ATTACK_ANGLE_135, ATTACK_ANGLE_225, ATTACK_ANGLE_315, ATTACK_ANGLE_STATE_END };
	
	
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);
	void			Key_Input(const _float& fTimeDelta);
	void			SetUp_OnTerrain(void);
	_vec3			Picking_OnTerrain(void);
	void            Update_State();
	_vec3           Get_World_Mouse(int x, int y);
	void            Dash(const _float& fTimeDelta);

private:
	CRcTex*			m_pBufferCom;
	CTransform*		m_pTransformCom;

	CTexture*       m_pStandTextureCom[STAND_END] = {};
	CTexture*       m_pRunTextureCom[RUN_END] = {};

	CTexture*       m_pMoveAttackTextureCom[MOVE_ATTACK_END] = {};
	CTexture*       m_pStandAttackTextureCom[STAND_ATTACK_END] = {};

	CTexture*       m_pDashTextureCom[DASH_END] = {};

	CTexture*		m_pDeathTextureCom;

	CCalculator*	m_pCalculatorCom;

private:	
	_float			m_fSpeed = 20.f;

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


public:
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

};

