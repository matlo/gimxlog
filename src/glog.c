/*
 Copyright (c) 2018 Mathieu Laurendeau <mat.lau@laposte.net>
 License: GPLv3
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gimxcommon/include/gerror.h>
#include <gimxcommon/include/glist.h>
#include <gimxlog/include/glog.h>

struct glog {
    char * name;
    GLOG_CALLBACK callback;
    GLIST_LINK(struct glog);
};

static void glog_unregister(struct glog * log) {

    GLIST_REMOVE(logs, log);

    free(log->name);
    free(log);
}

static GLIST_INST(struct glog, logs);
GLIST_DESTRUCTOR(logs, glog_unregister)

static struct glog * get_log(const char * name) {

    struct glog * current;
    for (current = GLIST_BEGIN(logs); current != GLIST_END(logs); current = current->next) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
    }
    return NULL;
}

void glog_register(const char * name, GLOG_CALLBACK callback) {

    struct glog * log = get_log(name);
    if (log != NULL) {
        fprintf(stderr, "%s:%d log name already used: %s\n", __FILE__, __LINE__, name);
        exit(-1);
    }
    log = calloc(1, sizeof(*log));
    if (log == NULL) {
        fprintf(stderr, "%s:%d calloc failed\n", __FILE__, __LINE__);
        exit(-1);
    }
    log->name = strdup(name);
    if (log->name == NULL) {
        fprintf(stderr, "%s:%d strdup failed", __FILE__, __LINE__);
        free(log);
        exit(-1);
    }
    log->callback = callback;
    GLIST_ADD(logs, log);
}

void glog_set_level(const char * name, e_glog_level level) {

    struct glog * log = get_log(name);
    if (log == NULL) {
        fprintf(stderr, "%s:%d log name not found: %s\n", __FILE__, __LINE__, name);
    } else {
        log->callback(level);
    }
}

void glog_set_all_levels(e_glog_level level) {

    struct glog * current;
    for (current = GLIST_BEGIN(logs); current != GLIST_END(logs); current = current->next) {
        current->callback(level);
    }
}
