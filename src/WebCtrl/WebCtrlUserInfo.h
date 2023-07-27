#pragma once

#include <QString>

class WebCtrlUserInfo
{
public:
    //平台本次链接的网络地址
    QString            mIP;
    unsigned short     mPort;


    WebCtrlUserInfo()
    {
        mIP = QString("0.0.0.0");
        mPort = 0;
    }

    WebCtrlUserInfo(const QString& ip, unsigned short port)
    {
        mIP = ip;
        mPort = port;
    }

    inline bool isValid() const
    {
        return (!mIP.isEmpty() && mIP != QString("0.0.0.0") && mPort != 0);
    }

    inline bool isInvalid() const
    {
        return (mIP.isEmpty() || mIP == QString("0.0.0.0") || mPort == 0);
    }

    inline bool operator== (const WebCtrlUserInfo& other) const
    {
        return (mIP == other.mIP && mPort == other.mPort);
    }

    inline bool operator != (const WebCtrlUserInfo& other) const
    {
        return (mIP != other.mIP || mPort != other.mPort);
    }

    inline bool operator> (const WebCtrlUserInfo& other) const
    {
        if (mIP > other.mIP) { return true; }
        if (mPort > other.mPort) { return true; }
        return false;
    }

    inline bool operator>= (const WebCtrlUserInfo& other) const
    {
        if (mIP >= other.mIP) { return true; }
        if (mPort >= other.mPort) { return true; }
        return false;
    }

    inline bool operator< (const WebCtrlUserInfo& other) const
    {
        if (mIP < other.mIP) { return true; }
        if (mPort < other.mPort) { return true; }
        return false;
    }

    inline bool operator<= (const WebCtrlUserInfo& other) const
    {
        if (mIP <= other.mIP) { return true; }
        if (mPort <= other.mPort) { return true; }
        return false;
    }
};

