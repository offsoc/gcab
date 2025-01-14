/*
 * LibGCab
 * Copyright (c) 2012, Marc-André Lureau <marcandre.lureau@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA
 */

#ifndef GCAB_PRIV_H
#define GCAB_PRIV_H

#include "config.h"

#include <glib-object.h>
#include "glib-compat.h"

#include "cabinet.h"
#include "gcab-file.h"
#include "gcab-folder.h"
#include "gcab-cabinet.h"
#include "gcab-enums.h"

/* Copied from GStream, FIXME that should be in GLib imho */
#define _GCAB_GET(__data, __idx, __size, __shift) \
    (((guint##__size) (((const guint8 *) (__data))[__idx])) << (__shift))

#define GCAB_READ_UINT32_LE(data)        (_GCAB_GET (data, 3, 32, 24) | \
                                          _GCAB_GET (data, 2, 32, 16) |  \
                                          _GCAB_GET (data, 1, 32,  8) |  \
                                          _GCAB_GET (data, 0, 32,  0))


struct _GCabFile
{
    GObject parent_instance;

    gchar *name;
    gchar *extract_name;
    GFile *file;
    cfile_t cfile;
};

struct _GCabFolder
{
    GObject parent_instance;

    GSList *files;
    GHashTable *hash;
    gint comptype;
    GByteArray *reserved;
    cfolder_t cfolder;
    GInputStream *stream;
};

GCabFolder *     gcab_folder_new_with_cfolder        (const cfolder_t *folder, GInputStream *stream);
GCabFile *       gcab_file_new_with_cfile            (const cfile_t *file);

gboolean         gcab_file_update_info               (GCabFile *file, GFileInfo *info);
gboolean         gcab_file_set_uoffset               (GCabFile *file, guint32 uoffset);

gsize            gcab_folder_get_ndatablocks         (GCabFolder *folder);
gboolean         gcab_folder_extract                 (GCabFolder *self,
                                                      GFile *path,
                                                      guint8 res_data,
                                                      GCabFileCallback file_callback,
                                                      GFileProgressCallback progress_callback,
                                                      gpointer callback_data,
                                                      GCancellable *cancellable,
                                                      GError **error);

#endif /* GCAB_PRIV_H */
