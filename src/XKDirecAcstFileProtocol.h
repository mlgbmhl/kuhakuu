#ifndef XKDIRECTACSTFILEPROTOCOL_H
#define XKDIRECTACSTFILEPROTOCOL_H

/********************************************************************
** 《XKQS定向声能设备文件管理通信协议v1.0》
** 上位机：与声能设备通信的控制器（计算机、手机）或上层软件
** 下位机：定向声能设备（含下位机软件）
** 下行通信：上位机至下位机的通信，简称下发
** 上行通信：下位机至上位机的通信，简称上报

** 通信方式：(1) 上位机与下位机通过10M/100M/1000M以太网连接；
			(2) 采用TCP/IP通信，自定义通信协议；
			(3) 约定上位机为客户端，下位机为服务器端；
			(4) 服务器默认IP地址192.168.1.101，默认端口10801;
			(5) IP及端口在部署阶段，可以根据需求重新分配。

** 字节序：多字节传输采用小端字节序，即低字节在前、高字节在后。
** 字符编码：若传输内容包含中文字符，则中文字符采用UTF-8编码格式

** 协议方向性：协议分为下行协议和上行协议
             下行协议：上位机下发至下位机的协议，如文件传输、查询、删除等
		     上行协议：下位机上报至上位机的协议，如文件列表等

** 协议格式：定长帧，单帧长度1032字节，具体以结构体形式提供；
			本协议提供12条指令，分别为：
			01. 下行：获取音频文件个数
			02. 下行：获取音频文件名列表
			03. 下行：删除指定音频文件
			04. 下行：准备下发音频文件，给定文件长度、文件名
			05. 下行：下发文件内容
			06. 下行：文件内容下发结束
			07. 下行：取消音频文件下发

			08. 上行：上报音频文件个数（收到 “1.获取音频文件个数” 指令后上报）
			09. 上行：上报音频文件列表（收到 “2.获取音频文件列表” 指令后依次上报，每条报文上报一个文件名）
			10. 上行：上报删除指定音频文件结果（收到 “3.删除指定音频文件” 指令后上报删除结果）
			11. 上行：上报文件传输进度（每收到一帧文件内容报文，则上传一次接收进度：已接收到的总字节数）
			12. 上行：上报音频文件传输结果（文件传输成功结束、取消、超时后自动上报）

** 附1：数据类型说明，若编译平台字节数与下述有区别，请替换为对应系统下字节数相等的数据类型
		unsigned short：2字节无符号数
		int           ：4字节有符号数
		unsigned int  ：4字节无符号数
		char          ：1字节有符号数

** 附2：支持的音频文件格式
        支持.wav、.mp3两种格式。

** 附3：音频文件传输流程说明
        04-07为音频文件下发相关指令，11为音频文件传输结果上报
		举例：准备下发《驱散.mp3》文件，文件长度100KB，即102400字节。
		    1：通过04指令，将文件长度102400及文件名“驱散.mp3”下发；
			2：将文件内容通过05指令，分多次下发，直至内容下发完成；
			    分支1：文件传输过程中，可通过07指令取消该音频文件下发，下位机通过11指令确认取消
			3：文件内容下发完成（步骤2完成后），下发06指令，告知指定文件名的音频文件传输结束
			4：下位机收到结束指令后完成指定文件接收，将成功结果通过11指令告知上位机；
			   或长时间未接收到新的文件内容数据，且未接收到取消或结束指令，则通过11指令告知文件传输失败（超时）
			特别说明：步骤1-3为下发一个音频文件的闭环流程，中间不得插入其他数据

** 附4：文件名重名处理，重名指文件名及后缀完全一致
        若新下发音频文件与已有文件重名，则下位机自动在文件名后添加数字加以区分；
		如：下位机已有《驱离.mp3》及《驱离1.mp3》，
		    此时上位机再下发一个《驱离.mp3》的文件，则会被自动更名为《驱离2.mp3》

** 附5：协议格式说明：为何不采用流或者变长帧报文，而采用定长帧报文？
                    简化通信协议，降低双方联调难度。
*********************************************************************/

