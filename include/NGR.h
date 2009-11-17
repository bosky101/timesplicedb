/*
 * Copyright (c) 2009 Artur Bergman <sky@crucially.net>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/types.h>


#include <sys/uio.h>


#define NGR_GAUGE   0
#define NGR_COUNTER 1

#define NGR_NULL (0)
#define WARN(args...) fprintf(stderr, ## args);
#define strneq(s1, s2, n) (strncmp((s1), (s2), n) == 0)


struct NGR_metric_t {
  unsigned   magic;
#define NGR_metric_magic 0x35fac554

  u_int32_t  width;      /* are we in 32bit mode or 64bit  (4 bytes versus 8 bytes) */
  int        fd;         /* the underlying file */
  int        base;       /* how many bytes the header consumes */
  time_t     created;    /* timestamp the first row in the series is */
  int        resolution; /* distance in seconds between each row in the series */
  int        version;    /* storage verson */
  int        columns;    /* how many columns this store has */
  int        *flags;     /* first is flag for db, rest is per column */
  char       **names;    /* first entry is the database name, rest is the column names */
  int        ngrerrno;   /* set to non 0 if the latest function recorded an error */
  int        syserrno;   /* underlying errno if result of a syscall */
  int        ranges;     /* refcount off ranges */
};

struct NGR_range_t {
  unsigned   magic;
#define NGR_range_magic 0x6428b5c9

  unsigned int *row;                  /* ptr into the first row in the range */
  void *area;                  /* if a range and a not an aggregate this points to the mmap of the file */ 
  int rows;                    /* how many rows exist in the range */
  size_t len;
  int mmap;                    /* if this is an mmaped range or not */
  int resolution;              /* resolution requested for this range  */
  int columns;                 /* how many columns are present in this range */
  struct NGR_agg_row_t *agg; /* If this is an aggregate range, this contains a pointer to extra data 
				  calculated during aggregaton
				  these are values we can get doing a single pass only
				  the reason this datacenter is dual purpose
				  is so you can call aggregate on already aggregated ranges
			       */
  struct NGR_metric_t *metric; /* where did we come from ? */
};

struct NGR_agg_row_t {
  double avg;    /* average value in interval */
  unsigned int max;    /* max value seen in interval */
  unsigned int min;    /* minimum value seen -- flag determines if 0 is considered minium or undefined value */
  double stddev; /* calculated stddev -- no idea if this is correct -- probably isn't */
  int rows_averaged; /* how many rows went into this */
};

struct NGR_metric_t * NGR_create(const char *filename, time_t create_time, int resolution, int columns, char **names, int *flags);

struct NGR_metric_t * NGR_open(const char *filename);


int NGR_last_row_idx (struct NGR_metric_t *obj, int column);

int NGR_cell (struct NGR_metric_t *obj, int row, int column);

struct NGR_range_t * NGR_range (struct NGR_metric_t *obj, int start, int end);

void NGR_range_free (struct NGR_range_t *range);

struct NGR_range_t * NGR_timespan (struct NGR_metric_t *obj, time_t start, time_t end);

struct NGR_range_t * NGR_aggregate (struct NGR_range_t *range, int interval, int data_type);

int NGR_insert (struct NGR_metric_t *obj, int column, time_t timestmp, int value);
