
#include "..\..\Header\RcTex.h"

CRcTex::CRcTex(void)
{
}

CRcTex::CRcTex(LPDIRECT3DDEVICE9 pGraphicDev)
	:CVIBuffer(pGraphicDev)
{
}

CRcTex::CRcTex(const CRcTex & rhs)
	: CVIBuffer(rhs)
{
}

CRcTex::~CRcTex()
{
}

HRESULT CRcTex::Ready_Buffer(void)
{
	m_dwFVF = FVF_TEX;
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = 4;
	m_dwTriCnt = 2;

	m_dwIdxSize = sizeof(INDEX32);

	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXTEX*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	// Lock : 버텍스 버퍼 메모리 공간의 접근을 막음, 버텍스 버퍼에 저장되어 있는 첫 번째 버텍스의 주소를 추출

	pVertex[0].vPos = { -1.f, 1.f, 0.f };
	pVertex[0].vTexUV = { 0.f, 0.f };

	pVertex[1].vPos = { 1.f, 1.f, 0.f };
	pVertex[1].vTexUV = { 1.f, 0.f };

	pVertex[2].vPos = { 1.f, -1.f, 0.f };
	pVertex[2].vTexUV = { 1.f, 1.f };

	pVertex[3].vPos = { -1.f, -1.f, 0.f };
	pVertex[3].vTexUV = { 0.f, 1.f };

	m_pVB->Unlock();

	INDEX32* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

void CRcTex::Render_Buffer(void)
{
	__super::Render_Buffer();
}


Engine::CComponent* CRcTex::Clone(void)
{
	return new CRcTex(*this);
}

CRcTex * CRcTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRcTex * pInstance = new CRcTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void Engine::CRcTex::Free(void)
{
	__super::Free();
}
