#pragma once

#define FONT_MODULE_FILENAME "custom_font.js"

/* Inject code */
static const char* inject_code_start = 
"// Font Module [start]";

static const char* inject_code =
"const custom_font = require('./custom_font.js');\n"
"custom_font.enable();";

static const char* inject_code_end = 
"// Font Module [end]";

/* Font module code */
static const char* font_module =
"\n"
"function enable() {\n"
"    var all = document.getElementsByTagName(\"*\");\n"
"	for (var i=0, max=all.length; i < max; i++) {\n"
"        all[i].style.fontFamily = font;\n"
"	}\n"
"}\n"
"\n"
"module.exports = {\n"
"    enable\n"
"};";