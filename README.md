# eWeb

The eWeb module is an implementation for handling HTTP server functionality on the ESP32. This module allows you to set up a web server and manage HTTP requests.

## Features

- Initialize the web server.
- Handle static file requests.
- Parse URL-encoded requests.
- Insert custom URI handlers.

## Dependencies

This module depends on the following components:
- [eStore](https://github.com/acevedoesteban999/eStore)
- [eWifi](https://github.com/acevedoesteban999/eWifi)

## Main Functions

- `void eweb_init(uint16_t max_uri)`: Initializes and starts the web server.
- `bool eweb_get_all_data_request(httpd_req_t *req, char *buffer)`: Retrieves all data from an HTTP request.
- `void eweb_set_custom_uris(uri_ctx_hanlder *uri_ctx_handlers, size_t uris_size)`: Registers custom URI handlers.
- `void insert_ctx_into_uri(uri_ctx_hanlder *uri)`: Inserts context into a URI handler.
- `esp_err_t eweb_static_handler(httpd_req_t *req)`: Handles static file requests.
- `bool eweb_get_bool_urlencoded(const char *input, const char *key, bool *value)`: Parses a boolean value from a URL-encoded request.
- `bool eweb_get_string_urlencoded(const char *input, const char *key, char *value, uint max_size)`: Parses a string from a URL-encoded request.
- `bool eweb_get_int_urlencoded(const char *input, const char *key, int *value)`: Parses an integer from a URL-encoded request.
- `bool eweb_get_float_urlencoded(const char *input, const char *key, float *value)`: Parses a float from a URL-encoded request.

## Example Usage

```c
#include "eWeb.h"

void app_main() {
    // Initialize and start the web server
    eweb_init(10);
}
```
