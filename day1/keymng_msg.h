#ifndef _KEYMNG_MSG_H_
#define _KEYMNG_MSG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define		KeyMng_ParamErr			200		//�������ʧ��
#define		KeyMng_TypeErr			201		//��������ʧ��
#define		KeyMng_MallocErr		202		//�����ڴ�ʧ��

#define		KeyMng_NEWorUPDATE		1		//1 ��ԿЭ�� 
#define		KeyMng_Check			2		//2 ��ԿУ��
#define		KeyMng_Revoke			3		//3 ��Կע��
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


