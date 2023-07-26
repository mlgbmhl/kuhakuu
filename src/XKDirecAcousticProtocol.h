#ifndef XKDIRECACOUSTICPROTOCOL_H
#define XKDIRECACOUSTICPROTOCOL_H

/********************************************************************
** ��XKQS���������豸�������ͨ��Э��v1.0��
** ��λ�����������豸ͨ�ŵĿ���������������ֻ������ϲ����
** ��λ�������������豸������λ�������

** ͨ�ŷ�ʽ��(1) ��λ������λ��ͨ��10M/100M/1000M��̫�����ӣ�
            (2) ����TCP/IPͨ�ţ��Զ���ͨ��Э�飻
			(3) Լ����λ��Ϊ�ͻ��ˣ���λ��Ϊ�������ˣ�
			(4) ������Ĭ��IP��ַ192.168.1.101��Ĭ�϶˿�10800;
			(5) IP���˿��ڲ���׶Σ����Ը����������·��䡣

** �ֽ��򣺶��ֽڴ������С���ֽ��򣬼����ֽ���ǰ�����ֽ��ں�

** Э���ʽ������֡����֡����18�ֽڣ������Խṹ����ʽ�ṩ��
            ��Э���ṩ20��ָ��ֱ�Ϊ��
			01. ���ܣ�ǿ�������������ǿ��
			02. ���ܣ��������������������
			03. ���ܣ�������Ƶ�ļ�������ָ�������Ƶ�ļ�
			04. ���ܣ�ֹͣ������Ƶ������ֹͣ���ţ�������

			05. ǿ�⣺��ʾ��1Hz��˸
			06. ǿ�⣺ֱ�䣬����
			07. ǿ�⣺������8Hz��˸
			08. ǿ�⣺�ر�

			09. ת̨����ʼ��ת
			10. ת̨����ʼ��ת
			11. ת̨����ʼ��ת
			12. ת̨����ʼ��ת
			13. ת̨����ʼ����ת
			14. ת̨����ʼ����ת
			15. ת̨����ʼ����ת
			16. ת̨����ʼ����ת
			17. ת̨��ת��ָ��λ��
			18. ת̨��ֹͣ�˶�

			19. �������豸��װ�㾭γ�ȡ�����
			20. �������豸��е��λ�뱱���ƫ��

** ��1����������˵����������ƽ̨�ֽ������������������滻Ϊ��Ӧϵͳ���ֽ�����ȵ���������
        unsigned short��2�ֽ��޷�����
		int           ��4�ֽ��з�����
		char          ��1�ֽ��з�����
*********************************************************************/

#include <string.h>
#include <math.h>

//ָ����1�ֽڶ���
#pragma pack(push, 1)

/********************����Ԫ�ض��壺�û����ù�ע******************/
static const double XKDAPFrame_UnitLon = 180.0 / pow(2.0, 31);
static const double XKDAPFrame_UnitLat = 180.0 / pow(2.0, 31);
static const double XKDAPFrame_UnitAlt = 0.01;
static const double XKDAPFrame_UnitAngle = 0.01;

struct XKDAPFrame
{
	//Լ��֡ͷ��֡β��ָ֡�����ֵ
	enum XKDAPCodeValue
	{
		//֡ͷ֡β
		FrameHead            = 0x7e7e,    //֡ͷ
		FrameTail            = 0xeded,    //֡β
		
		//����
		CodeAcousticStrong   = 0x0101,    //����ǿ���
		CodeAcousticWeak     = 0x0102,    //���������
		CodeAcousticPlay     = 0x0103,    //������Ƶ�ļ�
		CodeAcousticStop     = 0x0104,    //����ֹͣ����
		
		//ǿ��
		CodeLampWarning      = 0x0201,    //��ʾ
		CodeLampBeat         = 0x0202,    //ֱ��
		CodeLampBurstFlash   = 0x0203,    //����
		CodeLampStop         = 0x0204,    //�ر�
		
		//ת̨
		CodeRotaryUp         = 0x0301,    //��ת
		CodeRotaryDown       = 0x0302,    //��ת
		CodeRotaryLeft       = 0x0303,    //��ת
		CodeRotaryRight      = 0x0304,    //��ת
		CodeRotaryLeftUp     = 0x0305,    //����ת
		CodeRotaryLeftDown   = 0x0306,    //����ת
		CodeRotaryRightUp    = 0x0307,    //����ת
		CodeRotaryRightDown  = 0x0308,    //����ת
		CodeRotaryLocation   = 0x0309,    //ת��ָ��λ��
		CodeRotaryStop       = 0x030A,    //ֹͣ�˶�

		//�豸����
		CodeDevWGS           = 0x0A01,    //�����豸��װλ�õľ�γ�ȡ�����
		CodeDevDriftAngle    = 0x0A02,    //�豸��װ���ڻ�е��λʱ���豸�����뱱���ƫ��
	};					     

	static double ToLon(int value)
	{
		return value * XKDAPFrame_UnitLon;
	}

	static double ToLat(int value)
	{
		return value * XKDAPFrame_UnitLat;
	}

