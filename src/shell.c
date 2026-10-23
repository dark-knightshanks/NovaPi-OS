#include "printf.h"
#include "pl011.h"
#include "shell.h"

int str_cmp(const char *str1, const char *str2){
    while(*str1 != '\0' && (*str1 == *str2)){
        str1++;
        str2++;
    }
    return (*str1 - *str2);
}

void shell_readline(char *buffer, int max_length){
    int index = 0;
    while(1){
        char c = uart_recv();
        if(c == '\r' || c == '\n'){
            buffer[index] = '\0';
            uart_send('\r');
            uart_send('\n');
            return;
        }
        else if (c == '\b' || c == 127){
            if(index > 0){
                index = index - 1;
                uart_send('\b');
                uart_send(' ');
                uart_send('\b');
            }
        }
        else if (c >= 32 && c <= 126){
            if(index < (max_length - 1)){
                buffer[index] = c;
                index = index + 1;
                uart_send(c);
            }
        }
    }
}

int parse_cmdline(char *buffer, char *argv[], int max_args){
    int argc = 0;
    char *ptr = buffer;

    while(*ptr != '\0' && argc < max_args){
        while (*ptr == ' ' || *ptr == '\t'){
            ptr++;
        }
        if(*ptr == '\0'){ 
            break;
        }

        argv[argc] = ptr;
        argc++;

        while(*ptr != '\0' && *ptr != ' ' && *ptr != '\t'){
            ptr++;
        }

        if(*ptr != '\0'){ 
            *ptr = '\0';
            ptr++;
        }
    }
    return argc;
}
