#ifndef XKDIRECTACSTFILEPROTOCOL_H
#define XKDIRECTACSTFILEPROTOCOL_H

/********************************************************************
** ��XKQS���������豸�ļ�����ͨ��Э��v1.0��
** ��λ�����������豸ͨ�ŵĿ���������������ֻ������ϲ����
** ��λ�������������豸������λ�������
** ����ͨ�ţ���λ������λ����ͨ�ţ�����·�
** ����ͨ�ţ���λ������λ����ͨ�ţ�����ϱ�

** ͨ�ŷ�ʽ��(1) ��λ������λ��ͨ��10M/100M/1000M��̫�����ӣ�
			(2) ����TCP/IPͨ�ţ��Զ���ͨ��Э�飻
			(3) Լ����λ��Ϊ�ͻ��ˣ���λ��Ϊ�������ˣ�
			(4) ������Ĭ��IP��ַ192.168.1.101��Ĭ�϶˿�10801;
			(5) IP���˿��ڲ���׶Σ����Ը����������·��䡣

** �ֽ��򣺶��ֽڴ������С���ֽ��򣬼����ֽ���ǰ�����ֽ��ں�
** �ַ����룺���������ݰ��������ַ����������ַ�����UTF-8�����ʽ

** Э�鷽���ԣ�Э���Ϊ����Э�������Э��
             ����Э�飺��λ���·�����λ����Э�飬���ļ����䡢��ѯ��ɾ����
		     ����Э�飺��λ���ϱ�����λ����Э�飬���ļ��б��

** Э���ʽ������֡����֡����1032�ֽڣ������Խṹ����ʽ�ṩ��
			��Э���ṩ12��ָ��ֱ�Ϊ��
			01. ���У���ȡ��Ƶ�ļ�����
			02. ���У���ȡ��Ƶ�ļ����б�
			03. ���У�ɾ��ָ����Ƶ�ļ�
			04. ���У�׼���·���Ƶ�ļ��������ļ����ȡ��ļ���
			05. ���У��·��ļ�����
			06. ���У��ļ������·�����
			07. ���У�ȡ����Ƶ�ļ��·�

			08. ���У��ϱ���Ƶ�ļ��������յ� ��1.��ȡ��Ƶ�ļ������� ָ����ϱ���
			09. ���У��ϱ���Ƶ�ļ��б��յ� ��2.��ȡ��Ƶ�ļ��б� ָ��������ϱ���ÿ�������ϱ�һ���ļ�����
			10. ���У��ϱ�ɾ��ָ����Ƶ�ļ�������յ� ��3.ɾ��ָ����Ƶ�ļ��� ָ����ϱ�ɾ�������
			11. ���У��ϱ��ļ�������ȣ�ÿ�յ�һ֡�ļ����ݱ��ģ����ϴ�һ�ν��ս��ȣ��ѽ��յ������ֽ�����
			12. ���У��ϱ���Ƶ�ļ����������ļ�����ɹ�������ȡ������ʱ���Զ��ϱ���

** ��1����������˵����������ƽ̨�ֽ������������������滻Ϊ��Ӧϵͳ���ֽ�����ȵ���������
		unsigned short��2�ֽ��޷�����
		int           ��4�ֽ��з�����
		unsigned int  ��4�ֽ��޷�����
		char          ��1�ֽ��з�����

** ��2��֧�ֵ���Ƶ�ļ���ʽ
        ֧��.wav��.mp3���ָ�ʽ��

** ��3����Ƶ�ļ���������˵��
        04-07Ϊ��Ƶ�ļ��·����ָ�11Ϊ��Ƶ�ļ��������ϱ�
		������׼���·�����ɢ.mp3���ļ����ļ�����100KB����102400�ֽڡ�
		    1��ͨ��04ָ����ļ�����102400���ļ�������ɢ.mp3���·���
			2�����ļ�����ͨ��05ָ��ֶ���·���ֱ�������·���ɣ�
			    ��֧1���ļ���������У���ͨ��07ָ��ȡ������Ƶ�ļ��·�����λ��ͨ��11ָ��ȷ��ȡ��
			3���ļ������·���ɣ�����2��ɺ󣩣��·�06ָ���ָ֪���ļ�������Ƶ�ļ��������
			4����λ���յ�����ָ������ָ���ļ����գ����ɹ����ͨ��11ָ���֪��λ����
			   ��ʱ��δ���յ��µ��ļ��������ݣ���δ���յ�ȡ�������ָ���ͨ��11ָ���֪�ļ�����ʧ�ܣ���ʱ��
			�ر�˵��������1-3Ϊ�·�һ����Ƶ�ļ��ıջ����̣��м䲻�ò�����������

** ��4���ļ���������������ָ�ļ�������׺��ȫһ��
        �����·���Ƶ�ļ��������ļ�����������λ���Զ����ļ�����������ּ������֣�
		�磺��λ�����С�����.mp3����������1.mp3����
		    ��ʱ��λ�����·�һ��������.mp3�����ļ�����ᱻ�Զ�����Ϊ������2.mp3��

** ��5��Э���ʽ˵����Ϊ�β����������߱䳤֡���ģ������ö���֡���ģ�
                    ��ͨ��Э�飬����˫�������Ѷȡ�
*********************************************************************/

