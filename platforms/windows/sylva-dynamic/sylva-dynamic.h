// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� SYLVADYNAMIC_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// SYLVADYNAMIC_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef SYLVADYNAMIC_EXPORTS
#define SYLVADYNAMIC_API __declspec(dllexport)
#else
#define SYLVADYNAMIC_API __declspec(dllimport)
#endif

// �����Ǵ� sylva-dynamic.dll ������
class SYLVADYNAMIC_API Csylvadynamic {
public:
	Csylvadynamic(void);
	// TODO:  �ڴ�������ķ�����
};

extern SYLVADYNAMIC_API int nsylvadynamic;

SYLVADYNAMIC_API int fnsylvadynamic(void);
