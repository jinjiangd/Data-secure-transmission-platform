
#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "itcast_asn1_der.h"

/*
typedef struct ITCAST_ANYBUF_{
	unsigned char *pData;
	ITCAST_UINT32     dataLen;
	ITCAST_UINT32     unusedBits;  
	ITCAST_UINT32     memoryType;
	ITCAST_UINT32     dataType;
	struct ITCAST_ANYBUF_ *next;   
	struct ITCAST_ANYBUF_ *prev;
}ITCAST_ANYBUF;

ITCAST_INT
DER_ItAsn1_ReadPrintableString(ITASN1_PRINTABLESTRING *pDerPrintString,ITASN1_PRINTABLESTRING **ppPrintString);
*/
//DER原理
typedef struct _Teacher
{
	char	name[64];
	int		age ;
	char	*p;
	int		plen;
}Teacher;


//4k
int TeacherEncode2( Teacher *pStruct /*in*/, unsigned char *out /*in */, int *outlen)
{
	;
}
//DER TLV
// 
//编码 老师结构体
int TeacherEncode( Teacher *pStruct /*in*/, unsigned char **out /*out*/, int *outlen)
{
	int					ret = 0;
	ITCAST_ANYBUF		*pTmp=NULL, *pHeadBuf=NULL;
	ITCAST_ANYBUF		*pTmpBuf = NULL;
	ITCAST_ANYBUF		*pOutData = NULL;

	unsigned char		*tmpOut = NULL;
	int					tmpOutLen = 0;

	//需要 把C语言的char	name[64];====>ITCAST_ANYBUF 数据类型
	ret = DER_ITCAST_String_To_AnyBuf(&pTmp, pStruct->name, strlen(pStruct->name));
	if (ret != 0)
	{
		printf("func DER_ITCAST_String_To_AnyBuf() err:%d \n", ret);
		return ret;
	} 
	ret = DER_ItAsn1_WritePrintableString(pTmp, &pHeadBuf);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pTmp);
		printf("func DER_ItAsn1_ReadPrintableString() err:%d \n", ret);
		return ret;
	}
	DER_ITCAST_FreeQueue(pTmp); //释放内存 避免内存泄漏
	pTmp = pHeadBuf; //保证头结点pHeadBuf不变

	//编码age
	ret = DER_ItAsn1_WriteInteger(pStruct->age, &(pTmp->next) );
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadBuf); //
		printf("func DER_ItAsn1_WriteIntegerDER_ItAsn1_WriteInteger() err:%d \n", ret);
		return ret;
	}
	pTmp = pTmp->next;

	//编码p 编码指针做指向的内存空间的数据
	ret =  EncodeChar(pStruct->p, pStruct->plen, &(pTmp->next));
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadBuf); //
		printf("func EncodeChar() err:%d \n", ret);
		return ret;
	}
	pTmp = pTmp->next;

	//编码plen
	ret = DER_ItAsn1_WriteInteger(pStruct->plen, &(pTmp->next) );
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadBuf); //
		printf("func DER_ItAsn1_WriteInteger() err:%d \n", ret);
		return ret;
	}

	//整体编码结构体
	ret = DER_ItAsn1_WriteSequence(pHeadBuf, &pOutData);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadBuf); //
		printf("func DER_ItAsn1_WriteSequence() err:%d \n", ret);
		return ret;
	}
	DER_ITCAST_FreeQueue(pHeadBuf); //释放所有的malloc的业务结点

	// 
	tmpOutLen = pOutData->dataLen;
	tmpOut = (unsigned char *)malloc(tmpOutLen);
	if (tmpOut == NULL)
	{
		ret = -1;
		printf("func malloc() err:%d \n");
		return ret;
	}
	memcpy(tmpOut, pOutData->pData, tmpOutLen);

	//间接赋值
	*out =   tmpOut;  //*p放在等号的左边  :向指针做指向的控件中写数据
	
	*outlen = tmpOutLen;

	DER_ITCAST_FreeQueue(pOutData);

	return ret;
}

