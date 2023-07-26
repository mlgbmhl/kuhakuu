#ifndef PROTOCOLACSTROT_H_20210312_1406
#define PROTOCOLACSTROT_H_20210312_1406

#include <QByteArray>
#include <Windows.h>
#include <QString>
#include <QStringList>
#include <QHostAddress>

#pragma execution_character_set("utf-8")
#pragma pack( push, 1 )


//protocol-format of Acoustic and rotary and so on
typedef struct ProtcARFmt
{
	enum ePackInfo
	{
		Head     = 0x7e7e,    //帧头
		Tail     = 0x0a0d,    //帧尾
	};

	//指令码
	//指令应答 指令码与下发一一对应，但无数据包
	//网络心跳应答与下发对应，有数据包
	//RESPONSE 与 SET、GET拥有相同的指令码
	enum eCmdCode
	{
		//网络
		NetHand = 0x0001,    //NET 网络心跳DP

		//控制权Access
		AccApply       = 0x0101,   //SET 请求控制权
		AccQuery       = 0x0102,   //GET 查询控制权
		AccRepApply    = 0x0181,   //REPORT 控制权响应response
		AccRepState    = 0x0182,   //REPORT 上报控制权状态

		//声能
		AcstSetFlName  = 0x0201,  //SET 设置播放曲目
		AcstPlayCtrl   = 0x0202,  //SET 播放控制
		AcstMuteCtrl   = 0x0203,  //SET 静音控制
		AcstVolumeCtrl = 0x0204,  //SET 音量控制
		AcstLoopModel  = 0x0205,  //SET 设置播放模式
		AcstDeleteFile = 0x0206,  //SET 删除指定文件名的曲目
		AcstQueryCount = 0x0207,  //GET 查询曲目总数
		AcstQueryNames = 0x0208,  //GET 查询曲目列表
		AcstQueryCurr  = 0x0209,  //GET 查询当前播放的曲目名
		AcstQueryState = 0x020A,  //GET 查询声能设备状态
		AcstRepDelete  = 0x0281,  //REPORT 删除曲目结果响应
		AcstRepCount   = 0x0282,  //REPORT 上报曲目总数
		AcstRepNames   = 0x0283,  //REPORT 上报曲目列表
		AcstRepCurr    = 0x0284,  //REPORT 上报当前播放的曲目名称
		AcstRepState   = 0x0285,  //REPORT 上报声能设备状态

		//转台
		RotStop        = 0x0301,    //SET 转台停止
		RotTurnDirec   = 0x0302,    //SET 姿态控制：上下左右
		RotSetVel      = 0x0303,    //SET 转速设置
		RotTurnTo      = 0x0304,    //SET 转到驻留角
		RotZero        = 0x0305,    //SET 归零
		RotQueryState  = 0x0306,    //GET 查询状态状态
		RotRepState    = 0x0381,    //REPORT 上报转台状态

		//综合管理(synthetical 综合的)
		SynWorkModel   = 0x8101,    //SET 工作模式控制
		SynPrmAuto     = 0x8102,    //SET 自动模式参数设置
		SynSetAutoRep  = 0x8103,    //SET 自动上报使能控制
		SynPowerOffPC  = 0x8104,    //SET 工控机关机
		SynQueryState  = 0x8105,    //GET 查询综合状态
		SynQueryPrmAuto = 0x8106,   //GET 查询自动参数
		SynTiming       = 0x8107,   //SET 授时
		SynRepState    = 0x8181,    //REPORT 上报综合状态
		SynRepPrmAuto  = 0x8182,    //REPORT 上报自动模式参数
	};

	//报文类型
	enum eMsgType
	{
		MsgNone     = 0,  //缺省类型；本协议约定不可缺省
		MsgSet      = 1,  //下行控制
		MsgGet      = 2,  //下行查询
		MsgNet      = 3,  //下行/上行 网络心跳
		MsgResponse = 4,  //上行，对SET、GET报文的应答
		MsgReport   = 5,  //上行，执行结果响应、状态上报、参数上报等
	};

	typedef struct Prep
	{
		WORD    mHead;        //帧头0x7e7e
		WORD    mLen;         //单包数据长度，从帧头到帧尾（含）

		//以下两项在确定收发者时调用setAddr赋值（需要发送的结构体）
		DWORD   mAddrSrc;     //源地址：发送方IP
		DWORD   mAddrDst;     //目的地址：接收方IP
		
		//以下两项在结果提定义时赋值（需要发送的结构体）
		WORD    mType;        //报文类型
		WORD    mCmdCode;     //命令码：区分不同指令编码
		
		//除确有分包传输情况外（可能的是曲目列表），下面两项采用默认值 1,1
		WORD    mPackCount;   //总包数：未分包，1；分包根据实际情况填写
		WORD    mPackNo;      //当前包号：从1开始编号，最大值为总包数

		Prep()
		{
			memset( this, 0, sizeof(*this) );
			mHead      = Head;
			mPackCount = 1;
			mPackNo    = 1;
		}

		int ipToAddr(const QString& ip) const
		{
			return QHostAddress(ip).toIPv4Address();
		}

		QString addrToIP(int addr) const
		{
			return QHostAddress(addr).toString();
		}

		void setAddr(const QString& ipSrc, const QString& ipDst)
		{
			mAddrSrc = ipToAddr(ipSrc);
			mAddrDst = ipToAddr(ipDst);
		}

	}Prep;

	typedef QByteArray DataArea;  //数据域，变长

	typedef struct Post
	{
		WORD    mCheck;       //校验和，从帧头（含）到数据包的字节和，不含后缀
		WORD    mTail;        //帧尾，0x0a0d
		Post()
		{
			mCheck = 0;
			mTail  = Tail;
		}
	}Post;

	static const int gLenPrep = sizeof( ProtcARFmt::Prep );
	static const int gLenPost = sizeof( ProtcARFmt::Post );
	static const int gLenPrepPost = gLenPrep + gLenPost;

	static const int gLenMaxPack = 1024;  //单包最大长度
	static const int gLenMaxArea = gLenMaxPack - gLenPrepPost;  //单包数据域最大长度

	Prep     mPrep;
	DataArea mData;
	Post     mPost;

	//具体指令转到指定格式、计算校验和、打包；len为实际数据长度（结构体长度）
	//前缀中需要提前更新pData中的源地址和目的地址
    QByteArray pack( const void *pData, int len )
	{
		//输入合法性判决
		if (pData == NULL || len < gLenPrepPost)
		{
			return QByteArray();
		}

		//数据赋值
		const char *p = (const char*)pData;
		memcpy( &mPrep, &p[0], gLenPrep );
		QByteArray dataArea = QByteArray( &p[gLenPrep], len - gLenPrepPost );
		memcpy( &mPost, &p[len - gLenPost], gLenPost );

		//计算分包数
		int countPack = dataArea.size() / gLenMaxArea;
		countPack = (dataArea.size() % gLenMaxArea == 0) ? countPack : (countPack + 1);

		//当数据域为空时，分包数为1
		countPack = countPack < 1 ? 1 : countPack;
		
		//更新总包数
		mPrep.mPackCount = countPack;

		//分包处理
		QByteArray baRes;
		int packNo = 0;
		int index = 0;
		//for ( int i = 0; i < dataArea.size(); i = i + gLenMaxArea )
		for(packNo=0; packNo < countPack; )
		{
			//更新数据域
			mData = dataArea.mid(index, gLenMaxArea);
			index += mData.size();

			//更新包号
			mPrep.mPackNo = (++packNo);

			//更新长度：必须在数据域更新之后
			mPrep.mLen = gLenPrepPost + mData.size();

			//更新校验和：必须在除校验和外所有数据更新完成之后
			mPost.mCheck = checkSum();

			//打包并拼接
			baRes.append((const char*)&mPrep, gLenPrep);
			baRes.append(mData);
			baRes.append((const char*)&mPost, gLenPost);
		}

		//所有数据包依次拼接后返回
		return baRes;
	}

	//重载：data：结构体整体装入QByteArray
	QByteArray pack(const QByteArray& data)
	{
		return pack(data.constData(), data.size());
	}

	//返回数据包实际长度
	int getSize() const 
	{
		return (gLenPrepPost + mData.size());
	}

	//计算校验和
	static WORD checkSum(const char* p, int len)
	{
		WORD sum = 0;
		for (int i=0; i<len - gLenPost; i++)
		{
			sum += (WORD)(BYTE)p[i];
		}
		return sum;
	}

	//从数据包获取源地址
	static int getAddrSrc(const QByteArray& cmd)
	{
		if (cmd.size() < gLenPrepPost)
		{
			return 0;
		}

		Prep stP;
		memcpy(&stP, &cmd.constData()[0], gLenPrep);
		return stP.mAddrSrc;
	}

	//从一个数据包获取指令码
	static WORD getCode( const QByteArray& cmd )
	{
		if (cmd.size() < gLenPrepPost)
		{
			return 0;
		}

		Prep stP;
		memcpy(&stP, &cmd.constData()[0], gLenPrep);
		return stP.mCmdCode;
	}

private:
	//计算校验和：缺省输入为结构体自身
	WORD checkSum() const 
	{
		WORD sum = 0;

		char *p = (char*)&mPrep;
		for (int i=0; i<gLenPrep; i++)
		{
			sum += (WORD)(BYTE)p[i];
		}

		for (int i=0; i<mData.size(); i++)
		{
			sum += (WORD)(BYTE)mData.at(i);
		}

		return sum;
	}

}ProtcARFmt;


