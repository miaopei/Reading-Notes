#include"port.h"
struct valibdata
{
	char name[1024];
	int line;

};

void init_norm(void **data)//初始化数据
{
	
	struct valibdata *mydata = malloc(sizeof(struct valibdata));
	memset(mydata, 0, sizeof(struct valibdata));
	*data = mydata;
}

void input_norm(void *data, unsigned char *buf, int effectivelength)//插入数据
{
	if (NULL == data)
	{
		return;
	}
	if (NULL == buf)
	{
		return;
	}
	struct valibdata* mydata = (struct valibdata*)data;
	strncpy(mydata->name, buf, effectivelength);
	mydata->line = effectivelength;

}
void output_norm(void *data, unsigned char *buf, int *effectivelength)//输出数据
{
	if (NULL == data)
	{
		return;
	}
	if (NULL == buf)
	{
		return;
	}
	
	if (NULL == effectivelength)
	{
		return;
	}
	struct valibdata *mydata = (struct valibdata *)data;
	strncpy(buf, mydata->name, mydata->line);
	*effectivelength = mydata->line;
	printf("%s\n", mydata->name);
	printf("%d", *effectivelength);

}
void colse_norm(void *data)//关闭接口
{
	if (data == NULL)
	{
		return;
	}
	free(data);
	data = NULL;


}