/**
 * @author amshaocong@outlook.com
 * @date 2018-06-23
 */
#include <Exec.h>
#include <AST.h>
#include <Util.h>

#include <unistd.h>

const int MAXARGLEN = 20;

namespace detail {
    /**
     * @param iter1 begin
     * @param iter2  end-of-past
     * @return nullptr if iter1 == iter2
     */
    char **StringVector2PoniterArray(ConstIterator iter1, ConstIterator iter2) {
        if (iter1 == iter2) {
            return nullptr;
        }
        char **argv = new char *[MAXARGLEN];
        char **pargv = argv;
        int index = 0;
        for (; iter1 != iter2; ++iter1) {
            argv[index] = new char[1024];
            memset(argv[index], 0, sizeof(1024));
            strcat(argv[index], (*iter1).c_str());
            index++;
        }
        argv[index] = nullptr;
        return pargv;
    }
}

void Exec(ASTree *Node, int *pin, int *pout) {
    pid_t pid;
    std::string compath;
    if (Node == nullptr) {
        return;
    }
    switch (Node->type_) {
        case ELF:
            FindComFullPath(Node->cmds_[0], compath);
            if (compath.empty() == true) {
                printf("no such file or directory\n");
                return;
            } else {
                Node->cmds_[0] = compath;
                char **argv = detail::StringVector2PoniterArray(Node->cmds_.begin(), Node->cmds_.end());
                pid = fork();
                if (pid == 0) {
                    if (pout) {
                        dup2(*pout, STDOUT_FILENO);
                    } else if (pin) {
                        if (dup2(*pin, STDIN_FILENO) != STDIN_FILENO) {
                            printf("dup2 error to stdin\n");
                        }
                    }
                    execv(compath.c_str(), argv);
                    if (pout) { close(*pout); }
                    if (pin) { close(*pin); }
                } else if (pid > 0) {
                    int ret = waitpid(pid, NULL, 0);
                    if (ret < 0) {
                        printf("wait pid error with ret %d\n", ret);
                    }
                }
            }
            break;
        case COMSEQ:
            Exec(Node->left_, nullptr, nullptr);
            Exec(Node->right_, nullptr, nullptr);
            break;
        case PIPE:
            int pfd[2];
            pipe(pfd);
            ASTree *left = Node->left_;
            Exec(left, NULL, &pfd[1]);
            ASTree *right = Node->right_;
            Exec(right, &pfd[0], NULL);
            break;
    }
}