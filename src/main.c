#define _XOPEN_SOURCE 600
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/reboot.h>


double read_thermal_zone0() {
    FILE *f;
    double temp = 0.0f;

    f = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
    fscanf(f, "%lf", &temp);
    fclose(f);

    return temp / 1000;
}

void do_halt() {
    sync();
    sleep(10);
    reboot(RB_ENABLE_CAD);
    reboot(RB_HALT_SYSTEM);
}

long int env_lint(const char *name, long int defval) {
    long val;
    char *end;
    char *s = getenv(name);
    if (s == NULL)
        return defval;

    errno = 0;
    val = strtol(s, &end, 10);
    if (errno == ERANGE || end == s ) {
        printf("Invalid number given for %s (%s)", name, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return val;
}

int main(int argc, char *argv[]) {
    unsigned int counter = 0;
    double temp;

    double env_temp_off = (double) env_lint("TEMP_OFF", 70);
    int env_check_count = (int) env_lint("CHECK_COUNT", 5);
    int env_check_wait = (int) env_lint("CHECK_WAIT", 20);

    chdir("/");

    // disable buffering so journald gets our logs straight away
    setbuf(stdout, NULL);

    printf(
        "starting with a kill temp of %3.2f for a count of %d, checking every %d seconds.\n",
        env_temp_off, env_check_count, env_check_wait
    );

    while (true) {
        temp = read_thermal_zone0();
        printf("temp: %6.3fc\n", temp);
        if (temp > env_temp_off) {
            counter++;
            printf("temp: above %2.2f for count %d\n", env_temp_off, counter);
            if (counter > env_check_count) {
                printf("Halting...\n");
                do_halt();
            }
        } else {
            if (counter > 0)
                counter--;
        }
        sleep(env_check_wait);
    }

    return EXIT_SUCCESS;
}