#include <string.h>

//ָ����1�ֽڶ���
#pragma pack(push, 1)

/***************************����Ԫ�ض���*****************************/
struct XKDAFilePFrame
{
	//Լ��֡ͷ��֡β��ָ֡�����ֵ
	enum XKDAFilePCodeValue
	{
		//֡ͷ֡β���û��������б���ʱ��Ҫ
		FrameHead = 0x7d7d,    //֡ͷ
		FrameTail = 0xeded,    //֡β

		//���У��û����ù�ע
		CodeGetCount      = 0x0601,  //��ȡ��Ƶ�ļ�����
		CodeGetNames      = 0x0602,  //��ȡ��Ƶ�ļ����б�
		CodeDeleteFile    = 0x0603,  //ɾ��ָ����Ƶ�ļ�
		CodeFileReady     = 0x0604,  //׼���·���Ƶ�ļ�
		CodeFileSend      = 0x0605,  //�·���Ƶ�ļ�����
		CodeFileEnd       = 0x0606,  //�ļ����ݷ������
		CodeFileCancel    = 0x0607,  //ȡ���ļ�����

		//���У��û��������б���ʱ��Ҫ
		CodeReportCount     = 0x06A1,  //�ϱ���Ƶ�ļ�����
		CodeReportNames     = 0x06A2,  //�ϱ���Ƶ�ļ����б�
		CodeDeleteResult    = 0x06A3,  //�ϱ���Ƶ�ļ�ɾ�����
		CodeReportReceived  = 0x06A4,  //�ϱ��ļ����ս���
		CodeFileSendResult  = 0x06A5,  //�ϱ���Ƶ�ļ�������
	};
};
/***************************��������Ԫ�ض���*************************/



//���У���ȡ��Ƶ�ļ�����
struct XKCmdGetFileCount
{
	unsigned short mHead;
	unsigned short mCode;
	char           mRes[1026];
	unsigned short mTail;

	XKCmdGetFileCount()
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAFilePFrame::FrameHead;
		mCode = (unsigned short)XKDAFilePFrame::CodeGetCount;
		mTail = (unsigned short)XKDAFilePFrame::FrameTail;
	}
};

//���У���ȡ��Ƶ�ļ����б�
struct XKCmdGetFileNames
{
	unsigned short mHead;
	unsigned short mCode;
	char           mRes[1026];
	unsigned short mTail;

	XKCmdGetFileNames()
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAFilePFrame::FrameHead;
		mCode = (unsigned short)XKDAFilePFrame::CodeGetNames;
		mTail = (unsigned short)XKDAFilePFrame::FrameTail;
	}
};

//���У�ɾ��ָ����Ƶ�ļ�
struct XKCmdDeleteFile
{
	unsigned short mHead;
	unsigned short mCode;
	unsigned short mOrderNumber;    //��Ƶ�ļ���ţ���0��ʼ
	char           mRes[1024];
	unsigned short mTail;

	//������Ҫɾ������Ƶ�ļ���ţ���0��ʼ���
	XKCmdDeleteFile(int fileOrderNumber)
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAFilePFrame::FrameHead;
		mCode = (unsigned short)XKDAFilePFrame::CodeDeleteFile;
		mOrderNumber = (unsigned short)fileOrderNumber;
		mTail = (unsigned short)XKDAFilePFrame::FrameTail;
	}
};


//���У�׼���·���Ƶ�ļ��������ļ����ȡ��ļ���
struct XKCmdFileReady
{
	unsigned short mHead;
	unsigned short mCode;
	unsigned int   mSizeFile;  //�ļ����ȣ��ֽ���
	unsigned short mLenName;   //�ļ������ȣ��ֽ���
	char           mName[1020];
	unsigned short mTail;

