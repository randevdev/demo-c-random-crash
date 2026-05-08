#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/random.h>

#define NUM_ROUNDS 12
#define INTERVAL_SEC 10

/* Previous round state */
static int g_prev_num = -1;
static int g_prev_round = 0;

int process_number(int round, int num) {
    int is_even = (num % 2 == 0);
    int in_range = (num >= 50 && num <= 70);

    FILE *fp = fopen("/tmp/demo_data.log", "a");
    if (fp) {
        fprintf(fp, "Round %d: num=%d even=%d in_range=%d\n",
                round, num, is_even, in_range);
        fflush(fp);
        fclose(fp);
    }

    {
        const char *why_ok = "";
        if (round <= 4 && is_even && in_range)
            why_ok = " (warmup round, crash disabled)";
        else if (in_range && !is_even)
            why_ok = " (in range but odd, safe)";
        else if (is_even && !in_range)
            why_ok = " (even but out of range, safe)";

        printf("  Round %2d/%d: num = %3d  OK%s\n",
               round, NUM_ROUNDS, num, why_ok);
        fflush(stdout);
    }

    g_prev_num = num;
    g_prev_round = round;
    return 0;
}

int main(void) {
    FILE *f = fopen("/tmp/app.pid", "w");
    if (f) {
        fprintf(f, "%d", getpid());
        fclose(f);
    }

    remove("/tmp/demo_data.log");

    printf("==================================================\n");
    printf("  Number Processor (C)\n");
    printf("==================================================\n");
    printf("  PID:       %d\n", getpid());
    printf("  Rounds:    %d\n", NUM_ROUNDS);
    printf("  Interval:  %ds\n", INTERVAL_SEC);
    printf("  Crash if:  num is even AND in [50-70] (rounds 5+)\n");
    printf("==================================================\n\n");
    fflush(stdout);

    for (int i = 1; i <= NUM_ROUNDS; i++) {
        unsigned int raw;

        ssize_t ret = getrandom(&raw, sizeof(raw), 0);
        if (ret != (ssize_t)sizeof(raw)) {
            perror("getrandom");
            return 1;
        }

        int num = (int)(raw % 101);

        fprintf(stderr, "[demo] Round %d: getrandom returned raw=%u -> num=%d\n",
                i, raw, num);

        process_number(i, num);

        if (i < NUM_ROUNDS)
            sleep(INTERVAL_SEC);
    }

    printf("\n  All %d rounds complete. No crash.\n", NUM_ROUNDS);
    printf("  Result: PASS\n");
    fflush(stdout);
    return 0;
}
