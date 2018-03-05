/*************************************************************************
 > File Name: doit.c
 > Author: huangkaibo
 > Mail: huangkaibochn@gmail.com
 > Created Time: Fri 24 Nov 2017 03:17:53 PM CST
 ************************************************************************/

#include<stdio.h>
#include<sys/time.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<time.h>
#include<sys/resource.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

void main(int argc, char* argv[])
{
	if(argc<2) 
	{
		printf("请输入命令\n");
		exit(1);
	}

	//获取命令
	//comm是execv的第一个参数
	//arg是execv的第二个参数
	char comm[1024];
	char* arg[argc];
	strcpy(comm, argv[1]);
	arg[0] = argv[1];
	for(int i = 2; i < argc; ++i)
	{
		arg[i-1] = argv[i];
	}
	arg[argc-1] = NULL;

	printf("命令%s的输出如下\n", comm);

	struct timeval start, end;
	gettimeofday(&start, NULL);
	//子进程调用命令
	if(fork()==0)
	{
		//execv()不会从环境变量读取命令, 必须输入命令所在位置
		//execvp()可以从环境变量读取命令
		execvp(comm, arg);
		return;
	}
	
	//等待子进程运行完毕
	int status;
	wait(&status);
	gettimeofday(&end, NULL);

	//父进程获取子进程资源消耗情况
	struct rusage usage;
	getrusage(RUSAGE_CHILDREN, &usage);
	printf("\n命令的执行情况如下\n");
	printf("真实时间: %.3f 毫秒\n系统态时间: %.3f 毫秒\n用户态时间: %.3f 毫秒\n", (end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000.0, usage.ru_utime.tv_usec/1000.0+usage.ru_utime.tv_sec*1000.0, usage.ru_stime.tv_usec/1000.0+usage.ru_stime.tv_sec*1000.0);
	printf("被抢占次数: %ld\n主动放弃CPU次数: %ld\n缺页次数: %ld\n", usage.ru_nivcsw, usage.ru_nvcsw, usage.ru_minflt+usage.ru_majflt);
}

