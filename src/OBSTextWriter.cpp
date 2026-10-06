#include "ObsTextWriter.hpp"
#include <obs.h>

namespace ObsTextWriter { // Provides functions to set text and integer values for OBS sources

bool setText(const char* sourceName, const QString& text) // Sets the text of an OBS source with the given name to the specified text
{
    if (!sourceName || !*sourceName)
        return false;

    obs_source_t* src = obs_get_source_by_name(sourceName);
    if (!src)
        return false;

    obs_data_t* settings = obs_source_get_settings(src);
    if (!settings) {
        obs_source_release(src);
        return false;
    }

    // Text (GDI+) uses the "text" setting key
    obs_data_set_string(settings, "text", text.toUtf8().constData());
    obs_source_update(src, settings);

    obs_data_release(settings);
    obs_source_release(src);
    return true;
}

bool setText(const QString& sourceName, const QString& text) // Overloaded function to set the text of an OBS source using QString for the source name
{
    return setText(sourceName.toUtf8().constData(), text);
}

bool setInt(const char* sourceName, int value) // Sets the text of an OBS source with the given name to the string representation of the specified integer value
{
    return setText(sourceName, QString::number(value));
}

}