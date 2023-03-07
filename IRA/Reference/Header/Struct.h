#pragma once

#include "Typedef.h"

BEGIN(Engine)

typedef	 struct tagVertexColor
{
	_vec3			vPos;		// 위치
	_ulong			dwColor;	// 색상

}VTXCOL;

const	_ulong		FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

typedef	 struct tagVertexTexture
{
	_vec3			vPos;		// 위치
	_vec3			vNormal;
	_vec2			vTexUV;		

}VTXTEX;

const	_ulong		FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 ;

typedef	 struct tagVertexCube
{
	_vec3			vPos;		// 위치
	_vec3			vTexUV;		// 색상

}VTXCUBE;

const	_ulong		FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);


// D3DFVF_TEXCOORDSIZE3 : 텍스처의 UV값이 FLOAT형 3개의 크기 만큼이며 괄호 안 숫자의 의미는 본래 버텍스의 UV 값이 여러개 있을 수 잇는데 그 중 첫 번째 값을 사용하겠다는 의미

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





END