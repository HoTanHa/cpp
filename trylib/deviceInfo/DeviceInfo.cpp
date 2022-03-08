#include "DeviceInfo.h"
#include "../nlohmann/json.hpp"

#include <curl/curl.h>
#include <cstring>

using json = nlohmann::json;

const int kMaxLengthQuery = 2048;
const int kMaxNumCam = 4;

std::mutex DeviceInfo::mLocker_;
DeviceInfo *DeviceInfo::mInstancePtr = nullptr;

static DeviceInfoCallback deivce_info_callback_default;
static void deivce_info_callback_default(const char *response)
{
    printf("Callback: %s\r\n", response);
    json jsRes = json::parse(response);
    for (json::iterator it = jsRes.begin(); it != jsRes.end(); ++it)
    {
        std::cout << it.key() << ": " << it.value() << '\n';
    }
    if (jsRes.contains("cmd"))
    {
        auto cmd = jsRes.at("cmd");
        std::cout << cmd << "\n";
    }
    
    
}
static size_t write_callback_cURL(void *contents, size_t size, size_t nmemb, void *userp)
{
    char *cnt = (char *)contents;
    if (userp != NULL && strlen((char *)contents) < kMaxLengthQuery)
    {
        strcpy((char *)userp, (char *)contents);
    }
    return size * nmemb;
}

DeviceInfo::DeviceInfo(/* args */)
{
    std::cout << "Constructor Info Device" << std::endl;
    num_cam_ = 0;

    cam_status_ = new int[kMaxNumCam];
    stream_status_ = new int[kMaxNumCam];

    latitude_ = 0.0f;
    longtude_ = 0.0f;
    speed_ = 0;

    cpu_percent_ = 0.0f;
    sdCard_percent_ = 0.0f;
    sdCart_status_ = 0;
    key_status_ = 0;
    adc_value_ = 0.0f;
    ip_device_ = nullptr;
    imei1_ = nullptr;
    imei2_ = nullptr;
    version_code_ = nullptr;

    is_sending_info_ = 0;
    callback_ = &deivce_info_callback_default;
}

char *DeviceInfo::get_queryDeviceInfo()
{
    char *query = new char[kMaxLengthQuery];
    char queryParam[100];

    time_t timeStamp = time(NULL) * 1000;

    sprintf(query, "&id=%d&lat=%.6f&lon=%.6f&speed=%d&timestamp=%ld",
            serial_number_, latitude_, longtude_, speed_, timeStamp);
    // strcat(query, queryParam);
    sprintf(queryParam, "&hdop=0.000&altitude=0.0&heading=0.0");
    strcat(query, queryParam);
    sprintf(queryParam, "&adc=%.1f&cam=0&live=0&life=%d", adc_value_, count_life_);
    strcat(query, queryParam);
    sprintf(queryParam, "&svn=1&ip=127.0.0.1&disk=90&cpu=10&temp=10,20,10,10&key=%d",
            key_status_);
    strcat(query, queryParam);
    sprintf(queryParam, "&sd=%d&T90=1&cam1def=0,0,0,0&cam2def=0,0,0,0&cam1set=0,0,0,0&cam2set=0,0,0,0",
            sdCart_status_);
    strcat(query, queryParam);
    sprintf(queryParam, "&imei1=noSim&imei2=noSim");
    strcat(query, queryParam);
    return query;
}

void *DeviceInfo::send_deviceInfo_thread(void *arg)
{
    DeviceInfo *info = (DeviceInfo *)arg;
    char *url;
    CURL *curl;
    CURLcode res;
    struct curl_slist *headerlist = NULL;

    info->is_sending_info_ = 1;
    // curl_global_init(CURL_GLOBAL_ALL);
    headerlist = curl_slist_append(headerlist, "Expect:");
    headerlist = curl_slist_append(headerlist, "Connection: Close");
    curl = curl_easy_init();
    if (curl)
    {
        url = (char *)malloc(kMaxLengthQuery);
        char *query = info->get_queryDeviceInfo();
        sprintf(url, "http://live1.adsun.vn:8100/api/manage/deviceInfo?%s", query);

        curl_easy_setopt(curl, CURLOPT_URL, url);

        /* Forcing HTTP/3 will make the connection fail if the server isn't
           accessible over QUIC + HTTP/3 on the given host and port.
           Consider using CURLOPT_ALTSVC instead! */
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);

        char *response_buffer = (char *)malloc(kMaxLengthQuery);
        long response_code;
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response_buffer);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback_cURL);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10); // 10s
        // curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_3);
        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        else
        {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
            info->callback_(response_buffer);
        }
        /* always cleanup */
        curl_easy_cleanup(curl);
        free(response_buffer);
        free(query);
        free(url);
    }
    info->is_sending_info_ = 0;
    return NULL;
}

void DeviceInfo::set_serialNumber(int serial)
{
    this->serial_number_ = serial;
}

void DeviceInfo::set_numCam(int num)
{
    this->num_cam_ = num;
}

void DeviceInfo::set_location(double lat, double lon, int speed)
{
    this->latitude_ = lat;
    this->longtude_ = lon;
    this->speed_ = speed;
}

void DeviceInfo::get_location(double &lat, double &lon, int &speed)
{
    lat = this->latitude_;
    lon = this->longtude_;
    speed = this->speed_;
}

void DeviceInfo::set_cameraStatus(int camId, int isExist)
{
    if (camId >= 0 && camId < kMaxNumCam)
    {
        this->cam_status_[camId] = isExist;
    }
}
void DeviceInfo::set_liveStreamStatus(int camId, int isLive)
{
    if (camId >= 0 && camId < kMaxNumCam)
    {
        this->stream_status_[camId] = isLive;
    }
}

void DeviceInfo::set_cpuPercent(double percent)
{
    this->cpu_percent_ = percent;
}

void DeviceInfo::set_sdCard(int status, double percent)
{
    this->sdCart_status_ = status;
    this->sdCard_percent_ = percent;
}

void DeviceInfo::set_keyStatus(int status)
{
    this->key_status_ = status;
}

int DeviceInfo::get_keyStatus()
{
    return this->key_status_;
}

void DeviceInfo::set_imei(char *imei1, char *imei2)
{
    if (this->imei1_ != nullptr)
    {
        delete this->imei1_;
        this->imei1_ = nullptr;
    }
    if (this->imei2_ != nullptr)
    {
        delete this->imei2_;
        this->imei2_ = nullptr;
    }

    if (imei1 != NULL)
    {
        this->imei1_ = new std::string{imei1};
    }
    if (imei2 != NULL)
    {
        this->imei2_ = new std::string{imei2};
    }
}

void DeviceInfo::send_DeviceInfo()
{
    if (this->is_sending_info_)
    {
        return;
    }
    this->count_life_++;
    pthread_create(&thread_sendInfo_, NULL, send_deviceInfo_thread, this);
}

void DeviceInfo::set_DeviceInfoCallback(DeviceInfoCallback *callback)
{
    if (callback != nullptr)
    {
        this->callback_ = callback;
    }
}