/************************************
** 通用结构体
*************************************/
//提供静态打包接口
struct PrtARPack
{
	static QByteArray pack(const void* pData, int len)
	{
		ProtcARFmt cmd;
		return cmd.pack(pData, len);
	}

	static QByteArray pack(const QByteArray& data)
	{
		ProtcARFmt cmd;
		return cmd.pack(data);
	}
};

//空数据域通用：适用于解析
struct PrtARNoDataUniversal
{
	ProtcARFmt::Prep mPrep;
	ProtcARFmt::Post mPost;
};

//指令应答通用结构体
struct PrtARCmdReponse
{
	ProtcARFmt::Prep mPrep;
	ProtcARFmt::Post mPost;

	PrtARCmdReponse(const ProtcARFmt::Prep& cmdPrep)
	{
		//mPrep.mLen = sizeof(PrtARCmdReponse); //长度打包时自动更新
		mPrep.mType = ProtcARFmt::MsgResponse;
		mPrep.mCmdCode = cmdPrep.mCmdCode;
		mPrep.mAddrSrc = cmdPrep.mAddrDst;
		mPrep.mAddrDst = cmdPrep.mAddrSrc;
	}

	PrtARCmdReponse(const char* pData)
	{
		memcpy(&mPrep, &pData[0], ProtcARFmt::gLenPrep);
		mPrep.mType = ProtcARFmt::MsgResponse;

		DWORD tmp = mPrep.mAddrSrc;
		mPrep.mAddrSrc = mPrep.mAddrDst;
		mPrep.mAddrDst = tmp;
	}

