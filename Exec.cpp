/**
 * @author amshaocong@outlook.com
 * @date 2018-06-23
 */
#include <Exec.h>
#include <AST.h>
#include <Util.h>

#include <unistd.h>
#include <fcntl.h>

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
} // namespace detail

void Exec(ASTree *Node, int *pin, int *pout) {
    pid_t pid;
    int fd;
    std::string compath;
    if (Node == nullptr) {
        return;
    }
    switch (Node->type_) {
        case TELF:
            FindComFullPath(Node->cmds_[0], compath);
            if (compath.empty() == true) {
                printf("no such file or directory\n");
                return;
            } else {
                Node->cmds_[0] = compath;
                char **argv = detail::StringVector2PoniterArray(Node->cmds_.begin(), Node->cmds_.end());
                pid = fork();
                if (pid == 0) {
                    if (Node->right_) {
                        if (Node->attribute_ & FCAT) {
                            fd = open(Node->right_->cmds_[0].c_str(), 1);
                            if (fd >= 0) {
                                lseek(fd, 0, SEEK_END);
                                goto f1;
                            }
                        }
                        fd = creat(Node->right_->cmds_[0].c_str(), 0666);
                        if (fd < 0) {
                            printf("can't create\n");
                            exit(-1);
                        }
                        f1:
                        dup2(fd, STDOUT_FILENO);
                        close(fd);
                    }
                    if (pout) {
                        close(1);
                        dup(*pout);
                    }
                    if (pin) {
                        close(0);
                        dup(*pin);
                    }
                    if (pout) close(*pout);
                    if (pin) close(*pin);
                    execv(compath.c_str(), argv);
                } else if (pid > 0) {
                    if (pout) close(*pout);
                    if (pin) close(*pin);
                    int ret = waitpid(pid, NULL, 0);
                    if (ret < 0)
                        printf("error\n");
                }
            }
            break;
        case TLIST:
            Exec(Node->left_, nullptr, nullptr);
            Exec(Node->right_, nullptr, nullptr);
            break;
        case TPIPE:
            int pfd[2];
            pipe(pfd);
            int attribute = Node->attribute_;
            Node->left_->attribute_ |= FPOUT | (attribute & (FPIN));
            ASTree *left = Node->left_;
            Exec(left, pin, &pfd[1]);
            Node->right_->attribute_ |= FPIN | (attribute & (FPOUT));
            ASTree *right = Node->right_;
            Exec(right, &pfd[0], pout);
            break;
    }

}
