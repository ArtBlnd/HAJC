#ifndef __HAJC_TOOLS_HOST_H__
#define __HAJC_TOOLS_HOST_H__

#include <datatypes/NodeBase.h>
#include <tools/Utils.h>

// NOTE : The all functions that using platform API should have noexcept keyword

namespace HAJC
{

    enum HostPageFlags : uint64_t
    {
        HPF_REERVED   = 1 << 0,
        HPF_COMMIT    = 1 << 1,
        HPF_EXECUTE   = 1 << 2,
        HPF_READ      = 1 << 3,
        HPF_WRITE     = 1 << 4,
        HPF_NO_ACCESS = 1 << 5,
    };

    // This contains basic page infomations.
    // and support for OS level page allocations, deallocations, modifications. etc.
    struct HostPage : public IFlag<HostPageFlags>, public SLRawNode<HostPage>
    {
        HostPage*    base     = nullptr;
        void*        page     = nullptr;
        unsigned int pageSize = 0;
        unsigned int pageUsed = 0; 

        static size_t GetPageSize();
        static bool Create(HostPage* page, HostPage* base, unsigned int pages, uint64_t flags);
        static bool Delete(HostPage* page);
        static bool Modify(HostPage* page, uint64_t flags);
    };


}

#endif