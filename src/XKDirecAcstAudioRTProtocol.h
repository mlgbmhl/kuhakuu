#ifndef XKDIRECACSTAUDIORTPROTOCOL_H
#define XKDIRECACSTAUDIORTPROTOCOL_H

/********************************************************************
** 《XKQS定向声能设备实时音频传输通信协议v1.0》
** 上位机：与声能设备通信的控制器（计算机、手机）或上层软件
** 下位机：定向声能设备（含下位机软件）

** 通信方式：(1) 上位机与下位机通过10M/100M/1000M以太网连接；
			(2) 采用TCP/IP通信，自定义通信协议；
			(3) 约定上位机为客户端，下位机为服务器端；
			(4) 服务器默认IP地址192.168.1.101，默认端口10802;
			(5) IP及端口在部署阶段，可以根据需求重新分配。

** 字节序：多字节传输采用小端字节序，即低字节在前、高字节在后。

** 协议格式：定长帧，单帧长度1032字节，具体以结构体形式提供；
			本协议提供1条指令：
			01. 下行：实时音频数据下发

** 附1：数据类型说明，若编译平台字节数与下述有区别，请替换为对应系统下字节数相等的数据类型
		unsigned short：2字节无符号数
		char          ：1字节有符号数

** 附2：音频格式参数：
        采样率8000、单通道、采样长度16字节、编码PCM、采样类型有符号整型、字节序为小端字节序

** 附3：流程说明
        上位机实时采集PCM音频流数据，采集到数据之后立刻通过该协议将音频数据下发至下位机；
		若一次采集到的数据长度大于1024，则依序分帧传输即可。

** 附4：上位机启用实时喊话时，建议停止音频文件播放。

** 附5：协议格式说明：为何不采用流或者变长帧报文，而采用定长帧报文？
					简化通信协议，降低双方联调难度。
*********************************************************************/

#include <string.h>

//指定以1字节对齐
#pragma pack(push, 1)

/***********************基础元素定义：用户不用关注********************/
struct XKDAAudioRTPFrame
{
	//约定帧头、帧尾及帧指令码的值
	enum XKDAAudioRTPPCodeValue
	{
		//帧头帧尾
		FrameHead = 0x7c7c,    //帧头
		FrameTail = 0xeded,    //帧尾

		CodeAudioSend = 0x0801,    //下发实时音频数据
	};
};
/***************************结束基础元素定义*************************/


//实时音频数据下发
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

//解除1字节对齐，恢复默认对齐
#pragma pack(pop)

#endif // !XKDIRECACSTAUDIORTPROTOCOL_H

