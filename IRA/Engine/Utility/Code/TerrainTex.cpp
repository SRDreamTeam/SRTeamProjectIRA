 #include "..\..\Header\TerrainTex.h"

CTerrainTex::CTerrainTex(void)
{
}

CTerrainTex::CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev)
	:CVIBuffer(pGraphicDev)
	, m_pPos(nullptr)	
{
}

CTerrainTex::CTerrainTex(const CTerrainTex & rhs)
	:CVIBuffer(rhs)
	, m_hFile(rhs.m_hFile)
	, m_fh(rhs.m_fh)
	, m_ih(rhs.m_ih)
	, m_pPos(rhs.m_pPos)

{
}

CTerrainTex::~CTerrainTex()
{
}

HRESULT CTerrainTex::Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
	m_dwFVF = FVF_TEX;
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = dwCntX * dwCntZ;
	
	m_pPos = new _vec3[m_dwVtxCnt];

	m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	_ulong	dwByte = 0;

	m_hFile = CreateFile(L"../Bin/Resource/Texture/Terrain/Height.bmp", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	ReadFile(m_hFile, &m_fh, sizeof(BITMAPFILEHEADER), &dwByte, NULL);
	ReadFile(m_hFile, &m_ih, sizeof(BITMAPINFOHEADER), &dwByte, NULL);

	_ulong*	pPixel = new _ulong[m_ih.biHeight * m_ih.biWidth];

	ReadFile(m_hFile, pPixel, sizeof(_ulong) * m_ih.biHeight * m_ih.biWidth, &dwByte, NULL);

	CloseHandle(m_hFile);

	VTXTEX*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	// Lock : 버텍스 버퍼 메모리 공간의 접근을 막음, 버텍스 버퍼에 저장되어 있는 첫 번째 버텍스의 주소를 추출

	_ulong	dwIndex = 0;

	for (_ulong i = 0; i < dwCntZ; ++i)
	{
		for (_ulong j = 0; j < dwCntX; ++j)
		{
			dwIndex = i * dwCntX + j;

			pVertex[dwIndex].vPos = { _float(j * dwVtxItv), 0.f, _float(i  * dwVtxItv) };

			m_pPos[dwIndex] = pVertex[dwIndex].vPos;

			pVertex[dwIndex].vNormal = { 0.f, 0.f, 0.f };

			pVertex[dwIndex].vTexUV = { _float(j) / (dwCntX - 1) * 5.f,
										_float(i) / (dwCntZ - 1) * 5.f 
									  };
		}
	}


	INDEX32*		pIndex = nullptr;

	_ulong	dwTriIdx = 0;

	_vec3	vDest, vSrc, vNormal;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	for (_ulong i = 0; i < dwCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwCntX - 1; ++j)
		{
			dwIndex = i * dwCntX + j;

			// 오른쪽 위

			pIndex[dwTriIdx]._0 = dwIndex + dwCntX;
			pIndex[dwTriIdx]._1 = dwIndex + dwCntX + 1;
			pIndex[dwTriIdx]._2 = dwIndex + 1;

			vDest = pVertex[pIndex[dwTriIdx]._1].vPos - pVertex[pIndex[dwTriIdx]._0].vPos;
			vSrc = pVertex[pIndex[dwTriIdx]._2].vPos - pVertex[pIndex[dwTriIdx]._1].vPos;
			D3DXVec3Cross(&vNormal, &vDest, &vSrc);

			pVertex[pIndex[dwTriIdx]._0].vNormal += vNormal;
			pVertex[pIndex[dwTriIdx]._1].vNormal += vNormal;
			pVertex[pIndex[dwTriIdx]._2].vNormal += vNormal;
			dwTriIdx++;

			// 왼쪽 아래
				 
			pIndex[dwTriIdx]._0 = dwIndex + dwCntX;
			pIndex[dwTriIdx]._1 = dwIndex + 1;
			pIndex[dwTriIdx]._2 = dwIndex;

			vDest = pVertex[pIndex[dwTriIdx]._1].vPos - pVertex[pIndex[dwTriIdx]._0].vPos;
			vSrc = pVertex[pIndex[dwTriIdx]._2].vPos - pVertex[pIndex[dwTriIdx]._1].vPos;
			D3DXVec3Cross(&vNormal, &vDest, &vSrc);

			pVertex[pIndex[dwTriIdx]._0].vNormal += vNormal;
			pVertex[pIndex[dwTriIdx]._1].vNormal += vNormal;
			pVertex[pIndex[dwTriIdx]._2].vNormal += vNormal;

			dwTriIdx++;
		}
	}

	Safe_Delete_Array(pPixel);

	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
		D3DXVec3Normalize(&pVertex[i].vNormal, &pVertex[i].vNormal);
			
	m_pVB->Unlock();
	m_pIB->Unlock();

	return S_OK;
}

void CTerrainTex::Render_Buffer(void)
{
	__super::Render_Buffer();
}


Engine::CComponent* CTerrainTex::Clone(void)
{	
	return new CTerrainTex(*this);
}

CTerrainTex * CTerrainTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
	CTerrainTex * pInstance = new CTerrainTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(dwCntX, dwCntZ, dwVtxItv)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void Engine::CTerrainTex::Free(void)
{
	if (false == m_bClone)
		Safe_Delete_Array(m_pPos);

	__super::Free();
}
