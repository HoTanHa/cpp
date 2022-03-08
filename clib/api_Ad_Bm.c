#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <getopt.h>
#include <ctype.h>
#include <string.h>
#include <stddef.h>
#include "cJSON.h"

const char *devBinhMinh = "https://livedev.gpsbinhminh.vn:4430";
const char *tokenDevBm = "token: 4d1dfa94-bc3f-4fc3-84e1-7e175f56c19b";

char *tokenQL_Ad;
char *tokenQL_DevAd;
char *tokenQL_Bm;
char *tokenQL_DevBm;

const char *URL_AD = "http://live1.adsun.vn:8100";
const char *URL_AD_TEST = "http://live1.adsun.vn:8200";
const char *URL_BM = "http://live1.gpsbinhminh.vn:8102";
const char *URL_BM_TEST = "http://live1.gpsbinhminh.vn:8202";

const char *URL_LOGIN_AD = "http://auth.adsun.vn/Auth/Login";
const char *dataLogin_ad = "{\"username\":\"minhadsun\",\"pwd\":\"ad123456\"}";
const char *URL_LOGIN_AD_TEST = "https://namaauth.adsun.vn/Auth/Login";
const char *dataLogin_ad_test = "{\"username\":\"admin\",\"pwd\":\"ad123456\"}";
const char *URL_LOGIN_BM = "https://auth.gpsbinhminh.vn/Auth/Login";
const char *dataLogin_bm = "{\"username\":\"minhadsun\",\"pwd\":\"ad1234567\"}";
const char *URL_LOGIN_BM_TEST = "http://authtest.gpsbinhminh.vn/Auth/Login";
const char *dataLogin_bm_test = "{\"username\":\"minhadsun\",\"pwd\":\"ad1234567\"}";

static size_t
WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    char *cnt = (char *)contents;
    printf("Content Response: %s \r\n\r\n", cnt);
    if (userp != NULL && strlen(contents) < 1024)
    {
        strcpy(userp, contents);
    }
    return size * nmemb;
}

void getToken(int mRouter)
{
    if (mRouter == 1)
    {
        if (tokenQL_Ad != NULL)
        {
            return;
        }
        tokenQL_Ad = malloc(101);
    }
    else if (mRouter == 2)
    {
        if (tokenQL_DevAd != NULL)
        {
            return;
        }
        tokenQL_DevAd = malloc(101);
    }
    else if (mRouter == 3)
    {
        if (tokenQL_Bm != NULL)
        {
            return;
        }
        tokenQL_Bm = malloc(101);
    }
    else if (mRouter == 4)
    {
        if (tokenQL_DevBm != NULL)
        {
            return;
        }
        tokenQL_DevBm = malloc(101);
    }
    else
    {
        return;
    }
    char url[200];
    char *data;
    CURL *curl;
    CURLcode res;
    struct curl_slist *headerlist = NULL;
    curl_global_init(CURL_GLOBAL_ALL);
    static const char buf[] = "Expect:";

    headerlist = curl_slist_append(headerlist, "Content-Type: application/json; charset=utf-8");
    headerlist = curl_slist_append(headerlist, "Expect:");
    headerlist = curl_slist_append(headerlist, "Connection: Keep-alive");
    {
        printf("%s******************************--- ---*****************************\r\n", __FUNCTION__);
        curl = curl_easy_init();
        if (curl)
        {
            if (mRouter == 2)
            {
                sprintf(url, "%s", URL_LOGIN_AD_TEST);
                data = dataLogin_ad_test;
            }
            else if (mRouter == 3)
            {
                sprintf(url, "%s", URL_LOGIN_BM);
                data = dataLogin_bm;
            }
            else if (mRouter == 4)
            {
                sprintf(url, "%s", URL_LOGIN_BM_TEST);
                data = dataLogin_bm_test;
            }
            else
            {
                sprintf(url, "%s", URL_LOGIN_AD);
                data = dataLogin_ad;
            }

            printf("%s", url);
            printf("\r\n");
            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);

            // curl_easy_setopt(curl, CURLOPT_READDATA, &payloadData);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            char *response_buffer = malloc(1024);
            long response_code;
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, response_buffer);
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
            // curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_3);
            /* Perform the request, res will get the return code */
            res = curl_easy_perform(curl);
            /* Check for errors */
            if (res != CURLE_OK)
                fprintf(stderr, "curl_easy_perform() failed: %s\n",
                        curl_easy_strerror(res));
            else
            {
                // susscess
                curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
                if (response_code == 200)
                {
                    cJSON *json;
                    json = cJSON_Parse(response_buffer);
                    if (json)
                    {
                        cJSON *itemToken = cJSON_GetObjectItem(json, "Token");
                        if (itemToken)
                        {
                            char *token = cJSON_PrintUnformatted(itemToken);
                            printf("TOKEN:  %s\r\n", token);
                            if (mRouter == 1)
                            {
                                snprintf(tokenQL_Ad, 100, "token:%s", itemToken->valuestring);
                            }
                            else if (mRouter == 2)
                            {
                                snprintf(tokenQL_DevAd, 100, "token:%s", itemToken->valuestring);
                            }
                            else if (mRouter == 3)
                            {
                                snprintf(tokenQL_Bm, 100, "token:%s", itemToken->valuestring);
                            }
                            else if (mRouter == 4)
                            {
                                snprintf(tokenQL_DevBm, 100, "token:%s", itemToken->valuestring);
                            }
                        }
                        cJSON_Delete(json);
                    }
                }
            }
            /* always cleanup */
            curl_easy_cleanup(curl);
        }
        usleep(200000);
    }
}

