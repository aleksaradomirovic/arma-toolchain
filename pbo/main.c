/*
 * This is a collection of Arma 3 modding tools.
 * Copyright (C) 2024  Aleksa Radomirovic
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, 51 Franklin Street,
 * Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include "license.h"
#include "modes.h"

#include <argp.h>
#include <error.h>
#include <stdlib.h>

int (*mode_f)() = NULL;
const char * errstr = NULL;

const char * archive_path = NULL;

static const struct argp_option args_opts[] = {
    { "list", 't', 0, 0, "list files in archive", 1 },
    
    { "file", 'f', "ARCHIVE", 0, "use archive file ARCHIVE", 2 },

    { "version", 'V', 0, 0, "print version info", 3 },

    { 0 }
};

static int mode_fast_exit() {
    exit(0);
    return 0;
}

static int args_parser(int key, char * arg, struct argp_state * state) {
    switch(key) {
        case 't': {
            if(mode_f != NULL) argp_error(state, "operating mode already set");
            else mode_f = pbo_mode_list_contents;
        } break;
        case 'f': {
            archive_path = arg;
        } break;
        case 'V': {
            print_copyright_info();
            mode_f = mode_fast_exit;
        } break;
        default: return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

static const struct argp args_info = {
    .options = args_opts,
    .parser = args_parser,
};

int main(int argc, char * argv[]) {
    int status;

    int idx;
    status = argp_parse(&args_info, argc, argv, 0, &idx, NULL);
    if(status != 0) return -1;

    if(!mode_f) {
        error(-1, 0, "operating mode not defined");
    }

    status = mode_f();
    if(status != 0) {
        error(-1, errno, "error");
    }

    return 0;
}
