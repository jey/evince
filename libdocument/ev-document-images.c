/* ev-document-images.c
 *  this file is part of evince, a gnome document_links viewer
 * 
 * Copyright (C) 2006 Carlos Garcia Campos <carlosgc@gnome.org>
 *
 * Evince is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Evince is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 */

#include <config.h>
#include "ev-document-images.h"

GType
ev_document_images_get_type (void)
{
	static GType type = 0;

	if (G_UNLIKELY (type == 0)) {
		const GTypeInfo our_info = {
			sizeof (EvDocumentImagesIface),
			NULL,
			NULL,
		};

		type = g_type_register_static (G_TYPE_INTERFACE,
					       "EvDocumentImages",
					       &our_info, (GTypeFlags)0);
	}

	return type;
}

GList *
ev_document_images_get_images (EvDocumentImages *document_images,
			       gint              page)
{
	EvDocumentImagesIface *iface = EV_DOCUMENT_IMAGES_GET_IFACE (document_images);
	GList *retval;

	retval = iface->get_images (document_images, page);

	return retval;
}


