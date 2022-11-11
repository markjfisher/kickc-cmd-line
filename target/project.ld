// Atari XL/XE executable XEX file with a single segment
// https://www.atarimax.com/jindroush.atari.org/afmtexe.html

.plugin "dk.camelot64.kickass.xexplugin.AtariXex"
.file [name="%O", type="bin", segments="File"]

.segmentdef File [segments="Program", modify="XexFormat", _RunAddr=__start, _Order="CLIData,CLICode,Data,Code"]
.segmentdef Program [segments="Code, Data, CLIData, CLICode"]
.segmentdef Code [start=$2000]
.segmentdef Data [startAfter="Code"]
.segmentdef CLIData [startAfter="Data"]
.segmentdef CLICode [start=$3000, modify="XexInit", _InitAddr=parseArgs]
