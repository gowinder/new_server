#ifndef ___AUTOSIZEWITHRECYCLECONTAINER_HEADER___
#define ___AUTOSIZEWITHRECYCLECONTAINER_HEADER___
#include "struct.h"
#include "interface.h"
#include "basedef.h"

namespace gowinder
{

namespace baselib
{


const int MAX_ADV_EVENT_DATA_SIZE = 1024 * 256;

/// \author �粻Ϣ gowinder@hotmail.com
/// \brief  ����յ��ڴ�����ģ��?
/// \date [9/24/2013]
/// \detail �ɰ������С����̶�����ڴ��С,�л��ջ���, T_MAPΪ����С��ֵ���ݿ�(����Ϊmap), T_DATAΪ��ݿ�����?����Ϊlist)
/// \return	
template <class T_MAP, class T_DATA>
class auto_size_with_recycle_container
{
protected:
	T_MAP m_mapDataRecycle;		//!<	���������?

public:
	auto_size_with_recycle_container(void);
	virtual ~auto_size_with_recycle_container(void);


protected:
	BYTE*	GetAdvDataBySize(int nRealSize, int& nDataSize);
	void	RecycleData(int DataSize, BYTE* pData);
	int		GetDataSize(int nRealSize);
};


template <class T_MAP, class T_DATA>
auto_size_with_recycle_container<T_MAP, T_DATA>::auto_size_with_recycle_container(void)
{
}

template <class T_MAP, class T_DATA>
auto_size_with_recycle_container<T_MAP, T_DATA>::~auto_size_with_recycle_container(void)
{
	for(typename T_MAP::iterator iterMap = m_mapDataRecycle.begin(); iterMap != m_mapDataRecycle.end(); iterMap++)
	{
		T_DATA* pAdvData = (T_DATA*)iterMap->second;
		for(typename T_DATA::iterator iterList = pAdvData->begin(); iterList != pAdvData->end(); iterList++)
		{
		//	void* pData = (*iterList);
		//	SAFE_DELETE(pData);
			SAFE_DELETE((*iterList));
		}
		pAdvData->clear();
		SAFE_DELETE(pAdvData);
	}
	m_mapDataRecycle.clear();
}

template <class T_MAP, class T_DATA>
BYTE*	auto_size_with_recycle_container<T_MAP, T_DATA>::GetAdvDataBySize(int nRealSize, int& nDataSize)
{
	nDataSize = GetDataSize(nRealSize);
	if(nRealSize == 0)
		return NULL;

	typename T_MAP::iterator iter = m_mapDataRecycle.find(nDataSize);
	if(iter == m_mapDataRecycle.end())
	{
		return new BYTE[nDataSize];
	}
	else
	{
		T_DATA* list = iter->second;
		if(list->size() > 0)
		{
			BYTE* pData = list->back();
			list->pop_back();
			return pData;
		}
		else
			return new BYTE[nDataSize];
	}
}

template <class T_MAP, class T_DATA>
void	auto_size_with_recycle_container<T_MAP, T_DATA>::RecycleData(int nDataSize, BYTE* pData)
{
	typename T_MAP::iterator iter = m_mapDataRecycle.find(nDataSize);
	if(iter != m_mapDataRecycle.end())
	{
		T_DATA* pList = iter->second;
		pList->push_back(pData);
	}
	else
	{
		T_DATA* pList = new T_DATA;
		pList->push_back(pData);
		m_mapDataRecycle.insert(make_pair(nDataSize, pList));
	}
}

template <class T_MAP, class T_DATA>
int		auto_size_with_recycle_container<T_MAP, T_DATA>::GetDataSize(int nRealSize)
{  
	if((DWORD)nRealSize > MAX_ADV_EVENT_DATA_SIZE || nRealSize <= 0)
		return 0;
	const DWORD MEM_SIZE_STEP = 1024;
	if((DWORD)nRealSize <= MEM_SIZE_STEP)
		return MEM_SIZE_STEP;
	else if((DWORD)nRealSize > 1024 && nRealSize <= MEM_SIZE_STEP * 2)
		return MEM_SIZE_STEP * 2;
	else if((DWORD)nRealSize > MEM_SIZE_STEP * 2 && nRealSize <= MEM_SIZE_STEP * 4)
		return MEM_SIZE_STEP * 4;
	else if((DWORD)nRealSize > MEM_SIZE_STEP * 4 && nRealSize <= MEM_SIZE_STEP * 8)
		return MEM_SIZE_STEP * 8;
	else if((DWORD)nRealSize > MEM_SIZE_STEP * 8 && nRealSize <= MEM_SIZE_STEP * 16)
		return MEM_SIZE_STEP * 16;
	else if((DWORD)nRealSize > MEM_SIZE_STEP * 16 && nRealSize <= MEM_SIZE_STEP * 32)
		return MEM_SIZE_STEP * 32;
	else if((DWORD)nRealSize > MEM_SIZE_STEP * 32 && nRealSize <= MEM_SIZE_STEP * 64)
		return MEM_SIZE_STEP * 64;
	else if((DWORD)nRealSize > MEM_SIZE_STEP * 64 && nRealSize <= MEM_SIZE_STEP * 128)
		return MEM_SIZE_STEP * 128;
	else if((DWORD)nRealSize > MEM_SIZE_STEP * 128 && nRealSize <= MEM_SIZE_STEP * 256)
		return MEM_SIZE_STEP * 256;
	else if((DWORD)nRealSize > MEM_SIZE_STEP * 256 && nRealSize <= MAX_ADV_EVENT_DATA_SIZE)
		return MAX_ADV_EVENT_DATA_SIZE;
	else
		return 0;
}

}

}


#endif