#include <string.h>

//指定以1字节对齐
#pragma pack(push, 1)

/***************************基础元素定义*****************************/
struct XKDAFilePFrame
{
	//约定帧头、帧尾及帧指令码的值
	enum XKDAFilePCodeValue
	{
		//帧头帧尾：用户解析上行报文时需要
		FrameHead = 0x7d7d,    //帧头
		FrameTail = 0xeded,    //帧尾

		//下行：用户不用关注
		CodeGetCount      = 0x0601,  //获取音频文件个数
		CodeGetNames      = 0x0602,  //获取音频文件名列表
		CodeDeleteFile    = 0x0603,  //删除指定音频文件
		CodeFileReady     = 0x0604,  //准备下发音频文件
		CodeFileSend      = 0x0605,  //下发音频文件内容
		CodeFileEnd       = 0x0606,  //文件内容发送完成
		CodeFileCancel    = 0x0607,  //取消文件发送

		//上行：用户解析上行报文时需要
		CodeReportCount     = 0x06A1,  //上报音频文件个数
		CodeReportNames     = 0x06A2,  //上报音频文件名列表
		CodeDeleteResult    = 0x06A3,  //上报音频文件删除结果
		CodeReportReceived  = 0x06A4,  //上报文件接收进度
		CodeFileSendResult  = 0x06A5,  //上报音频文件传输结果
	};
};
/***************************结束基础元素定义*************************/



//下行：获取音频文件个数
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

//下行：获取音频文件名列表
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

//下行：删除指定音频文件
struct XKCmdDeleteFile
{
	unsigned short mHead;
	unsigned short mCode;
	unsigned short mOrderNumber;    //音频文件序号，从0开始
	char           mRes[1024];
	unsigned short mTail;

	//传入需要删除的音频文件序号，从0开始编号
	XKCmdDeleteFile(int fileOrderNumber)
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAFilePFrame::FrameHead;
		mCode = (unsigned short)XKDAFilePFrame::CodeDeleteFile;
		mOrderNumber = (unsigned short)fileOrderNumber;
		mTail = (unsigned short)XKDAFilePFrame::FrameTail;
	}
};


//下行：准备下发音频文件，给定文件长度、文件名
struct XKCmdFileReady
{
	unsigned short mHead;
	unsigned short mCode;
	unsigned int   mSizeFile;  //文件长度：字节数
	unsigned short mLenName;   //文件名长度：字节数
	char           mName[1020];
	unsigned short mTail;

	//传入文件长度（字节数）、文件名 及 文件名长度（字节数）
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


//下行：下发文件内容
struct XKCmdFileSend
{
	unsigned short mHead;
	unsigned short mCode;
	unsigned short mLenValid;
	char           mFileData[1024];
	unsigned short mTail;

	//传入文件内容及内容字节数
	//单帧报文最大传输1024字节（1KB），故一个文件需要分为多次传输：依序传输，不可乱序
	//最后一帧时，长度可能不足1024字节，此时 sizeData 填写真实剩余长度
	//sizeData 取值0-1024，根据本帧实际传输文件内容长度填写
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

//下行：文件内容下发结束
struct XKCmdFileEnd
{
	unsigned short mHead;
	unsigned short mCode;
	unsigned short mLenName;     //文件名长度：字节数
	char           mName[1024];
	unsigned short mTail;

	//传入文件名 及 文件名长度（字节数）
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

//下行：取消音频文件下发
struct XKCmdFileCancel
{
	unsigned short mHead;
	unsigned short mCode;
	unsigned short mLenName;     //文件名长度：字节数
	char           mName[1024];
	unsigned short mTail;

	//传入文件名 及 文件名长度（字节数）
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





//上报音频文件个数（收到 “1.获取音频文件个数” 指令后上报）
struct XKReportFileCount
{
	unsigned short mHead;
	unsigned short mCode;
	unsigned short mCount;      //音频文件个数
	char           mRes[1024];
	unsigned short mTail;

