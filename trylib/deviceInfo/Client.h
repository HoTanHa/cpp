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
    void get_params();

public:
    Client(json *js_client, time_t timeUpdate);
    Client(const char *js);
    ~Client();

    int64_t get_serial_number();
    bool is_sdCard_error();
    int get_cam_error();
    double_t get_version();
    long getTimestamp();

    bool isDeviceNotConnectTooLong();
    friend std::ostream &operator<<(std::ostream &o, const Client &c);
};

Client::Client(json *js_client, time_t timeUpdate)
{
    this->client_ = js_client; // new json(nullptr);
    // this->client_->merge_patch(*js_client);
    this->time_update_ = timeUpdate;
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

int Client::get_cam_error()
{
    if (this->id_ > 600000000 && this->id_ < 700000000)
    {
        if ((timestamp_ > (time_update_ - 100) && hdop_ > 0))
        {
            return this->altitude_;
        }
    }

    if (this->id_ > 800000000 && this->id_ < 900000000)
    {
        return this->altitude_;
    }

    return 0;
}

double_t Client::get_version()
{
    return this->svn_;
}

long Client::getTimestamp()
{
    return timestamp_;
}

bool Client::isDeviceNotConnectTooLong()
{
    return (timestamp_ < (time_update_ - 5 * 3600));
}

std::ostream &operator<<(std::ostream &o, const Client &c)
{
    o << "Serial: " << c.id_ << " \tdisk: " << c.disk_ << "\tlife: " << c.life_;
    return o;
}

#endif //__CLIENT_H__