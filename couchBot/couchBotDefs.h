#ifndef _COUCHBOTDEFS_H
#define _COUCHBOTDEFS_H

// Parameters
const String outEnStr    = "outputEnable";
const String revRightStr = "reverseRight"; //
const String revLeftStr  = "reverseLeft"; //
const String dzEnaStr    = "deadZoneEnable"; //
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
const String calibrCmdStr = "calibrate";
const String quitStr     = "quit"; //for any and all long running functions

// Miscellaneous

const String invalidCmdString = "Invalid Command";
const String invalidPrmString = "Invalid Parameter";
const String boolTrue = "true";
const String boolFalse = "false";

#endif // _COUCHBOTDEFS_H