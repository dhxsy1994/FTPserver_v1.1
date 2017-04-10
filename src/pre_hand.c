#include "fun.h"

int pre_hand()
{
	int ret;//预处理切换目录
	ret = chdir("usr");
	if(-1 == ret)
	{
		perror("chdir");
		return -1;
	}
}
