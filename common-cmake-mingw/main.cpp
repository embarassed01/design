#include "common-cmake-mingw.h"

int main(int argc, char ** argv) {
    if (argc != 2)
    {
        fprintf(stderr, "error: must input one param (infile, or outfile path)\n");
        return -1;
    }
    const char * in_file = argv[1];

    int in_file_size = get_file_size(in_file);
    u8 * out_buf = (u8*)malloc(in_file_size + 128);
    memset(out_buf, 0x00, in_file_size + 128);

    read_file(in_file, out_buf, in_file_size + 128);

    return 0;
}
