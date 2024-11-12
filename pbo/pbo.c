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
#include "pbo.h"

#include "cfs/path.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

static void free_pbo_properties(struct pbo_property * list) {
    for(; list != NULL;) {
        struct pbo_property * next = list->next;
        free(list->key);
        free(list->value);
        free(list);
        list = next;
    }
}

void free_pbo_entries(struct pbo_entry * list) {
    for(; list != NULL;) {
        struct pbo_entry * next = list->next;
        free(list->path);
        free_pbo_properties(list->properties);
        free(list);
        list = next;
    }
}

static int read_string(FILE * f, char ** strptr) {
    char * str = NULL;
    for(size_t i = 0, sz = 0;; i++) {
        if(i >= sz) {
            char * nstr = realloc(str, sz += 16);
            if(!nstr) {
                int err = errno;
                free(str);
                errno = err;
                return -1;
            }
            str = nstr;
        }

        int c = fgetc(f);
        if(c == EOF) {
            int err = errno;
            free(str);
            errno = err;
            return -1;
        }

        str[i] = (char) c;
        if(c == '\0') break;
    }

    *strptr = str;
    return 0;
}

static int read_pbo_entry_fields(FILE * f, struct pbo_entry * ent) {
    if(
        // fread(&(ent->mime_type)    , 4, 1, f) != 1 ||
        // fread(&(ent->original_size), 4, 1, f) != 1 ||
        // fread(&(ent->offset)       , 4, 1, f) != 1 ||
        // fread(&(ent->time_stamp)   , 4, 1, f) != 1 ||
        // fread(&(ent->data_size)    , 4, 1, f) != 1
        fseek(f, 20, SEEK_CUR) != 0
    ) {
        return -1;
    }

    return 0;
}

static int read_pbo_property(FILE * f, struct pbo_property ** proptr) {
    int status;

    char * key;
    status = read_string(f, &key);
    if(status != 0) {
        return -1;
    } else if(strlen(key) == 0) {
        free(key);
        *proptr = NULL;
        return 0;
    }

    struct pbo_property * pro = malloc(sizeof(struct pbo_property));
    if(!pro) {
        int err = errno;
        free(key);
        errno = err;
        return -1;
    }
    pro->key = key;
    pro->next = NULL;

    status = read_string(f, &pro->value);
    if(status != 0) {
        int err = errno;
        free(key);
        free(pro);
        errno = err;
        return -1;
    }

    *proptr = pro;
    return 0;
}

static int list_pbo_properties(FILE * f, struct pbo_property ** listptr) {
    int status;

    struct pbo_property * list = NULL;
    for(struct pbo_property * curr = NULL;;) {
        struct pbo_property * next;
        status = read_pbo_property(f, &next);
        if(status != 0) {
            int err = errno;
            free_pbo_properties(list);
            errno = err;
            return -1;
        }

        if(curr != NULL) {
            curr->next = next;
        } else {
            list = next;
        }
        curr = next;

        if(next == NULL) break;
    }

    *listptr = list;
    return 0;
}

static int read_pbo_entry(FILE * f, struct pbo_entry ** entptr) {
    int status;

    struct pbo_entry * ent = malloc(sizeof(struct pbo_entry));
    if(!ent) {
        return -1;
    }

    ent->properties = NULL;
    ent->next = NULL;

    status = read_string(f, &ent->path);
    if(status != 0) {
        int err = errno;
        free(ent);
        errno = err;
        return -1;
    }
    status = win_to_host_path(ent->path);
    if(status != 0) {
        int err = errno;
        free(ent->path);
        free(ent);
        errno = err;
        return -1;
    }

    status = read_pbo_entry_fields(f, ent);
    if(status != 0) {
        int err = errno;
        free(ent->path);
        free(ent);
        errno = err;
        return -1;
    }

    *entptr = ent;
    return 0;
}

int list_pbo_entries(FILE * pbo, struct pbo_entry ** listptr) {
    int status;

    struct pbo_entry * list = NULL;
    for(struct pbo_entry * curr = NULL;;) {
        struct pbo_entry * next;
        status = read_pbo_entry(pbo, &next);
        if(status != 0) {
            int err = errno;
            free_pbo_entries(list);
            errno = err;
            return -1;
        }

        if(strlen(next->path) == 0) {
            if(curr == NULL) {
                status = list_pbo_properties(pbo, &next->properties);
                if(status != 0) {
                    int err = errno;
                    free_pbo_entries(next);
                    free_pbo_entries(list);
                    errno = err;
                    return -1;
                }
            } else {
                free_pbo_entries(next);
                break;
            }
        }

        if(curr != NULL) {
            curr->next = next;
        } else {
            list = next;
        }
        curr = next;
    }

    *listptr = list;
    return 0;
}
