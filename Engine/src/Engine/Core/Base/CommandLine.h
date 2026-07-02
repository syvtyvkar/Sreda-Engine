// (c) Nikita Rogalev. All rights reserved.

#pragma once // Preprocessor directive for multiple inclusion guard

#include <string>
#include <map>
#include <unordered_map>
#include <string>

namespace Engine
{
    class CommandLine
    {
        CommandLine() = default;
        ~CommandLine() = default;

    public:

        void Parse(int argc, char **argv)
        {
            m_args.clear();
            for (int i =1; i < argc; ++i)
            {   
                std::string arg(argv[i]);
                if (arg._Starts_with("=="))
                {
                    auto eq = arg.find("=");
                    if (eq != std::string::npos)
                    {
                        m_args[arg.substr(2,eq - 2)] = arg.substr(eq + 1);
                    }else
                    {
                        m_args[arg.substr(2)] = "true";
                    }
                }
            }
        }

        std::string Get(const std::string& InKey, const std::string& InDef = "") const
        {
            auto it = m_args.find(InKey);
            return it != m_args.end() ? it->second : InDef;
        }

        bool Has(const std::string& InKey) const
        {
            return m_args.find(InKey) != m_args.end();
        }
    private:
        std::unordered_map<std::string, std::string> m_args;
    };
}