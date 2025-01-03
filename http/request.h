#ifndef REQUEST
#define REQUEST

#include "config.h"
#include <stdlib.h>

struct HttpHeader
{
    char name[MAX_HEADER_NAME_LENGTH];
    char value[MAX_HEADER_VALUE_LENGTH];
};

struct HttpRequest
{
    char method[MAX_METHOD_LENGTH];
    char url[MAX_URL_LENGTH];
    char version[MAX_VERSION_LENGTH];
    struct HttpHeader headers[MAX_HEADERS];
    int header_count;
    char *body;
    size_t body_length;
};

void url_decode(char *src, char *dest);

void parse_http_request(char *buffer, struct HttpRequest *request);

/**
 * Gets the relative path from the HTTP Request
 * 
 * Caution: must call free()
 * 
 */
char *get_requested_file(struct HttpRequest *request, const char *directory);

#endif