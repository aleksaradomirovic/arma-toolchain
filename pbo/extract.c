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
#include "modes.h"
#include "pbo.h"

#include "cfs/path.h"
#include "cfs/dir.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

static int pipe_from_archive(FILE * archive, FILE * exf, size_t size) {
    for(size_t i = 0; i < size; i++) {
        int c = fgetc(archive);
        if(c == EOF) return -1;
        if(fputc(c, exf) == EOF) return -1;
    }

    return 0;
}

int pbo_mode_extract() {
    int status;
    
    FILE * archive = fopen(archive_path, "rb");
    if(!archive) {
        errstr = archive_path;
        return -1;
    }

    struct pbo_entry * list;
    status = list_pbo_entries(archive, &list);
    if(status != 0) {
        int err = errno;
        fclose(archive);
        errno = err;
        errstr = "error while reading archive";
        return -1;
    }

    for(struct pbo_entry * ent = list; ent != NULL; ent = ent->next) {
        if(ent == list && strlen(ent->path) == 0) continue;
        char * parentdir = malloc(strlen(ent->path) + 1);
        if(!parentdir) {
            int err = errno;
            free_pbo_entries(list);
            fclose(archive);
            errno = err;
            errstr = "error while extracting archive";
            return -1;
        }

        strcpy(parentdir, ent->path);
        if(parent_path(parentdir) != 0) {
            int err = errno;
            free(parentdir);
            free_pbo_entries(list);
            fclose(archive);
            errno = err;
            errstr = "error while extracting archive";
            return -1;
        }

        if(create_directory_recursive(parentdir) != 0) {
            int err = errno;
            free(parentdir);
            free_pbo_entries(list);
            fclose(archive);
            errno = err;
            errstr = "error while extracting archive";
            return -1;
        }

        free(parentdir);

        FILE * exf = fopen(ent->path, "w");
        if(!exf) {
            int err = errno;
            free_pbo_entries(list);
            fclose(archive);
            errno = err;
            errstr = "error while extracting archive";
            return -1;
        }

        if(pipe_from_archive(archive, exf, ent->data_size) != 0) {
            int err = errno;
            free_pbo_entries(list);
            fclose(exf);
            fclose(archive);
            errno = err;
            errstr = "error while extracting archive";
            return -1;
        }

        if(fclose(exf) != 0) {
            int err = errno;
            free_pbo_entries(list);
            fclose(archive);
            errno = err;
            errstr = "error while extracting archive";
            return -1;
        }
    }

    free_pbo_entries(list);
    status = fclose(archive);
    if(status != 0) {
        errstr = archive_path;
        return -1;
    }
    return 0;
}
