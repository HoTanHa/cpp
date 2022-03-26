// #include deviceInfo/DeviceInfo.h
// #include webServer/HttpServer.h
// #include listBuffer/ListNodeBuffer.h

#include "nlohmann/json.hpp"
#include "deviceInfo/Client.h"
#include <unistd.h>
#include <cstring>
#include <curl/curl.h>
#include <iostream>
#include <chrono>
#include <regex>
#include <bits/stdc++.h>

using json = nlohmann::json;
struct memory
{
    char *response;
    size_t size;
};
void ignoreLine()
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
static size_t write_callback_cURL(void *data, size_t size, size_t nmemb, void *userp)
{

    size_t realsize = size * nmemb;
    struct memory *mem = (struct memory *)userp;

    char *ptr = (char *)realloc(mem->response, mem->size + realsize + 1);
    if (ptr == NULL)
        return 0; /* out of memory! */

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;
    return realsize;
}

char *getAllSocketClient()
{
    char *url = (char *)"http://live1.adsun.vn:8100/api/manage/getAllSocketClient";
    CURL *curl;
    CURLcode res;
    struct curl_slist *headerlist = NULL;
    struct memory chunk = {0};
    curl_global_init(CURL_GLOBAL_ALL);
    headerlist = curl_slist_append(headerlist, "Expect:");
    headerlist = curl_slist_append(headerlist, "Connection: Close");
    curl = curl_easy_init();
    if (curl)
    {

        curl_easy_setopt(curl, CURLOPT_URL, url);

        /* Forcing HTTP/3 will make the connection fail if the server isn't
           accessible over QUIC + HTTP/3 on the given host and port.
           Consider using CURLOPT_ALTSVC instead! */
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);

        long response_code;
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
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
        }
        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    return chunk.response;
}

char *getAllDeviceDataKeyOn()
{
    // char *url = (char *)"http://live1.adsun.vn:8100/api/manage/getAllDeviceDataKeyOn";
    char *url = (char *)"http://live1.adsun.vn:8100/api/manage/getAllDeviceInfo";
    CURL *curl;
    CURLcode res;
    struct curl_slist *headerlist = NULL;
    struct memory chunk = {0};
    curl_global_init(CURL_GLOBAL_ALL);
    headerlist = curl_slist_append(headerlist, "Expect:");
    headerlist = curl_slist_append(headerlist, "Connection: Close");
    headerlist = curl_slist_append(headerlist, "pass: haphomatau");
    curl = curl_easy_init();
    if (curl)
    {

        curl_easy_setopt(curl, CURLOPT_URL, url);

        /* Forcing HTTP/3 will make the connection fail if the server isn't
           accessible over QUIC + HTTP/3 on the given host and port.
           Consider using CURLOPT_ALTSVC instead! */
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);

        long response_code;
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback_cURL);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30); // 10s
        // curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_3);
        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        else
        {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        }
        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    return chunk.response;
}

char *getDeviceInfo(long serial)
{
    char url[200];
    CURL *curl;
    CURLcode res;

    struct memory chunk = {0};
    struct curl_slist *headerlist = NULL;

    headerlist = curl_slist_append(headerlist, "Expect:");
    headerlist = curl_slist_append(headerlist, "token:2d312b6d5ae84eff963181bb1068dad8");
    headerlist = curl_slist_append(headerlist, "Connection:close");
    curl = curl_easy_init();
    if (curl)
    {
        sprintf(url, "http://live1.adsun.vn:8100/api/manage/getDeviceInfoById?device=%ld", serial);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback_cURL);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
        // curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_3);
        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    usleep(200000);
    return chunk.response;
}

void addSerialFaultToFile(long serial)
{
    std::fstream f;
    f.open("serial.txt", std::ios::app);
    if (f.good())
    {
        f << serial << "\n";
    }
    f.close();
}

std::vector<long> serialFault;
void readSerialFaultChecked()
{
    std::fstream f;
    std::string serial;
    f.open("serial.txt", std::ios::in);
    if (f)
    {
        while (!f.eof())
        {
            std::getline(f, serial);
            if (serial.length() == 9)
            {
                serialFault.push_back(std::atol(serial.c_str()));
            }
        }
    }
    f.close();
}