	QByteArray pack()
	{
		return PrtARPack::pack(this, sizeof(*this));
	}
};


/************************************
** 以下为具体指令结构体
*************************************/
//网络心跳
struct PrtARNetHand
{
	ProtcARFmt::Prep mPrep;

	struct 
	{
		unsigned int mNo;    //握手编号
	}mData;

	ProtcARFmt::Post mPost;

	PrtARNetHand()
	{
		mData.mNo = 0;
		mPrep.mType = ProtcARFmt::NetHand;
		mPrep.mCmdCode = ProtcARFmt::NetHand;
	}
};


/***************************************************
** 控制权处理
****************************************************/
//控制权请求
struct PrtAccApply
{
	enum eCmdIdt
	{
		Apply         = 0,  //请求控制权
		ApplyCancel   = 1,  //取消控制权申请
		FreeAccept    = 2,  //同意释放控制权
		FreeReject    = 3,  //拒绝释放控制权
	};

	ProtcARFmt::Prep mPrep;

	struct
	{
		BYTE mCmd;    //指令码
	}mData;

	ProtcARFmt::Post mPost;
};

//查询控制权
typedef PrtARNoDataUniversal PrtAccQuery;

//控制权响应
struct PrtAccRepApply
{
	ProtcARFmt::Prep mPrep;

