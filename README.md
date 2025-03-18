# eWeb

The eWeb module is an implementation for handling HTTP server functionality on the ESP32. This module allows you to set up a web server and manage HTTP requests.

## Dependencies

This module depends on the following components:

- [eWifi](https://github.com/acevedoesteban999/eWifi)
- [eSTR](https://github.com/acevedoesteban999/eSTR)
- [eFree](https://github.com/acevedoesteban999/eFree)

## How to Use

CMakeLists.txt

```
    SRCS 
        "main.c"
        "uri_handlers.c"
        
    INCLUDE_DIRS 
        "include"
        
    EMBED_FILES 
        "example.min.html"
        "example.min.js"
        "example.min.css"
    
    REQUIRES  
        eWifi
        eSTR
        eFree   
        
```

uri_handlers.c

```c
#include "eWeb.h"

extern const char example_min_html_asm_start[] asm("_binary_example_min_html_start");
extern const char example_min_html_asm_end[] asm("_binary_example_min_html_end");

extern const char example_min_js_asm_start[] asm("_binary_example_min_js_start");
extern const char example_min_js_asm_end[] asm("_binary_example_min_js_end");

extern const char example_min_css_asm_start[] asm("_binary_example_min_css_start");
extern const char example_min_css_asm_end[] asm("_binary_example_min_css_end");

uri_ctx_hanlder STATIC_URIS[] = {
    {{"/example.min.html", HTTP_GET, eweb_static_html_handler, NULL}, true, {example_min_html_asm_start,example_min_html_asm_end,"text/html",NULL,NULL}},
    {{"/example.min.js", HTTP_GET, eweb_static_handler, NULL}, true, {example_min_html_asm_start,example_min_html_asm_end,"text/javascript",NULL,NULL}},
    {{"/example.min.css", HTTP_GET, eweb_static_handler, NULL}, true, {example_min_html_asm_start,example_min_html_asm_end,"text/css",NULL,NULL}},
};

const int STATIC_URIS_LEN = sizeof(static_uris)/sizeof(uri_ctx_hanlder);

```

### Main Code

```c
#include "eWeb.h"
#include "uri_handlers.c"

void app_main() {

    //Inicizalize
    eweb_init(STATIC_URIS_LEN);
    //Set Uri into WebHandler
    eweb_set_uri_hanlders(STATIC_URIS,STATIC_URIS_LEN);

}
```