std::vector<long> serialChecked;
void readSerialChecked()
{
    std::fstream f;
    std::string serial;
    f.open("serialChecked.txt", std::ios::in);
    if (f)
    {
        while (!f.eof())
        {
            std::getline(f, serial);
            if (serial.length() == 9)
            {
                serialChecked.push_back(std::atol(serial.c_str()));
            }
        }
    }
    f.close();
}

void getListDeviceUseApiRequestAndSaveFile()
{
    time_t timeGetRecord;
    time(&timeGetRecord);

    std::cout << "get list device use api request..." << timeGetRecord << "\n";
    char *dataKeyOn1 = getAllDeviceDataKeyOn();
    json js = json::parse(dataKeyOn1);
    js["timeGetRecord"] = timeGetRecord;
    std::fstream f;
    f.open("data.json", std::ios::out);
    if (f.good())
    {
        f << js;
    }
    f.close();
    delete dataKeyOn1;
}

std::string *readDataDeviceKeyOnFile()
{
    std::fstream f;
    std::string serial;
    std::string *dataFile = new std::string("");
    f.open("data.json");
    if (f)
    {
        while (!f.eof())
        {
            std::getline(f, serial);
            dataFile->append(serial);
        }
    }
    f.close();
    return dataFile;
}

long getTimeLastRequest()
{
    time_t timeGetRecord;
    std::ifstream i("data.json");
    json jsRes = json::parse(i);

    if (!jsRes.contains("status"))
    {
        return 0;
    }

    int64_t status = jsRes.at("status").get<int64_t>();
    if (status == 0)
    {
        return 0;
    }
    timeGetRecord = jsRes.at("timeGetRecord").get<uint64_t>();
    return timeGetRecord;
}

void getListDevice()
{
    time_t timeGetRecord;
    struct tm *timeinfo;
    std::ifstream i("data.json");
    json jsRes = json::parse(i);

    if (!jsRes.contains("status"))
    {
        return;
    }

    int64_t status = jsRes.at("status").get<int64_t>();
    if (status == 0)
    {
        return;
    }
    timeGetRecord = jsRes.at("timeGetRecord").get<uint64_t>();
    timeinfo = localtime(&timeGetRecord);
    std::cout << "The current date/time is: " << asctime(timeinfo) << "\r\n";

    json dataDeviceInfos = jsRes.at("data");

    std::vector<std::vector<long>> serialNumberKeyOns;
    std::vector<Client> deviceClient;
    serialNumberKeyOns.resize(10);
    int numberOfserial;
    if (dataDeviceInfos.is_array())
    {
        numberOfserial = dataDeviceInfos.size();
        for (json::iterator it = dataDeviceInfos.begin(); it != dataDeviceInfos.end(); ++it)
        {
            json client = it.value();
            Client device(&client, timeGetRecord);
            deviceClient.push_back(device);
        }
    }
    int numOfSerial6 = 0;
    int numOfSerial8 = 0;
    int numOfSerial9 = 0;
    for (auto it = deviceClient.begin(); it != deviceClient.end(); ++it)
    {

        if (it->is_serial_8_quectel())
        {
            numOfSerial8++;
        }
        else if (it->is_serial_6_simcom())
        {
            numOfSerial6++;
        }
        else
        {
            numOfSerial9++;
        }
    }

    std::cout << "Number of serial is KeyOn: " << numberOfserial << "\r\n";
    std::cout << "Number of serial is KeyOn seri9: " << numOfSerial9 << "\r\n";
    std::cout << "Number of serial is KeyOn seri8: " << numOfSerial8 << "\r\n";
    std::cout << "Number of serial is KeyOn seri6: " << numOfSerial6 << "\r\n";

    std::cout << "\r\n";
    std::cout << "\r\n";
}
void getListDeviceCamError(bool isSerial6, int vsMin, int vsMax)
{
    time_t timeGetRecord;
    struct tm *timeinfo;
    std::ifstream i("data.json");
    json jsRes = json::parse(i);

    if (!jsRes.contains("status"))
    {
        return;
    }

    int64_t status = jsRes.at("status").get<int64_t>();
    if (status == 0)
    {
        return;
    }
    timeGetRecord = jsRes.at("timeGetRecord").get<uint64_t>();
    timeinfo = localtime(&timeGetRecord);
    std::cout << "The current date/time is: " << asctime(timeinfo) << "\r\n";

    json dataDeviceInfos = jsRes.at("data");

    std::vector<std::vector<long>> serialNumberKeyOns;
    std::vector<Client> deviceClient;
    serialNumberKeyOns.resize(10);
    int numberOfserial = 0;
    int altitude;
    long serialNumber;
    if (dataDeviceInfos.is_array())
    {
        for (json::iterator it = dataDeviceInfos.begin(); it != dataDeviceInfos.end(); ++it)
        {
            json client = it.value();
            Client device(&client, timeGetRecord);
            deviceClient.push_back(device);
        }
    }
    for (auto it = deviceClient.begin(); it != deviceClient.end(); ++it)
    {
        serialNumber = it->get_serial_number();
        if (!isSerial6)
        {
            if (it->is_serial_8_quectel() && it->isCarKeyOn())
            {
                altitude = it->get_cam_error();
                serialNumberKeyOns.at(altitude).push_back(serialNumber);
            }
        }
        else if (it->is_serial_6_simcom())
        {
            if (it->get_version() > vsMin && it->get_version() < vsMax && it->isCarKeyOn())
            {
                altitude = it->get_cam_error();
                serialNumberKeyOns.at(altitude).push_back(serialNumber);
            }
        }
    }

    for (size_t i = 0; i < serialNumberKeyOns.size(); i++)
    {
        numberOfserial += serialNumberKeyOns.at(i).size();
        if (serialNumberKeyOns.at(i).size() == 0)
        {
            continue;
        }

        std::sort(serialNumberKeyOns.at(i).begin(), serialNumberKeyOns.at(i).end());
        std::cout << "....Altitude: " << std::setw(3) << i
                  << "\t\tNumber: " << std::setw(5) << serialNumberKeyOns.at(i).size()
                  << "\r\n";

        if (i != 0)
        {
            std::cout << "\t";
            for (auto it = serialNumberKeyOns.at(i).begin(); it != serialNumberKeyOns.at(i).end(); ++it)
            {
                std::cout << *it << ", ";
            }
            std::cout << "\r\n";
        }
        std::cout << "\r\n";
    }
    std::cout << "..........SUM : " << numberOfserial << "..............\r\n\r\n";
}

