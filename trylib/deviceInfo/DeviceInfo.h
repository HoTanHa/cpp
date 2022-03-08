#ifndef __DEVICEINFO_H__
#define __DEVICEINFO_H__

#include <pthread.h>

#include <iostream>
#include <string>
#include <mutex>

typedef void (DeviceInfoCallback)(const char *response);

class DeviceInfo
{
private:
    /* data */
    static DeviceInfo *mInstancePtr;
    static std::mutex mLocker_;
    DeviceInfo(/* args */);

    int count_life_;
    int serial_number_;
    int num_cam_;

    int *cam_status_;
    int *stream_status_;

    double latitude_;
    double longtude_;
    int speed_;

    double cpu_percent_;
    double sdCard_percent_;
    int sdCart_status_;
    int key_status_;
    double adc_value_;
    std::string *ip_device_;
    std::string *imei1_;
    std::string *imei2_;
    std::string *version_code_;

    int is_sending_info_;
    pthread_t thread_sendInfo_;
    char *get_queryDeviceInfo();
    static void *send_deviceInfo_thread(void *arg);
    DeviceInfoCallback *callback_;

public:
    static DeviceInfo *getInstance()
    {
        mLocker_.lock();
        if (nullptr == mInstancePtr)
        {
            mInstancePtr = new DeviceInfo();
        }
        mLocker_.unlock();
        return mInstancePtr;
    }

    void set_serialNumber(int serial);
    void set_numCam(int num);
    void set_location(double lat, double lon, int speed);
    void get_location(double &lat, double &lon, int &speed);

    void set_cameraStatus(int camId, int isExist);
    void set_liveStreamStatus(int camId, int isLive);

    void set_cpuPercent(double percent);
    void set_sdCard(int status, double percent);

    void set_keyStatus(int status);
    int get_keyStatus();

    void set_imei(char* imei1, char* imei2);

    void send_DeviceInfo();



    void set_DeviceInfoCallback(DeviceInfoCallback *callback);
};

#endif //__DEVICEINFO_H__