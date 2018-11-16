#include <tools/Argument.h>
#include <tools/Debug.h>

#include <map>

namespace HAJC
{
    std::map <std::string, Argument*>* optionTable = nullptr;
    std::map <std::string, Argument*>& GetOptionTable()
    {
        if(optionTable == nullptr)
        {
            optionTable = new std::map <std::string, Argument*>();
        }

        return *optionTable;
    }

    Argument::Argument(char* Key, char* Desc)
        : aiKey(Key), aiDescription(Desc)
    {
        GetOptionTable().insert(std::make_pair(Key, this));
    }

    bool ExtractKVFromArgument(std::string& key, std::string& val, const std::string& argument)
    {
        std::string& scope = key;
        for(char c : argument)
        {
            if(c == '=')
            {
                scope = val;
            }

            scope += c;
        }

        if(argument.empty() || key.empty() || val.empty())
        {
            return false;
        }

        return true;
    }

    bool Argument::Init(std::vector<std::string>& arguments)
    {
        for (std::string& argument : arguments)
        {
            auto& table = GetOptionTable();

            std::string key = "";
            std::string val = "";
            if(!ExtractKVFromArgument(key, val, argument) || table.find(key) == table.end())
            {
                // TODO : emit warning for unknown or empty argument.
                continue;
            }

            Argument* info = table[argument];
            if(ArgumentB* arg = dynamic_cast<ArgumentB*>(info))
            {
                arg->Set(val == "1" || val == "true");
            }
            else if(ArgumentI* arg = dynamic_cast<ArgumentI*>(info))
            {
                arg->Set(std::atoi(val.c_str()));
            }
            else if(ArgumentD* arg = dynamic_cast<ArgumentD*>(info))
            {
                arg->Set(std::atof(val.c_str()));
            }
            else if(ArgumentS* arg = dynamic_cast<ArgumentS*>(info))
            {
                arg->Set(val);   
            }

            // unhandled argument type.
            return false;
        }

        return true;
    }

    void Argument::Show()
    {
        for(auto [Key, Val] : *optionTable)
        {
            // TODO : Format key - value info table and print.
        }
    }

} // namespace HAJC