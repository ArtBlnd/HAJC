#include <tools/Argument.h>
#include <tools/Debug.h>
#include <tools/Singleton.h>

namespace HAJC
{
    Singleton<ArgumentTable> argumentTable;

    Argument::Argument(char* Key, char* Desc)
        : aiKey(Key), aiDescription(Desc)
    {
        argumentTable->insert(std::make_pair(Key, this));
    }

    void ExtractKVFromArgument(std::string& key, std::string& val, const std::string& argument)
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
    }

    bool Argument::Init(std::vector<std::string>& arguments)
    {
        for (std::string& argument : arguments)
        {
            auto& table = *argumentTable;

            std::string key = "";
            std::string val = "";
            ExtractKVFromArgument(key, val, argument);

            if(table.find(key) == table.end())
            {
                // TODO : emit warning for unknown or empty argument.
                continue;
            }

            Argument* info = table[argument];
            if(ArgumentB* arg = dynamic_cast<ArgumentB*>(info))
            {
                if(val.empty())
                {
                    arg->Set(true);
                }
                else
                {
                    arg->Set(val == "1" || val == "true");
                }
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

    unsigned int GetLongestKeyLength()
    {
        unsigned int length = 0;
        for(auto [Key, Val] : *argumentTable)
        {
            if(length < Key.length())
            {
                length = Key.length();
            }
        }

        return length;
    }

    void InsertSpace(unsigned int spaces)
    {
        for(unsigned int i = 0; i < spaces; ++i)
        {
            std::cout << ' ';
        }
    }

    void Argument::Show()
    {
        const unsigned int maxKeyLength = GetLongestKeyLength();

        for(auto [Key, Val] : *argumentTable)
        {
            printf(" --%s", Key.c_str());
            InsertSpace(maxKeyLength - Key.length());
            printf(" : %s\n", Val->aiDescription);
        }
    }

} // namespace HAJC