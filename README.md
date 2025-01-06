# eWeb

The eWeb module is an implementation for handling HTTP server functionality on the ESP32. This module allows you to set up a web server and manage HTTP requests.

## Dependencies

This module depends on the following components:
- [eStore](https://github.com/acevedoesteban999/eStore)
- [eWifi](https://github.com/acevedoesteban999/eWifi)

## How to Use

CMakeLists.txt
```
   EMBED_FILES 
        "example.html"
        "example.js"
        "example.css"
        ... 
```
uri_handlers.c
```c
#include "eWeb.h"

extern const char example_html_asm_start[] asm("_binary_example_html_start");
extern const char example_html_asm_end[] asm("_binary_example_html_end");

extern const char example_js_asm_start[] asm("_binary_example_js_start");
extern const char example_js_asm_end[] asm("_binary_example_js_end");

extern const char example_css_asm_start[] asm("_binary_example_css_start");
extern const char example_css_asm_end[] asm("_binary_example_css_end");

uri_ctx_hanlder static_uris[] = {
    {{"/example.html", HTTP_GET, eweb_static_html_handler, NULL}, true, {example_html_asm_start,example_html_asm_end,"text/html"}},
    {{"/example.js", HTTP_GET, eweb_static_handler, NULL}, true, {example_html_asm_start,example_html_asm_end,"text/javascript"}},
    {{"/example.css", HTTP_GET, eweb_static_handler, NULL}, true, {example_html_asm_start,example_html_asm_end,"text/css"}},
};

size_t get_uri_handlers() {
    size_t size = sizeof(static_uris)/sizeof(static_uris[0]);
    for(size_t i =0;i<size;i++)
        eweb_insert_ctx_into_uri(&static_uris[i]);
    return size;
}
```

### Main Code
```c
#include "eWeb.h"
#include "uri_handlers.c"

void app_main() {
    
    uri_ctx_hanlder *uris = static_uris;
    size_t uri_size = get_uri_handlers();

    eweb_init(uri_size);
    eweb_set_custom_uris(uris,uri_size);

}
```


