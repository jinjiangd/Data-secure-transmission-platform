#ifndef _KEYMNG_MSG_H_
#define _KEYMNG_MSG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define		KeyMng_ParamErr			200		//输入参数失败
#define		KeyMng_TypeErr			201		//输入类型失败
#define		KeyMng_MallocErr		202		//分配内存失败

#define		KeyMng_NEWorUPDATE		1		//1 密钥协商 
#define		KeyMng_Check			2		//2 密钥校验
#define		KeyMng_Revoke			3		//3 密钥注销
		 	; 	
#define  ID_MsgKey_Req  60


#define ID_MsgType_Teacher  50

typedef struct _Teacher
{
	char	name[64];
	int		age ;
	char	*p;
	int		plen;
}Teacher;

#define ID_MsgType_Student  51
typedef struct _Student
{
	char	name[64];
	int		age ;
	char	*p;
	int		plen;
}Student;


int MsgEncode(
	void			*pStruct , /*in*/
	int				type,
	unsigned char	**outData, /*out*/
	int				*outLen );

int MsgDecode( 
	unsigned char *inData,/*in*/
	int           inLen,
	void          **pStruct /*out*/,
	int           *type /*out*/);

int MsgMemFree(void **point,int type);




#ifdef __cplusplus
}
#endif

#endif