	enum eCmdIdt
	{
		NotifApply    = 0,  //通知“申请释放控制权”
		NotifCancel   = 1,  //通知“取消申请”
		NotifAcquire  = 2,  //通知“获得控制权”
		NotifLost     = 3,  //通知“失去控制权”
	};

	struct
	{
		BYTE mCmd;    //指令码
	}mData;

	ProtcARFmt::Post mPost;

	PrtAccRepApply()
	{
		mPrep.mType = ProtcARFmt::MsgReport;
		mPrep.mCmdCode = ProtcARFmt::AccRepApply;
		mData.mCmd = NotifApply;
	}
};

//上报控制权
struct PrtAccRepState
{
	ProtcARFmt::Prep mPrep;

	enum eCmdIdt
	{
		Acquired     = 0,  //已获得控制权
		Lost         = 1,  //已失去控制权
		ApplyFirst   = 2,  //申请中，第一顺位
		ApplyWait    = 3,  //申请中，非第一顺位
	};

	struct
	{
		BYTE mCmd;    //指令码
	}mData;

	ProtcARFmt::Post mPost;

	PrtAccRepState()
	{
		mPrep.mType = ProtcARFmt::MsgReport;
		mPrep.mCmdCode = ProtcARFmt::AccRepState;
		mData.mCmd = Acquired;
	}
};




/***************************************************
** 声能设备
****************************************************/
//设置播放曲目：注意该结构体不能整体用memcpy、memset
struct PrtAcstSetFlName
{
	ProtcARFmt::Prep mPrep;
	
	struct
	{
		BYTE    mLen;    //曲目名字节数：注意，是字节数
		QString mName;   //曲目名

		//文件名最小有效长度
		int nameMinLenValid() const
		{
			return 5;  //"1.mp3" "1.wav"
		}

		//数据域最小有效长度
		int dataMinLenValid() const
		{
			return sizeof(mLen) + nameMinLenValid();  
		}
	}mData;

	ProtcARFmt::Post mPost;

	//用于解析：赋值
	bool fromBytes(const char* pData, int len)
	{
		if (len < ProtcARFmt::gLenPrepPost + mData.dataMinLenValid())
		{
			return false;
		}

		int index = 0;
		memcpy(&mPrep, &pData[index], ProtcARFmt::gLenPrep);
		index += ProtcARFmt::gLenPrep;
		if (mPrep.mLen > len)
		{
			return false;
		}

		//解析mLen
		memcpy(&mData.mLen, &pData[index], sizeof(mData.mLen));
		index += sizeof(mData.mLen);
		if (mData.mLen < mData.nameMinLenValid() ||
			mPrep.mLen != ProtcARFmt::gLenPrepPost + mData.mLen + sizeof(mData.mLen))
		{
			return false;
			
		}

		mData.mName = QString::fromUtf8( QByteArray(&pData[index], mData.mLen) );
		index += mData.mLen;

		memcpy(&mPost, &pData[index], ProtcARFmt::gLenPost);
		return true;
	}
};

//播放控制
struct PrtAcstPlayCtrl
{
	enum eIdtCmd
	{
		Playing    = 0,  //播放
		PlayPause  = 1,  //暂停
		PlayStop   = 2,  //结束
	};

