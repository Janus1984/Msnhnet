#ifndef MSNHMEMORYMANAGER_H
#define MSNHMEMORYMANAGER_H
#include "Msnhnet/config/MsnhnetCfg.h"
#include <stddef.h>

namespace Msnhnet
{

class MemoryManager
{
public:
    static bool memAlign;

    // 这里static是必须的，否则编译会报错。why？
    template<typename _Tp> 
    static inline _Tp* alignPtr(_Tp* ptr, int n=(int)sizeof(_Tp))
    {
        return (_Tp*)(((size_t)ptr + n-1) & -n);
    }

    // 对应ncnn中的fastMalloc
    // 为什么要用模板来写？因为ncnn中的malloc，返回void指针，而这里用的new，返回T类型的指针
    template<typename T>
    static T* effcientNew(size_t size)
    {
        if(memAlign)
        {
            uint8_t* udata = new uint8_t[size*sizeof(T) + sizeof(void*) + EFFCIENT_ALIGN]();
            uint8_t** adata = alignPtr((uint8_t**)udata + 1, EFFCIENT_ALIGN);
            adata[-1] = udata;
            return (T*)adata;
        }
        else
        {
            T* adata = new T[size]();
            return adata;
        }

    }

    // 对应ncnn中的fastfree
    template<typename T>
    static void effcientDelete(T* ptr)
    {
        if(ptr)
        {
            if(memAlign)
            {
                uint8_t* udata = ((uint8_t**)ptr)[-1];
                if(udata>=(uint8_t*)ptr || ((uint8_t*)ptr - udata) > (ptrdiff_t)(sizeof(void*)+EFFCIENT_ALIGN))
                {
                    throw Exception(1,"Memory error",__FILE__,__LINE__,__FUNCTION__);
                }
                delete[] udata;
                udata = nullptr;
            }
            else
            {
                delete[] ptr;
                ptr = nullptr;
            }
        }
    }
};

}
#endif 