	//�����ļ����ȣ��ֽ��������ļ��� �� �ļ������ȣ��ֽ�����
	XKCmdFileReady(unsigned int fileSize, const char *fileName, int sizeName)
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAFilePFrame::FrameHead;
		mCode = (unsigned short)XKDAFilePFrame::CodeFileReady;
		mSizeFile = (unsigned int)fileSize;
		mLenName = (unsigned short)sizeName;
		memcpy(&mName[0], fileName, sizeName);
		mTail = (unsigned short)XKDAFilePFrame::FrameTail;
	}
};


//���У��·��ļ�����
struct XKCmdFileSend
{
	unsigned short mHead;
	unsigned short mCode;
	unsigned short mLenValid;
	char           mFileData[1024];
	unsigned short mTail;

	//�����ļ����ݼ������ֽ���
	//��֡���������1024�ֽڣ�1KB������һ���ļ���Ҫ��Ϊ��δ��䣺�����䣬��������
	//���һ֡ʱ�����ȿ��ܲ���1024�ֽڣ���ʱ sizeData ��д��ʵʣ�೤��
	//sizeData ȡֵ0-1024�����ݱ�֡ʵ�ʴ����ļ����ݳ�����д
	XKCmdFileSend(const char *fileData, int sizeData)
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAFilePFrame::FrameHead;
		mCode = (unsigned short)XKDAFilePFrame::CodeFileSend;
		mLenValid = (unsigned short)sizeData;
		memcpy(&mFileData[0], fileData, sizeData);
		mTail = (unsigned short)XKDAFilePFrame::FrameTail;
	}
};

//���У��ļ������·�����
struct XKCmdFileEnd
{
	unsigned short mHead;
	unsigned short mCode;
	unsigned short mLenName;     //�ļ������ȣ��ֽ���
	char           mName[1024];
	unsigned short mTail;

	//�����ļ��� �� �ļ������ȣ��ֽ�����
	XKCmdFileEnd(const char *fileName, int sizeName)
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAFilePFrame::FrameHead;
		mCode = (unsigned short)XKDAFilePFrame::CodeFileEnd;
		mLenName = (unsigned short)sizeName;
		memcpy(&mName[0], fileName, sizeName);
		mTail = (unsigned short)XKDAFilePFrame::FrameTail;
	}
};

//���У�ȡ����Ƶ�ļ��·�
struct XKCmdFileCancel
{
	unsigned short mHead;
	unsigned short mCode;
	unsigned short mLenName;     //�ļ������ȣ��ֽ���
	char           mName[1024];
	unsigned short mTail;

	//�����ļ��� �� �ļ������ȣ��ֽ�����
	XKCmdFileCancel(const char *fileName, int sizeName)
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAFilePFrame::FrameHead;
		mCode = (unsigned short)XKDAFilePFrame::CodeFileCancel;
		mLenName = (unsigned short)sizeName;
		memcpy(&mName[0], fileName, sizeName);
		mTail = (unsigned short)XKDAFilePFrame::FrameTail;
	}
};





//�ϱ���Ƶ�ļ��������յ� ��1.��ȡ��Ƶ�ļ������� ָ����ϱ���
struct XKReportFileCount
{
	unsigned short mHead;
	unsigned short mCode;
	unsigned short mCount;      //��Ƶ�ļ�����
	char           mRes[1024];
	unsigned short mTail;

	//��������
	XKReportFileCount()
	{
		memset(this, 0, sizeof(*this));
	}

	//�ϱ�����
	XKReportFileCount(int count)
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAFilePFrame::FrameHead;
		mCode = (unsigned short)XKDAFilePFrame::CodeGetCount;
		mCount = (unsigned short)count;
		mTail = (unsigned short)XKDAFilePFrame::FrameTail;
	}
};

//���У��ϱ���Ƶ�ļ��б��յ� ��2.��ȡ��Ƶ�ļ��б� ָ��������ϱ���
//ÿ�������ϱ�һ����Ƶ�ļ����������ϱ�
//�磺��ǰ��12����Ƶ�ļ���������0-11�ϱ���12����Ƶ�ļ�����ÿ֡�����ϱ�һ������12֡����
struct XKCmdReportNames
{
	unsigned short mHead;
	unsigned short mCode;
	unsigned short mCount;        //��Ƶ�ļ��ܸ���
	unsigned short mOrderNumber;  //�ļ���ţ���0��ʼ��ţ�ȡֵ 0 �� mCount-1
	unsigned short mLenName;      //�ļ������ȣ��ֽ���
	char           mName[1020];
	unsigned short mTail;