	ProtcARFmt::Prep mPrep;

	struct
	{
		BYTE mCmd;  //指令码：0-播放，1-暂停，2-结束
	}mData;

	ProtcARFmt::Post mPost;
};

//静音控制
struct PrtAcstMuteCtrl
{
	enum eIdtMute
	{
		MuteOn  = 0,  //静音
		MuteOff = 1,  //非静音
	};

	ProtcARFmt::Prep mPrep;

	struct
	{
		BYTE  mMute;  //0-静音；1-非静音
	}mData;

	ProtcARFmt::Post mPost;
};

//音量控制
struct PrtAcstVolumeCtrl
{
	enum eIdtVolume
	{
		VolPlayer    = 0, //播放器音量
		VolDevice    = 1, //设备音量
		VolBoth      = 2, //播放器音量和设备音量
	};

	ProtcARFmt::Prep mPrep;

	struct
	{
		BYTE    mIdt;  //0-播放器音量；1-设备音量； 2-播放器音量和设备音量
		BYTE    mVolPlayer;  //0-100
		BYTE    mVolDevice;  //0-100
	}mData;

	ProtcARFmt::Post mPost;
};

//播放模式
struct PrtAcstLoopModel
{
	enum eIdtLoop
	{
		LoopAll = 0,  //列表循环
		LoopOne = 1,  //单曲循环
	};

	ProtcARFmt::Prep mPrep;

	struct 
	{
		BYTE    mLoop;    //0-列表循环； 1-单曲循环
	}mData;

	ProtcARFmt::Post mPost;
};

//删除指定曲目：解析用，与设置播放曲目结构体相同
typedef PrtAcstSetFlName PrtAcstDeleteFile;

//查询曲目总数
typedef PrtARNoDataUniversal PrtAcstQueryCount;

//查询曲目列表
typedef PrtARNoDataUniversal PrtAcstQueryNames;

//查询当前播放的曲目名称
typedef PrtARNoDataUniversal PrtAcstQueryCurr;

//查询声能设备状态
typedef PrtARNoDataUniversal PrtAcstQueryState;

//删除曲目结果响应
struct PrtAcstRepDelete
{
	enum eIdtResualt
	{
		Succeed      = 0,  //删除成功
		ErrorName    = 1,  //删除失败：文件名错误
		ErrorUsing   = 2,  //删除失败：文件正在被占用
		ErrorUndef   = 3,  //删除失败：其他错误
	};

	ProtcARFmt::Prep mPrep;

	struct
	{
		BYTE mResult;   //结果代码：0删除成功；其他删除失败
	}mData;

	ProtcARFmt::Post mPost;

	PrtAcstRepDelete(eIdtResualt idt = Succeed)
	{
		mPrep.mType = ProtcARFmt::MsgReport;
		mPrep.mCmdCode = ProtcARFmt::AcstRepDelete;
		mData.mResult = Succeed;
	}
};

//上报曲目总数
struct PrtAcstRepCount
{
	ProtcARFmt::Prep mPrep;

	struct 
	{
		WORD mCount;    //曲目总数
	}mData;

	ProtcARFmt::Post mPost;

	PrtAcstRepCount()
	{
		mPrep.mType = ProtcARFmt::MsgReport;
		mPrep.mCmdCode = ProtcARFmt::AcstRepCount;
		mData.mCount = 0;
	}
};

//上报曲目列表：特殊结构体，该结构体不能整体用memcpy、memset
struct PrtAcstRepNames
{
	ProtcARFmt::Prep mPrep;

	struct
	{
		WORD             mCount;
		QStringList      mNames;
	}mData;

	ProtcARFmt::Post mPost;

