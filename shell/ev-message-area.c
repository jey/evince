/* ev-message-area.c
 *  this file is part of evince, a gnome document viewer
 *
 * Copyright (C) 2007 Carlos Garcia Campos
 *
 * Author:
 *   Carlos Garcia Campos <carlosgc@gnome.org>
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
#include <gtk/gtkaccessible.h>
#include <gtk/gtklabel.h>
#include <gtk/gtkhbox.h>
#include <gtk/gtkvbox.h>
#include <gtk/gtkimage.h>
#include <gtk/gtkstock.h>

#include "ev-message-area.h"

#define EV_MESSAGE_AREA_GET_PRIVATE(obj) \
	(G_TYPE_INSTANCE_GET_PRIVATE ((obj), EV_TYPE_MESSAGE_AREA, EvMessageAreaPrivate))

struct _EvMessageAreaPrivate {
	GtkWidget *image;
	GtkWidget *label;
	GtkWidget *secondary_label;
	
	guint      message_type : 3;
};

enum {
	PROP_0,
	PROP_MESSAGE_TYPE,
	PROP_TEXT,
	PROP_SECONDARY_TEXT,
	PROP_IMAGE
};

static void ev_message_area_set_property (GObject      *object,
					  guint         prop_id,
					  const GValue *value,
					  GParamSpec   *pspec);
static void ev_message_area_get_property (GObject      *object,
					  guint         prop_id,
					  GValue       *value,
					  GParamSpec   *pspec);

G_DEFINE_TYPE (EvMessageArea, ev_message_area, GEDIT_TYPE_MESSAGE_AREA)

static void
ev_message_area_class_init (EvMessageAreaClass *class)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (class);

	gobject_class->set_property = ev_message_area_set_property;
	gobject_class->get_property = ev_message_area_get_property;

	g_object_class_install_property (gobject_class,
					 PROP_MESSAGE_TYPE,
					 g_param_spec_enum ("message-type",
							    "Message Type",
							    "The type of message",
							    GTK_TYPE_MESSAGE_TYPE,
							    GTK_MESSAGE_INFO,
							    G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
	g_object_class_install_property (gobject_class,
					 PROP_TEXT,
					 g_param_spec_string ("text",
							      "Text",
							      "The primary text of the message dialog",
							      NULL,
							      G_PARAM_READWRITE));
	g_object_class_install_property (gobject_class,
					 PROP_SECONDARY_TEXT,
					 g_param_spec_string ("secondary-text",
							      "Secondary Text",
							      "The secondary text of the message dialog",
							      NULL,
							      G_PARAM_READWRITE));
	g_object_class_install_property (gobject_class,
					 PROP_IMAGE,
					 g_param_spec_object ("image",
							      "Image",
							      "The image",
							      GTK_TYPE_WIDGET,
							      G_PARAM_READWRITE));

	g_type_class_add_private (gobject_class, sizeof (EvMessageAreaPrivate));
}

static void
ev_message_area_init (EvMessageArea *area)
{
	GtkWidget *hbox, *vbox;
	
	area->priv = EV_MESSAGE_AREA_GET_PRIVATE (area);

	hbox = gtk_hbox_new (FALSE, 12);
	vbox = gtk_vbox_new (FALSE, 12);
	
	area->priv->label = gtk_label_new (NULL);
	gtk_label_set_use_markup (GTK_LABEL (area->priv->label), TRUE);
	gtk_label_set_line_wrap (GTK_LABEL (area->priv->label), TRUE);
	gtk_label_set_selectable (GTK_LABEL (area->priv->label), TRUE);
	gtk_misc_set_alignment (GTK_MISC (area->priv->label), 0.0, 0.5);
	GTK_WIDGET_SET_FLAGS (area->priv->label, GTK_CAN_FOCUS);
	gtk_box_pack_start (GTK_BOX (vbox), area->priv->label, TRUE, TRUE, 0);
	gtk_widget_show (area->priv->label);
	
	area->priv->secondary_label = gtk_label_new (NULL);
	gtk_label_set_use_markup (GTK_LABEL (area->priv->secondary_label), TRUE);
	gtk_label_set_line_wrap (GTK_LABEL (area->priv->secondary_label), TRUE);
	gtk_label_set_selectable (GTK_LABEL (area->priv->secondary_label), TRUE);
	gtk_misc_set_alignment (GTK_MISC (area->priv->secondary_label), 0.0, 0.5);
	GTK_WIDGET_SET_FLAGS (area->priv->secondary_label, GTK_CAN_FOCUS);
	gtk_box_pack_start (GTK_BOX (vbox), area->priv->secondary_label, TRUE, TRUE, 0);

	area->priv->image = gtk_image_new_from_stock (NULL, GTK_ICON_SIZE_DIALOG);
	gtk_misc_set_alignment (GTK_MISC (area->priv->image), 0.5, 0.0);
	gtk_box_pack_start (GTK_BOX (hbox), area->priv->image, FALSE, FALSE, 0);
	gtk_widget_show (area->priv->image);

	gtk_box_pack_start (GTK_BOX (hbox), vbox, TRUE, TRUE, 0);
	gtk_widget_show (vbox);

	gedit_message_area_set_contents (GEDIT_MESSAGE_AREA (area), hbox);
	gtk_widget_show (hbox);
}

static void
ev_message_area_set_image_for_type (EvMessageArea *area,
				    GtkMessageType type)
{
	const gchar *stock_id = NULL;
	AtkObject   *atk_obj;

	switch (type) {
	case GTK_MESSAGE_INFO:
		stock_id = GTK_STOCK_DIALOG_INFO;
		break;
	case GTK_MESSAGE_QUESTION:
		stock_id = GTK_STOCK_DIALOG_QUESTION;
		break;
	case GTK_MESSAGE_WARNING:
		stock_id = GTK_STOCK_DIALOG_WARNING;
		break;
	case GTK_MESSAGE_ERROR:
		stock_id = GTK_STOCK_DIALOG_ERROR;
		break;
	case GTK_MESSAGE_OTHER:
		break;
	default:
		g_warning ("Unknown GtkMessageType %u", type);
		break;
	}

	if (stock_id)
		gtk_image_set_from_stock (GTK_IMAGE (area->priv->image), stock_id,
					  GTK_ICON_SIZE_DIALOG);

	atk_obj = gtk_widget_get_accessible (GTK_WIDGET (area));
	if (GTK_IS_ACCESSIBLE (atk_obj)) {
		atk_object_set_role (atk_obj, ATK_ROLE_ALERT);
		if (stock_id) {
			GtkStockItem item;

			gtk_stock_lookup (stock_id, &item);
			atk_object_set_name (atk_obj, item.label);
		}
	}
}

static void
ev_message_area_set_property (GObject      *object,
			      guint         prop_id,
			      const GValue *value,
			      GParamSpec   *pspec)
{
	EvMessageArea *area = EV_MESSAGE_AREA (object);

	switch (prop_id) {
	case PROP_MESSAGE_TYPE:
		area->priv->message_type = g_value_get_enum (value);
		ev_message_area_set_image_for_type (area, area->priv->message_type);
		break;
	case PROP_TEXT:
		ev_message_area_set_text (area, g_value_get_string (value));
		break;
	case PROP_SECONDARY_TEXT:
		ev_message_area_set_secondary_text (area, g_value_get_string (value));
		break;
	case PROP_IMAGE:
		ev_message_area_set_image (area, (GtkWidget *)g_value_get_object (value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
ev_message_area_get_property (GObject     *object,
			      guint        prop_id,
			      GValue      *value,
			      GParamSpec  *pspec)
{
	EvMessageArea *area = EV_MESSAGE_AREA (object);

	switch (prop_id) {
	case PROP_MESSAGE_TYPE:
		g_value_set_enum (value, (GtkMessageType) area->priv->message_type);
		break;
	case PROP_TEXT:
		g_value_set_string (value, gtk_label_get_label (GTK_LABEL (area->priv->label)));
		break;
	case PROP_SECONDARY_TEXT:
		g_value_set_string (value, gtk_label_get_label (GTK_LABEL (area->priv->secondary_label)));
		break;
	case PROP_IMAGE:
		g_value_set_object (value, area->priv->image);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

GtkWidget *
ev_message_area_new (GtkMessageType type,
		     const gchar   *text,
		     const gchar   *first_button_text,
		     ...)
{
	GtkWidget *widget;

	widget = g_object_new (EV_TYPE_MESSAGE_AREA,
			       "message-type", type,
			       "text", text,
			       NULL);
	if (first_button_text) {
		va_list args;
		
		va_start (args, first_button_text);
		gedit_message_area_add_buttons_valist (GEDIT_MESSAGE_AREA (widget),
						       first_button_text,
						       args);
		va_end (args);
	}

	return widget;
}

void
ev_message_area_set_image (EvMessageArea *area,
			   GtkWidget     *image)
{
	GtkWidget *parent;

	g_return_if_fail (EV_IS_MESSAGE_AREA (area));

	area->priv->message_type = GTK_MESSAGE_OTHER;

	parent = area->priv->image->parent;
	gtk_container_add (GTK_CONTAINER (parent), image);
	gtk_container_remove (GTK_CONTAINER (parent), area->priv->image);
	gtk_box_reorder_child (GTK_BOX (parent), image, 0);

	area->priv->image = image;

	g_object_notify (G_OBJECT (area), "image");
}

void
ev_message_area_set_text (EvMessageArea *area,
			  const gchar   *str)
{
	g_return_if_fail (EV_IS_MESSAGE_AREA (area));

	if (str) {
		gchar *msg;

		msg = g_strdup_printf ("<b>%s</b>", str);
		gtk_label_set_markup (GTK_LABEL (area->priv->label), msg);
		g_free (msg);
	} else {
		gtk_label_set_markup (GTK_LABEL (area->priv->label), NULL);
	}

	g_object_notify (G_OBJECT (area), "text");
}

void
ev_message_area_set_secondary_text (EvMessageArea *area,
				    const gchar   *str)
{
	g_return_if_fail (EV_IS_MESSAGE_AREA (area));

	if (str) {
		gchar *msg;

		msg = g_strdup_printf ("<small>%s</small>", str);
		gtk_label_set_markup (GTK_LABEL (area->priv->secondary_label), msg);
		g_free (msg);
		gtk_widget_show (area->priv->secondary_label);
	} else {
		gtk_label_set_markup (GTK_LABEL (area->priv->secondary_label), NULL);
		gtk_widget_hide (area->priv->secondary_label);
	}

	g_object_notify (G_OBJECT (area), "secondary-text");
}
