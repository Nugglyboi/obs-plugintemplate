#include <obs-module.h>
#include <obs-frontend-api.h>
#include <plugin-support.h>

#include <QDockWidget>
#include <QMainWindow>
#include <QTimer>

#include "ScoreboardDock.hpp"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US")

#include "ScoreboardDock.hpp"

static QDockWidget *scoreboardDock = nullptr;
static bool dock_created = false;

static void create_scoreboard_dock() // Creates the scoreboard dock and adds it to the OBS frontend
{
    if (dock_created)
        return;

    QWidget *widget = new ScoreboardDock(nullptr);

    bool result = obs_frontend_add_dock_by_id(
        "scoreboard",
        "Scoreboard",
        widget
    );

    obs_log(LOG_INFO, "Dock registration result: %s",
        result ? "true" : "false");

    if (!result) {
        delete widget;
        return;
    }

    dock_created = true;
}

static void frontend_event(enum obs_frontend_event event, void *) // Callback function for OBS frontend events
{
    if (event == OBS_FRONTEND_EVENT_FINISHED_LOADING)
    {
        QTimer::singleShot(500, [] {
            create_scoreboard_dock();
        });
    }
}


bool obs_module_load(void) // Called when the plugin is loaded, sets up the frontend event callback and logs the plugin version
{
    obs_log(LOG_INFO, "plugin loaded successfully (version %s)", PLUGIN_VERSION);

    obs_frontend_add_event_callback(frontend_event, nullptr);

    return true;
}


void obs_module_unload(void) // Called when the plugin is unloaded, removes the scoreboard dock from the OBS frontend
{
    obs_frontend_remove_dock("scoreboard");
}