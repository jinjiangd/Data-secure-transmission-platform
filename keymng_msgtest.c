
#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "keymng_msg.h"



void main()
{
	int				ret = 0;
	Teacher			t1;

	unsigned char	*out = NULL;
	int				outlen = 0;

	Teacher			*pOutStruct = NULL;
	int				iTmpTag = 0;

	memset(&t1, 0, sizeof(Teacher));
	t1.age = 32;
	strcpy(t1.name, "myname");
	t1.p = (char *)malloc(100);
	strcpy(t1.p, "ppppppppp");
	t1.plen = strlen(t1.p);

	//������ʦ
	ret = MsgEncode(&t1, ID_MsgType_Teacher, &out, &outlen);
	if (ret != 0)
	{
		printf("func MsgEncode() err:%d \n", ret);
		return ret;
	}
		
	//
	ret= MsgDecode( out, outlen, &pOutStruct, &iTmpTag);
	if (ret != 0)
	{
		printf("func MsgDecode() err:%d \n", ret);
		return ret;
	}


	MsgMemFree(&out, 0);  //1 �ͷ��ڴ� 2 ����Ұָ��  Ҫ��ָ����ָ����ڴ�ռ��ͷŵ�;ͬʱ��ʵ���޸�NULLL
	MsgMemFree(&pOutStruct, iTmpTag);

	system("pause");
}
