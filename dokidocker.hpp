#include <sys/wait.h>  //waitpid
#include <sys/mount.h>  //mount
#include <fcntl.h>  //open
#include <unistd.h> //execv,sethostname ,chroot,fchdir
#include <sched.h>  //clone
#include <string>
#include <cstring>
#include <csignal>
#include <cstdlib>
#include <iostream>

#define STACK_SIZE (1024*1024)

using namespace std;

namespace dokidocker
{
    //docker config
    typedef struct docker_config
    {
        string host_name; //主机名
        string rootfs_dir; //rootfs路径
    }docker_config;

    //process starus(error,exit,waited)
    typedef int proc_status;
    proc_status proc_err = -1;
    proc_status proc_exit = 0;
    proc_status proc_wait = 1;

    typedef int proc_pid;

    class docker
    {
    private:

        docker_config config; //docker config
        char child_stack[STACK_SIZE]; //child process stack size

        //set docker host name
        void set_hostname()
        {
            sethostname(this->config.host_name.c_str(),this->config.host_name.length());
        }

        //set rootfs dir
        void set_rootfs_dir()
        {
            chdir(this->config.rootfs_dir.c_str());
            chroot(".");
        }

        //mount proc file system
        void mount_procfs()
        {
            mount("none","/proc","proc",0, nullptr);
            mount("none","/sys","sysfs",0, nullptr);
        }

        void set_network()
        {
            system("ip link set lo up");
            system("ip link set dev veth1 name eth0");
            system("ip addr add 10.0.0.2/24 dev eth0");
            system("ip link set eth0 up");
        }

        //set bash when dokidocker start
        void start_bash()
        {
            // convert c++ std::string to c char * (safe)
            string bash = "/bin/bash";
            char *c_bash = new char[bash.size()+1]; //多一个字节储存'\0'

            strcpy(c_bash,bash.c_str());
            char* const child_args[] = {c_bash,NULL};
            execv(child_args[0],child_args);
            delete []c_bash;
            c_bash=nullptr;
        }

    public:
        docker(docker_config &d_config):config(d_config) //构建函数
        {}

        //set docker config before start this dokidocker
        static int set_config(void *args)
        {
            auto _this= static_cast<docker*>(args);
            _this->set_hostname();
            _this->set_rootfs_dir();
            _this->mount_procfs();
            _this->set_network();
            _this->start_bash();

            return 1;
        }
        void start()
        {
            cout << "Welcome to DokiDocker"<<endl;
            //create new process by clone function
            proc_pid child_proc = clone(set_config,child_stack+STACK_SIZE,
                                       CLONE_NEWUTS|
                                       CLONE_NEWNS|
                                       CLONE_NEWIPC|
                                       CLONE_NEWPID|
                                       CLONE_NEWNET|
                                       SIGCHLD,
                                       this);

            char *cmd;
            asprintf(&cmd,"ip link set veth1 netns %d",child_proc);
            system("ip link add veth0 type veth peer name veth1");
            system(cmd);
            system("ip link set veth0 up");
            system("ip addr add 10.0.0.1/24 dev veth0");
            free(cmd);

            waitpid(child_proc, nullptr,0);
            cout << "GoodBye~" <<endl;
        }
    };
}
