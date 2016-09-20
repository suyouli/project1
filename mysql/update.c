//修改
#include <stdio.h>
#include <mysql/mysql.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
typedef struct student
{
	int ID;
	char name[20];
	char sex[10];
	int age;
	char tel[20];
	struct student *next;
}STU;
int main()
{
	char sql[1024];
	MYSQL db,*pdb;//MYSQL结构
	STU st;
	mysql_init(&db);//调用mysql_init()初始化MYSQL结构
	pdb=mysql_real_connect(&db,"localhost","root","12345","china",0,NULL,0);//mysql_real_connect()与运行在主机上的MYSQL数据库引擎建立连接
							//MYSQL结构地址|主机名或IP地址|用户名|密码|数据库名称|不是0时用TCP/IP端口号|
							//返回值：失败NULL，成功:&db
	if(pdb==NULL)
	{
		printf("mysql connect failed\n");
	}
	else
	{
		printf("mysql connect success\n");
	}
	printf("请输入要修改的学号,姓名,性别,年龄,电话\n");
	scanf("%d%s%s%d%s",&st.ID,st.name,st.sex,&st.age,st.tel);
	sprintf(sql,"update students set name=\'%s\',sex=\'%s\',age=%d,tel=\'%s\'where ID=%d;",st.name,st.sex,st.age,st.tel,st.ID);
		//sprintf把格式化的数据写到字符串sql,即打印到字符串sql中
	if(mysql_query(&db,sql))//数据库查询，成功返回0
	{
		printf("updata failed\n");
		return -2;
	}
	mysql_close(&db);
	return 0;
}
