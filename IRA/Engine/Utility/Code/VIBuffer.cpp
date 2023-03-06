#include "..\..\Header\VIBuffer.h"

CVIBuffer::CVIBuffer(void)
{
}

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_pVB(nullptr)
	, m_pIB(nullptr)
	, m_dwFVF(0)
	, m_dwVtxSize(0)
	, m_dwVtxCnt(0)
	, m_dwTriCnt(0)
	, m_dwIdxSize(0)
	, m_IdxFmt(D3DFMT_UNKNOWN)
{
}

CVIBuffer::CVIBuffer(const CVIBuffer & rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_dwFVF(rhs.m_dwFVF)
	, m_dwVtxSize(rhs.m_dwVtxSize)
	, m_dwVtxCnt(rhs.m_dwVtxCnt)
	, m_dwTriCnt(rhs.m_dwTriCnt)
	, m_dwIdxSize(rhs.m_dwIdxSize)
	, m_IdxFmt(rhs.m_IdxFmt)
{
	m_pVB->AddRef();
	m_pIB->AddRef();
}

CVIBuffer::~CVIBuffer()
{
}

HRESULT CVIBuffer::Ready_Buffer(void)
{
	if (FAILED(m_pGraphicDev->CreateVertexBuffer(m_dwVtxCnt * m_dwVtxSize,	// 정점 버퍼의 사이즈
												0,			// 버퍼를 만드는 방식(0인 경우 정적 버퍼, D3DUSAGE_DYNAMIC을 지정하면 동적 버퍼)
												m_dwFVF,	// 정점의 옵션
												D3DPOOL_MANAGED,
												&m_pVB,
												nullptr)))
	{
		MSG_BOX("Createvertexbuffer Failed");
		return E_FAIL;
	}

	if (FAILED(m_pGraphicDev->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt,	// 인덱스 버퍼의 사이즈
													0,			// 버퍼를 만드는 방식(0인 경우 정적 버퍼, D3DUSAGE_DYNAMIC을 지정하면 동적 버퍼)
													m_IdxFmt,	// 인덱스의 옵션
													D3DPOOL_MANAGED,
													&m_pIB,
													nullptr)))
	{
		MSG_BOX("CreateIndexbuffer Failed");
		return E_FAIL;
	}
	

	return S_OK;
}

void CVIBuffer::Render_Buffer(void)
{
	// 그리기 장치에 현재 그리고자 하는 버텍스 정보를 전달하는 함수
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);

	// 1. 몇 번째 슬롯에 전달할 것인가
	// 2. 어떤 정보를 전달할 것인가
	// 3. 어디서부터 그리기 연산을 수행할 것인가
	// 4. 어떤 단위로 그리기 연산을 수행할 것인가

	m_pGraphicDev->SetFVF(m_dwFVF);

	m_pGraphicDev->SetIndices(m_pIB);

	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);


	/*m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_dwTriCnt);

	// 그리기 함수
	// 1. 플래그 값에 따라 삼각형을 그리는 방법이 결정
	// 2. 몇 번째 버텍스부터 그릴 것인가
	// 3. 그릴 삼각형의 개수*/
}

void Engine::CVIBuffer::Free(void)
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}


