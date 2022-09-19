//
// Created by Apple on 2022/9/16.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern char ** environ;

int main(int argc,char **argv)
{
    for(;*environ != NULL; ++environ) {
        printf("%s\n",*environ);
    }
}