
#include <string>
#include <memory>
#include <unistd.h>
#include <stdio.h>
#include <cstring>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <net/if.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <cstdio>

const char* SRC_ROOT = "/tmp/build";
const char* DST_ROOT = "/opt/redpitaya";

// FILE* logFile;
void signalHandlerStrong( int signum )
{
}

void signalHandlerDefault( int signum )
{
    exit(0);
}


void copyFile(const char* _src, const char* _dst)
{
    using namespace std;
    ifstream source(_src, ios::binary);
    ofstream dest(_dst, ios::binary);

    dest << source.rdbuf();

    source.close();
    dest.close();
}

void createDir(const char* dir)
{
    struct stat st = {0};
    if (stat(dir, &st) == -1) {
        mkdir(dir, 0777);
    }
}

void listdir(const char *root, const char *d_name, int level)
{
    DIR *dir;
    struct dirent *entry;

    char name[1024];
    int len = snprintf(name, sizeof(name)-1, "%s%s", root, d_name);
    name[len] = 0;

    if (!(dir = opendir(name)))
        return;
    if (!(entry = readdir(dir)))
        return;
    do {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            char path[1024];
            int len = snprintf(path, sizeof(path)-1, "%s/%s", d_name, entry->d_name);
            path[len] = 0;

            // printf("%s/%s\n", level*2, name, entry->d_name);
            std::string dir(DST_ROOT);
            dir = dir + path;
            createDir(dir.c_str());
            listdir(root, path, level + 1);
        }
        else
        {
            std::string from(name);
            from = from + "/"+ entry->d_name;
            std::string to(DST_ROOT);
            to = to + d_name + "/" + entry->d_name;
            // printf("%s/%s\n", d_name, entry->d_name);
            printf("%s -> %s\n", from.c_str(), to.c_str());
            copyFile(from.c_str(), to.c_str());
        }
    } while ((entry = readdir(dir)));
    closedir(dir);
}

void StartDaemon()
{
    pid_t pid;

    /* Fork off the parent process */
    pid = fork();

    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* On success: The child process becomes session leader */
    if (setsid() < 0)
        exit(EXIT_FAILURE);

    /* Close all open file descriptors */
    int x;
    for (x = sysconf(_SC_OPEN_MAX); x>0; x--)
    {
        close (x);
    }
    /* Open the log file */
}


int main(int argc, char *argv[])
{


    /* Catch, ignore and handle signals */
    //TODO: Implement a working signal handler */

    StartDaemon();
    signal(SIGCHLD, signalHandlerStrong);
    signal(SIGHUP, signalHandlerStrong);
    signal(SIGABRT, signalHandlerStrong);
    signal(SIGFPE, signalHandlerStrong);
    signal(SIGILL, signalHandlerStrong);
    signal(SIGINT, signalHandlerStrong);
    signal(SIGSEGV, signalHandlerStrong);
    signal(SIGTERM, signalHandlerStrong);
    signal(SIGUSR1, signalHandlerStrong);
    signal(SIGUSR2, signalHandlerStrong);

    // logFile = fopen ("/root/daemon.log","a");
    //fprintf (logFile, "----------------------------------------------------\n"); fflush(logFile);
    //fprintf (logFile, "[StartDaemon] Daemon have started\n"); fflush(logFile);
	// sleep(1);
 //    /*int res = 0;*/
	/*res = */system("killall nginx");
 //    //fprintf (logFile, "> nohup killall nginx : %d\n", res); fflush(logFile);
 //    sleep(1);
	/*res = */system("mount -o rw,remount /opt/redpitaya");

    /*res = */system("killall hostapd");
 //    //fprintf (logFile, "> nohup killall hostapd : %d\n", res); fflush(logFile);
 //    sleep(1);

 //    //fprintf (logFile, "> nohup mount -o rw,remount /opt/redpitaya : %d\n", res); fflush(logFile);
 //    sleep(1);

	createDir(DST_ROOT);
    listdir(SRC_ROOT, "",0);
 //    //fprintf (logFile, "> nohup /bin/cp -fvr /tmp/build/* /opt/redpitaya/ : %d\n", res); fflush(logFile);
 //    sleep(17);

	/*res = */system("nohup reboot & disown");
 //    //fprintf (logFile, "> nohup reboot : %d\n", res); fflush(logFile);
 //    sleep(1);
    system("reboot");
    signal(SIGCHLD, signalHandlerDefault);
    signal(SIGHUP, signalHandlerDefault);
    signal(SIGABRT, signalHandlerDefault);
    signal(SIGFPE, signalHandlerDefault);
    signal(SIGILL, signalHandlerDefault);
    signal(SIGINT, signalHandlerDefault);
    signal(SIGSEGV, signalHandlerDefault);
    signal(SIGTERM, signalHandlerDefault);
    signal(SIGUSR1, signalHandlerDefault);
    signal(SIGUSR2, signalHandlerDefault);
    system("nohup reboot & disown");

    // fclose(logFile);
    return 0;
}