void getDeviceSdcard95percent()
{
    time_t timeGetRecord;
    struct tm *timeinfo;

    std::ifstream i("data.json");
    json jsRes = json::parse(i);
    if (!jsRes.contains("status"))
    {
        return;
    }

    int64_t status = jsRes.at("status").get<int64_t>();
    if (status == 0)
    {
        return;
    }
    timeGetRecord = jsRes.at("timeGetRecord").get<uint64_t>();
    timeinfo = localtime(&timeGetRecord);
    std::cout << "The current date/time is get data: " << asctime(timeinfo) << "\r\n";
    json dataDeviceInfos = jsRes.at("data");

    std::vector<long> serialSdCard90;
    std::vector<Client> deviceClient;
    if (dataDeviceInfos.is_array())
    {
        for (json::iterator it = dataDeviceInfos.begin(); it != dataDeviceInfos.end(); ++it)
        {
            json client = it.value();
            Client device(&client, timeGetRecord);
            deviceClient.push_back(device);
        }
    }
    for (auto it = deviceClient.begin(); it != deviceClient.end(); ++it)
    {
        if (it->is_serial_6_simcom() || it->is_serial_8_quectel())
        {
            if (it->is_sdCard_error())
            {
                serialSdCard90.push_back(it->get_serial_number());
            }
        }
    }

    std::cout << "Number of serial has sdCard >=90%: " << serialSdCard90.size() << "\r\n";
    std::sort(serialSdCard90.begin(), serialSdCard90.end());
    for (auto it = serialSdCard90.begin(); it != serialSdCard90.end(); ++it)
    {
        std::cout << *it << ", ";
    }
    std::cout << "\r\n";
    std::cout << "\r\n";
}

