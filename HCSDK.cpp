extern "C"{
    #include "HCNetSDK.h"
}
#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <QString>

namespace HK {
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

    bool set_osd(long user_id,
                 const char* speed, const char* distance,
                 const char* train_order, const char* train_number,
                 const char* location, const char* driver_number,
                 bool is_record, bool is_usb)
    {
        char OSDOne[100] = "";	 // top right
        char OSDTwo[100] = "";	 // bottom left
        char OSDThree[100] = ""; // bottom right
        NET_DVR_SHOWSTRING_V30 struShowString = { 0 };

        sprintf_s(OSDOne, "%skm/h %skm", speed, distance);
        QString str1 {"车次:%s 车号:%s"};
        char s_str1[100] = {0};
        memcpy(s_str1, str1.toLocal8Bit().data(), 50);
        sprintf_s(OSDTwo, s_str1, train_order, train_number);

        QString str2 {"%s 司机:%s"};
        char s_str2[100] = {0};
        memcpy(s_str2, str2.toLocal8Bit().data(), 50);
        sprintf_s(OSDThree, s_str2, location, driver_number);

        struShowString.struStringInfo[0].wShowString = 1;
        struShowString.struStringInfo[0].wStringSize = static_cast<WORD>(strlen(OSDOne));
        strcpy_s(struShowString.struStringInfo[0].sString, OSDOne);
        struShowString.struStringInfo[0].wShowStringTopLeftX = 500;
        struShowString.struStringInfo[0].wShowStringTopLeftY = 32;

        struShowString.struStringInfo[1].wShowString = 1;
        struShowString.struStringInfo[1].wStringSize = static_cast<WORD>(strlen(OSDTwo));
        strcpy_s(struShowString.struStringInfo[1].sString, OSDTwo);
        struShowString.struStringInfo[1].wShowStringTopLeftX = 0;
        struShowString.struStringInfo[1].wShowStringTopLeftY = 540;

        struShowString.struStringInfo[2].wShowString = 1;
        struShowString.struStringInfo[2].wStringSize = static_cast<WORD>(strlen(OSDThree));
        strcpy_s(struShowString.struStringInfo[2].sString, OSDThree);
        struShowString.struStringInfo[2].wShowStringTopLeftX = 360; //360 320
        struShowString.struStringInfo[2].wShowStringTopLeftY = 540;

        if (is_record)
        {
            struShowString.struStringInfo[3].wShowString = 1;
            struShowString.struStringInfo[3].wShowStringTopLeftX = 30;
            struShowString.struStringInfo[3].wShowStringTopLeftY = 100;
            char show_str[100] = {0};
            if(is_usb)
            {
                strcpy_s(show_str, 100, "REC USB");
            }else{
                strcpy_s(show_str, 100, "REC");

            }
            struShowString.struStringInfo[3].wStringSize = static_cast<WORD>(strlen(show_str));
            strcpy_s(struShowString.struStringInfo[3].sString, show_str);
        }
        else
        {
            struShowString.struStringInfo[3].wShowString = 0;
        }

        struShowString.dwSize = sizeof(struShowString);

        return NET_DVR_SetDVRConfig(
                    user_id, NET_DVR_SET_SHOWSTRING_V30, 1,
                    &struShowString, sizeof(NET_DVR_SHOWSTRING_V30));
    }

    bool set_time(long user_id, unsigned long year,unsigned long month,unsigned long day,
                  unsigned long hour,unsigned long minute,unsigned long second) {
        NET_DVR_TIME NvrTime = {0};
        NvrTime.dwYear = year;
        NvrTime.dwMonth = month;
        NvrTime.dwDay = day;

        NvrTime.dwHour = hour;
        NvrTime.dwMinute = minute;
        NvrTime.dwSecond = second;

        return NET_DVR_SetDVRConfig(user_id, NET_DVR_SET_TIMECFG, 0, &NvrTime, sizeof(NvrTime));
    }
    bool sync_time(long user_id) {
        NET_DVR_TIME NvrTime = {0};
        boost::posix_time::ptime timeLocal = boost::posix_time::second_clock::local_time();
        NvrTime.dwYear = timeLocal.date().year();
        NvrTime.dwMonth = timeLocal.date().month();
        NvrTime.dwDay = timeLocal.date().day();

        NvrTime.dwHour = timeLocal.time_of_day().hours();
        NvrTime.dwMinute = timeLocal.time_of_day().minutes();
        NvrTime.dwSecond = timeLocal.time_of_day().seconds();
        return NET_DVR_SetDVRConfig(user_id, NET_DVR_SET_TIMECFG, 0, &NvrTime, sizeof(NvrTime));
    }
}
