#ifndef PROGRAMSETTINGS_H
#define PROGRAMSETTINGS_H

struct ProgramSettings {
    bool reverseOrder = false;
};

const struct ProgramSettings default_set;
extern struct ProgramSettings properties;

#endif // PROGRAMSETTINGS_H