	//解析方用
	XKReportFileCount()
	{
		memset(this, 0, sizeof(*this));
	}

	//上报方用
	XKReportFileCount(int count)
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAFilePFrame::FrameHead;
		mCode = (unsigned short)XKDAFilePFrame::CodeGetCount;
		mCount = (unsigned short)count;
		mTail = (unsigned short)XKDAFilePFrame::FrameTail;
	}
};

//上行：上报音频文件列表（收到 “2.获取音频文件列表” 指令后依次上报）
//每条报文上报一个音频文件名，依序上报
//如：当前有12个音频文件，则依序0-11上报这12个音频文件名，每帧报文上报一个，共12帧报文
struct XKCmdReportNames
{
	unsigned short mHead;
	unsigned short mCode;
	unsigned short mCount;        //音频文件总个数
	unsigned short mOrderNumber;  //文件序号，从0开始编号，取值 0 至 mCount-1
	unsigned short mLenName;      //文件名长度：字节数
	char           mName[1020];
	unsigned short mTail;

	//解析方用
	XKCmdReportNames()
	{
		memset(this, 0, sizeof(*this));
	}

	//上报方用：传入文件长度（字节数）、文件名 及 文件名长度（字节数）
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

//上报删除指定音频文件结果（收到 “3.删除指定音频文件” 指令后上报删除结果）
struct XKReportDeleteResult
{
	//结果码
	enum eCodeResult
	{
		Succeed          = 0x00,    //删除成功
		Failed           = 0x01,    //删除失败：未给定删除失败原因
		FailedUsing      = 0x02,    //删除失败：文件正在使用中
		FailedNameError  = 0x03,    //删除失败：给定文件序号或文件名错误
	};

	unsigned short mHead;
	unsigned short mCode;
	unsigned short mResult;      //删除结果代码：见结果码定义
	char           mRes[1024];
	unsigned short mTail;

	//解析方用
	XKReportDeleteResult()
	{
		memset(this, 0, sizeof(*this));
	}

	//上报方用
	XKReportDeleteResult(eCodeResult result)
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAFilePFrame::FrameHead;
		mCode = (unsigned short)XKDAFilePFrame::CodeDeleteResult;
		mResult = (unsigned short)result;
		mTail = (unsigned short)XKDAFilePFrame::FrameTail;
	}
};


//上报文件传输进度（每收到一帧文件内容报文，则上传一次接收进度：已接收到的总字节数）
struct XKReportReceived
{
	unsigned short mHead;
	unsigned short mCode;
	unsigned int   mSizeTotal;    //文件总字节数
	unsigned int   mSizeReceived; //已接收字节数
	char           mRes[1018];
	unsigned short mTail;

	//解析方用
	XKReportReceived()
	{
		memset(this, 0, sizeof(*this));
	}

	//上报方用
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

//上报音频文件传输结果（文件传输成功结束、取消、超时后自动上报）
struct XKReportFileSendResult
{
	//结果码
	enum eCodeResult
	{
		Succeed        = 0x00,       //接收成功
		Failed         = 0x01,       //接收失败：未给定失败原因
		FailedCancel   = 0x02,       //接收失败：取消发送
		FailedTimeout  = 0x03,       //接收失败：超时
		FailedOverflow = 0x04,       //接受失败：音频文件数量溢出或存储空间不足
	};

	unsigned short mHead;
	unsigned short mCode;
	unsigned short mResult;      //结果代码：见结果码定义
	char           mRes[1024];
	unsigned short mTail;

	//解析方用
	XKReportFileSendResult()
	{
		memset(this, 0, sizeof(*this));
	}

	//上报方用
	XKReportFileSendResult(eCodeResult result)
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAFilePFrame::FrameHead;
		mCode = (unsigned short)XKDAFilePFrame::CodeFileSendResult;
		mResult = (unsigned short)result;
		mTail = (unsigned short)XKDAFilePFrame::FrameTail;
	}
};


//解除1字节对齐，恢复默认对齐
#pragma pack(pop)

#endif // !XKDIRECTACSTFILEPROTOCOL_H

