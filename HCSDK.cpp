#include "HCNetSDK.h"
#include <string>

namespace HK{

    bool initSDK(){
        bool isok = NET_DVR_Init();
        if(isok)
        {
            NET_DVR_SetConnectTime(5000, 1);
            NET_DVR_SetReconnect(10000, true);
            return true;
        }
        return false;
    }

    bool cleanupSDK(){
        return NET_DVR_Cleanup();
    }

    long login(const char* ip){
        NET_DVR_DEVICEINFO_V30 DeviceInfoTmp;
        char sDVRIP[100] = {'\0'};
        strcpy_s(sDVRIP, 100, ip);
        char sUserName[100]{"admin"};
        char sPassword[100]{"hk123456"};
        return NET_DVR_Login_V30(sDVRIP,8000,sUserName,sPassword,&DeviceInfoTmp);
    }

    bool logout(long userID){
        return NET_DVR_Logout(userID);
    }

    long preview(long long hwnd, long userID){
//        NET_DVR_CLIENTINFO ClientInfo;
//        ClientInfo.hPlayWnd     = reinterpret_cast<HWND>(hwnd);
//        ClientInfo.lChannel     = 1;
//        ClientInfo.lLinkMode    = 0;
//        ClientInfo.sMultiCastIP = NULL;
//        long IRealPlayHandle= NET_DVR_RealPlay_V30(userID,&ClientInfo,NULL,NULL,TRUE);
//        return IRealPlayHandle;

        NET_DVR_PREVIEWINFO struPlayInfo = { 0 };
        struPlayInfo.hPlayWnd = reinterpret_cast<HWND>(hwnd);
        struPlayInfo.lChannel = 1;

        struPlayInfo.dwStreamType = 0;
        struPlayInfo.dwLinkMode = 0;

        return  NET_DVR_RealPlay_V40(userID, &struPlayInfo, NULL, NULL);
    }

    bool stop_preview(long preview_id){
        return NET_DVR_StopRealPlay(preview_id);
    }
}
