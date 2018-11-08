#ifndef __HAJC_DATA_TYPES_NODE_BASE_H__
#define __HAJC_DATA_TYPES_NODE_BASE_H__

namespace HAJC
{
    template <class Ty>
    struct SLRawNode
    {
        Ty* nodeNext;
    };


    template <class Ty>
    struct DLRawNode
    {
        Ty* nodeNext;
        Ty* nodePrev;
    };

    template <class Ty>
    struct SLNode
    {
        Ty node;
        SLNode<Ty>* nodeNext;
    };

    template <class Ty>
    struct DLNode
    {
        Ty node;
        DLNode<Ty>* nodeNext;
        DLNode<Ty>* nodePrev;
    };
}

#endif