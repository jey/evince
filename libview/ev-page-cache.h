/* this file is part of evince, a gnome document viewer
 *
 *  Copyright (C) 2005 Red Hat, Inc
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

#if !defined (__EV_EVINCE_VIEW_H_INSIDE__) && !defined (EVINCE_COMPILATION)
#error "Only <evince-view.h> can be included directly."
#endif

#ifndef __EV_PAGE_CACHE_H__
#define __EV_PAGE_CACHE_H__

#include <gtk/gtk.h>

#include <evince-document.h>

G_BEGIN_DECLS
#define EV_TYPE_PAGE_CACHE            (ev_page_cache_get_type ())
#define EV_PAGE_CACHE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), EV_TYPE_PAGE_CACHE, EvPageCache))
#define EV_IS_PAGE_CACHE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), EV_TYPE_PAGE_CACHE))

GType          ev_page_cache_get_type            (void) G_GNUC_CONST;

/* Navigation */
gint           ev_page_cache_get_current_page    (EvPageCache *page_cache);
void           ev_page_cache_set_current_page    (EvPageCache *page_cache,
						  int          page);
void           ev_page_cache_set_current_page_history  (EvPageCache *page_cache,
							int          page);
gboolean       ev_page_cache_set_page_label      (EvPageCache *page_cache,
						  const gchar *page_label);

EvPageCache   *ev_page_cache_get		 (EvDocument *document);

G_END_DECLS

#endif /* __EV_PAGE_CACHE_H__ */