void liveStreamAllCamera(int isLive, long serial, int numCam, int mRouter)
{
    if (isLive)
    {
        printf("...................... START STREAM ..............%ld......\r\n", serial);
    }
    else
    {
        printf("...................... STOP STREAM  ..............%ld......\r\n", serial);
    }

    char url[200];
    CURL *curl;
    CURLcode res;
    struct curl_slist *headerlist = NULL;
    curl_global_init(CURL_GLOBAL_ALL);
    static const char buf[] = "Expect:";
    if (mRouter == 2)
    {
        headerlist = curl_slist_append(headerlist, tokenQL_DevAd);
    }
    else if (mRouter == 3)
    {
        headerlist = curl_slist_append(headerlist, tokenQL_Bm);
    }
    else if (mRouter == 4)
    {
        headerlist = curl_slist_append(headerlist, tokenQL_DevBm);
    }
    else
    {
        headerlist = curl_slist_append(headerlist, tokenQL_Ad);
    }
    headerlist = curl_slist_append(headerlist, "Expect:");
    headerlist = curl_slist_append(headerlist, "Connection: Keep-alive");
    for (int i = 0; i < numCam; i++)
    {
        printf("%s******************************--- %d ---*****************************\r\n", __FUNCTION__, i);
        curl = curl_easy_init();
        if (curl)
        {
            if (isLive)
            {
                sprintf(url, "%s/api/video/playStreamv2?device=%ld&camera=%d&endUserDevice=Linux-Chrome", devBinhMinh, serial, i);
            }
            else
            {
                sprintf(url, "%s/api/video/stopstream?device=%ld&camera=%d", devBinhMinh, serial, i);
            }
            printf(url);
            printf("\r\n");
            curl_easy_setopt(curl, CURLOPT_URL, url);

            /* Forcing HTTP/3 will make the connection fail if the server isn't
               accessible over QUIC + HTTP/3 on the given host and port.
               Consider using CURLOPT_ALTSVC instead! */
            curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);

            char *response_buffer = malloc(1024);
            long response_code;
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, response_buffer);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
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
    }
}

void deleteVideoWithCommandLine()
{
    CURL *curl;
    CURLcode res;

    struct curl_slist *headerlist = NULL;
    curl_global_init(CURL_GLOBAL_ALL);
    static const char buf[] = "Expect:";
    headerlist = curl_slist_append(headerlist, "token:90d03299-e3c6-41f9-b505-7c15d92ee46a");
    headerlist = curl_slist_append(headerlist, "Expect:");
    headerlist = curl_slist_append(headerlist, "Connection: close");
    char url[200];

    int camId = 2;
    char *sdCard = "/mnt/media_rw/1D12-0FB3";
    long serial = 639877359; // 639877854; // 999959999;
    time_t timeStart = 1635523200;
    int soGio = 25;

    // int camId = 2;
    // char *sdCard = "/mnt/media_rw/40C3-18F3";
    // long serial = 639877854;//639877359;
    // time_t timeStart = 1634853600;//1634799600;
    // int soGio = 24*9;

    struct tm tm;

    for (int i = 0; i < soGio; i++)
    {
        printf("******************************... %d ...**************************************\r\n", i);
        time_t time = timeStart + 3600 * i;

        localtime_r(&time, &tm);

        // sprintf(url, "http://live1.adsun.vn:8100/api/manage/devicecmd?device=%ld&cmd=devicecmd:rm%20-r%20/%s/cam%d/cam%d_%04d%02d%02d_%02d_%ld",
        //         serial, sdCard, camId, camId, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, time);

        sprintf(url, "http://live1.adsun.vn:8100/api/manage/devicecmd?device=%ld&cmd=devicecmd:rm\04520-r\04520/%s/cam%d/cam%d_%04d%02d%02d_%02d_%ld",
                serial, sdCard, camId, camId, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, time);

        // sprintf(url, "http://live1.adsun.vn:8200/api/manage/devicecmd?device=%ld&cmd=devicecmd:rm%%20-r%%20/%s/cam%d/cam%d_%04d%02d%02d_%02d_%ld",
        //         serial, sdCard, camId, camId, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, time);
        printf("%s \r\n", url);
        curl = curl_easy_init();
        if (curl)
        {
            curl_easy_setopt(curl, CURLOPT_URL, url);

            /* Forcing HTTP/3 will make the connection fail if the server isn't
               accessible over QUIC + HTTP/3 on the given host and port.
               Consider using CURLOPT_ALTSVC instead! */
            curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);

            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
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
        usleep(10000);
    }
}

