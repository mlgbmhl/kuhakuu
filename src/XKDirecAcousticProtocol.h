#ifndef XKDIRECACOUSTICPROTOCOL_H
#define XKDIRECACOUSTICPROTOCOL_H

/********************************************************************
** 《XKQS定向声能设备声光控制通信协议v1.0》
** 上位机：与声能设备通信的控制器（计算机、手机）或上层软件
** 下位机：定向声能设备（含下位机软件）

** 通信方式：(1) 上位机与下位机通过10M/100M/1000M以太网连接；
            (2) 采用TCP/IP通信，自定义通信协议；
			(3) 约定上位机为客户端，下位机为服务器端；
			(4) 服务器默认IP地址192.168.1.101，默认端口10800;
			(5) IP及端口在部署阶段，可以根据需求重新分配。

** 字节序：多字节传输采用小端字节序，即低字节在前、高字节在后。

** 协议格式：定长帧，单帧长度18字节，具体以结构体形式提供；
            本协议提供20条指令，分别为：
			01. 声能：强打击（音量档：强）
			02. 声能：弱打击（音量档：弱）
			03. 声能：播放音频文件：播放指定序号音频文件
			04. 声能：停止播放音频（声能停止播放）：备用

			05. 强光：警示，1Hz闪烁
			06. 强光：直射，常亮
			07. 强光：爆闪，8Hz闪烁
			08. 强光：关闭

			09. 转台：开始上转
			10. 转台：开始下转
			11. 转台：开始左转
			12. 转台：开始右转
			13. 转台：开始左上转
			14. 转台：开始左下转
			15. 转台：开始右上转
			16. 转台：开始右下转
			17. 转台：转向指定位置
			18. 转台：停止运动

			19. 整机：设备安装点经纬度、海拔
			20. 整机：设备机械零位与北向的偏角

** 附1：数据类型说明，若编译平台字节数与下述有区别，请替换为对应系统下字节数相等的数据类型
        unsigned short：2字节无符号数
		int           ：4字节有符号数
		char          ：1字节有符号数
*********************************************************************/

#include <string.h>
#include <math.h>

//指定以1字节对齐
#pragma pack(push, 1)

/********************基础元素定义：用户不用关注******************/
static const double XKDAPFrame_UnitLon = 180.0 / pow(2.0, 31);
static const double XKDAPFrame_UnitLat = 180.0 / pow(2.0, 31);
static const double XKDAPFrame_UnitAlt = 0.01;
static const double XKDAPFrame_UnitAngle = 0.01;

struct XKDAPFrame
{
	//约定帧头、帧尾及帧指令码的值
	enum XKDAPCodeValue
	{
		//帧头帧尾
		FrameHead            = 0x7e7e,    //帧头
		FrameTail            = 0xeded,    //帧尾
		
		//声能
		CodeAcousticStrong   = 0x0101,    //声能强打击
		CodeAcousticWeak     = 0x0102,    //声能弱打击
		CodeAcousticPlay     = 0x0103,    //播放音频文件
		CodeAcousticStop     = 0x0104,    //声能停止播放
		
		//强光
		CodeLampWarning      = 0x0201,    //警示
		CodeLampBeat         = 0x0202,    //直射
		CodeLampBurstFlash   = 0x0203,    //爆闪
		CodeLampStop         = 0x0204,    //关闭
		
		//转台
		CodeRotaryUp         = 0x0301,    //上转
		CodeRotaryDown       = 0x0302,    //下转
		CodeRotaryLeft       = 0x0303,    //左转
		CodeRotaryRight      = 0x0304,    //右转
		CodeRotaryLeftUp     = 0x0305,    //左上转
		CodeRotaryLeftDown   = 0x0306,    //左下转
		CodeRotaryRightUp    = 0x0307,    //右上转
		CodeRotaryRightDown  = 0x0308,    //右下转
		CodeRotaryLocation   = 0x0309,    //转向指定位置
		CodeRotaryStop       = 0x030A,    //停止运动

		//设备整体
		CodeDevWGS           = 0x0A01,    //设置设备安装位置的经纬度、海拔
		CodeDevDriftAngle    = 0x0A02,    //设备安装后处于机械零位时，设备法向与北向的偏角
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

/***********************结束基础元素定义************************/


//声能强打击（音量档：强）
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

//声能弱打击（音量档：弱）
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


//声能：播放指定序号的音频；输入音频文件序号，从0开始；默认0
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

//声能：停止播放，备用
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



//强光：警示
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

//强光：直射
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

//强光：爆闪
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

//强光：关闭
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



//转台：上转
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

//转台：下转
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

//转台：左转
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

//转台：右转
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

//转台：左上转
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

//转台：左下转
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

//转台：右上转
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

//转台：右下转
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

//转台：转向指定位置，经纬度（°）、海拔（米）
struct XKCmdRotaryLocation
{
	unsigned short mHead;
	unsigned short mCode;
	int            mLon;
	int            mLat;
	int            mAlt;
	unsigned short mTail;

	//直接传入WGS84经纬度及海拔
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

//转台：停止运动
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


//设备：设置设备安装位置经纬度（°）、海拔（米）
struct XKCmdDevWGS
{
	unsigned short mHead;
	unsigned short mCode;
	int            mLon;
	int            mLat;
	int            mAlt;
	unsigned short mTail;

	//直接传入WGS84经纬度及海拔
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

//设备：设备安装后处于机械零位时，设备法向与北向的偏角
//严格定义：地理北向到设备法向的偏角，沿顺时针增大，取值[0.00, 360.0)
struct XKCmdDevDriftAngle
{
	unsigned short mHead;
	unsigned short mCode;
	int            mAzi;
	char           mRes[8];
	unsigned short mTail;

	//azi：设备机械零位时，法向与北向的偏角，取值[0.00, 360.0)
	XKCmdDevDriftAngle(double azi)
	{
		memset(this, 0, sizeof(*this));
		mHead = (unsigned short)XKDAPFrame::FrameHead;
		mCode = (unsigned short)XKDAPFrame::CodeDevDriftAngle;
		mAzi  = (int)(azi / XKDAPFrame_UnitAngle);
		mTail = (unsigned short)XKDAPFrame::FrameTail;
	}
};

//解除1字节对齐，恢复默认对齐
#pragma pack(pop)


#endif // !XKDIRECACOUSTICPROTOCOL_H