void getDeviceSerial6use2g()
{
    time_t timeGetRecord;
    struct tm *timeinfo;

    std::ifstream i("data.json");
    json jsRes = json::parse(i);
    if (!jsRes.contains("status"))
    {
        return;
    }

    int64_t status = jsRes.at("status").get<int64_t>();
    if (status == 0)
    {
        return;
    }
    timeGetRecord = jsRes.at("timeGetRecord").get<uint64_t>();
    timeinfo = localtime(&timeGetRecord);
    std::cout << "The current date/time is get data: " << asctime(timeinfo) << "\r\n";
    json dataDeviceInfos = jsRes.at("data");

    std::vector<long> serial6is2G;
    if (dataDeviceInfos.is_array())
    {
        for (json::iterator it = dataDeviceInfos.begin(); it != dataDeviceInfos.end(); ++it)
        {
            json client = it.value();
            Client device(&client, timeGetRecord);
            if (device.is_gsm_2G())
            {
                serial6is2G.push_back(device.get_serial_number());
            }
        }
    }

    std::cout << "Number of serial 6 is using 2G : " << serial6is2G.size() << "\r\n";
    std::sort(serial6is2G.begin(), serial6is2G.end());
    for (auto it = serial6is2G.begin(); it != serial6is2G.end(); ++it)
    {
        std::cout << *it << ", ";
    }
    std::cout << "\r\n";
    std::cout << "\r\n";
}

void getDeviceNotConnectted()
{
    time_t timeGetRecord;
    struct tm *timeinfo;

    std::ifstream i("data.json");
    json jsRes = json::parse(i);
    if (!jsRes.contains("status"))
    {
        return;
    }

    int64_t status = jsRes.at("status").get<int64_t>();
    if (status == 0)
    {
        return;
    }
    timeGetRecord = jsRes.at("timeGetRecord").get<uint64_t>();
    timeinfo = localtime(&timeGetRecord);
    std::cout << "The current date/time is get data: " << asctime(timeinfo) << "\r\n";
    json dataDeviceInfos = jsRes.at("data");

    std::vector<long> serialDisconnect;
    if (dataDeviceInfos.is_array())
    {
        for (json::iterator it = dataDeviceInfos.begin(); it != dataDeviceInfos.end(); ++it)
        {
            json client = it.value();
            Client device(&client, timeGetRecord);
            if (device.isDeviceNotConnectTooLong())
            {
                serialDisconnect.push_back(device.get_serial_number());
            }
        }
    }

    std::cout << "Number of serial Disconnect: " << serialDisconnect.size() << "\r\n";
    std::sort(serialDisconnect.begin(), serialDisconnect.end());
    for (auto it = serialDisconnect.begin(); it != serialDisconnect.end(); ++it)
    {
        std::cout << *it << ", ";
    }
    std::cout << "\r\n";
    std::cout << "\r\n";
}

void getNumberDeviceAsVersion(int version)
{
    time_t timeGetRecord;
    struct tm *timeinfo;

    std::ifstream i("data.json");
    json jsRes = json::parse(i);
    if (!jsRes.contains("status"))
    {
        return;
    }

    int64_t status = jsRes.at("status").get<int64_t>();
    if (status == 0)
    {
        return;
    }
    timeGetRecord = jsRes.at("timeGetRecord").get<uint64_t>();
    timeinfo = localtime(&timeGetRecord);
    std::cout << "The current date/time is get data: " << asctime(timeinfo) << "\r\n";
    json dataDeviceInfos = jsRes.at("data");

    std::vector<Client> deviceClient;
    int NumDevice = 0;
    if (dataDeviceInfos.is_array())
    {
        for (json::iterator it = dataDeviceInfos.begin(); it != dataDeviceInfos.end(); ++it)
        {
            json client = it.value();
            Client device(&client, timeGetRecord);
            deviceClient.push_back(device);
        }
    }
    for (auto it = deviceClient.begin(); it != deviceClient.end(); ++it)
    {
        if (it->get_version() == version)
        {
            NumDevice++;
        }
    }

    std::cout << "Number of devices in version " << version << " :" << NumDevice << "\r\n";
    std::cout << "\r\n";
    std::cout << "\r\n";
}