//典型的内存打包 
//老师结构体 解码
int TeacherDecode(unsigned char *out, int outlen, Teacher **pStruct /*out*/)
{
	int					ret = 0;
	ITCAST_ANYBUF		*pTmp=NULL, *pHead=NULL;
	ITCAST_ANYBUF		*pTmpDABuf = NULL;
	ITCAST_ANYBUF		*pOutData = NULL;
	ITCAST_ANYBUF		*inAnyBuf = NULL;
	int					tmpNum  = 0;
	Teacher				*pTmpStru = NULL;

	//组一个anybuf
	inAnyBuf = (ITCAST_ANYBUF *)malloc(sizeof(ITCAST_ANYBUF));
	if (inAnyBuf == NULL)
	{
		ret = -1;
		printf("func malloc() err:%d \n", ret);
		return ret;
	}
	memset(inAnyBuf, 0, sizeof(ITCAST_ANYBUF)); //特别注意

	inAnyBuf->dataLen = outlen;
	inAnyBuf->pData = (unsigned char *)malloc(outlen);
	if (inAnyBuf->pData == NULL)
	{
		DER_ITCAST_FreeQueue(inAnyBuf);
		ret = -2;
		printf("func malloc() err:%d \n", ret);
		return ret;
	}
	memcpy(inAnyBuf->pData, out, outlen);

	//分配老师结点
	pTmpStru = (Teacher *)malloc(sizeof(Teacher));
	if (pTmpStru == NULL)
	{
		DER_ITCAST_FreeQueue(inAnyBuf);
		ret = -3;
		printf("func malloc() err:%d  malloc Teacher err\n", ret);
		return ret;
	}
	memset(pTmpStru, 0, sizeof(Teacher));

	ret = DER_ItAsn1_ReadSequence(inAnyBuf, &pHead);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(inAnyBuf);
		free(pTmpStru);
		printf("func DER_ItAsn1_ReadSequence() err:%d \n", ret);
		return ret;
	}
	pTmp = pHead; //相当于两个辅助指针变量 同时指向链表的头部

	DER_ITCAST_FreeQueue(inAnyBuf); //释放内存  避免内存泄漏

	//解码name
	ret = DER_ItAsn1_ReadPrintableString(pTmp, &pTmpDABuf);
	if (ret != 0)
	{
		printf("func DER_ItAsn1_ReadPrintableString() err:%d \n", ret);
		return ret;
	}
	memcpy(pTmpStru->name, pTmpDABuf->pData, pTmpDABuf->dataLen);
	pTmp = pTmp->next;
	DER_ITCAST_FreeQueue(pTmpDABuf); //释放结点


	//解码age
	ret = DER_ItAsn1_ReadInteger(pTmp, &(pTmpStru->age) ) ;
	if (ret != 0)
	{
		printf("func DER_ItAsn1_ReadInteger() err:%d \n", ret);
		return ret;
	}
	pTmp = pTmp->next;

	//解码p
	ret = DER_ItAsn1_ReadPrintableString(pTmp, &pTmpDABuf);
	if (ret != 0)
	{
		printf("func DER_ItAsn1_ReadPrintableString() err:%d \n", ret);
		return ret;
	}
	pTmpStru->p = malloc(pTmpDABuf->dataLen + 1);
	if (pTmpStru->p == NULL)
	{
		DER_ITCAST_FreeQueue(pTmpDABuf); //释放结点
		ret = -4;
		printf("func malloc() err:%d  malloc pTmpStru->p err\n", ret);
		return ret;
	}
	memset(pTmpStru->p , 0, pTmpDABuf->dataLen + 1);
	memcpy(pTmpStru->p,  pTmpDABuf->pData, pTmpDABuf->dataLen);
	pTmp = pTmp->next;
	DER_ITCAST_FreeQueue(pTmpDABuf); //释放结点

	//解码plen
	ret = DER_ItAsn1_ReadInteger(pTmp, &(pTmpStru->plen) ) ;
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pTmpDABuf); //释放结点
		printf("func DER_ItAsn1_ReadInteger() err:%d \n", ret);
		return ret;
	}

	DER_ITCAST_FreeQueue(pHead); //释放结点 wbm modify 
	//*p实参的地址 去间接的修改实参的值
	*pStruct = pTmpStru; 
	return ret;
}

int TeacherFree(Teacher *pStruct)
{
	if (pStruct == NULL)
	{
		return -1;
	}

	if (pStruct->p)
	{
		free(pStruct->p);
	}
	free(pStruct);
	return 0;
}

int mywritefile(unsigned char *buf, int len)
{
	FILE *fp = NULL;
	fp = fopen("c:/teacher.ber","wb+");	
	if(fp == NULL)
	{
		printf("fopen file error \n");
		return -1;
	}	
	fwrite(buf, 1, len, fp);	
	fclose(fp);
	return 0;
}

int main01()
{
	int				ret = 0;
	Teacher			t1;

	unsigned char	*out = NULL;
	int				outlen = 0;

	Teacher			*pOutStruct = NULL;

	memset(&t1, 0, sizeof(Teacher));
	t1.age = 32;
	strcpy(t1.name, "myname");
	t1.p = (char *)malloc(100);
	strcpy(t1.p, "ppppppppp");
	t1.plen = strlen(t1.p);

	//编码
	ret = TeacherEncode(&t1 , &out, &outlen);
	if (ret != 0)
	{
		printf("func TeacherEncode() err:%d \n", ret);
		return ret;
	}

	{
		/*
		char mybuf[1024];
		//编码
		ret = TeacherEncode2(&t1 , mybuf, &outlen);
		if (ret != 0)
		{
			printf("func TeacherEncode() err:%d \n", ret);
			return ret;
		}
		*/
	
	}
	
	mywritefile(out,outlen);

	//解码
	ret = TeacherDecode(out, outlen, &pOutStruct);
	if (ret != 0)
	{
		printf("func TeacherEncode() err:%d \n", ret);
		return ret;
	}

	if (  (t1.age ==  pOutStruct->age &&
			strcmp(t1.name, pOutStruct->name)==0) )
	{
		printf("编码和解码结果一样 ok\n");
	}
	else
	{
		printf("编码和解码 err\n");
	}

	TeacherFree(pOutStruct);

	if (t1.p != NULL) free(t1.p);
	
	printf("hello...\n");
	system("pause");
}
