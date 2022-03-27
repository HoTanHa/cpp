#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "../nlohmann/json.hpp"

#include <bits/stdc++.h>
using json = nlohmann::json;

class Client
{
private:
    json *client_;
    double adc_;
    int32_t altitude_;
    int32_t cam_;
    int32_t disk_;
    int32_t hdop_; // time start key=1;
    int64_t id_;   // serial;
    int32_t key_;
    int32_t life_;
    int32_t sd_;
    int64_t timestamp_;
    time_t time_update_;
    double svn_;
    std::string str_updateTime_;
    void get_params();
    std::string str_ip_;
    std::string str_imei2_;
    bool isSerial6_;
    bool isSerial8_;

public:
    Client(json *js_client, time_t timeUpdate);
    Client(const char *js);
    ~Client();

    int64_t get_serial_number();
    bool is_sdCard_error();
    int get_cam_error();
    int get_version();
    long getTimestamp();
    bool isCarKeyOn();
    bool is_gsm_2G();
    bool is_has_imei2();

    bool is_serial_6_simcom();
    bool is_serial_8_quectel();

    bool isDeviceNotConnectTooLong();
    friend std::ostream &operator<<(std::ostream &o, const Client &c);
};

Client::Client(json *js_client, time_t timeUpdate)
{
    this->client_ = js_client; // new json(nullptr);
    // this->client_->merge_patch(*js_client);
    this->time_update_ = timeUpdate;
    isSerial6_ = false;
    isSerial8_ = false;
    this->get_params();
}

Client::Client(const char *jsDeviceInfo)
{
    try
    {
        json jsInfo = json::parse(jsDeviceInfo);
        this->client_ = new json(jsInfo.at("data"));
        this->time_update_ = time(NULL);
        this->get_params();
        delete this->client_;
    }
    catch (const std::exception &e)
    {
    }
}

Client::~Client()
{
}

void Client::get_params()
{
    if (client_->contains("id") && client_->at("id").is_string())
    {
        auto s2 = client_->at("id").get<std::string>();
        this->id_ = atol(s2.c_str());
        this->altitude_ = client_->at("altitude").get<int32_t>();
        this->timestamp_ = client_->at("timestamp").get<int64_t>() / 1000;
        this->life_ = client_->at("life").get<int32_t>();
        this->hdop_ = client_->at("hdop").get<int32_t>();

        if (client_->contains("disk") && client_->at("disk").is_number() && client_->contains("sd") && client_->at("sd").is_number())
        {
            this->disk_ = client_->at("disk").get<int32_t>();
            this->sd_ = client_->at("sd").get<int32_t>();
        }
        else
        {
            this->disk_ = 0;
            this->sd_ = 0;
        }

        this->adc_ = 0;
        if (client_->contains("adc") && client_->at("adc").is_number_float())
        {
            this->adc_ = client_->at("adc").get<double_t>();
        }

        this->svn_ = 0.0f;
        if (client_->contains("svn") && client_->at("svn").is_number_float())
        {
            this->svn_ = client_->at("svn").get<double_t>();
        }
        if (client_->contains("updateTime") && client_->at("updateTime").is_string())
        {
            this->str_updateTime_ = client_->at("updateTime").get<std::string>();
        }
        this->cam_ = 0;
        if (client_->contains("cam") && client_->at("cam").is_number())
        {
            this->cam_ = client_->at("cam").get<int32_t>();
        }

        if (client_->contains("ip") && client_->at("ip").is_string())
        {
            this->str_ip_ = client_->at("ip").get<std::string>();
        }
        if (client_->contains("imei2") && client_->at("imei2").is_string())
        {
            this->str_imei2_ = client_->at("imei2").get<std::string>();
        }

        if (altitude_ == 4 && cam_ == 1)
        {
            this->altitude_ = 5;
        }
        if (this->id_ > 600000000 && this->id_ < 700000000)
        {
            isSerial6_ = true;
        }
        if (this->id_ > 800000000 && this->id_ < 900000000)
        {
            isSerial8_ = true;
        }
        if (client_->contains("key") && client_->at("key").is_number())
        {
            this->key_ = client_->at("key").get<int32_t>();
        }
    }
}

int64_t Client::get_serial_number()
{
    return this->id_;
}
bool Client::is_sdCard_error()
{
    if (this->disk_ > 95 && this->sd_ == 1 && this->life_ > 16)
    {
        return true;
    }
    return false;
}

bool Client::isCarKeyOn()
{
    if (this->isSerial6_)
    {
        if (hdop_ > 0)
        {
            return true;
        }
        return false;
    }

    if (this->isSerial8_)
    {
        return this->key_ > 0 ? true : false;
    }
    return false;
}

int Client::get_cam_error()
{
    if (this->isSerial6_)
    {
        if ((timestamp_ > (time_update_ - 100) && hdop_ > 0))
        {
            if (get_version() == 2221 && cam_ == 0 && key_ > 0)
            {
                return 10;
            }

            return this->altitude_;
        }
        return 0;
    }

    if (this->isSerial8_)
    {
        return this->altitude_;
    }

    return 0;
}

int Client::get_version()
{
    return (int)(this->svn_ * 1000);
}

long Client::getTimestamp()
{
    return timestamp_;
}

bool Client::isDeviceNotConnectTooLong()
{
    return (timestamp_ < (time_update_ - 5 * 3600));
}

bool Client::is_gsm_2G()
{
    try
    {
        std::size_t found = this->str_ip_.find("2G");
        if (found != std::string::npos)
        {
            return true;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return false;
}

bool Client::is_serial_6_simcom()
{
    return isSerial6_;
}
bool Client::is_serial_8_quectel()
{
    return isSerial8_;
}

bool Client::is_has_imei2()
{
    if (this->str_imei2_.length() > 10)
    {
        return true;
    }
    return false;
}

std::ostream &operator<<(std::ostream &o, const Client &c)
{

    o << "Serial: " << std::setw(10) << c.id_ << " \tdisk: " << std::setw(5) << c.disk_
      << "\tlife: " << std::setw(5) << c.life_ << "\taltidute: " << std::setw(5) << c.altitude_
      << "\tcam: " << c.cam_ << "\thdop: " << std::setw(5) << c.hdop_ << "\tsvn: " << std::setw(5) << c.svn_ << "\r\n"
      << "Time update: " << c.str_updateTime_;
    return o;
}

// struct versionNode
// {
//     int version;
//     int num;
//     struct versionNode *next;
// };

// void add_a_device_to_list(struct versionNode *head, int version)
// {
//     if (head == NULL)
//     {
//         head = new versionNode();
//         head->num = 1;
//         head->version = version;
//         head->next = NULL;
//     }
//     else
//     {
//         bool isExist = false;
//         struct versionNode *current = head;
//         while (current->next != NULL)
//         {
//             if (version == current->version)
//             {
//                 current->num++;
//                 isExist = true;
//                 break;
//             }
//             current = current->next;
//         }
//         if (isExist)
//         {
//             return;
//         }
//         current->next = new versionNode();
//         current->next->version = version;
//         current->next->num = 1;
//         current->next->next = NULL;
//     }
// }

// void free_List(struct versionNode *head)
// {
//     struct versionNode *current = head;
//     struct versionNode *temp;
//     while (current->next != NULL)
//     {
//         temp = current.next;
//         delete current;
//         current = temp;
//     }
// }

#endif //__CLIENT_H__