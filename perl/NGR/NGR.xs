#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "ppport.h"

#include <../../include/NGR.h>

#include "const-c.inc"

MODULE = NGR		PACKAGE = NGR::C	PREFIX=NGR_

INCLUDE: const-xs.inc

struct NGR_range_t *
NGR_aggregate(range, interval, data_type)
	struct NGR_range_t *	range
	int	interval
	int	data_type

struct NGR_metric_t *
NGR_create(filename, create_time, resolution, columns)
	char *	filename
	time_t	create_time
	int	resolution
	int	columns

int
NGR_entry(obj, column, idx)
	struct NGR_metric_t *	obj
	int	column
	int	idx

int
NGR_insert(obj, column, timestmp, value)
	struct NGR_metric_t *	obj
	int	column
	time_t	timestmp
	int	value

int
NGR_last_entry_idx(obj, column)
	struct NGR_metric_t *	obj
	int	column

struct NGR_metric_t *
NGR_open(filename)
	char *	filename

struct NGR_range_t *
NGR_range(obj, column, start, end)
	struct NGR_metric_t *	obj
	int	column
	int	start
	int	end

void
NGR_range_free(range)
	struct NGR_range_t *	range

struct NGR_range_t *
NGR_timespan(obj, column, start, end)
	struct NGR_metric_t *	obj
	int	column
	time_t	start
	time_t	end
