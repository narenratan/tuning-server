#include <string.h>
#include <stdlib.h>
#include <future>

#include "clap/include/clap/clap.h"

#include "constants.h"
#include "RemoteTuning.h"

struct MyPlugin
{
    clap_plugin_t plugin;
    const clap_host_t *host;
    httplib::Server *svr;
    std::future<void> f;
};

const char *_features[] = {
    CLAP_PLUGIN_FEATURE_UTILITY,
    NULL,
};

static const clap_plugin_descriptor_t pluginDescriptor = {
    .clap_version = CLAP_VERSION_INIT,
    .id = "naren.TuningServer",
    .name = "Tuning server",
    .vendor = "naren",
    .url = "https://github.com/narenratan",
    .manual_url = "https://github.com/narenratan",
    .support_url = "https://github.com/narenratan",
    .version = "1.0.0",
    .description = "Send and receive MTS-ESP tuning as json",
    .features = _features,
};

static const clap_plugin_t pluginClass = {
    .desc = &pluginDescriptor,
    .plugin_data = nullptr,

    .init = [](const clap_plugin *_plugin) -> bool {
        MyPlugin *plugin = (MyPlugin *)_plugin->plugin_data;
        (void)plugin;
        return true;
    },

    .destroy =
        [](const clap_plugin *_plugin) {
            MyPlugin *plugin = (MyPlugin *)_plugin->plugin_data;
            free(plugin);
        },

    .activate = [](const clap_plugin *_plugin, double sampleRate, uint32_t minimumFramesCount,
                   uint32_t maximumFramesCount) -> bool {
        MyPlugin *plugin = (MyPlugin *)_plugin->plugin_data;
        plugin->svr = new httplib::Server();
        plugin->svr->Get(ENDPOINT, sendTuning);
        plugin->svr->Post(ENDPOINT, setTuning);
        plugin->f = std::async(std::launch::async, [&]() { plugin->svr->listen("0.0.0.0", PORT); });
        plugin->svr->wait_until_ready();
        return true;
    },

    .deactivate =
        [](const clap_plugin *_plugin) {
            MyPlugin *plugin = (MyPlugin *)_plugin->plugin_data;
            plugin->svr->stop();
            plugin->f.get();
            if (isMaster)
            {
                MTS_DeregisterMaster();
            }
        },

    .start_processing = [](const clap_plugin *_plugin) -> bool { return true; },

    .stop_processing = [](const clap_plugin *_plugin) {},

    .reset = [](const clap_plugin *_plugin) {},

    .process = [](const clap_plugin *_plugin, const clap_process_t *process)
        -> clap_process_status { return CLAP_PROCESS_CONTINUE; },

    .get_extension = [](const clap_plugin *plugin, const char *id) -> const void * {
        return nullptr;
    },

    .on_main_thread = [](const clap_plugin *_plugin) {},
};

static const clap_plugin_factory_t pluginFactory = {
    .get_plugin_count = [](const clap_plugin_factory *factory) -> uint32_t { return 1; },

    .get_plugin_descriptor = [](const clap_plugin_factory *factory, uint32_t index)
        -> const clap_plugin_descriptor_t * { return index == 0 ? &pluginDescriptor : nullptr; },

    .create_plugin = [](const clap_plugin_factory *factory, const clap_host_t *host,
                        const char *pluginID) -> const clap_plugin_t * {
        if (!clap_version_is_compatible(host->clap_version) ||
            strcmp(pluginID, pluginDescriptor.id))
        {
            return nullptr;
        }

        MyPlugin *plugin = (MyPlugin *)calloc(1, sizeof(MyPlugin));
        plugin->host = host;
        plugin->plugin = pluginClass;
        plugin->plugin.plugin_data = plugin;
        return &plugin->plugin;
    },
};

extern "C" const clap_plugin_entry_t clap_entry = {
    .clap_version = CLAP_VERSION_INIT,

    .init = [](const char *path) -> bool { return true; },

    .deinit = []() {},

    .get_factory = [](const char *factoryID) -> const void * {
        return strcmp(factoryID, CLAP_PLUGIN_FACTORY_ID) ? nullptr : &pluginFactory;
    },
};
