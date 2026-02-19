/*
 * writer.c
 *
 * Assignment: Writer utility implemented in C
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    FILE *file;

    openlog("writer", LOG_PID, LOG_USER);

    if (argc != 3)
    {
        syslog(LOG_ERR, "Invalid number of arguments. Expected 2, got %d", argc - 1);
        fprintf(stderr, "Error: writer requires 2 arguments: <writefile> <writestr>\n");
        closelog();
        return 1;
    }

    char *writefile = argv[1];
    char *writestr = argv[2];

    syslog(LOG_DEBUG, "Writing %s to %s", writestr, writefile);

    file = fopen(writefile, "w");

    if (file == NULL)
    {
        syslog(LOG_ERR, "Error opening file %s: %s", writefile, strerror(errno));
        fprintf(stderr, "Error: Could not create file %s\n", writefile);
        closelog();
        return 1;
    }

    if (fprintf(file, "%s", writestr) < 0)
    {
        syslog(LOG_ERR, "Error writing to file %s: %s", writefile, strerror(errno));
        fprintf(stderr, "Error: Could not write to file %s\n", writefile);
        fclose(file);
        closelog();
        return 1;
    }

    if (fclose(file) != 0)
    {
        syslog(LOG_ERR, "Error closing file %s: %s", writefile, strerror(errno));
        fprintf(stderr, "Error: Could not close file %s\n", writefile);
        closelog();
        return 1;
    }

    closelog();

    return 0;
}

