#pragma once

#include "Typedef.h"

BEGIN(Engine)

typedef	 struct tagVertexColor
{
	_vec3			vPos;		// ��ġ
	_ulong			dwColor;	// ����

}VTXCOL;

const	_ulong		FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

typedef	 struct tagVertexTexture
{
	_vec3			vPos;		// ��ġ
	_vec3			vNormal;
	_vec2			vTexUV;		

}VTXTEX;

const	_ulong		FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 ;

typedef	 struct tagVertexCube
{
	_vec3			vPos;		// ��ġ
	_vec3			vTexUV;		// ����

}VTXCUBE;

const	_ulong		FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);


// D3DFVF_TEXCOORDSIZE3 : �ؽ�ó�� UV���� FLOAT�� 3���� ũ�� ��ŭ�̸� ��ȣ �� ������ �ǹ̴� ���� ���ؽ��� UV ���� ������ ���� �� �մµ� �� �� ù ��° ���� ����ϰڴٴ� �ǹ�

typedef	struct tagIndex16
{
	_ushort		_0;
	_ushort		_1;
	_ushort		_2;

}INDEX16;

typedef	struct tagIndex32
{
	_ulong		_0;
	_ulong		_1;
	_ulong		_2;

}INDEX32;



typedef	struct tagPlayerInfo
{
	int Maxhp;
	int Nowhp;
	int MaxEnergy;
	int NowEnergy;
	int Key;
	int Money;
	int Gem;

}PLAYER_INFO;





typedef	struct tagFrontUiInfo
{
	_int		iNum;	//  FrontUi ���� �� ������Ʈ���� ���� �� �ĺ���ȣ
	_int		iMax;	// �θ� �ڽ� ��� ���� �� Max �� ex) MaxHp
	_int		iNow;	// �θ� �ڽ� ��� ���� �� Now �� ex) NowHp

}FRONTUI;

END

typedef struct tagTerrainData
{
	_ulong	dwVtxCntX;
	_ulong	dwVtxCntZ;
	_ulong	dwVtxItv;
	_byte	byDrawID;
	_byte	byDrawOption;
	_float	fDetailLevel;
	_int	iHeightMapLevel;
} TERRAINDATA;

typedef struct tagTerrainDataPointer
{
	_ulong	dwVtxCnt;
	_vec3* pPos;
} TERRAINDATA_P;

typedef struct tagStaticObjectData
{
	_vec3	vScale;
	_vec3	vAngle;
	_vec3	vPos;
	_byte	byDrawID;
	_byte	byDrawOption;
	_vec3	vColliderPos;
	_float	fColliderRadius;
}STATICOBJDATA;