#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include"blake2.h"

typedef unsigned char byte;

byte *read_urandom_device(void) 
{
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0) return NULL;

    byte *buffer = (byte *)malloc(16 * sizeof(byte));
    if (buffer == NULL) 
    {
        close(fd);
        return NULL;
    }

    ssize_t total_bytes = 0;
    while (total_bytes < 16) 
    {
        ssize_t bytes_read = read(fd, buffer + total_bytes, 16 - total_bytes);
        if (bytes_read < 0) 
        {
            if (errno == EINTR) continue;
            close(fd);
            free(buffer);
            return NULL;
        }

        total_bytes += bytes_read;
    }

    close(fd);
    return buffer;
}

byte *generate_hash(void) 
{
    byte *input = read_urandom_device();
    if (input == NULL) return NULL;

    byte *key = read_urandom_device();
    if (key == NULL) 
    {
        free(input);
        return NULL;
    }

    byte *output = (byte *)malloc(16 * sizeof(byte));
    if (output == NULL) 
    {
        free(input);
        free(key);
        return NULL;
    }

    if (blake2b(output, 16, input, 16, key, 16) < 0) 
    {
        free(input);
        free(key);
        free(output);
        return NULL;
    }

    free(input);
    free(key);
    return output;
}

void compute_cpu_resources(size_t n) 
{
    for (size_t i = 0; i < n; i++) 
    {
        byte *digest = generate_hash();

        fprintf(stdout, "DIGEST: ");
        for (size_t j = 0; j < 16; j++) 
        {
            fprintf(stdout, "%02x", digest[j]);
        }
        putchar('\n');

        free(digest);
    }
}

int main(void) 
{
    compute_cpu_resources(/* amount */);
    return 0;
}
