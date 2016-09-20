#include "mysock.h"

int main(int argc, char ** argv)
{
	int ret;

	ret = creatServer(atoi(argv[1]));

	return 0;
}
