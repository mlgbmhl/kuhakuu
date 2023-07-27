#include "WebCtrlUserManager.h"

WebCtrlUserManager::WebCtrlUserManager()
    : QObject(NULL),
    mNumMax(255)
{
    mUserInfos.clear();
}

WebCtrlUserManager::~WebCtrlUserManager()
{

}

WebCtrlUserManager* WebCtrlUserManager::GetInstance()
{
    static WebCtrlUserManager ins;
    return &ins;
}

bool WebCtrlUserManager::appendUser(const QString & ip, const ushort & port)
{
    return appendUser(UserInfo(ip, port));
}

bool WebCtrlUserManager::appendUser(const UserInfo & user)
{
    if (mUserInfos.contains(user) || isFull())
    {
        return false;
    }

    mUserInfos.append(user);
    return true;
}

bool WebCtrlUserManager::removeUser(const QString & ip, const ushort & port)
{
    return removeUser(UserInfo(ip, port));
}

bool WebCtrlUserManager::removeUser(const UserInfo & user)
{
    if (!mUserInfos.contains(user))
    {
        return false;
    }

    mUserInfos.removeOne(user);
    return true;
}

QVector<WebCtrlUserInfo> WebCtrlUserManager::getUserInfoAll() const
{
    return mUserInfos;
}

bool WebCtrlUserManager::isExistedUser(const QString & ip, ushort port) const
{
    return isExistedUser(UserInfo(ip, port));
}

bool WebCtrlUserManager::isExistedUser(const UserInfo & user) const
{
    return mUserInfos.contains(user);
}

bool WebCtrlUserManager::isAvailableUser(const QString & ip, const ushort & port) const
{
    return isAvailableUser(UserInfo(ip, port));
}

bool WebCtrlUserManager::isAvailableUser(const UserInfo & user) const
{
    return mUserInfos.contains(user);
}

