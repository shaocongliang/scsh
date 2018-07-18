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
    int fd;
    int attr;
    pid_t pid;
    char **argv = nullptr;
    std::string compath;
    if (Node == nullptr) {
        return;
    }
    switch (Node->type_) {
        case TPAR:
        case TELF:
            attr = Node->attribute_;
            pid = 0;
            if ((attr & FPAR) == 0) {
                pid = fork();
            }
            if (pid == -1)
                exit(-1);
            if (pid > 0) {  // 父进程
                if (pout) close(*pout);
                if (pin) close(*pin);
                if (attr & FPRS) printf("[%d]\n", pid);
                if (attr & FAND)
                    return;
                int ret = waitpid(pid, NULL, 0);
                if (ret < 0)
                    printf("error\n");
                return;
            }
            if (Node->right_) {
                if (attr & FCAT) {
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
            if ((attr & FINT) && !(Node->left_) && !(attr & FPIN)) {
                fd = open("/dev/null", 0);
                dup2(fd, STDIN_FILENO);
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

            if(Node->sub_){
                ASTree *n = Node->sub_;
                Exec(n, pin, pout);
                return;
            }
            FindComFullPath(Node->cmds_[0], compath);
            if (compath.empty() == true) {
                printf("no such file or directory\n");
                return;
            }
            Node->cmds_[0] = compath;
            argv = detail::StringVector2PoniterArray(Node->cmds_.begin(), Node->cmds_.end());
            execv(compath.c_str(), argv);
            return;
        case TLIST:
            Exec(Node->left_, nullptr, nullptr);
            Exec(Node->right_, nullptr, nullptr);
            return;
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
            return;
    }

}
