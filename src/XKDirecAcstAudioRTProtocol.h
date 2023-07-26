#ifndef XKDIRECACSTAUDIORTPROTOCOL_H
#define XKDIRECACSTAUDIORTPROTOCOL_H

/********************************************************************
** ��XKQS���������豸ʵʱ��Ƶ����ͨ��Э��v1.0��
** ��λ�����������豸ͨ�ŵĿ���������������ֻ������ϲ����
** ��λ�������������豸������λ�������

** ͨ�ŷ�ʽ��(1) ��λ������λ��ͨ��10M/100M/1000M��̫�����ӣ�
			(2) ����TCP/IPͨ�ţ��Զ���ͨ��Э�飻
			(3) Լ����λ��Ϊ�ͻ��ˣ���λ��Ϊ�������ˣ�
			(4) ������Ĭ��IP��ַ192.168.1.101��Ĭ�϶˿�10802;
			(5) IP���˿��ڲ���׶Σ����Ը����������·��䡣

** �ֽ��򣺶��ֽڴ������С���ֽ��򣬼����ֽ���ǰ�����ֽ��ں�

** Э���ʽ������֡����֡����1032�ֽڣ������Խṹ����ʽ�ṩ��
			��Э���ṩ1��ָ�
			01. ���У�ʵʱ��Ƶ�����·�

** ��1����������˵����������ƽ̨�ֽ������������������滻Ϊ��Ӧϵͳ���ֽ�����ȵ���������
		unsigned short��2�ֽ��޷�����
		char          ��1�ֽ��з�����

** ��2����Ƶ��ʽ������
        ������8000����ͨ������������16�ֽڡ�����PCM�����������з������͡��ֽ���ΪС���ֽ���

** ��3������˵��
        ��λ��ʵʱ�ɼ�PCM��Ƶ�����ݣ��ɼ�������֮������ͨ����Э�齫��Ƶ�����·�����λ����
		��һ�βɼ��������ݳ��ȴ���1024���������֡���伴�ɡ�

** ��4����λ������ʵʱ����ʱ������ֹͣ��Ƶ�ļ����š�

** ��5��Э���ʽ˵����Ϊ�β����������߱䳤֡���ģ������ö���֡���ģ�
					��ͨ��Э�飬����˫�������Ѷȡ�
*********************************************************************/

#include <string.h>

//ָ����1�ֽڶ���
#pragma pack(push, 1)

/***********************����Ԫ�ض��壺�û����ù�ע********************/
struct XKDAAudioRTPFrame
{
	//Լ��֡ͷ��֡β��ָ֡�����ֵ
	enum XKDAAudioRTPPCodeValue
	{
		//֡ͷ֡β
		FrameHead = 0x7c7c,    //֡ͷ
		FrameTail = 0xeded,    //֡β

		CodeAudioSend = 0x0801,    //�·�ʵʱ��Ƶ����
	};
};
/***************************��������Ԫ�ض���*************************/


//ʵʱ��Ƶ�����·�
struct XKCmdAudioRT
{
	unsigned short mHead;
	unsigned short mCode;
	unsigned short mLenValid;
	char           mData[1024];
	unsigned short mTail;

	XKCmdAudioRT(const char *data, int size)
	{
		mHead = (unsigned short)XKDAAudioRTPFrame::FrameHead;
		mCode = (unsigned short)XKDAAudioRTPFrame::CodeAudioSend;
		mLenValid = (unsigned short)size;
		memcpy(&mData[0], data, size);
		mTail = (unsigned short)XKDAAudioRTPFrame::FrameTail;
	}
};

//���1�ֽڶ��룬�ָ�Ĭ�϶���
#pragma pack(pop)

#endif // !XKDIRECACSTAUDIORTPROTOCOL_H

