#ifndef _COUCHBOTDEFS_H
#define _COUCHBOTDEFS_H

// Parameter EEPROM addresses
const int outEnAddr    = 0;  // boolean
const int revRightAddr = 1;  // boolean
const int revLeftAddr  = 2;  // boolean
const int dzEnAddr     = 3;  // boolean
const int veloDZAddr   = 4;  // uint16_t
const int diffDZAddr   = 6;  // uint16_t
const int maxDiffAddr  = 8;  // uint16_t
const int maxPWMAddr   = 10; // uint8_t
const int minPWMAddr   = 11; // uint8_t
const int CH1_MAXAddr  = 12; // uint16_t
const int CH1_MINAddr  = 14; // uint16_t
const int CH2_MAXAddr  = 16; // uint16_t
const int CH2_MINAddr  = 18; // uint16_t
const int CH3_MAXAddr  = 20; // uint16_t
const int CH3_MINAddr  = 22; // uint16_t
const int CH8_MINAddr  = 24; // uint16_t
const int CH8_MAXAddr  = 26; // uint16_t

// Parameters
const String outEnStr    = "outputEnable";
const String revRightStr = "reverseRight";
const String revLeftStr  = "reverseLeft";
const String dzEnaStr    = "deadZoneEnable";
const String veloDZStr   = "veloDeadZone";
const String diffDZStr   = "diffDeadZone";
const String maxDiffStr  = "maxDifference";
const String maxPWMStr   = "maxPWM";
const String minPWMStr   = "minPWM";
const String ch1maxStr   = "ch1max";
const String ch1minStr   = "ch1min";
const String ch2maxStr   = "ch2max";
const String ch2minStr   = "ch2min";
const String ch3maxStr   = "ch3max";
const String ch3minStr   = "ch3min";
const String ch8maxStr   = "ch8max";
const String ch8minStr   = "ch8min";

// Commands

const String helpCmdStr = "help";
const String lstParamCmdStr = "lpv";
const String debugCmdStr = "dbg";
const String enableCmdStr = "enable";
const String disableCmdStr = "disable";
const String lstChngParamCmdStr = "lcp";
const String chngParamCmdStr = "cp";
const String saveCmdStr = "save";
const String reloadCmdStr = "reload";

// Miscellaneous

const String invalidCmdString = "Invalid Command";
const String invalidPrmString = "Invalid Parameter";
const String boolTrue = "true";
const String boolFalse = "false";

#endif // _COUCHBOTDEFS_H