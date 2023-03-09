#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainTex : public CVIBuffer
{
private:
	explicit CTerrainTex(void);
	explicit CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrainTex(const CTerrainTex& rhs);
	virtual ~CTerrainTex();

public:
	const _vec3*	Get_VtxPos(void) { return m_pPos; }
	_ulong			Get_VtxCntX(void) const { return m_ih.biWidth; }
	_ulong			Get_VtxCntZ(void) const { return m_ih.biHeight; }

	HRESULT		Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);
	void		Render_Buffer(void);

private:
	HANDLE				m_hFile;
	BITMAPINFOHEADER	m_ih;
	BITMAPFILEHEADER	m_fh;
	_vec3*				m_pPos;

public:
	virtual CComponent* Clone(void) override;
	static CTerrainTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);

private:
	virtual void Free(void) override;
};

END