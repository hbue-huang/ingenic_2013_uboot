/*
 * Generate a random Ethernet address with proper randomness
 * SECURITY FIX: Replaced weak rand() with /dev/urandom
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>

/* Use /dev/urandom for cryptographically secure random */
static int secure_random(void)
{
    uint32_t val;
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd >= 0) {
        ssize_t ret = read(fd, &val, sizeof(val));
        close(fd);
        if (ret == sizeof(val)) {
            return (int)(val & 0x7FFFFFFF);
        }
    }
    /* Fallback - should not happen on Linux */
    fprintf(stderr, "Warning: Could not read from /dev/urandom\n");
    return 0;
}

int
main(int argc, char *argv[])
{
    unsigned long ethaddr_low, ethaddr_high;

    /* SECURITY FIX: Use secure random source instead of weak rand() */
    ethaddr_high = (secure_random() & 0xfeff) | 0x0200;
    ethaddr_low = secure_random();

    printf("%02lx:%02lx:%02lx:%02lx:%02lx:%02lx\n",
        ethaddr_high >> 8, ethaddr_high & 0xff,
        ethaddr_low >> 24, (ethaddr_low >> 16) & 0xff,
        (ethaddr_low >> 8) & 0xff, ethaddr_low & 0xff);

    return (0);
}
