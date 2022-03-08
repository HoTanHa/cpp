#include "HttpServer.h"
#include "mongoose.h"
#include "../log/log.h"

const char *kType_html = "text/html";
const char *kType_plain = "text/plain";
const char *kType_json = "application/json";
const char *kType_js = "text/javascript";
const char *kType_css = "text/css";
const char *kType_icon = "image/x-icon";

const struct mg_str kGet_Public = MG_MK_STR("/publics");
const struct mg_str kGet_favicon = MG_MK_STR("/favicon.ico");
const struct mg_str kApi_getSerial = MG_MK_STR("/api/getSerial");
const struct mg_str kApi_getTimelineData = MG_MK_STR("/api/getTimelineData");
const struct mg_str kApi_startSetupCamera = MG_MK_STR("/api/startSetupCamera");
const struct mg_str kApi_getNumberCamera = MG_MK_STR("/api/getNumberCamera");
const struct mg_str kApi_getImageStream = MG_MK_STR("/api/getImageStream");
const struct mg_str kApi_copyToUsb = MG_MK_STR("/api/copyToUsb");
const struct mg_str kApi_getCopyProcess = MG_MK_STR("/api/getCopyProcess");
const struct mg_str kApi_getLimitVideo = MG_MK_STR("/api/getLimitVideo");
const struct mg_str kApi_getKey = MG_MK_STR("/api/getKey");
const struct mg_str kApi_getVideo = MG_MK_STR("/api/getVideo");
const struct mg_str kApi_listHour = MG_MK_STR("/api/listHour");
const struct mg_str kApi_timeInterval = MG_MK_STR("/api/timeInterval");
const struct mg_str kApi_getListHour = MG_MK_STR("/api/getListHour");
const struct mg_str kApi_getListVideo = MG_MK_STR("/api/getListVideo");
const struct mg_str kApi_info = MG_MK_STR("/api/info");
const struct mg_str kApi_image = MG_MK_STR("/api/image");
const struct mg_str kApi_getImage = MG_MK_STR("/api/getImage");
const struct mg_str kApi_infoDevice = MG_MK_STR("/api/infoDevice");
const struct mg_str kApi_dataWorkTime = MG_MK_STR("/api/dataWorkTime");
const struct mg_str kApi_dataDungDo = MG_MK_STR("/api/dataDungDo");
const struct mg_str kApi_dataHanhTrinh = MG_MK_STR("/api/dataHanhTrinh");
const struct mg_str kApi_dataSpeed = MG_MK_STR("/api/dataSpeed");
const struct mg_str kApi_overSpeed = MG_MK_STR("/api/overSpeed");
const struct mg_str kApi_getInfoSetup = MG_MK_STR("/api/getInfoSetup");

static const struct mg_str ms_get_method = MG_MK_STR("GET");
static const struct mg_str ms_post_method = MG_MK_STR("POST");

static int is_equal(const struct mg_str *s1, const struct mg_str *s2)
{
    return (s1->len == s2->len) && (memcmp(s1->p, s2->p, s2->len) == 0);
}

void Send_HttpResponseError404(struct mg_connection *nc)
{
    mg_http_send_error(nc, 404, "Not Found.");
}

void Send_HttpResponse(struct mg_connection *connection, const char *mimeType, char *content, int contentLength)
{
    char *buffer = (char *)malloc(contentLength + 100);
    sprintf(buffer, "Cache: no-cache\r\n"
                    "Content-Type: %s\r\n"
                    "Content-Length: %d\r\n\r\n "
                    "%s",
            mimeType, contentLength, content);
    mg_send_response_line(connection, 200, buffer);
    free(buffer);
}

