#include <string>

#include "cpp-httplib/httplib.h"
#include "MTS-ESP/Master/libMTSMaster.h"
#include "MTS-ESP/Client/libMTSClient.h"

static bool isMaster = false;

std::string tuningToJson()
{
    MTSClient *client = MTS_RegisterClient();
    std::stringstream ss;
    ss << std::setprecision(17) << "[";
    for (int i = 0; i < 127; ++i)
    {
        ss << MTS_NoteToFrequency(client, i, 0) << ",";
    }
    ss << MTS_NoteToFrequency(client, 127, 0) << "]";
    MTS_DeregisterClient(client);
    return ss.str();
}

void sendTuning(const httplib::Request &, httplib::Response &res)
{
    res.set_content(tuningToJson(), "application/json");
}

void jsonToFrequencies(std::string json, double *frequencies)
{
    std::stringstream ss = std::stringstream(json.substr(1, json.size() - 2));
    for (int i = 0; i < 128; ++i)
    {
        std::string substr;
        std::getline(ss, substr, ',');
        frequencies[i] = std::stod(substr);
    }
}

void setTuningFromResult(const httplib::Result &res)
{
    if (res && res->status == httplib::StatusCode::OK_200)
    {
        double frequencies[128];
        jsonToFrequencies(res->body, frequencies);
        MTS_SetNoteTunings(frequencies);
    }
    else
    {
        auto err = res.error();
        std::cout << "HTTP error: " << httplib::to_string(err) << std::endl;
    }
}

void setTuning(const httplib::Request &req, httplib::Response &res)
{
    if (!isMaster)
    {
        if (!MTS_CanRegisterMaster())
        {
            std::cout << "Could not set tuning since MTS-ESP source already running" << std::endl;
            return;
        }
        std::cout << "Registering server as MTS-ESP source" << std::endl;
        MTS_RegisterMaster();
        isMaster = true;
    }
    double frequencies[128];
    jsonToFrequencies(req.body, frequencies);
    MTS_SetNoteTunings(frequencies);
    std::cout << "Tuning set successfully" << std::endl;
}
