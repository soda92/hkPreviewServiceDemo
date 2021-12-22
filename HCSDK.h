#ifndef HCSDK_H
#define HCSDK_H

namespace HK {
    bool initSDK();
    bool cleanupSDK();
    long login(const char* ip);
    bool logout(long userID);
    long preview(long long hwnd, long userID);
    bool stop_preview(long preview_id);
}


#endif // HCSDK_H
