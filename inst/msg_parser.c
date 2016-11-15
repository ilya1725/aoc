/**
 * Serial Message Parser
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * Type of a message
 */
typedef struct {
    uint8_t size;
    uint8_t data[256];
} msg_t;

/**
 * Error codes
 */
typedef enum {
    NO_ERROR = 0,
    ERROR_BAD_PARAM = -1
} error_codes_t;

#define START_MARKER_0    0x21
#define START_MARKER_1    0x22

/**
 * Decoding states
 */
typedef enum {
    SEARCHING,               // Data is coming and we are searching for message
    MARKER_0,                // Found the first start marker
    MARKER_1,                // Found the second start marker
    MSG_SIZE,                // On the msg size 
    MSG_PROPER,              // Reading the message itself
    MSG_OUTPUT               // Printing of the message, transient state
} states_t;

/**
 * Print the application help.
 */
static int
help(char *prog)
{
    printf("Usage: %s [OPTIONS]\n", prog);
    printf("\n");
    printf("  -h|-?  Print this message and exit.\n");
    printf("\n");
    printf("The program reads from stdin and write to stdout.\n");
    printf("Status or debugging messages may be reported on stderr.\n");
    printf("\n");

    printf("%s %s %s\n", prog, __DATE__, __TIME__);
    printf("\n");

    exit(EXIT_SUCCESS);
}

/**
 * helper function to print a message, properly formatted
 */
error_codes_t 
print_msg(FILE *stream, const msg_t *msg) {
    if (!stream || !msg) {
        return ERROR_BAD_PARAM;
    }

    /* Do the actual format */
    fprintf(stream, "{%3d}", msg->size);
    for (uint8_t i = 0; i < msg->size; i++) {
        fprintf(stream, " %02X", msg->data[i]);
    }
    fprintf(stream, "\n");
    
    return NO_ERROR;
}

/**
 * helper function to initialize a message
 */
error_codes_t
init_msg(msg_t *msg) {
    if (!msg) {
        return ERROR_BAD_PARAM;
    }

    msg->size = 0;

    return NO_ERROR;
}

int main (int argc, char ** argv) {

    int opt;

    while ((opt = getopt (argc, argv, "?hv")) != -1) {
        switch (opt) {
        case 'h':
        case '?':
            help(argv[0]);
            break;
        }
    }

    /* Keep reading the input stream until we find the start markers.
     * Then let's check the validity of the message.
     */
    states_t read_state = SEARCHING;
    msg_t    this_msg;
    uint8_t  msg_data_cnt = 0;
    uint32_t msg_search_cnt = 0;

    int c = getchar();
    while (c != EOF) {
        switch (read_state) {
            case SEARCHING:
                if (c == START_MARKER_0) {
                    read_state = MARKER_0;
                } else {
                    msg_search_cnt++;
                }
                break;
            case MARKER_0:
                if (c == START_MARKER_1) {
                    read_state = MARKER_1;
                } else {
                    // We didn't get the 2nd marker after 1st, thus move to searching again
                    read_state = SEARCHING;
                }
                break;
            case MARKER_1:
                /* We get size at this point.
                 * There is no way to really verify its validity.
				 * It is returned from getchar as int and we cast it to uint8.
				 * maximum value of uint8 is 256.
				 */
                read_state = MSG_PROPER;
                
                init_msg(&this_msg);
                this_msg.size = (uint8_t)c;
                msg_data_cnt = 0;
                
                // Print some debug stuff
                if (msg_search_cnt > 0) {
                    fprintf(stderr, "dbg: searching for %d points\n", msg_search_cnt);
                }
                fprintf(stderr, "dbg: found msg of size %d\n", this_msg.size);

                // The size could actually be zero
                if (this_msg.size == 0) {
                    read_state = MSG_OUTPUT;
                    continue;
                }
                break;
            case MSG_SIZE:
                break;
            case MSG_PROPER:
                this_msg.data[msg_data_cnt] = (uint8_t)c;
                msg_data_cnt++;
                
                /* There is no provision for the 'tail' of the message indicating where it ends.
                 * As a result, we will read everything from the start to the size regardless of the data.
                 * The data may contain more start message markers, but they will be ignored.
                 */
                if (msg_data_cnt >= this_msg.size) {
                    // Done reading the full image
                    read_state = MSG_OUTPUT;
                    continue;
                }
                break;
            case MSG_OUTPUT:
                print_msg(stdout, &this_msg);
                read_state = SEARCHING;
                msg_search_cnt = 0;
                break;
            default:
                fprintf(stderr, "Invalid state: %d\n", read_state);
        }

        c = getchar();
    }

	if (ferror(stdin)) {
        fprintf(stderr, "Error reading data\n");
	}

    return 0;
}