	typedef struct MscUnit
	{
		WORD        mOrder;    //曲目序号
		BYTE        mLen;      //文件名长度
		QByteArray  mName;     //文件名

		MscUnit(int order, const QString& name)
		{
			mOrder = order;
			mName = name.toUtf8();

			//当有中文时，文件名字符数 与 字节数 不相等
			mLen = mName.size();  
		}

		QByteArray toByteArray() const
		{
			int len = sizeof(MscUnit) - sizeof(QByteArray);
			const char *p = (const char*)this;
			QByteArray res = QByteArray(&p[0], len);
			return res.append(mName);
		}
	}MscUnit;

	PrtAcstRepNames()
	{
		mPrep.mType = ProtcARFmt::MsgReport;
		mPrep.mCmdCode = ProtcARFmt::AcstRepNames;
		mData.mCount = 0;
	}

	QByteArray toByteArray()
	{
		QByteArray baRes;

		//压入前缀
		baRes.append((const char*)&mPrep, sizeof(mPrep));

		//更新曲目总数并压入缓存
		mData.mCount = mData.mNames.size();
		baRes.append((const char*)&mData.mCount, sizeof(mData.mCount));

		//将所有曲目名信息拼接起来
		for (int i = 0; i < mData.mCount; i++)
		{
			baRes.append( MscUnit(i + 1, mData.mNames.at(i)).toByteArray() );
		}

		//压入后缀
		baRes.append((const char*)&mPost, sizeof(mPost));

		//将整个不定长帧内容简单拼接后返回，等待进一步打包工作
		return baRes;
	}
};

//上报当前播放的曲目名称，该结构体不能整体用memcpy、memset
struct PrtAcstRepCurr
{
	ProtcARFmt::Prep mPrep;

	struct
	{
		BYTE    mLen;   //文件名长度：字节数
		QString mName;  //文件名
	}mData;

	ProtcARFmt::Post mPost;

	PrtAcstRepCurr()
	{
		mPrep.mType = ProtcARFmt::MsgReport;
		mPrep.mCmdCode = ProtcARFmt::AcstRepCurr;
		mData.mLen = 0;
	}

	QByteArray toByteArray()
	{
		//更新长度
		QByteArray baName = mData.mName.toUtf8();
		mData.mLen = baName.size();

		QByteArray baData;
		baData.append((const char*)&mPrep, ProtcARFmt::gLenPrep);
		baData.append((const char*)&mData.mLen, sizeof(mData.mLen));
		baData.append(baName);
		baData.append((const char*)&mPost, ProtcARFmt::gLenPost);
		
		//返回简单拼接后的数据，等待进一步打包
		return baData;
	}
};

//上报声能设备状态
struct PrtAcstRepState
{
	enum eIdtPlay
	{
		Playing    = 0,  //播放
		PlayPause  = 1,  //暂停
		PlayEnd    = 2,  //结束
	};

	enum eIdtMute
	{
		MuteOn = 0,  //静音
		MuteOff = 1,  //非静音
	};

	enum eIdtLoop
	{
		LoopAll = 0,  //列表循环
		LoopOne = 1,  //单曲循环
	};

	ProtcARFmt::Prep mPrep;

	struct DataArea
	{
		BYTE    mPlay;  //播放状态：0播放，1暂停，2结束
		BYTE    mMute;  //静音状态：0静音，1非静音
		BYTE    mVolPlayer;  //播放器音量0-100
		BYTE    mVolDevice;  //工控机音量0-100
		BYTE    mLoop;       //播放模式：0列表循环，1单曲循环
		DataArea()
		{
			memset(this, 0, sizeof(*this));
		}
	}mData;

	ProtcARFmt::Post mPost;

	PrtAcstRepState()
	{
		mPrep.mType = ProtcARFmt::MsgReport;
		mPrep.mCmdCode = ProtcARFmt::AcstRepState;
	}
};




/***************************************************
** 伺服设备
****************************************************/
//转台停止
typedef PrtARNoDataUniversal PrtRotStop;

//转台姿态控制
struct PrtRotTurnDirec
{
	enum eIdtCmd
	{
		TurnUp    = 0,
		TurnDown,
		TurnLeft,
		TurnRight,
	};

