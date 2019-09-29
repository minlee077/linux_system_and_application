// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2000-2001 Silicon Graphics, Inc.  All Rights Reserved.
 */
#ifndef __XFS_ITABLE_H__
#define	__XFS_ITABLE_H__

/* In-memory representation of a userspace request for batch inode data. */
struct xfs_ibulk {
	struct xfs_mount	*mp;
	void __user		*ubuffer; /* user output buffer */
	xfs_ino_t		startino; /* start with this inode */
	unsigned int		icount;   /* number of elements in ubuffer */
	unsigned int		ocount;   /* number of records returned */
	unsigned int		flags;    /* see XFS_IBULK_FLAG_* */
};

/* Only iterate within the same AG as startino */
#define XFS_IBULK_SAME_AG	(XFS_IWALK_SAME_AG)

/* Return value that means we want to abort the walk. */
#define XFS_IBULK_ABORT		(XFS_IWALK_ABORT)

/*
 * Advance the user buffer pointer by one record of the given size.  If the
 * buffer is now full, return the appropriate error code.
 */
static inline int
xfs_ibulk_advance(
	struct xfs_ibulk	*breq,
	size_t			bytes)
{
	char __user		*b = breq->ubuffer;

	breq->ubuffer = b + bytes;
	breq->ocount++;
	return breq->ocount == breq->icount ? XFS_IBULK_ABORT : 0;
}

/*
 * Return stat information in bulk (by-inode) for the filesystem.
 */

typedef int (*bulkstat_one_fmt_pf)(struct xfs_ibulk *breq,
		const struct xfs_bulkstat *bstat);

int xfs_bulkstat_one(struct xfs_ibulk *breq, bulkstat_one_fmt_pf formatter);
int xfs_bulkstat(struct xfs_ibulk *breq, bulkstat_one_fmt_pf formatter);
void xfs_bulkstat_to_bstat(struct xfs_mount *mp, struct xfs_bstat *bs1,
		const struct xfs_bulkstat *bstat);

typedef int (*inumbers_fmt_pf)(struct xfs_ibulk *breq,
		const struct xfs_inumbers *igrp);

int xfs_inumbers(struct xfs_ibulk *breq, inumbers_fmt_pf formatter);
void xfs_inumbers_to_inogrp(struct xfs_inogrp *ig1,
		const struct xfs_inumbers *ig);

#endif	/* __XFS_ITABLE_H__ */
