// sylva-dynamic.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "sylva-dynamic.h"


// ���ǵ���������һ��ʾ��
SYLVADYNAMIC_API int nsylvadynamic=0;

// ���ǵ���������һ��ʾ����
SYLVADYNAMIC_API int fnsylvadynamic(void)
{
    return 42;
}

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� sylva-dynamic.h
Csylvadynamic::Csylvadynamic()
{
    return;
}
