#ifndef __HAJC_TOOLS_ARGUMENTS_H__
#define __HAJC_TOOLS_ARGUMENTS_H__

#include <string>
#include <vector>
#include <unordered_map>

namespace HAJC
{

    class ArgumentVisitor
    {
        friend struct ArgumentTable;
        std::string* Value;

    public:
        int         AsInt();    // Get argument value as int.
        double      AsDouble(); // Get argument value as double.
        bool        AsBool();   // Get argument value as bool.
        std::string AsString(); // Get argument value as string.

        bool IsExists();

    protected:
        explicit ArgumentVisitor(std::string* value);
    };

    struct ArgumentTable
    {
        std::unordered_map<std::string, std::string> Table;

        void InitArgument(const std::string&& Key, const std::string&& Val);

        ArgumentVisitor operator[](std::string key)
        {
            if(Table.find(key) == Table.end())
            {
                return ArgumentVisitor(nullptr);
            }

            return ArgumentVisitor(&Table[key]);
        }
    };

    void ArgumentParseCallback(std::vector<std::string>& arguments);
    ArgumentTable& GetArgumentTable();
} // namespace HAJC

#endif