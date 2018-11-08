#include <tools/Argument.h>
#include <tools/Debug.h>

namespace HAJC
{

    int ArgumentVisitor::AsInt()
    {
        noway_assert(!IsExists(), "Argument is not initialized!");
        return std::stoi(*Value);
    }

    double ArgumentVisitor::AsDouble()
    {
        noway_assert(!IsExists(), "Argument is not initialized!");
        return std::stod(*Value);
    }

    bool ArgumentVisitor::AsBool()
    {
        noway_assert(!IsExists(), "Argument is not initialized!");
        if(*Value == "true"  || *Value == "1")
        {
            return true;
        }

        if(*Value == "false" || *Value == "0" || Value->empty())
        {
            return false;
        }

        noway_assert(true, "This is not a bool type argument!!");
        return false;
    }

    std::string ArgumentVisitor::AsString()
    {
        return *Value;
    }

    bool ArgumentVisitor::IsExists()
    {
        return Value != nullptr;
    }

    ArgumentVisitor::ArgumentVisitor(std::string* value) : Value(value)
    {
        
    }

    ArgumentTable argumentTable;
    inline bool RemovePrefixIfArgument(std::string& token)
    {
        unsigned int prefixIndex = 0;
        for(char c : token)
        {
            if(c == '-')
            {
                prefixIndex++;
            }
            break;
        }

        token = std::string(&token[prefixIndex]);
        return (!!prefixIndex);
    }

    void ArgumentParseCallback(std::vector<std::string>& arguments)
    {
        CONTRACT_CALL_TRACE;

        // (-|--|)key_or_value([= ]value|)

        for(std::string& argument : arguments)
        {
            if(!RemovePrefixIfArgument(argument))
            {
                argument.clear();
                // TODO : Warning for wrong argument syntax.
            }
        }

        for(std::string& argument : arguments)
        {
            if(argument.empty())
            {
                // We've removed argument that has wrong syntax.
                // just skipping it.
                continue;
            }

            std::string Key = "";
            std::string Val = "";

            std::string& stringScope = Key;
            for(char c : argument)
            {
                if(c == '=')
                {
                    stringScope = Val;
                }

                stringScope += c;
            }
            argumentTable.InitArgument(std::move(Key), std::move(Val));
        }
    }

    ArgumentTable& GetArgumentTable()
    {
        return argumentTable;
    }

    inline void ArgumentTable::InitArgument(const std::string && Key, const std::string && Val)
    {
        Table.insert(std::make_pair(std::move(Key), std::move(Val)));
    }

} // namespace HAJC