void getTotalNumberDeviceAsVersion(bool is6)
{
    time_t timeGetRecord;
    struct tm *timeinfo;

    std::ifstream i("data.json");
    json jsRes = json::parse(i);
    if (!jsRes.contains("status"))
    {
        return;
    }

    int64_t status = jsRes.at("status").get<int64_t>();
    if (status == 0)
    {
        return;
    }
    timeGetRecord = jsRes.at("timeGetRecord").get<uint64_t>();
    timeinfo = localtime(&timeGetRecord);
    std::cout << "The current date/time is get data: " << asctime(timeinfo) << "\r\n";
    json dataDeviceInfos = jsRes.at("data");

    std::vector<int> versionClient;

    if (dataDeviceInfos.is_array())
    {
        for (json::iterator it = dataDeviceInfos.begin(); it != dataDeviceInfos.end(); ++it)
        {
            json client = it.value();
            Client device(&client, timeGetRecord);
            if (is6)
            {
                if (device.is_serial_6_simcom())
                {
                    versionClient.push_back(device.get_version());
                }
            }
            else
            {

                if (device.is_serial_8_quectel())
                {
                    versionClient.push_back(device.get_version());
                }
            }
        }
    }

    std::sort(versionClient.begin(), versionClient.end());
    std::map<int, int> duplicate;
    auto beg = std::begin(versionClient) + 1;
    for (; beg != std::end(versionClient); ++beg)
    {
        if (*beg == *(beg - 1))
        {
            duplicate[*beg]++;
        }
    }
    for (const auto &i : duplicate)
    {
        std::cout << "Version: " << std::setw(5) << i.first << " has: " << std::setw(5) << i.second + 1 << " deivce" << '\n';
    }

    std::cout << "\r\n";
    std::cout << "\r\n";
}

void getTotalNumberDeviceSimCom()
{
    getTotalNumberDeviceAsVersion(true);
}
void getTotalNumberDeviceQuectel()
{
    getTotalNumberDeviceAsVersion(false);
}

void getDeviceHasSim2(int version)
{
    time_t timeGetRecord;
    struct tm *timeinfo;

    std::ifstream i("data.json");
    json jsRes = json::parse(i);
    if (!jsRes.contains("status"))
    {
        return;
    }

    int64_t status = jsRes.at("status").get<int64_t>();
    if (status == 0)
    {
        return;
    }
    timeGetRecord = jsRes.at("timeGetRecord").get<uint64_t>();
    timeinfo = localtime(&timeGetRecord);
    std::cout << "The current date/time is get data: " << asctime(timeinfo) << "\r\n";
    json dataDeviceInfos = jsRes.at("data");

    std::vector<long> serialSim2;
    if (dataDeviceInfos.is_array())
    {
        for (json::iterator it = dataDeviceInfos.begin(); it != dataDeviceInfos.end(); ++it)
        {
            json client = it.value();
            Client device(&client, timeGetRecord);
            if (device.get_version() == version && device.is_has_imei2())
            {
                serialSim2.push_back(device.get_serial_number());
            }
        }
    }

    std::cout << "Number of serial has sim2 in version..." << version << "....: " << serialSim2.size() << "\r\n";
    std::sort(serialSim2.begin(), serialSim2.end());
    for (auto it = serialSim2.begin(); it != serialSim2.end(); ++it)
    {
        std::cout << *it << ", ";
    }
    std::cout << "\r\n";
    std::cout << "\r\n";
}

double getDouble()
{
    double vs;
    while (1)
    {
        std::cin >> vs;
        if (std::cin.fail())
        {
            std::cin.clear(); // put us back in 'normal' operation mode
            ignoreLine();     // and remove the bad input
        }
        else
        {
            break;
        }
    }
    return vs;
}

long getLong()
{
    long vs;
    while (1)
    {
        std::cin >> vs;
        if (std::cin.fail())
        {
            std::cin.clear(); // put us back in 'normal' operation mode
            ignoreLine();     // and remove the bad input
        }
        else
        {
            break;
        }
    }
    return vs;
}

