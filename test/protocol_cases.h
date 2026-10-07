#pragma once

struct MegaProtocolCase {
    const char* name;
    const char* frame;
    bool valid;
};

static const MegaProtocolCase MEGA_PROTOCOL_CASES[] = {
    {"valid command", "cmd:1:ACS_G:on#", true},
    {"valid setpoint command", "cmd:25:TEMP_ACS:45#", true},
    {"truncated command", "cmd:1:ACS_G:on", false},
    {"empty frame", "#", false},
    {"empty payload", "cmd:2:#", false},
    {"non numeric command sequence", "cmd:x:ACS_G:on#", false},
};