	//��������
	XKCmdReportNames()
	{
		memset(this, 0, sizeof(*this));
	}

	//�ϱ����ã������ļ����ȣ��ֽ��������ļ��� �� �ļ������ȣ��ֽ�����
	XKCmdReportNames(int count, int orderNumber, const char *fileName, int sizeName)
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAFilePFrame::FrameHead;
		mCode = (unsigned short)XKDAFilePFrame::CodeReportNames;
		mCount = (unsigned short)count;
		mOrderNumber = (unsigned short)orderNumber;
		mLenName = (unsigned short)sizeName;
		memcpy(&mName[0], fileName, sizeName);
		mTail = (unsigned short)XKDAFilePFrame::FrameTail;
	}
};

//�ϱ�ɾ��ָ����Ƶ�ļ�������յ� ��3.ɾ��ָ����Ƶ�ļ��� ָ����ϱ�ɾ�������
struct XKReportDeleteResult
{
	//�����
	enum eCodeResult
	{
		Succeed          = 0x00,    //ɾ���ɹ�
		Failed           = 0x01,    //ɾ��ʧ�ܣ�δ����ɾ��ʧ��ԭ��
		FailedUsing      = 0x02,    //ɾ��ʧ�ܣ��ļ�����ʹ����
		FailedNameError  = 0x03,    //ɾ��ʧ�ܣ������ļ���Ż��ļ�������
	};

	unsigned short mHead;
	unsigned short mCode;
	unsigned short mResult;      //ɾ��������룺������붨��
	char           mRes[1024];
	unsigned short mTail;

	//��������
	XKReportDeleteResult()
	{
		memset(this, 0, sizeof(*this));
	}

	//�ϱ�����
	XKReportDeleteResult(eCodeResult result)
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAFilePFrame::FrameHead;
		mCode = (unsigned short)XKDAFilePFrame::CodeDeleteResult;
		mResult = (unsigned short)result;
		mTail = (unsigned short)XKDAFilePFrame::FrameTail;
	}
};


//�ϱ��ļ�������ȣ�ÿ�յ�һ֡�ļ����ݱ��ģ����ϴ�һ�ν��ս��ȣ��ѽ��յ������ֽ�����
struct XKReportReceived
{
	unsigned short mHead;
	unsigned short mCode;
	unsigned int   mSizeTotal;    //�ļ����ֽ���
	unsigned int   mSizeReceived; //�ѽ����ֽ���
	char           mRes[1018];
	unsigned short mTail;

	//��������
	XKReportReceived()
	{
		memset(this, 0, sizeof(*this));
	}

	//�ϱ�����
	XKReportReceived(unsigned int sizeTotal, unsigned int sizeReceived)
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAFilePFrame::FrameHead;
		mCode = (unsigned short)XKDAFilePFrame::CodeReportReceived;
		mSizeTotal = (unsigned int)sizeTotal;
		mSizeReceived = (unsigned int)sizeReceived;
		mTail = (unsigned short)XKDAFilePFrame::FrameTail;
	}
};

//�ϱ���Ƶ�ļ����������ļ�����ɹ�������ȡ������ʱ���Զ��ϱ���
struct XKReportFileSendResult
{
	//�����
	enum eCodeResult
	{
		Succeed        = 0x00,       //���ճɹ�
		Failed         = 0x01,       //����ʧ�ܣ�δ����ʧ��ԭ��
		FailedCancel   = 0x02,       //����ʧ�ܣ�ȡ������
		FailedTimeout  = 0x03,       //����ʧ�ܣ���ʱ
		FailedOverflow = 0x04,       //����ʧ�ܣ���Ƶ�ļ����������洢�ռ䲻��
	};

	unsigned short mHead;
	unsigned short mCode;
	unsigned short mResult;      //������룺������붨��
	char           mRes[1024];
	unsigned short mTail;

	//��������
	XKReportFileSendResult()
	{
		memset(this, 0, sizeof(*this));
	}

	//�ϱ�����
	XKReportFileSendResult(eCodeResult result)
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAFilePFrame::FrameHead;
		mCode = (unsigned short)XKDAFilePFrame::CodeFileSendResult;
		mResult = (unsigned short)result;
		mTail = (unsigned short)XKDAFilePFrame::FrameTail;
	}
};


//���1�ֽڶ��룬�ָ�Ĭ�϶���
#pragma pack(pop)

#endif // !XKDIRECTACSTFILEPROTOCOL_H

