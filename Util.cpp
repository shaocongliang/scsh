/**
 * @author amshaocong@outlook.com
 * @date 2018-06-23
 */
#include <Util.h>
#include <iostream>
#include <glob.h>
#include <unistd.h>

using namespace std;

const char *slash("/");

void SplitString(const string &in, vector<string> &tokens, const std::string &delim)
{
    std::string::size_type pos1, pos2;
    pos2 = in.find(delim);
    pos1 = 0;
    while(std::string::npos != pos2)
    {
        tokens.push_back(in.substr(pos1, pos2-pos1));

        pos1 = pos2 + delim.size();
        pos2 = in.find(delim, pos1);
    }
    if(pos1 != in.length())
        tokens.push_back(in.substr(pos1));
}

bool IsAny(const char c, const char *s)
{
    while (*s)
    {
        if (c == *s++)
        {
            return true;
        }
    }
    return false;
}

namespace detail{
    void findComFullPathImpl(const std::string &path, const std::string &com, std::string &fullpath)
    {
        std::string searchPath;
        searchPath.append(path).append(slash).append(com);
        glob_t gt;
        int ret = glob(searchPath.c_str(), GLOB_TILDE, NULL, &gt);
        if(ret)
        {
            return;
        }
        for(char **p = gt.gl_pathv; *p != nullptr; ++p)
        {
            fullpath.assign(*p, strlen(*p));
        }
    }
}

void FindComFullPath(const std::string &com, std::string &fullpath)
{
   vector<string> paths;
   char *pres = nullptr;
   pres = getenv("PATH");
   SplitString(pres, paths, ":");
   for(auto it : paths)
   {
       detail::findComFullPathImpl(it, com, fullpath);
       if(fullpath.empty() == false)
       {
           return;
       }
   }
   return;
}

void PutChar(const char *prompt)
{
    write(STDOUT_FILENO, prompt, strlen(prompt));
}
void PrintTokens(const std::vector<std::string> &tokens){
    for(auto it : tokens) {
        std::cout << it << " ";
    }
    std::cout << std::endl;
}
void InorderPrintAST(const ASTree *node)
{
    if(node == nullptr) {
        return;
    }
    InorderPrintAST(node->left_);
    for(std::size_t i = 0; i < node->cmds_.size(); ++i) {
        cout << node->cmds_[i] << " ";
    }
    cout << endl;
    InorderPrintAST(node->right_);
}