void Send_Response_File(struct mg_connection *connection, char *fileName)
{
    char *typeOfFile;
    char *pch;
    pch = strrchr(fileName, '.');
    if (!pch)
    {
        Send_HttpResponseError404(connection);
        return;
    }

    if (strcmp(pch, ".js") == 0)
    {
        typeOfFile = (char *)kType_js;
    }
    else if (strcmp(pch, ".css") == 0)
    {
        typeOfFile = (char *)kType_css;
    }
    else if (strcmp(pch, ".ico") == 0)
    {
        typeOfFile = (char *)kType_icon;
        printf("icon icon icon ......\r\n");
    }
    else if (strcmp(pch, ".html") == 0)
    {
        typeOfFile = (char *)kType_html;
    }
    else
    {
        typeOfFile = (char *)kType_plain;
    }

    char *buffer = 0;
    long length;
    FILE *f = fopen(fileName, "rb");
    if (f)
    {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = (char *)malloc(length);
        if (buffer)
        {
            fread(buffer, 1, length, f);
        }
        fclose(f);
    }

    if (buffer)
    {
        Send_HttpResponse(connection, typeOfFile, buffer, length);
        free(buffer);
    }
    else
    {
        Send_HttpResponseError404(connection);
    }
}

std::mutex HttpServer::mLocker_;
HttpServer *HttpServer::mInstancePtr = nullptr;
struct mg_serve_http_opts HttpServer::s_http_server_opts;
struct mg_serve_http_opts HttpServer::file_serve_opts;

HttpServer::HttpServer(/* args */)
{
    is_running_ = 0;
    port = 8080;
}

HttpServer::~HttpServer()
{
}

void *HttpServer::http_server_thread(void *arg)
{
    printf("Start server \r\n");

    HttpServer *http_server = (HttpServer *)arg;
    http_server->is_running_ = 1;
    pthread_create(&http_server->file_server_tid_, NULL, file_server_thread, http_server);

    char s_http_port[10];
    sprintf(s_http_port, "%d", http_server->port);
    mg_mgr_init(&http_server->mgr, NULL);

    http_server->connection = mg_bind(&http_server->mgr, s_http_port, event_request_handler);
    if (http_server->connection == NULL)
    {
        http_server->is_running_ = 0;
        return NULL;
    }

    // Set up HTTP server parameters
    mg_set_timer(http_server->connection, 1000);
    mg_register_http_endpoint(http_server->connection, "/hello", handle_request_hello);
    mg_register_http_endpoint(http_server->connection, "/api", handle_request_api);
    mg_register_http_endpoint(http_server->connection, "/setup", handle_request_setup);

    mg_set_protocol_http_websocket(http_server->connection);

    HttpServer::s_http_server_opts.document_root = "/home/htha/Code/cpp/trylib/publics"; // Serve current directory
    HttpServer::s_http_server_opts.enable_directory_listing = "no";

    while (http_server->is_running_)
    {
        mg_mgr_poll(&http_server->mgr, 1000);
    }
    usleep(100000);
    mg_mgr_free(&http_server->mgr);

    pthread_join(http_server->file_server_tid_, NULL);
    printf("Stop server \r\n");
    return NULL;
}

