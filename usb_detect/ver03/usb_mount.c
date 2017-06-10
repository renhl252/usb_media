#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <unistd.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>

void MountFS(char *devName, char *basePath)
{
	 //int access(const char *filename, int mode);
	int ret = access(basePath,F_OK);
	if (ret != 0)
	{
		umount2(basePath, MNT_FORCE);
		//int mkdir(const char *pathname, mode_t mode);
		ret=mkdir(basePath, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
		if (0!=ret)
		{
			printf("mkdir() error = [%d]%s\n",errno,strerror(errno));
		}
	}
	
	ret = mount(devName, basePath, "vfat", S_WRITE, "iocharset=utf8");
	if (0!=ret)
	{
		printf("mount() error = [%d]%s\n",errno,strerror(errno));
		return ;
	}
}

void umountFS(char *basePath)
{
	int ret = access(basePath,F_OK);
	if (ret != 0)
	{
		printf("[%s] is not existence!\n",basePath);
		return ;
	}
	ret = umount2(basePath, MNT_FORCE);
	if (0!=ret)
	{
		printf("umount2() error = [%d]%s\n",errno,strerror(errno));
		return ;
	}
	//int rmdir(const char *pathname);
	ret = rmdir(basePath);
	if (0!=ret)
	{
		printf("rmdir()  error = [%d]%s\n",errno,strerror(errno));
		return ;
	}
}