	ProtcARFmt::Prep mPrep;

	struct
	{
		BYTE    mIdt;  //0上，1下，2左，3右
	}mData;

	ProtcARFmt::Post mPost;
};

//转台转速设置
struct PrtRotSetVel
{
	enum eIdt
	{
		AziOnly   = 0,      //仅方位有效
		PitOnly   = 1,      //仅俯仰有效
		AziAndPit = 2,      //方位、俯仰均有效
		//其余皆无效
	};

	ProtcARFmt::Prep mPrep;

	struct
	{
		BYTE mIdt;       //标记位：0方位，1俯仰，2方位俯仰
		WORD mVelAzi;    //方位转速，量化单位0.01度每秒
		WORD mVelPit;    //俯仰转速，量化单位0.01度每秒
	}mData;

	ProtcARFmt::Post mPost;
};

//转台转到驻留角
struct PrtRotTurnTo
{
	enum eIdt
	{
		AziOnly = 0,      //仅方位有效
		PitOnly = 1,      //仅俯仰有效
		AziAndPit = 2,    //方位、俯仰均有效
		//其余皆无效
	};

	ProtcARFmt::Prep mPrep;

	struct 
	{
		BYTE  mIdt;    //标记位，见eIdt
		WORD  mAzi;    //方位角，量化单位0.01度
		short mPit;    //俯仰角，量化单位0.01度
	}mData;

	ProtcARFmt::Post mPost;
};

//转台归零
struct PrtRotZero
{
	enum eIdt
	{
		AziOnly = 0,      //仅方位有效
		PitOnly = 1,      //仅俯仰有效
		AziAndPit = 2,    //方位、俯仰均有效
		//其余皆无效
	};

	ProtcARFmt::Prep mPrep;

	struct
	{
		BYTE    mIdt;    //0方位，1俯仰，2方位俯仰
	}mData;

	ProtcARFmt::Post mPost;
};

//查询转台状态
typedef PrtARNoDataUniversal PrtRotQueryState;

//上报转台状态
struct PrtRotRepState
{
	enum eIdtMove
	{
		MoveArrived = 0,     //达位
		Moving = 1,          //运动中
		MoveInvalid = 0xFF,  //运动标记位无效，备用
	};

	ProtcARFmt::Prep mPrep;

	struct DataArea
	{
		BYTE    mIdtMove;  //当前备用，无效，0xFF
		WORD    mAzi;      //方位驻留角，0-359.99，量化单位0.01°
		short   mPit;      //俯仰驻留角，-90至90，量化单位0.01°
		WORD    mAziDelt;  //安装时方位偏角，0-359.99，量化单位0.01°
		short   mPitDelt;  //安装时俯仰偏角，-60至60，量化单位0.01°
		WORD    mVelAzi;   //方位转速，量化单位0.01°/s
		WORD    mVelPit;   //俯仰转速，量化单位0.01°/s

		DataArea()
		{
			memset(this, 0, sizeof(*this));
			mIdtMove = 0xFF;
		}
	}mData;

	ProtcARFmt::Post mPost;

	PrtRotRepState()
	{
		mPrep.mType = ProtcARFmt::MsgReport;
		mPrep.mCmdCode = ProtcARFmt::RotRepState;
	}
};




/************************************
** 综合管理
*************************************/
//工作模式控制
struct PrtSynWorkModel
{
	enum eIdtModel
	{
		WorkAuto    = 0,  //自动模式
		WorkManul   = 1,  //手动模式
	};

	ProtcARFmt::Prep mPrep;

	struct DataArea
	{
		BYTE    mIdt;    //0自动，1手动
	}mData;

	ProtcARFmt::Post mPost;
};

//自动模式参数设置
struct PrtSynPrmAuto
{
	ProtcARFmt::Prep mPrep;

	struct DataArea
	{
		WORD    mSecStart;    //每天工作起始时间0-1439，默认0
		WORD    mSecEnd;      //每天工作结束时间0-1439，默认0

