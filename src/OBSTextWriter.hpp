#pragma once
#include <QString>

namespace ObsTextWriter {

// Returns true if the source existed and was updated
bool setText(const char* sourceName, const QString& text);

// Convenience overloads
bool setText(const QString& sourceName, const QString& text);
bool setInt (const char* sourceName, int value);

}