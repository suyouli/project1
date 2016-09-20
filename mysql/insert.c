//插入
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
	STU *head=NULL;
	STU *p;
	mysql_init(&db);//调用mysql_init()初始化MYSQL结构
	pdb=mysql_real_connect(&db,"localhost","root","12345","china",0,NULL,0);//mysql_real_connect()与运行在主机上的MYSQL数据库引擎建立连接
							//MYSQL结构地址|主机名或IP地址|用户名|密码|数据库名称|不是0时用TCP/IP端口号|
							//返回值：失败NULL，成功:&db
	if(pdb==NULL)
	{
		printf("mysql connect failed\n");
	}
	else
		printf("mysql connect success\n");
	while(1)
	{
		p=(STU*)malloc(sizeof(STU));
		assert(p!=NULL);//assert宏的原型定义在<assert.h>中，起作用是如果它的条件返回错误，则终止程序执行，作用是计算里面表达式，
						//如果其值为假（0）,那么它先向stderr打印一条出错信息，然后通过abort终止程序运行
		printf("请输入学生的学号,姓名,性别,年龄,电话:\n");
		scanf("%d%s%s%d%s",&p->ID,p->name,p->sex,&p->age,p->tel);
		if(p->ID==0)
			break;
		p->next=head;
		head=p;
	}//头插
	/*把链表里的信息写到sql中 */
	p=head;
	while(p!=NULL)
	{
		sprintf(sql,"insert into students values(%d,\'%s\',\'%s\',%d,\'%s\')",p->ID,p->name,p->sex,p->age,p->tel);
		//sprintf把格式化的数据写到字符串sql,即打印到字符串sql中
		if(mysql_query(&db,sql))//数据库查询，成功返回0
		{
			printf("insert failed\n");
			return -1;
		}
		p=p->next;
	}
	mysql_close(&db);
	return 0;
}