		WORD    mAziStart;    //方位起始角，0-359.99，量化单位0.01°
		WORD    mAziEnd;      //方位终止角，0-359.99，量化单位0.01°
		WORD    mAziStep;     //方位扫描步进，5-180，量化单位0.01°
		 
		short   mPitStart;    //俯仰起始角，量化单位0.01°（当前采用该角度作为俯仰驻留角）
		short   mPitEnd;      //俯仰终止角，量化单位0.01°（当前无效，备用，填0）
		WORD    mPitStep;     //俯仰扫描步进，量化单位0.01°（当前无效，备用，填0）

		WORD    mVelAzi;      //方位转速，量化单位0.01度每秒
		WORD    mVelPit;	  //俯仰转速，量化单位0.01度每秒

		WORD    mSecResident; //驻留时间，s

		DataArea()
		{
			memset(this, 0, sizeof(*this));
		}
	}mData;

	ProtcARFmt::Post mPost;
};

//自动上报使能控制
struct PrtSynSetAutoRep
{
	enum eIdtRepAuto
	{
		RepAutoOn   = 0,  //自动上报开
		RepAutoOff  = 1,  //自动上报关
	};

	ProtcARFmt::Prep mPrep;

	struct DataArea
	{
		BYTE    mAcst;  //0自动上报开；1自动上报关
		BYTE    mRot;   //0自动上报开；1自动上报关
		BYTE    mSyn;   //0自动上报开；1自动上报关
	}mData;

	ProtcARFmt::Post mPost;
};

//工控机关机
typedef PrtARNoDataUniversal PrtSynPowerOffPC;

//查询综合状态
typedef PrtARNoDataUniversal PrtSynQueryState;

//查询自动参数
typedef PrtARNoDataUniversal PrtSynQueryPrmAuto;

//授时
struct PrtSynTiming
{
	ProtcARFmt::Prep mPrep;

	struct DataArea
	{
		unsigned short    mYear;
		unsigned char     mMon;
		unsigned char     mDay;
		unsigned char     mHour;    //0-23
		unsigned char     mMinute;  //0-59
		unsigned char     mSec;     //0-59

		DataArea()
		{
			memset(this, 0, sizeof(DataArea));
		}
	}mData;

	ProtcARFmt::Post mPost;

	PrtSynTiming()
	{
		mPrep.mType = ProtcARFmt::MsgSet;
		mPrep.mCmdCode = ProtcARFmt::SynTiming;
	}
};

//上报综合状态
struct PrtSynRepState
{
	enum eIdtModel
	{
		WorkAuto  = 0,  //自动模式
		WorkManul = 1,  //手动模式
	};

	enum eIdtRepAuto
	{
		RepAutoOn = 0,  //自动上报开
		RepAutoOff = 1,  //自动上报关
	};

	ProtcARFmt::Prep mPrep;

	struct DataArea
	{
		PrtSynWorkModel::DataArea    mWorkModel;  //工作模式：0自动；1手动
		PrtSynSetAutoRep::DataArea   mRepAuto;    //自动上报使能：0自动上报开；1自动上报关
		DataArea()
		{
			memset(this, 0, sizeof(*this));
		}
	}mData;

	ProtcARFmt::Post mPost;

	PrtSynRepState()
	{
		mPrep.mType = ProtcARFmt::MsgReport;
		mPrep.mCmdCode = ProtcARFmt::SynRepState;
	}
};

//上报自动模式参数
struct PrtSynRepPrmAuto
{
	ProtcARFmt::Prep          mPrep;
	PrtSynPrmAuto::DataArea   mData;
	ProtcARFmt::Post          mPost;

	PrtSynRepPrmAuto()
	{
		mPrep.mType = ProtcARFmt::MsgReport;
		mPrep.mCmdCode = ProtcARFmt::SynRepPrmAuto;
	}
};


#pragma pack(pop)

#endif  //PROTOCOLACSTROT_H_20210312_1406


