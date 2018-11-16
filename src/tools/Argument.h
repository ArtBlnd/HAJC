#ifndef __HAJC_TOOLS_ARGUMENTS_H__
#define __HAJC_TOOLS_ARGUMENTS_H__

#include <string>
#include <vector>
#include <unordered_map>

namespace HAJC
{
    class Argument
    {
        char* aiKey;
        char* aiVal;
        char* aiDescription;

    protected:
        explicit Argument(char* Key, char* Desc);
        virtual ~Argument() = default;

    public:
        static bool Init(std::vector<std::string>& arguments);
        static void Show();
    };

    template <class Ty>
    class ArgumentInfo : public Argument
    {
        Ty value;

    public:
        explicit ArgumentInfo(char* Key, char* Desc) : Argument(Key, Desc), value(Ty()) { }
        virtual ~ArgumentInfo() = default;

        Ty Set(Ty newVal)
        {
            std::swap(value, newVal);
            return newVal;
        }

        Ty Get()
        {
            return value;
        }
    };

    typedef ArgumentInfo<bool>        ArgumentB;
    typedef ArgumentInfo<int>         ArgumentI;
    typedef ArgumentInfo<double>      ArgumentD;
    typedef ArgumentInfo<std::string> ArgumentS;

    

} // namespace HAJC

#define DEFINE_ARGUMENT_S(name, key, desc) HAJC::ArgumentS name(#key, desc)
#define DEFINE_ARGUMENT_I(name, key, desc) HAJC::ArgumentI name(#key, desc)
#define DEFINE_ARGUMENT_F(name, key, desc) HAJC::ArgumentD name(#key, desc)
#define DEFINE_ARGUMENT_S(name, key, desc) HAJC::ArgumentS name(#key, desc)

#endif