int main()
{
    /*
        // std::vector<long> serialVector;
        // readSerialFaultChecked();
        // std::sort(serialFault.begin(), serialFault.end());

        // readSerialChecked();
        // std::sort(serialChecked.begin(), serialChecked.end());
        // char *allClient = getAllSocketClient();
        // json jsRes = json::parse(allClient);
        // long serialNumber;
        // if (jsRes.contains("clients"))
        // {
        //     json cmd = jsRes.at("clients");
        //     if (cmd.is_array())
        //     {
        //         for (json::iterator it = cmd.begin(); it != cmd.end(); ++it)
        //         {
        //             json client = it.value();
        //             if (client.contains("deviceId") && client.at("deviceId").is_string())
        //             {
        //                 auto s2 = client.at("deviceId").get<std::string>();
        //                 serialNumber = atol(s2.c_str());
        //                 serialVector.push_back(serialNumber);
        //             }
        //         }
        //     }
        // }
        // delete allClient;

        // std::sort(serialVector.begin(), serialVector.end());
        // std::fstream f;
        // f.open("serialAll.txt", std::ios::out);
        // if (f.good())
        // {
        //     for (size_t i = 0; i < serialVector.size(); i++)
        //     {
        //         f << serialVector.at(i) << "\n";
        //     }
        // }
        // f.close();

        // char *responseInfo;
        // long serialToCheck;
        // for (size_t i = 0; i < serialVector.size(); i++)
        // {
        //     serialToCheck = serialVector.at(i);

        //     if (std::find(serialFault.begin(), serialFault.end(), serialToCheck) != serialFault.end())
        //     {
        //         continue;
        //     }

        //     if (std::find(serialChecked.begin(), serialChecked.end(), serialToCheck) != serialChecked.end())
        //     {
        //         continue;
        //     }

        //     // f.open("serialChecked.txt", std::ios::app);
        //     // if (f.good())
        //     // {
        //     //     f << serialToCheck << "\n";
        //     // }
        //     // f.close();

        //     responseInfo = getDeviceInfo(serialVector.at(i));
        //     json jDeviceInfo = json::parse(responseInfo);
        //     if (jDeviceInfo.contains("data"))
        //     {
        //         json jData = jDeviceInfo.at("data");
        //         int64_t altitude = jData.at("altitude").get<int64_t>();
        //         int64_t key = jData.at("key").get<int64_t>();

        //         if (key > 0)
        //         {
        //             f.open("serialChecked.txt", std::ios::app);
        //             if (f.good())
        //             {
        //                 f << serialToCheck << "\n";
        //             }
        //             f.close();
        //         }

        //         if (altitude > 0)
        //         {
        //             serialFault.push_back(serialVector.at(i));
        //             addSerialFaultToFile(serialVector.at(i));
        //             std::cout << jData.at("id") << "..altitude:" << altitude << "..life:" << jData.at("life") << std::endl;
        //             std::cout << jData << std::endl;
        //         }
        //     }
        //     delete responseInfo;
        //     usleep(1000000);
        // }

        // std::cout << "*****************************" << std::endl;
        // for (size_t i = 0; i < serialFault.size(); i++)
        // {
        //     std::cout << serialFault.at(i) << ",";
        // }

        // std::cout << std::endl;
        // std::cout << std::endl;
    */
    int c = 0;
    while (1)
    {
        system("clear");
        std::cout << "\r\n\r\n";
        std::cout << "***************************************************\r\n";
        std::cout << "***************************************************\r\n";
        std::cout << "\t" << std::setw(6) << "0. "
                  << "Thong so tong quan Xe dang mo may.\r\n ";
        std::cout << "\t" << std::setw(6) << "1. "
                  << "Liet ke so serial 6 loi.\r\n";
        std::cout << "\t" << std::setw(6) << "2. "
                  << "Liet ke so serial 8 loi.\r\n";
        std::cout << "\t" << std::setw(6) << "3. "
                  << "Liet ke so serial disk >95%\r\n";
        std::cout << "\t" << std::setw(6) << "4. "
                  << "Lay Device Info cuar thiet bi.\r\n";
        std::cout << "\t" << std::setw(6) << "5. "
                  << "Lay Device disconnect too long.\r\n";
        std::cout << "\t" << std::setw(6) << "6. "
                  << "Lay Device Info trong line\r\n";
        std::cout << "\t" << std::setw(6) << "7. "
                  << "Lay Device serial 6 use 2G\r\n";
        std::cout << "\t" << std::setw(6) << "8. "
                  << "Lay Numbre of Device in version: \r\n";
        std::cout << "\t" << std::setw(6) << "9. "
                  << "Lay Total Device SIMCOM: \r\n";
        std::cout << "\t" << std::setw(6) << "10. "
                  << "Lay Totak Device QUECTEL: \r\n";
        std::cout << "\t" << std::setw(6) << "11. "
                  << "Lay Number Device HasTwoSim in version: \r\n";
        std::cout << "\t........................................\r\n";
        std::cout << "\t" << std::setw(6) << "1000. "
                  << "Lay du lieu sau 30p.\r\n";
        std::cout << "***************************************************\r\n";
        std::cout << "Enter the key: ";
        std::cin >> c;
        if (std::cin.fail())
        {
            std::cin.clear(); // put us back in 'normal' operation mode
            ignoreLine();     // and remove the bad input
        }
        else
        {
            if (c == 1000 || c == 0)
            {
                int timeGetData = 7200;
                if (c == 1000)
                {
                    timeGetData = 30 * 60;
                }
                time_t timeNow;
                time(&timeNow);
                time_t timeLastRequest = getTimeLastRequest();
                if (timeNow - timeLastRequest > timeGetData)
                {
                    getListDeviceUseApiRequestAndSaveFile();
                }
                getListDevice();
            }
            else if (c == 1)
            {
                int vsMin;
                std::cout << "Enter the version min (double): ";
                vsMin = (int)(getDouble() * 1000);

                int vsMax;
                std::cout << "Enter the version max (double): ";
                vsMax = (int)(getDouble() * 1000);

                getListDeviceCamError(true, vsMin - 1, vsMax + 1);
            }
            else if (c == 2)
            {
                getListDeviceCamError(false, 1000, 10000);
            }
            else if (c == 3)
            {
                getDeviceSdcard95percent();
            }
            else if (c == 4)
            {
                long serial;
                std::cout << "Nhap so serial: ";
                serial = getLong();
                if (serial > 100000000 && serial < 999999999)
                {
                    char *info = getDeviceInfo(serial);
                    Client *device = new Client(info);
                    std::cout << info << "\r\n";
                    std::cout << *device << "\r\n";
                    delete info;
                    delete device;
                }
            }
            else if (c == 5)
            {
                getDeviceNotConnectted();
            }
            else if (c == 6)
            {
                std::vector<long> serialCheck;
                std::fstream f;
                std::string serial;
                std::string *dataFile = new std::string("");
                f.open("abc.txt");
                if (f)
                {
                    while (!f.eof())
                    {
                        std::getline(f, serial);
                        dataFile->append(serial);
                        dataFile->append("  ");
                    }
                }
                f.close();

                std::regex word_regex("([689][0-9]{8})");
                auto words_begin =
                    std::sregex_iterator(dataFile->begin(), dataFile->end(), word_regex);
                auto words_end = std::sregex_iterator();
                for (std::sregex_iterator i = words_begin; i != words_end; ++i)
                {
                    std::smatch match = *i;
                    serialCheck.push_back(std::atol(match.str().c_str()));
                }

                for (std::vector<long>::iterator it = serialCheck.begin(); it != serialCheck.end(); ++it)
                {
                    long serial = *it;
                    char *info = getDeviceInfo(serial);
                    Client *device = new Client(info);
                    std::cout << *device << "\r\n\r\n";
                    delete info;
                    delete device;
                    sleep(2);
                }
                std::cout << "\r\n";

                delete dataFile;
                serialCheck.clear();
            }
            else if (c == 7)
            {
                getDeviceSerial6use2g();
            }
            else if (c == 8)
            {
                int vsMax;
                std::cout << "Enter the version max (double): ";
                vsMax = (int)(getDouble() * 1000);
                getNumberDeviceAsVersion(vsMax);
            }
            else if (c == 9)
            {
                getTotalNumberDeviceSimCom();
            }
            else if (c == 10)
            {
                getTotalNumberDeviceQuectel();
            }
            else if (c == 11)
            {
                std::cout << "Enter the version max (double): ";
                int version = (int)(getDouble() * 1000);
                getDeviceHasSim2(version);
            }

            ignoreLine();
        }
        sleep(1);
        std::cout << "Enter the key to continue...........";
        ignoreLine();
    }

    return 0;
}

/// g++ -Wall --std=c++11 main.cpp nlohmann/json.hpp deviceInfo/Client.h -lcurl