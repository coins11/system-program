#include<assert.h>
#include<stdlib.h>
#include<string.h>

typedef struct {
    enum { PRO_HTTP, PRO_HTTPS } protocol;
    char *hostname;
    u_short port;
    char *path;
} parseduri_t;

/*
    int uri_parser(const char *string, parseduri_t **uri_);
    description: Parses string `string' as URI, and stores result in `*uri_'.
    return value:
        0: success to parse
        -1: memory allocation error
        -2: parsing error
*/
int uri_parser(const char *string, parseduri_t **uri_)
{
    size_t iter;
    parseduri_t *uri;

    assert(uri_);

    uri = malloc(sizeof(parseduri_t));
    if(!uri) return -1;
    /* URI scheme name */
    if(strncmp("http://", string, 7) == 0){
        string += 7;
        uri->protocol = PRO_HTTP;
    }
    else if(strncmp("https://", string, 8) == 0){
        string += 8;
        uri->protocol = PRO_HTTP;
    }
    else{
        free(uri);
        return -2;
    }
    /* host name */
    for(iter = 0;
        string[iter] != '\0' && string[iter] != '/' && string[iter] != ':';
        iter++);
    uri->hostname = strndup(string, iter);
    if(!uri->hostname){
        free(uri);
        return -1;
    }
    string += iter;
    /* port number */
    if(*string == ':'){
        char *tmp;
        string++;
        uri->port = strtol(string, &tmp, 10);
        string = tmp;
    }
    else uri->port = 80;
    /* path */
    if(*string == '/') string++;
    else if(*string != '\0'){
        free(uri->hostname);
        free(uri);
        return -2;
    }
    uri->path = strdup(string);

    *uri_ = uri;
    return 0;
}



int main(int argc, char **argv)
{
    
    return 0;
}
