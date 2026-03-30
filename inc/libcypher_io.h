#include <sys/types.h>
#include <stdint.h>

#define CY_BUFF_SIZE 2048

typedef struct 
{
    size_t cy_len;
    uint8_t cy_pad_flag;
    uint8_t cy_pad_len;
    uint8_t cy_key_flag;
    uint8_t cy_key_type;
    uint8_t cy_enc_flag;
    uint8_t cy_enc_type;
    uint8_t cy_hash_flag;
    uint8_t cy_hash_type;
}CY_HEADER;

typedef struct
{
    CY_HEADER head;
    uint8_t *buffer;
}CY_BUFF;

void cy_state(const char *str, int _errnum);

void cy_read(int fd, CY_BUFF *buf);

void cy_read_line(int fd, CY_BUFF *buf);

void cy_write(int fd, const CY_BUFF buf);