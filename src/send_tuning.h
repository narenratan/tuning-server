#include "libMTSClient.h"

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
