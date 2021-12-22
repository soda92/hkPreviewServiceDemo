#ifndef HCSDK_H
#define HCSDK_H

namespace HK {
    bool initSDK();
    bool cleanupSDK();
    long login(const char* ip);
    bool logout(long userID);
    long preview(long long hwnd, long userID);
    bool stop_preview(long preview_id);
    bool set_osd(long user_id,
                 const char* speed, const char* distance,
                 const char* train_order, const char* train_number,
                 const char* location, const char* driver_number,
                 bool is_record, bool is_usb);
    bool set_time(long user_id, unsigned long year,unsigned long month,unsigned long day,
                  unsigned long hour,unsigned long minute,unsigned long second);
    bool sync_time(long user_id);
}


#endif // HCSDK_H
