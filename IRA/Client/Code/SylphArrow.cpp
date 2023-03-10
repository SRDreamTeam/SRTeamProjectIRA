#include "stdafx.h"
#include "..\Header\SylphArrow.h"
#include "Export_Function.h"
#include "Effect_Player_Arrow_Hit.h"
#include "Effect_Player_Damage_Font.h"
#include "Player.h"

CSylphArrow::CSylphArrow(LPDIRECT3DDEVICE9 pGraphicDev)
	: CArrow(pGraphicDev)
{
}

CSylphArrow::CSylphArrow(const CSylphArrow& rhs)
	: CArrow(rhs)
{
}

CSylphArrow::~CSylphArrow()
{
}

HRESULT CSylphArrow::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fSpeed = 20.f;

	m_pTransformCom->Set_Scale_Ratio({ 3.f, 3.f, 1.f });

	m_pTransformCom->Rotation(ROT_Y, m_Arrow_Angle);

	m_vDir = m_target_Dir;
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	_vec3 Dir;
	D3DXVec3Normalize(&Dir, &m_vDir);

	m_Fire_Pos += Dir * 5.f;

	m_pTransformCom->Set_Pos(m_Fire_Pos.x, m_Fire_Pos.y - 2.f, m_Fire_Pos.z);

	

	
	
	__super::Ready_GameObject();

	return S_OK;
}

_int CSylphArrow::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_bHit) {
		Create_Hit_Effect();
		Create_Damage_Font();
		return OBJ_DEAD;
	}

	m_AccTime += m_AccMaxTime * fTimeDelta * 1.f;

	if (m_AccTime > m_AccMaxTime) {
		m_AccTime = 0.f;
		m_bHit = true;
	}

	m_pTransformCom->Move_Pos(&(m_vDir * fTimeDelta * m_fSpeed));

	if (m_iState == ARROW_IDLE) {
		m_fIdleFrame += 5.f * fTimeDelta * 1.5f;
		if (5.f < m_fIdleFrame)
			m_fIdleFrame = 0.f;
	}

	if (m_iState == ARROW_DEATH) {
		m_fDeathFrame += 3.f * fTimeDelta * 2.f;
		if (3.f < m_fDeathFrame) {
			m_fDeathFrame = 3.f;
			//m_bDead = true;
			//m_bHit = true;
		}
			
	}

	Engine::Add_RenderGroup(RENDER_ALPHA, this);


	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CSylphArrow::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CSylphArrow::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);
	
	if (m_iState == ARROW_IDLE) {
		m_pTextureCom[ARROW_IDLE]->Set_Texture(0);
	}



	m_pTransformCom->m_vInfo[INFO_POS];

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, true);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CSylphArrow::Add_Component(void)
{
	
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	

	pComponent = m_pTextureCom[ARROW_IDLE] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Arrow_Sylph_Idle"));
	NULL_CHECK_RETURN(m_pTextureCom[ARROW_IDLE], E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Arrow_Sylph_Idle", pComponent });

	


	return S_OK;
}


void CSylphArrow::SetUp_OnTerrain(void)
{
	

}

void CSylphArrow::Change_State(void)
{
	



}

void CSylphArrow::Frame_Check(const _float& fTimeDelta)
{
	
}

CSylphArrow* CSylphArrow::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vDir,_float Angle)
{
	CSylphArrow* pInstance = new CSylphArrow(pGraphicDev);

	if (pInstance != nullptr) {
		pInstance->m_target_Dir = vDir;
		pInstance->m_Fire_Pos = vPos;
		pInstance->m_Arrow_Angle = Angle;
	}

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}


	return pInstance;
}

bool CSylphArrow::Final_Damage(void)
{

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));

	int CriticalRate = (int)(pPlayer->m_Critical_Rate);

	bool Critical;

	int Random = rand() % 100 + 1;

	if (Random <= CriticalRate) {
		Critical = true;
	}
	else {
		Critical = false;
	}

	m_fRandom_Value = (float)(rand() % (2 * (587 / 8 - 587 / 16 + 1)) - (587 / 8 - 587 / 16 + 1));

	m_fDamage = (m_fPower * 0.5f + m_fRandom_Value) * (1.f + 1.35f * Critical);

	int temp = (int)m_fDamage;
	int cnt = 0;

	while (temp > 0) {
		temp /= 10;
		cnt++;
	}

	temp = (int)m_fDamage;

	for (int i = 0; i < cnt; i++) {

		if (i == cnt - 1) {
			m_Font_List.emplace_front(temp);
			break;
		}

		m_Font_List.emplace_front(temp % 10);
		temp /= 10;

	}


	return Critical;

}

void CSylphArrow::Create_Hit_Effect(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject;

	pGameObject = CEffect_Player_Arrow_Hit::Create(m_pGraphicDev, ARROW_TYPE_SYLPH, m_pTransformCom->m_vInfo[INFO_POS]);

	if (pGameObject == nullptr)
		return;

	pGameLogicLayer->Add_BulletObject(OBJ_EFFECT, pGameObject);


}

void CSylphArrow::Create_Damage_Font(void)
{

	bool Cri = Final_Damage();

	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject;


	for (int i = 0; i < m_fAttack_Num; i++) {

		int j = 0;

		for (auto iter : m_Font_List) {
			_vec3 pos = m_pTransformCom->m_vInfo[INFO_POS];

			if (Cri == true)
				pos.x += 1.7f * j;
			else {
				pos.x += 1.7f * 0.7 * j;
			}

			pGameObject = CEffect_Player_Damage_Font::Create(m_pGraphicDev, pos, (int)iter, Cri);

			if (pGameObject == nullptr)
				return;

			pGameLogicLayer->Add_BulletObject(OBJ_EFFECT, pGameObject);

			j++;
		}

	}


}

void CSylphArrow::Free(void)
{
	__super::Free();
}