void getDeviceStatusOrDeviceInfo(long serial, int mRouter, int isGetInfo)
{
    char url[200];
    CURL *curl;
    CURLcode res;
    struct curl_slist *headerlist = NULL;
    curl_global_init(CURL_GLOBAL_ALL);
    static const char buf[] = "Expect:";
    if (mRouter == 2)
    {
        headerlist = curl_slist_append(headerlist, tokenQL_DevAd);
    }
    else if (mRouter == 3)
    {
        headerlist = curl_slist_append(headerlist, tokenQL_Bm);
    }
    else if (mRouter == 4)
    {
        headerlist = curl_slist_append(headerlist, tokenQL_DevBm);
    }
    else
    {
        headerlist = curl_slist_append(headerlist, tokenQL_Ad);
    }

    headerlist = curl_slist_append(headerlist, "Expect:");
    headerlist = curl_slist_append(headerlist, "Connection: close");
    {
        printf("%s******************************--- ---*****************************\r\n", __FUNCTION__);
        curl = curl_easy_init();
        if (curl)
        {
            if (isGetInfo)
            {

                if (mRouter == 2)
                {
                    sprintf(url, "%s/api/manage/getDeviceStatusById?device=%ld", URL_AD_TEST, serial);
                }
                else if (mRouter == 3)
                {
                    sprintf(url, "%s/api/manage/getDeviceStatusById?device=%ld", URL_BM, serial);
                }
                else if (mRouter == 4)
                {
                    sprintf(url, "%s/api/manage/getDeviceStatusById?device=%ld", URL_BM_TEST, serial);
                }
                else
                {
                    sprintf(url, "%s/api/manage/getDeviceStatusById?device=%ld", URL_AD, serial);
                }
            }
            else
            {
                if (mRouter == 2)
                {
                    sprintf(url, "%s/api/manage/getDeviceInfoById?device=%ld", URL_AD_TEST, serial);
                }
                else if (mRouter == 3)
                {
                    sprintf(url, "%s/api/manage/getDeviceInfoById?device=%ld", URL_BM, serial);
                }
                else if (mRouter == 4)
                {
                    sprintf(url, "%s/api/manage/getDeviceInfoById?device=%ld", URL_BM_TEST, serial);
                }
                else
                {
                    sprintf(url, "%s/api/manage/getDeviceInfoById?device=%ld", URL_AD, serial);
                }
            }
            printf("%s", url);
            printf("\r\n");
            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);

            char *response_buffer = malloc(1024);
            long response_code;
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, response_buffer);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
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
    }
}

void getDeviceStatus(long serial, int mRouter)
{
    getDeviceStatusOrDeviceInfo(serial, mRouter, 0);
}

void getDeviceInfo(long serial, int mRouter)
{
    getDeviceStatusOrDeviceInfo(serial, mRouter, 1);
}

void sendCmdToDeivce(long serial, int mRouter, const char *cmd)
{
    char url[200];
    CURL *curl;
    CURLcode res;
    struct curl_slist *headerlist = NULL;
    curl_global_init(CURL_GLOBAL_ALL);
    static const char buf[] = "Expect:";
    if (mRouter == 2)
    {
        headerlist = curl_slist_append(headerlist, tokenQL_DevAd);
    }
    else if (mRouter == 3)
    {
        headerlist = curl_slist_append(headerlist, tokenQL_Bm);
    }
    else if (mRouter == 4)
    {
        headerlist = curl_slist_append(headerlist, tokenQL_DevBm);
    }
    else
    {
        headerlist = curl_slist_append(headerlist, tokenQL_Ad);
    }

    headerlist = curl_slist_append(headerlist, "Expect:");
    headerlist = curl_slist_append(headerlist, "Connection: close");
    {
        printf("%s******************************--- ---*****************************\r\n", __FUNCTION__);
        curl = curl_easy_init();
        if (curl)
        {
            if (mRouter == 2)
            {
                sprintf(url, "%s/api/manage/devicecmd?device=%ld&cmd=%s", URL_AD_TEST, serial, cmd);
            }
            else if (mRouter == 3)
            {
                sprintf(url, "%s/api/manage/devicecmd?device=%ld&cmd=%s", URL_BM, serial, cmd);
            }
            else if (mRouter == 4)
            {
                sprintf(url, "%s/api/manage/devicecmd?device=%ld&cmd=%s", URL_BM_TEST, serial, cmd);
            }
            else
            {
                sprintf(url, "%s/api/manage/devicecmd?device=%ld&cmd=%s", URL_AD, serial, cmd);
            }

            printf("%s", url);
            printf("\r\n");
            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);

            char *response_buffer = malloc(1024);
            long response_code;
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, response_buffer);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
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
    }
}