void *HttpServer::file_server_thread(void *arg)
{
    printf("Start server File\r\n");

    HttpServer *http_server = (HttpServer *)arg;
    mg_mgr_init(&http_server->file_mgr, NULL);
    http_server->file_connection = mg_bind(&http_server->file_mgr, "8082", file_event_handle);
    mg_set_timer(http_server->file_connection, 1000);
    mg_set_protocol_http_websocket(http_server->file_connection);
    HttpServer::file_serve_opts.document_root = "/home/htha/Videos/cam1_20211011_11_1633924800"; // Serve current directory
    HttpServer::file_serve_opts.enable_directory_listing = "yes";

    while (http_server->is_running_)
    {
        mg_mgr_poll(&http_server->file_mgr, 1000);
    }
    usleep(100000);
    mg_mgr_free(&http_server->file_mgr);
    printf("Stop server File\r\n");
    return NULL;
}
void HttpServer::event_request_handler(struct mg_connection *connection, int ev, void *p)
{
    if (ev == MG_EV_HTTP_REQUEST)
    {
        struct http_message *hm = (struct http_message *)p;
        if (hm->uri.len == 1)
        {
            const char *fileName = "publics/index.html";
            // Send_Response_File(connection, (char *)fileName);
            mg_http_serve_file(connection, hm, fileName, mg_mk_str("text/html"), mg_mk_str(""));
            return;
        }
        else
        {
            mg_serve_http(connection, hm, HttpServer::s_http_server_opts);
        }
    }
}
void HttpServer::handle_request_hello(struct mg_connection *connection, int ev, void *p)
{
    if (ev == MG_EV_HTTP_REQUEST)
    {
        const char *content = "Hello World!";
        Send_HttpResponse(connection, kType_plain, (char *)content, strlen(content));
    }
}
void HttpServer::handle_request_api(struct mg_connection *connection, int ev, void *p)
{
    if (ev == MG_EV_HTTP_REQUEST)
    {
        struct http_message *hm = (struct http_message *)p;
        if (is_equal(&hm->method, &ms_get_method))
        {
            if (is_equal(&hm->uri, &kApi_getLimitVideo))
            {
                time_t t = time(NULL);
                char *response = (char *)malloc(128);
                sprintf(response, "{\"start\":%ld,\"end\":%ld} ", t - 100000, t);
                Send_HttpResponse(connection, kType_json, response, strlen(response));
                free(response);
            }
            else if (is_equal(&hm->uri, &kApi_getNumberCamera))
            {
                const char *response = "{\"status\":1,\"numCam\":4} ";
                Send_HttpResponse(connection, kType_json, (char *)response, strlen(response));
            }
            else if (is_equal(&hm->uri, &kApi_infoDevice))
            {
                const char *response = "{\"TenCongTy\":\"ADSUN JSC\","
                                       "\"TenSanPham\":\"TMS-CAM-ND10\","
                                       "\"Serial\":638228604,"
                                       "\"BienSo\":\"N/A\","
                                       "\"ppDoTocDo\":1,"
                                       "\"ConfigXungKm\":8000,\"VanTocMax\":90,"
                                       "\"NgayLapDat\":\"2021/07/01\",\"NgayCapNhat\":\"2021/07/01\","
                                       "\"GsmStatus\":2,\"GpsStatus\":0,\"SdcardStatus\":1,"
                                       "\"DungLuong\":134217728,"
                                       "\"ThongTinTaiXe\":\"HO_VA_TEN_LAI_XE_1, GIAY_PHEP_LAI_XE_1\","
                                       "\"ThoiGianLienTuc\":0,\"GpsInfo\":\".0,.0\","
                                       "\"VanToc\":0,\"ThoiGianTb\":\"2021/08/23 04:03:57 PM\"} ";
                Send_HttpResponse(connection, kType_json, (char *)response, strlen(response));
            }
            else if (is_equal(&hm->uri, &kApi_getListVideo))
            {
            }
        }
        else
        {
            const char *content = "Handle api";
            Send_HttpResponse(connection, kType_plain, (char *)content, strlen(content));
        }
    }
}

void HttpServer::handle_request_setup(struct mg_connection *connection, int ev, void *p)
{
    if (ev == MG_EV_HTTP_REQUEST)
    {
        const char *content = "Handle setup";
        Send_HttpResponse(connection, kType_plain, (char *)content, strlen(content));
    }
}

void HttpServer::file_event_handle(struct mg_connection *connection, int ev, void *p)
{
    if (ev == MG_EV_HTTP_REQUEST)
    {
        struct http_message *hm = (struct http_message *)p;
        mg_serve_http(connection, hm, HttpServer::file_serve_opts);
    }
}

void HttpServer::start_server(bool isAdsun)
{
    if (is_running_)
    {
        return;
    }

    system("rm publics/favicon.ico");
    if (isAdsun)
    {
        link("publics/favicon_ad.ico", "publics/favicon.ico");
    }
    else
    {
        link("publics/favicon_bm.ico", "publics/favicon.ico");
    }

    pthread_create(&http_server_tid_, NULL, http_server_thread, this);
}

void HttpServer::stop_server()
{
    this->is_running_ = 0;
    pthread_join(this->http_server_tid_, NULL);
}