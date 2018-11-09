#ifndef __HAJC_DATATYPES_STRING_H__
#define __HAJC_DATATYPES_STRING_H__

#include <string>
#include <Tools/Memory.h>

namespace HAJC
{
    template <class StrTy>
    struct StringHolder
    {
        StrTy* shBuffer;
        size_t shBufferLength;
        size_t shBufferBytes;

        bool empty() const
        {
            return shBufferLength == 0;
        }

        void clear()
        {
            FastZeroFill(shBuffer, shBufferBytes);
        }
    };

    using AStringHolder = typename StringHolder<char>;
    using WStringHolder = typename StringHolder<wchar_t>;

    template <class StrTy, size_t SmallLen>
    class String : public StringHolder<typename StrTy>, public IPooled
    {
        StrTy SmallBuf[SmallLen];

        size_t GetStringLength(const StrTy* buffer)
        {
            StrTy zero = 0;
            
            unsigned int length = 0;
            for(;buffer[length] != zero; ++length);

            return length;
        }

        void CopyInit(const StrTy* buffer, size_t length, size_t bytes)
        {
            memcpy(shBuffer, buffer, sizeof(StrTy) * length + 1);
            shBufferLength = length;
            shBufferBytes  = bytes;
        }

        void MoveInit(const StrTy* buffer, size_t length, size_t bytes)
        {
            shBuffer       = (StrTy*)buffer;
            shBufferLength = length;
            shBufferBytes  = bytes;
        }

        // default move initializer
        explicit String(const StrTy* buffer, size_t length, size_t bytes)
        {
            MoveInit(buffer, length, bytes);
        }
    
    public:
        // default initializer.
        String() : String(SmallBuf, 0, SmallLen) { }

        void resize(size_t newSize)
        {
            StrTy* newBuffer = new(mpBase) StrTy[newSize];

            if(!empty())
            {
                memcpy(newBuffer, shBuffer, sizeof(StrTy) * shBufferLength + 1);
            }

            std::swap(shBuffer, newBuffer);
            delete[] newBuffer;
        }

        // default copy initilaizer.
        String(const StrTy* buffer, size_t length) : String()
        {
            CopyInit(buffer, length, length);
        }

        // specalized copy initializer.
        String(const StrTy* buffer) 
            : String(buffer, GetStringLength(buffer)) { }
        String(const StringHolder<StrTy>& buffer) 
            : String(buffer.shBuffer, buffer.shBufferLength) { }
        String(const std::basic_string<StrTy>& buffer) 
            : String(buffer.c_str(), buffer.length()) { }

        // specalized move initializer.
        String(const StringHolder<StrTy>&& buffer) 
            : String(buffer.shBuffer, buffer.shBufferLength, buffer.shBufferBytes) { 
            buffer.clear();
        }
        String(const std::basic_string<StrTy>&& buffer)
            : String(buffer.c_str(), buffer.length(), buffer.size()) { 
            buffer.clear();
        }

        // specalized copy operator.
        String& operator=(const StringHolder<StrTy>& buffer)
        {
            CopyInit(buffer.shBuffer, buffer.shBufferBytes, buffer.shBufferLength);
            return *this;
        }

        String& operator=(const std::basic_string<StrTy>& buffer)
        {
            CopyInit(buffer.c_str(), buffer.length(), buffer.size());
            return *this;
        }
        
        String& operator=(StrTy* buffer)
        {
            size_t length = GetStringLength(buffer);
            CopyInit(buffer, length, length);
            return *this;
        }

        // specalized move operator.
        String& operator=(const StringHolder<StrTy>&& buffer)
        {
            MoveInit(buffer.shBuffer, buffer.shBufferBytes, buffer.shBufferLength);
            buffer.clear();
            return *this;
        }

        String& operator=(const std::basic_string<StrTy>&& buffer)
        {
            MoveInit(buffer.c_str(), buffer.length(), buffer.size());
            buffer.clear();
            return *this;
        }

        String& operator+=(const StrTy c)
        {
            if(shBufferLength == shBufferBytes)
            {
                resize(shBufferBytes + SmallLen);
            }

            shBuffer[shBufferLength++] = c;
            return *this;
        }
    };

    template <size_t SmallLen>
    using AString = typename String<char   , SmallLen>;
    template <size_t SmallLen>
    using WString = typename String<wchar_t, SmallLen>;
}

#endif
