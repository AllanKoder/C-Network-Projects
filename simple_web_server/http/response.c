#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include "request.h"
#include "config.h"

// Networks
#include <arpa/inet.h>
#include <sys/socket.h>

#define HEADERS_404 "HTTP/1.1 404 Not Found\r\n"
#define HEADERS_HTML "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n"
#define HEADERS_TEXT "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n"
#define HEADERS_DOWNLOAD "HTTP/1.1 200 OK\r\nContent-Type: application/octet-stream\r\n"

void send_message(int socket, const char *message)
{
    size_t length = strlen(message);
    ssize_t bytes_sent = send(socket, message, length, 0);

    if (bytes_sent == -1)
    {
        perror("send");
    }
    else if ((size_t)bytes_sent < length)
    {
        fprintf(stderr, "Partial send\n");
    }
}

void send_404_page(int socket)
{
    send_message(socket, HEADERS_404);
}

void send_response(int socket, const char *headers, const char *content, size_t content_length) {
    size_t max_size = MAX_HEADERS_SIZE + content_length + 1;
    char *response_buffer = malloc(max_size);

    if (response_buffer == NULL) {
        perror("send_response malloc");
        return;
    }

    snprintf(response_buffer, max_size,
             "%sContent-Length: %lu\r\n\r\n%s",
             headers, content_length, content); // Construct response

    printf("Responding:\n%s\n", response_buffer);
    send_message(socket, response_buffer);
    free(response_buffer);
}

void send_html(int socket, const char *html) {
    size_t content_length = strlen(html);
    send_response(socket, HEADERS_HTML, html, content_length);
}

void send_text(int socket, const char *text) {
    size_t content_length = strlen(text);
    send_response(socket, HEADERS_TEXT, text, content_length);
}

void send_download(int socket, const char *filename, const char *bytes) {
    size_t content_length = strlen(bytes);
    char headers[MAX_HEADERS_SIZE];

    snprintf(headers, sizeof(headers),
             HEADERS_DOWNLOAD 
             "Content-Disposition: attachment; filename=\"%s\"\r\n",
             filename); // Construct headers

    send_response(socket, headers, bytes, content_length);
}