	static double ToAlt(int value)
	{
		return value * XKDAPFrame_UnitAlt;
	}

	static double ToAngle(int value)
	{
		return value * XKDAPFrame_UnitAngle;
	}
};

/***********************��������Ԫ�ض���************************/


//����ǿ�������������ǿ��
struct XKCmdAcousticStrong
{
	unsigned short mHead;
	unsigned short mCode;
	char           mRes[12];
	unsigned short mTail;

	XKCmdAcousticStrong()
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAPFrame::FrameHead;
		mCode = (unsigned short)XKDAPFrame::CodeAcousticStrong;
		mTail = (unsigned short)XKDAPFrame::FrameTail;
	}
};

//�����������������������
struct XKCmdAcousticWeak
{
	unsigned short mHead;
	unsigned short mCode;
	char           mRes[12];
	unsigned short mTail;

	XKCmdAcousticWeak()
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAPFrame::FrameHead;
		mCode = (unsigned short)XKDAPFrame::CodeAcousticWeak;
		mTail = (unsigned short)XKDAPFrame::FrameTail;
	}
};


//���ܣ�����ָ����ŵ���Ƶ��������Ƶ�ļ���ţ���0��ʼ��Ĭ��0
struct XKCmdAcousticPlay
{
	unsigned short mHead;
	unsigned short mCode;
	unsigned short mFileOrderNo;
	char           mRes[10];
	unsigned short mTail;

	XKCmdAcousticPlay(unsigned short fileOrderNo = 0)
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAPFrame::FrameHead;
		mCode = (unsigned short)XKDAPFrame::CodeAcousticPlay;
		mFileOrderNo = fileOrderNo;
		mTail = (unsigned short)XKDAPFrame::FrameTail;
	}
};

//���ܣ�ֹͣ���ţ�����
struct XKCmdAcousticStop
{
	unsigned short mHead;
	unsigned short mCode;
	char           mRes[12];
	unsigned short mTail;

	XKCmdAcousticStop()
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAPFrame::FrameHead;
		mCode = (unsigned short)XKDAPFrame::CodeAcousticStop;
		mTail = (unsigned short)XKDAPFrame::FrameTail;
	}
};



//ǿ�⣺��ʾ
struct XKCmdLampWarning
{
	unsigned short mHead;
	unsigned short mCode;
	char           mRes[12];
	unsigned short mTail;

	XKCmdLampWarning()
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAPFrame::FrameHead;
		mCode = (unsigned short)XKDAPFrame::CodeLampWarning;
		mTail = (unsigned short)XKDAPFrame::FrameTail;
	}
};

//ǿ�⣺ֱ��
struct XKCmdLampBeat
{
	unsigned short mHead;
	unsigned short mCode;
	char           mRes[12];
	unsigned short mTail;

	XKCmdLampBeat()
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAPFrame::FrameHead;
		mCode = (unsigned short)XKDAPFrame::CodeLampBeat;
		mTail = (unsigned short)XKDAPFrame::FrameTail;
	}
};

//ǿ�⣺����
struct XKCmdLampBurstFlash
{
	unsigned short mHead;
	unsigned short mCode;
	char           mRes[12];
	unsigned short mTail;

	XKCmdLampBurstFlash()
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAPFrame::FrameHead;
		mCode = (unsigned short)XKDAPFrame::CodeLampBurstFlash;
		mTail = (unsigned short)XKDAPFrame::FrameTail;
	}
};

//ǿ�⣺�ر�
struct XKCmdLampStop
{
	unsigned short mHead;
	unsigned short mCode;
	char           mRes[12];
	unsigned short mTail;

	XKCmdLampStop()
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAPFrame::FrameHead;
		mCode = (unsigned short)XKDAPFrame::CodeLampStop;
		mTail = (unsigned short)XKDAPFrame::FrameTail;
	}
};



//ת̨����ת
struct XKCmdRotaryUp
{
	unsigned short mHead;
	unsigned short mCode;
	char           mRes[12];
	unsigned short mTail;

	XKCmdRotaryUp()
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAPFrame::FrameHead;
		mCode = (unsigned short)XKDAPFrame::CodeRotaryUp;
		mTail = (unsigned short)XKDAPFrame::FrameTail;
	}
};

//ת̨����ת
struct XKCmdRotaryDown
{
	unsigned short mHead;
	unsigned short mCode;
	char           mRes[12];
	unsigned short mTail;

	XKCmdRotaryDown()
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAPFrame::FrameHead;
		mCode = (unsigned short)XKDAPFrame::CodeRotaryDown;
		mTail = (unsigned short)XKDAPFrame::FrameTail;
	}
};

//ת̨����ת
struct XKCmdRotaryLeft
{
	unsigned short mHead;
	unsigned short mCode;
	char           mRes[12];
	unsigned short mTail;

	XKCmdRotaryLeft()
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAPFrame::FrameHead;
		mCode = (unsigned short)XKDAPFrame::CodeRotaryLeft;
		mTail = (unsigned short)XKDAPFrame::FrameTail;
	}
};

//ת̨����ת
struct XKCmdRotaryRight
{
	unsigned short mHead;
	unsigned short mCode;
	char           mRes[12];
	unsigned short mTail;