void sendCmdRebootToDevice(long serial, int mRouter)
{
    sendCmdToDeivce(serial, mRouter, "devicecmd:reboot");
}

void sendCmdUse480tToDevice(long serial, int mRouter)
{
    sendCmdToDeivce(serial, mRouter, "setupDevice:setImage:3:480");
}
void sendCmdUse720tToDevice(long serial, int mRouter)
{
    sendCmdToDeivce(serial, mRouter, "setupDevice:setImage:3:720");
}
void sendCmdMicOn(long serial, int mRouter)
{
    sendCmdToDeivce(serial, mRouter, "setupDevice:micOn");
}
void sendCmdMicOff(long serial, int mRouter)
{
    sendCmdToDeivce(serial, mRouter, "setupDevice:micOff");
}
char *routerOption = "......Chooose your option: ROUTER  ............\r\n"
                     "\t1. Adsun \r\n"
                     "\t2. Adsun Dev \r\n"
                     "\t3. BinhMinh\r\n"
                     "\t4. BinhMinh Dev\r\n";

char *choosen = "......Chooose your option:  ............\r\n"
                "\t1. start livestream \r\n"
                "\t2. stop liveStream \r\n"
                "\t3. get Info, Status Device\r\n"
                "\t4. send reboot\r\n"
                "\t5. send useImage 480\r\n"
                "\t6. send useImage 720\r\n"
                "\t7. set mic ON\r\n"
                "\t8. set mic Off\r\n";

int main(int argc, char **argv)
{
    int opt;
    long serial;
    int router;
    int status = 0;

    // static char *mainopts = "s:r:";
    // do
    // {
    //     opt = getopt(argc, argv, mainopts);

    //     switch (opt)
    //     {
    //     case 's':
    //         serial = atol(optarg);
    //         break;
    //     case 'r':
    //         router = atoi(optarg);
    //         break;
    //     default:
    //         status = -1;
    //         break;
    //     }

    // } while ((opt != -1) && (status == 0));
    while (1)
    {
        printf("\r\n***************************************\r\n");
        printf("Nhap so serialNumber: ");
        scanf("%ld", &serial);

        printf("Your serial number: %ld \r\n", serial);

        char c = 0;

        printf("%s", routerOption);
        while (!isdigit(c))
        {
            printf("Nhap mot ki tu NUMBER 1-4: ");
            c = getchar();
        }
        router = c - 0x30;
        if (router < 1 || router > 4)
        {
            router = 1;
        }
        /******************************************************/
        getToken(router);
        usleep(1000000);
        /*****************************************************/

        printf("%s", choosen);
        c = 0;
        while (!isalnum(c))
        {
            printf("Nhap mot ki tu NUMBER or ALPHABET: ");
            c = getchar();
        }

        if (c == '1')
        {
            liveStreamAllCamera(1, serial, 4, router);
        }
        if (c == '2')
        {
            liveStreamAllCamera(0, serial, 4, router);
        }
        if (c == '3')
        {
            getDeviceInfo(serial, router);
            getDeviceStatus(serial, router);
        }
        if (c == '4')
        {
            sendCmdRebootToDevice(serial, router);
        }
        if (c == '5')
        {
            sendCmdUse480tToDevice(serial, router);
        }
        if (c == '6')
        {
            sendCmdUse720tToDevice(serial, router);
        }
        if (c == '7')
        {
            sendCmdMicOn(serial, router);
        }
        if (c == '8')
        {
            sendCmdMicOff(serial, router);
        }

        printf("Do you want to clean the screen: (Y/N): ");
        c = 0;
        while (!isalnum(c))
        {
            c = getchar();
        }
        if (c == 'y' || c == 'Y')
        {
            system("clear");
        }
    }
    return 0;
}