	XKCmdRotaryRight()
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAPFrame::FrameHead;
		mCode = (unsigned short)XKDAPFrame::CodeRotaryRight;
		mTail = (unsigned short)XKDAPFrame::FrameTail;
	}
};

//ת̨������ת
struct XKCmdRotaryLeftUp
{
	unsigned short mHead;
	unsigned short mCode;
	char           mRes[12];
	unsigned short mTail;

	XKCmdRotaryLeftUp()
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAPFrame::FrameHead;
		mCode = (unsigned short)XKDAPFrame::CodeRotaryLeftUp;
		mTail = (unsigned short)XKDAPFrame::FrameTail;
	}
};

//ת̨������ת
struct XKCmdRotaryLeftDown
{
	unsigned short mHead;
	unsigned short mCode;
	char           mRes[12];
	unsigned short mTail;

	XKCmdRotaryLeftDown()
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAPFrame::FrameHead;
		mCode = (unsigned short)XKDAPFrame::CodeRotaryLeftDown;
		mTail = (unsigned short)XKDAPFrame::FrameTail;
	}
};

//ת̨������ת
struct XKCmdRotaryRightUp
{
	unsigned short mHead;
	unsigned short mCode;
	char           mRes[12];
	unsigned short mTail;

	XKCmdRotaryRightUp()
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAPFrame::FrameHead;
		mCode = (unsigned short)XKDAPFrame::CodeRotaryRightUp;
		mTail = (unsigned short)XKDAPFrame::FrameTail;
	}
};

//ת̨������ת
struct XKCmdRotaryRightDown
{
	unsigned short mHead;
	unsigned short mCode;
	char           mRes[12];
	unsigned short mTail;

	XKCmdRotaryRightDown()
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAPFrame::FrameHead;
		mCode = (unsigned short)XKDAPFrame::CodeRotaryRightDown;
		mTail = (unsigned short)XKDAPFrame::FrameTail;
	}
};

//ת̨��ת��ָ��λ�ã���γ�ȣ��㣩�����Σ��ף�
struct XKCmdRotaryLocation
{
	unsigned short mHead;
	unsigned short mCode;
	int            mLon;
	int            mLat;
	int            mAlt;
	unsigned short mTail;

	//ֱ�Ӵ���WGS84��γ�ȼ�����
	XKCmdRotaryLocation(double lon, double lat, double alt = 0.0)
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAPFrame::FrameHead;
		mCode = (unsigned short)XKDAPFrame::CodeRotaryLocation;
		mLon  = (int)(lon / XKDAPFrame_UnitLon);
		mLat  = (int)(lat / XKDAPFrame_UnitLat);
		mAlt  = (int)(alt / XKDAPFrame_UnitAlt);
		mTail = (unsigned short)XKDAPFrame::FrameTail;
	}
};

//ת̨��ֹͣ�˶�
struct XKCmdRotaryStop
{
	unsigned short mHead;
	unsigned short mCode;
	char           mRes[12];
	unsigned short mTail;

	XKCmdRotaryStop()
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAPFrame::FrameHead;
		mCode = (unsigned short)XKDAPFrame::CodeRotaryStop;
		mTail = (unsigned short)XKDAPFrame::FrameTail;
	}
};


//�豸�������豸��װλ�þ�γ�ȣ��㣩�����Σ��ף�
struct XKCmdDevWGS
{
	unsigned short mHead;
	unsigned short mCode;
	int            mLon;
	int            mLat;
	int            mAlt;
	unsigned short mTail;

	//ֱ�Ӵ���WGS84��γ�ȼ�����
	XKCmdDevWGS(double lon, double lat, double alt = 0.0)
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAPFrame::FrameHead;
		mCode = (unsigned short)XKDAPFrame::CodeDevWGS;
		mLon = (int)(lon / XKDAPFrame_UnitLon);
		mLat = (int)(lat / XKDAPFrame_UnitLat);
		mAlt = (int)(alt / XKDAPFrame_UnitAlt);
		mTail = (unsigned short)XKDAPFrame::FrameTail;
	}
};

//�豸���豸��װ���ڻ�е��λʱ���豸�����뱱���ƫ��
//�ϸ��壺�������豸�����ƫ�ǣ���˳ʱ������ȡֵ[0.00, 360.0)
struct XKCmdDevDriftAngle
{
	unsigned short mHead;
	unsigned short mCode;
	int            mAzi;
	char           mRes[8];
	unsigned short mTail;

	//azi���豸��е��λʱ�������뱱���ƫ�ǣ�ȡֵ[0.00, 360.0)
	XKCmdDevDriftAngle(double azi)
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAPFrame::FrameHead;
		mCode = (unsigned short)XKDAPFrame::CodeDevDriftAngle;
		mAzi  = (int)(azi / XKDAPFrame_UnitAngle);
		mTail = (unsigned short)XKDAPFrame::FrameTail;
	}
};

//���1�ֽڶ��룬�ָ�Ĭ�϶���
#pragma pack(pop)


#endif // !XKDIRECACOUSTICPROTOCOL_H


