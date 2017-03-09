/* giwxyplot.c  -  GiwXYPlot widget's source
Copyright (C) 2006  Alexandre Pereira Bueno, Eduardo Parente Ribeiro

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

Maintainers
Alexandre Pereira Bueno - alpebu@yahoo.com.br
James Scott Jr <skoona@users.sourceforge.net>
*/



#include <math.h>
#include <stdio.h>
#include <gtk/gtkmain.h>
#include <gtk/gtksignal.h>
#include <gdk/gdk.h>

#include <giwxyplot.h>

#define XYPLOT_DEFAULT_SIZE 300

/* Forward declarations */

static void giw_xyplot_class_init               (GiwXYPlotClass    *klass);
static void giw_xyplot_init                     (GiwXYPlot         *xyplot);
static void giw_xyplot_destroy                  (GtkObject        *object);
static void giw_xyplot_realize                  (GtkWidget        *widget);
static void giw_xyplot_size_request             (GtkWidget      *widget,
					       GtkRequisition *requisition);
static void giw_xyplot_size_allocate            (GtkWidget     *widget,
					       GtkAllocation *allocation);
static gint giw_xyplot_expose                   (GtkWidget        *widget,
						GdkEventExpose   *event);
static gint giw_xyplot_button_press             (GtkWidget        *widget,
						GdkEventButton   *event);
static gint giw_xyplot_button_release           (GtkWidget        *widget,
						GdkEventButton   *event);
static gint giw_xyplot_motion_notify            (GtkWidget        *widget,
						GdkEventMotion   *event);
static gint giw_xyplot_scroll                   (GtkWidget        *widget,
						GdkEventScroll   *event);
static void giw_xyplot_style_set	              (GtkWidget      *widget,
			                        GtkStyle       *previous_style);

/* Local data */

static GtkWidgetClass *parent_class = NULL;

// To calculate all sizes and things for drawing
static void xyplot_calculate_sizes (GiwXYPlot *xyplot);

// Converts values to pixels coordinates (pixels relative to the origin of the plot)
static void value2pixel(GiwXYPlot *xyplot, gdouble xv, gdouble yv, guint *x, guint *y);

// Converts pixels coordinates to values (pixels relative to the origin of the plot)
static void pixel2value(GiwXYPlot *xyplot, guint xp, guint yp, gdouble *x, gdouble *y);

// A function that calculates the iszes of the layouts of the legends
void xyplot_calculate_legends_sizes(GiwXYPlot *xyplot);

// A function to build the legends
void xyplot_build_legends(GiwXYPlot *xyplot);

// Frees the legends
void xyplot_free_legends(GiwXYPlot *xyplot);

/*********************
* Widget's Functions *
*********************/

guint
giw_xyplot_get_type ()
{
  static guint xyplot_type = 0;

  if (!xyplot_type)
    {
      GtkTypeInfo xyplot_info =
      {
	"GiwXYPlot",
	sizeof (GiwXYPlot),
	sizeof (GiwXYPlotClass),
	(GtkClassInitFunc) giw_xyplot_class_init,
	(GtkObjectInitFunc) giw_xyplot_init,
	/*(GtkArgSetFunc)*/ NULL,
	/*(GtkArgGetFunc)*/ NULL,
      };

      xyplot_type = gtk_type_unique (gtk_widget_get_type (), &xyplot_info);
    }

  return xyplot_type;
}

static void
giw_xyplot_class_init (GiwXYPlotClass *class)
{
  GtkObjectClass *object_class;
  GtkWidgetClass *widget_class;

  object_class = (GtkObjectClass*) class;
  widget_class = (GtkWidgetClass*) class;

  parent_class = gtk_type_class (gtk_widget_get_type ());

  object_class->destroy = giw_xyplot_destroy;

  widget_class->realize = giw_xyplot_realize;
  widget_class->expose_event = giw_xyplot_expose;
  widget_class->size_request = giw_xyplot_size_request;
  widget_class->size_allocate = giw_xyplot_size_allocate;
  widget_class->button_press_event = giw_xyplot_button_press;
  widget_class->button_release_event = giw_xyplot_button_release;
  widget_class->motion_notify_event = giw_xyplot_motion_notify;
  widget_class->scroll_event = giw_xyplot_scroll;
  widget_class->style_set = giw_xyplot_style_set;
}

static void
giw_xyplot_init (GiwXYPlot *xyplot)
{
  g_return_if_fail (xyplot != NULL);
  g_return_if_fail (GIW_IS_XYPLOT (xyplot));

  xyplot->xmin=-1.0;
  xyplot->xmax=1.0;
  xyplot->ymin=-1.0;
  xyplot->ymax=1.0;

  xyplot->legends_digits=8;
  xyplot->hmajor_ticks=11;
  xyplot->hminor_ticks=3;
  xyplot->vmajor_ticks=11;
  xyplot->vminor_ticks=3;

  xyplot->hmajor_grid=TRUE;
  xyplot->hminor_grid=TRUE;
  xyplot->vmajor_grid=TRUE;
  xyplot->vminor_grid=TRUE;

  xyplot->data_list=NULL;
  xyplot->data_amount=0;

  // Mouse policy, everything disabled
  xyplot->mouse_zoom_enabled=FALSE;
  xyplot->mouse_displace_enabled=FALSE;
  xyplot->wheel_zoom_enabled=FALSE;
  xyplot->mouse_autorange_enabled=FALSE;
}

GtkWidget*
giw_xyplot_new (void)
{
  GiwXYPlot *xyplot;

  xyplot = gtk_type_new (giw_xyplot_get_type ());

  return GTK_WIDGET (xyplot);
}

static void
giw_xyplot_destroy (GtkObject *object)
{
  GiwXYPlot *xyplot;

  g_return_if_fail (object != NULL);
  g_return_if_fail (GIW_IS_XYPLOT (object));

  xyplot = GIW_XYPLOT (object);

  if (xyplot->plotting_area) {
    g_object_unref(xyplot->plotting_area);
    xyplot->plotting_area = NULL;
  }

  if (xyplot->back_gc) {
    g_object_unref(xyplot->back_gc);
    xyplot->back_gc = NULL;
  }

  if (xyplot->data_gc) {
    g_object_unref(xyplot->data_gc);
    xyplot->data_gc = NULL;
  }

  if (xyplot->hmajor_grid_gc) {
    g_object_unref(xyplot->hmajor_grid_gc);
    xyplot->hmajor_grid_gc = NULL;
  }

  if (xyplot->hminor_grid_gc) {
    g_object_unref(xyplot->hminor_grid_gc);
    xyplot->hminor_grid_gc = NULL;
  }

  if (xyplot->vmajor_grid_gc) {
    g_object_unref(xyplot->vmajor_grid_gc);
    xyplot->vmajor_grid_gc = NULL;
  }

  if (xyplot->vminor_grid_gc) {
    g_object_unref(xyplot->vminor_grid_gc);
    xyplot->vminor_grid_gc = NULL;
  }

  if (GTK_OBJECT_CLASS (parent_class)->destroy)
    (* GTK_OBJECT_CLASS (parent_class)->destroy) (object);
}

static void
giw_xyplot_realize (GtkWidget *widget)
{
  GiwXYPlot *xyplot;
  GdkWindowAttr attributes;
  gint attributes_mask;
  GdkGCValues gc_values;
  GdkGCValuesMask gc_values_mask;
  GdkColor black;
  GdkColor white;
  GdkColormap *colormap;

  g_return_if_fail (widget != NULL);
  g_return_if_fail (GIW_IS_XYPLOT (widget));

  GTK_WIDGET_SET_FLAGS (widget, GTK_REALIZED);
  xyplot = GIW_XYPLOT (widget);

  attributes.x = widget->allocation.x;
  attributes.y = widget->allocation.y;
  attributes.width = widget->allocation.width;
  attributes.height = widget->allocation.height;
  attributes.wclass = GDK_INPUT_OUTPUT;
  attributes.window_type = GDK_WINDOW_CHILD;
  attributes.event_mask = gtk_widget_get_events (widget) |
    GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK |
    GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK |
    GDK_POINTER_MOTION_HINT_MASK;
  attributes.visual = gtk_widget_get_visual (widget);
  attributes.colormap = gtk_widget_get_colormap (widget);

  attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;
  widget->window = gdk_window_new (widget->parent->window, &attributes, attributes_mask);

  widget->style = gtk_style_attach (widget->style, widget->window);

  gdk_window_set_user_data (widget->window, widget);

  gtk_style_set_background (widget->style, widget->window, GTK_STATE_ACTIVE);

  colormap=gdk_drawable_get_colormap(widget->window);

  gdk_color_white(colormap, &white);
  gdk_color_black(colormap, &black);

  // Configuring background's GC
  gc_values.foreground=white;
  gc_values.line_style=GDK_LINE_SOLID;
  gc_values.line_width=1;
  gc_values_mask=GDK_GC_FOREGROUND | GDK_GC_LINE_STYLE | GDK_GC_LINE_WIDTH;
  xyplot->back_gc=gdk_gc_new_with_values (widget->window, &gc_values, gc_values_mask);
  // Major grid's GC, the same  (but black)
  gc_values.foreground=black;
  xyplot->hmajor_grid_gc=gdk_gc_new_with_values (widget->window, &gc_values, gc_values_mask);
  xyplot->vmajor_grid_gc=gdk_gc_new_with_values (widget->window, &gc_values, gc_values_mask);

  // GC fot the minor grids (black on off dashed line with 1 pixel width)
  gc_values.foreground=black;
  gc_values.line_style=GDK_LINE_ON_OFF_DASH;
  gc_values.line_width=1;
  gc_values_mask=GDK_GC_FOREGROUND | GDK_GC_LINE_STYLE | GDK_GC_LINE_WIDTH;
  xyplot->hminor_grid_gc=gdk_gc_new_with_values (widget->window, &gc_values, gc_values_mask);
  xyplot->vminor_grid_gc=gdk_gc_new_with_values (widget->window, &gc_values, gc_values_mask);

  // The data_gc doesn't need anything special
  xyplot->data_gc=gdk_gc_new (widget->window);

  // Create the initial legends
  xyplot_build_legends(xyplot);

  // Calculating legends sizes
  xyplot_calculate_legends_sizes(xyplot);

  // Now calculate everything, and create the plotting_area's pixmap
  xyplot_calculate_sizes(xyplot);
}

static void
giw_xyplot_size_request (GtkWidget      *widget,
		       GtkRequisition *requisition)
{
  g_return_if_fail (widget != NULL);
  g_return_if_fail (GIW_IS_XYPLOT (widget));
  g_return_if_fail (requisition != NULL);

  requisition->width = XYPLOT_DEFAULT_SIZE;
  requisition->height = XYPLOT_DEFAULT_SIZE;
}

static void
giw_xyplot_size_allocate (GtkWidget     *widget,
			GtkAllocation *allocation)
{
  GiwXYPlot *xyplot;

  g_return_if_fail (widget != NULL);
  g_return_if_fail (GIW_IS_XYPLOT (widget));
  g_return_if_fail (allocation != NULL);

  widget->allocation = *allocation;
  xyplot=GIW_XYPLOT(widget);

  if (GTK_WIDGET_REALIZED (widget))
    gdk_window_move_resize (widget->window,
			allocation->x,
			allocation->y,
			allocation->width,
			allocation->height);

  xyplot_calculate_sizes(xyplot);
}

static gint
giw_xyplot_expose (GtkWidget      *widget,
		 GdkEventExpose *event)
{
  GiwXYPlot *xyplot;
  gint loop1, loop2;
  guint x, y;
  GdkRectangle rect;
  GdkPoint *points; // To draw the data's lines (if they exists)
  XYPlotData *data;
  GList *current; // Pointer to browse in the data's list

  g_return_val_if_fail (widget != NULL, FALSE);
  g_return_val_if_fail (GIW_IS_XYPLOT (widget), FALSE);
  g_return_val_if_fail (event != NULL, FALSE);

  if (event->count > 0)
    return FALSE;

  xyplot=GIW_XYPLOT(widget);

  rect.x=0;rect.y=0;rect.width=widget->allocation.width;rect.height=widget->allocation.height;

  // Drawing background
  gtk_paint_flat_box (widget->style,
			widget->window,
			GTK_STATE_NORMAL,
			GTK_SHADOW_NONE,
			&rect,
			widget,
			NULL,
			0,
			0,
			-1,
			-1);

  // Filling the plotting area
  gdk_draw_rectangle (xyplot->plotting_area,
	                xyplot->back_gc,
			TRUE,
			0,
			0,
			xyplot->plotting_rect.width,
			xyplot->plotting_rect.height);


  // Drawing data, if there is data
  if (xyplot->data_amount>0){ // Preventing from bugs
    current=g_list_first(xyplot->data_list); // Getting the first one
    data=(XYPlotData*)current->data;
  }
  for(loop2=0; loop2<xyplot->data_amount; loop2++){
    // Drawing the points
    if ( (data->style == GIW_XYPLOT_DATA_POINTS) ||
         (data->style==GIW_XYPLOT_DATA_LINES_AND_POINTS) ){
      gdk_gc_set_rgb_fg_color (xyplot->data_gc, &data->p_color);

      for (loop1=0; loop1<data->size; loop1++)
        // Veryfing if the point is inside the range (inside the plotting area)
        if ((data->x[loop1] < xyplot->xmax) &&
            (data->x[loop1] > xyplot->xmin) &&
            (data->y[loop1] < xyplot->ymax) &&
            (data->y[loop1] > xyplot->ymin)) {
        value2pixel(xyplot, data->x[loop1], data->y[loop1], &x, &y);

        // Putting y coordinate in computer way (not cartesian way)
        y=xyplot->plotting_rect.height-y;
        gdk_draw_arc(xyplot->plotting_area,
			xyplot->data_gc,
			TRUE,
			x-data->p_width/2,
			y-data->p_width/2,
			data->p_width,
			data->p_width,
			0,
			360*64);
      }
    }

    // Drawing the lines
    if ( (data->style == GIW_XYPLOT_DATA_LINES) ||
         (data->style == GIW_XYPLOT_DATA_LINES_AND_POINTS)) {
      points=(GdkPoint*)g_malloc( (sizeof(GdkPoint) * data->size));
      gdk_gc_set_rgb_fg_color (xyplot->data_gc, &data->l_color);
      gdk_gc_set_line_attributes (xyplot->data_gc, data->l_width, data->line_style, GDK_CAP_NOT_LAST, GDK_JOIN_MITER);

      for (loop1=0; loop1<data->size; loop1++){
        value2pixel(xyplot, data->x[loop1], data->y[loop1], (guint *)&points[loop1].x, (guint *)&points[loop1].y);

        // Putting y coordinate in computer way (not cartesian way)
        points[loop1].y=xyplot->plotting_rect.height-points[loop1].y;
      }
      if (data->size)
        gdk_draw_lines (xyplot->plotting_area, xyplot->data_gc, points, data->size);
      g_free(points);
    }

    if (loop2!=xyplot->data_amount-1){ // Preventing from bugs (to not try to take the next data when the current is the last)
      current=current->next; // Getting the next one
      data=(XYPlotData*)current->data;
    }
  }

  // Drawing the plotting area
  gdk_draw_drawable (widget->window, xyplot->back_gc, xyplot->plotting_area, 0, 0, xyplot->plotting_rect.x, xyplot->plotting_rect.y, xyplot->plotting_rect.width, xyplot->plotting_rect.height);

  // If the mouse is zooming, draws the zoom rectangle
  if (xyplot->mouse_zoom_enabled && (xyplot->mouse_button == xyplot->mouse_zoom_button))
    gdk_draw_rectangle (widget->window,
	                widget->style->fg_gc[widget->state],
			FALSE,
			xyplot->zoom_rect.x,
			xyplot->zoom_rect.y,
			xyplot->zoom_rect.width,
			xyplot->zoom_rect.height);

  // Drawing an external rectangle, to separate the plotting area, of the external area
  gdk_draw_rectangle (widget->window,
	                widget->style->fg_gc[widget->state],
			FALSE,
			xyplot->plotting_rect.x,
			xyplot->plotting_rect.y,
			xyplot->plotting_rect.width,
			xyplot->plotting_rect.height);

  //Drawing horizontal minor ticks
  if (xyplot->hmajor_ticks>1 && (xyplot->hminor_ticks != 0) )
    for (loop1=0; loop1 < xyplot->hmajor_ticks-1; loop1++)
      for (loop2=1; loop2 <= xyplot->hminor_ticks; loop2++)
        gdk_draw_line(widget->window,
			widget->style->fg_gc[widget->state],
			xyplot->plotting_rect.x+loop1*xyplot->d_hminor*(xyplot->hminor_ticks+1)+loop2*xyplot->d_hminor,
			xyplot->plotting_rect.y+xyplot->plotting_rect.height,
			xyplot->plotting_rect.x+loop1*xyplot->d_hminor*(xyplot->hminor_ticks+1)+loop2*xyplot->d_hminor,
			xyplot->plotting_rect.y+xyplot->plotting_rect.height+3);


  //Drawing horizontal major ticks and legends
  if (xyplot->hmajor_ticks>1)
    for (loop1=0; loop1 < xyplot->hmajor_ticks; loop1++){
      // Major ticks
      gdk_draw_line(widget->window,
			widget->style->fg_gc[widget->state],
			xyplot->plotting_rect.x+loop1*xyplot->d_hmajor,
			xyplot->plotting_rect.y+xyplot->plotting_rect.height,
			xyplot->plotting_rect.x+loop1*xyplot->d_hmajor,
			xyplot->plotting_rect.y+xyplot->plotting_rect.height+5);
      // The legends
      if (xyplot->legends_digits!=0)
        gtk_paint_layout (widget->style,
			widget->window,
			GTK_STATE_NORMAL,
			FALSE,
			&rect,
			widget,
			NULL,
                        xyplot->plotting_rect.x+loop1*xyplot->d_hmajor-0.5*xyplot->legends_width,
			xyplot->plotting_rect.y+xyplot->plotting_rect.height+0.25*xyplot->legends_height,
			xyplot->h_legends[loop1]);
    }

  //Drawing the horizontal minor grid
  if (xyplot->hminor_grid && (xyplot->vmajor_ticks > 1) && (xyplot->vminor_ticks != 0))
    for (loop1=0; loop1 < xyplot->vmajor_ticks-1; loop1++)
      for (loop2=1; loop2 <= xyplot->vminor_ticks; loop2++)
        gdk_draw_line(widget->window,
			xyplot->hminor_grid_gc,
			xyplot->plotting_rect.x,
			xyplot->plotting_rect.y+loop1*xyplot->d_vmajor+loop2*xyplot->d_vminor,
			xyplot->plotting_rect.x+xyplot->plotting_rect.width,
			xyplot->plotting_rect.y+loop1*xyplot->d_vmajor+loop2*xyplot->d_vminor);

  // Drawing the horizontal major grid
  if (xyplot->hmajor_grid && (xyplot->vmajor_ticks > 1) )
    for (loop1=1; loop1 < (xyplot->vmajor_ticks-1); loop1++)
      gdk_draw_line(widget->window,
			xyplot->hmajor_grid_gc,
			xyplot->plotting_rect.x,
			xyplot->plotting_rect.y+loop1*xyplot->d_vmajor,
			xyplot->plotting_rect.x+xyplot->plotting_rect.width,
			xyplot->plotting_rect.y+loop1*xyplot->d_vmajor);

  //Drawing vertical minor ticks
  if ( (xyplot->vmajor_ticks > 1) && (xyplot->vminor_ticks != 0) )
    for (loop1=0; loop1 < xyplot->vmajor_ticks-1; loop1++)
      for (loop2=1; loop2 <= xyplot->vminor_ticks; loop2++)
        gdk_draw_line(widget->window,
			widget->style->fg_gc[widget->state],
			xyplot->plotting_rect.x,
			xyplot->plotting_rect.y+loop1*xyplot->d_vminor*(xyplot->vminor_ticks+1)+loop2*xyplot->d_vminor,
			xyplot->plotting_rect.x-3,
			xyplot->plotting_rect.y+loop1*xyplot->d_vminor*(xyplot->vminor_ticks+1)+loop2*xyplot->d_vminor);

  //Drawing vertical major ticks and legends
  if (xyplot->vmajor_ticks>1)
    for (loop1=0; loop1 < xyplot->vmajor_ticks; loop1++){
      // Major ticks
      gdk_draw_line(widget->window,
			widget->style->fg_gc[widget->state],
			xyplot->plotting_rect.x,
			xyplot->plotting_rect.y+loop1*xyplot->d_vmajor,
			xyplot->plotting_rect.x-5,
			xyplot->plotting_rect.y+loop1*xyplot->d_vmajor);

      // The legends
      if (xyplot->legends_digits!=0)
        gtk_paint_layout (widget->style,
			widget->window,
			GTK_STATE_NORMAL,
			FALSE,
			&rect,
			widget,
			NULL,
                        0.25*xyplot->legends_width,
			xyplot->plotting_rect.y+xyplot->plotting_rect.height-loop1*xyplot->d_vmajor-0.5*xyplot->legends_height,
			xyplot->v_legends[loop1]);
    }

  //Drawing the horizontal minor grid
  if (xyplot->vminor_grid && (xyplot->hmajor_ticks > 1) && (xyplot->hminor_ticks != 0) )
    for (loop1=0; loop1 < xyplot->hmajor_ticks-1; loop1++)
      for (loop2=1; loop2 <= xyplot->hminor_ticks; loop2++)
          gdk_draw_line(widget->window,
			xyplot->vminor_grid_gc,
			xyplot->plotting_rect.x+loop1*xyplot->d_hmajor+loop2*xyplot->d_hminor,
			xyplot->plotting_rect.y,
			xyplot->plotting_rect.x+loop1*xyplot->d_hmajor+loop2*xyplot->d_hminor,
			xyplot->plotting_rect.y+xyplot->plotting_rect.height);

  // Drawing the vertical major grid
  if (xyplot->vmajor_grid && (xyplot->hmajor_ticks > 1))
    for (loop1=1; loop1 < (xyplot->hmajor_ticks-1); loop1++)
      gdk_draw_line(widget->window,
			xyplot->vmajor_grid_gc,
			xyplot->plotting_rect.x+loop1*xyplot->d_hmajor,
			xyplot->plotting_rect.y,
			xyplot->plotting_rect.x+loop1*xyplot->d_hmajor,
			xyplot->plotting_rect.y+xyplot->plotting_rect.height);

  return(0);
}

static gint
giw_xyplot_button_press (GtkWidget      *widget,
		       GdkEventButton *event)
{
  GiwXYPlot *xyplot;

  g_return_val_if_fail (widget != NULL, FALSE);
  g_return_val_if_fail (GIW_IS_XYPLOT (widget), FALSE);
  g_return_val_if_fail (event != NULL, FALSE);

  xyplot = GIW_XYPLOT (widget);

  // Checking if mouse zooming is enabled, and if the mouse is insside the plotting area
  if (xyplot->mouse_zoom_enabled && (event->button == xyplot->mouse_zoom_button) )
    if ( (event->x > xyplot->plotting_rect.x) &&
         (event->x < (xyplot->plotting_rect.x + xyplot->plotting_rect.width)) &&
         (event->y > xyplot->plotting_rect.y) &&
         (event->y < (xyplot->plotting_rect.y + xyplot->plotting_rect.height)) ){
      xyplot->mouse_button=event->button;

      xyplot->zoom_point.x=event->x;
      xyplot->zoom_point.y=event->y;
  }

  // Checking if mouse moving is enabled, and if the mouse is insside the plotting area
  if (xyplot->mouse_displace_enabled && (event->button == xyplot->mouse_displace_button))
    if ( (event->x > xyplot->plotting_rect.x) &&
         (event->x < (xyplot->plotting_rect.x + xyplot->plotting_rect.width)) &&
         (event->y > xyplot->plotting_rect.y) &&
         (event->y < (xyplot->plotting_rect.y + xyplot->plotting_rect.height)) ){
      xyplot->mouse_button=event->button;

      xyplot->move_point.x=event->x;
      xyplot->move_point.y=event->y;
  }

  if ( xyplot->mouse_autorange_enabled &&
      (event->button == xyplot->mouse_autorange_button) )
    giw_xyplot_set_autorange(xyplot, NULL);

  return FALSE;
}

static gint
giw_xyplot_button_release (GtkWidget      *widget,
			  GdkEventButton *event)
{
  GiwXYPlot *xyplot;
  gdouble xmin, xmax, ymin, ymax;
  gint xleft, ytop, xright, ybottom;

  g_return_val_if_fail (widget != NULL, FALSE);
  g_return_val_if_fail (GIW_IS_XYPLOT (widget), FALSE);
  g_return_val_if_fail (event != NULL, FALSE);

  xyplot = GIW_XYPLOT (widget);

   if (xyplot->mouse_zoom_enabled &&
       (event->button == xyplot->mouse_zoom_button) &&
       (xyplot->mouse_button == xyplot->mouse_zoom_button)){

    // Transforming the rectangle's coordinates to coordinates relative to the origin of the plotting area
    xleft=xyplot->zoom_rect.x-xyplot->plotting_rect.x;
    xright=xyplot->zoom_rect.x+xyplot->zoom_rect.width-xyplot->plotting_rect.x;
    ybottom=xyplot->plotting_rect.y+xyplot->plotting_rect.height-xyplot->zoom_rect.y-xyplot->zoom_rect.height;
    ytop=xyplot->plotting_rect.y+xyplot->plotting_rect.height-xyplot->zoom_rect.y;

    // Getting the values
    pixel2value(xyplot, xleft, ybottom, &xmin, &ymin);
    pixel2value(xyplot, xright, ytop, &xmax, &ymax);

    giw_xyplot_set_range(xyplot, xmin, xmax, ymin, ymax);

    xyplot->mouse_button=0;
  }

  if (xyplot->mouse_displace_enabled &&
      (event->button == xyplot->mouse_displace_button) &&
      (xyplot->mouse_button == xyplot->mouse_displace_button) ){
    xyplot->mouse_button=0; // Only return to normal
  }

  return FALSE;
}

static gint
giw_xyplot_motion_notify (GtkWidget      *widget,
			 GdkEventMotion *event)
{
  GiwXYPlot *xyplot;
  guint x, y;
  gdouble px, py, mx, my;

  g_return_val_if_fail (widget != NULL, FALSE);
  g_return_val_if_fail (GIW_IS_XYPLOT (widget), FALSE);
  g_return_val_if_fail (event != NULL, FALSE);

  xyplot = GIW_XYPLOT (widget);

  x = event->x;
  y = event->y;

  if (event->is_hint || (event->window != widget->window))
      gdk_window_get_pointer (widget->window, (gint *)&x, (gint *)&y, NULL);

  if (xyplot->mouse_zoom_enabled &&
      (xyplot->mouse_button == xyplot->mouse_zoom_button) ){
    // Putting inside the plotting area, if it isn't
    if (x < xyplot->plotting_rect.x)
      x=xyplot->plotting_rect.x;

    if(x > (xyplot->plotting_rect.x + xyplot->plotting_rect.width))
      x=(xyplot->plotting_rect.x + xyplot->plotting_rect.width);

    if (y < xyplot->plotting_rect.y)
      y=xyplot->plotting_rect.y;

    if (y > (xyplot->plotting_rect.y + xyplot->plotting_rect.height))
      y=(xyplot->plotting_rect.y + xyplot->plotting_rect.height);

    // Adjusting a rectangle
    if (xyplot->zoom_point.x < x){
      xyplot->zoom_rect.x=xyplot->zoom_point.x;
      xyplot->zoom_rect.width=x-xyplot->zoom_point.x;
    }else{
      xyplot->zoom_rect.x=x;
      xyplot->zoom_rect.width=xyplot->zoom_point.x-x;
    }

    if (xyplot->zoom_point.y < y){
      xyplot->zoom_rect.y=xyplot->zoom_point.y;
      xyplot->zoom_rect.height=y-xyplot->zoom_point.y;
    }else{
      xyplot->zoom_rect.y=y;
      xyplot->zoom_rect.height=xyplot->zoom_point.y-y;
    }

    gtk_widget_queue_draw(GTK_WIDGET(xyplot));
  }

  if (xyplot->mouse_displace_enabled && (xyplot->mouse_button==xyplot->mouse_displace_button))
    // Veryfing if the point is inside the plotting area
    if ( (x > xyplot->plotting_rect.x) &&
         (x < (xyplot->plotting_rect.width + xyplot->plotting_rect.x)) &&
         (y > xyplot->plotting_rect.y) &&
         (y < (xyplot->plotting_rect.height + xyplot->plotting_rect.y)) ){
      pixel2value(xyplot, xyplot->move_point.x, xyplot->move_point.y, &px, &py);
      pixel2value(xyplot, x, y, &mx, &my);

      giw_xyplot_set_range(xyplot, xyplot->xmin+(px-mx), xyplot->xmax+(px-mx), xyplot->ymin-(py-my), xyplot->ymax-(py-my));
      xyplot->move_point.x=x;
      xyplot->move_point.y=y;
  }

  return FALSE;
}

static
gint giw_xyplot_scroll (GtkWidget *widget,
			GdkEventScroll   *event)
{
  GiwXYPlot *xyplot;
  gdouble x_range = 0.0, y_range = 0.0,
          xmin = 0.0, xmax = 0.0,
          ymin = 0.0, ymax = 0.0;

  g_return_val_if_fail (widget != NULL, FALSE);
  g_return_val_if_fail (GIW_IS_XYPLOT (widget), FALSE);
  g_return_val_if_fail (event != NULL, FALSE);

  xyplot=GIW_XYPLOT(widget);

  if (xyplot->wheel_zoom_enabled){
    if (event->direction==GDK_SCROLL_UP){
      x_range=xyplot->wheel_zoom_factor*(xyplot->xmax-xyplot->xmin);
      y_range=xyplot->wheel_zoom_factor*(xyplot->ymax-xyplot->ymin);
    }

    if (event->direction==GDK_SCROLL_DOWN){
      x_range=(xyplot->xmax-xyplot->xmin)/xyplot->wheel_zoom_factor;
      y_range=(xyplot->ymax-xyplot->ymin)/xyplot->wheel_zoom_factor;
    }

    xmin=xyplot->xmin+((xyplot->xmax-xyplot->xmin)/2.0)-(x_range/2.0);
    xmax=xyplot->xmin+((xyplot->xmax-xyplot->xmin)/2.0)+(x_range/2.0);

    ymin=xyplot->ymin+((xyplot->ymax-xyplot->ymin)/2.0)-(y_range/2.0);
    ymax=xyplot->ymin+((xyplot->ymax-xyplot->ymin)/2.0)+(y_range/2.0);

    giw_xyplot_set_range(xyplot, xmin, xmax, ymin, ymax);
  }
  return TRUE;
}

static void
giw_xyplot_style_set (GtkWidget *widget,
			GtkStyle *previous_style)
{
  GiwXYPlot *xyplot;

  g_return_if_fail (widget != NULL);
  g_return_if_fail (GIW_IS_XYPLOT (widget));

  xyplot = GIW_XYPLOT (widget);

  // The only thing to fo is recalculate the layout's sizes
  xyplot_calculate_legends_sizes(xyplot);
  xyplot_calculate_sizes(xyplot);
  gtk_widget_queue_draw(GTK_WIDGET(xyplot));
}


/*****************
* User Functions *
*****************/
void
giw_xyplot_set_range (GiwXYPlot *xyplot,
			gdouble xmin,
			gdouble xmax,
			gdouble ymin,
			gdouble ymax)
{
  g_return_if_fail (xyplot != NULL);
  g_return_if_fail (GIW_IS_XYPLOT (xyplot));

  if (xmin < xmax){
    xyplot->xmin=xmin;
    xyplot->xmax=xmax;
  }

  if (ymin < ymax){
    xyplot->ymin=ymin;
    xyplot->ymax=ymax;
  }

  xyplot_free_legends(xyplot);
  xyplot_build_legends(xyplot);
  xyplot_calculate_sizes(xyplot);
  gtk_widget_queue_draw(GTK_WIDGET(xyplot));
}

void
giw_xyplot_set_autorange (GiwXYPlot *xyplot, XYPlotData *data)
{
  guint loop, loop2;
  gdouble xmax, xmin, ymax, ymin;
  gdouble dx, dy; // Distance between the upper and lower values
  XYPlotData *current_data; // pointer to browse in tha data structure
  GList *current_node; // pointer to browse in the list

  g_return_if_fail (xyplot != NULL);
  g_return_if_fail (GIW_IS_XYPLOT (xyplot));

  // The range, if there isn't any data..
  xmax=1.0;
  xmin=-1.0;
  ymax=1.0;
  ymin=-1.0;

  if (data!=NULL){
    // Searching for the upper and lower values in a specific data structure
    xmax=data->x[0];
    xmin=data->x[0];
    ymax=data->y[0];
    ymin=data->y[0];
    for (loop=1; loop<data->size; loop++){
      if (data->x[loop] > xmax)
        xmax=data->x[loop];
      if (data->x[loop] < xmin)
        xmin=data->x[loop];
      if (data->y[loop] > ymax)
        ymax=data->y[loop];
      if (data->y[loop] < ymin)
        ymin=data->y[loop];
    }
  }

  if ( (data == NULL) && (xyplot->data_amount > 0) ){
    // Searching for the upper and lower values in all the datas
    current_node=g_list_first(xyplot->data_list);
    current_data=(XYPlotData*)current_node->data;
    xmin=current_data->x[0];
    xmax=current_data->x[0];  // Only to initiate with some value of the plot, and not any memoru garbage
    ymin=current_data->y[0];
    ymax=current_data->y[0];
    for (loop=0; loop<xyplot->data_amount; loop++){
      for (loop2=0; loop2<current_data->size; loop2++){
        if (xmin > current_data->x[loop2])
          xmin=current_data->x[loop2];
        if (xmax < current_data->x[loop2])
          xmax=current_data->x[loop2];
        if (ymin > current_data->y[loop2])
          ymin=current_data->y[loop2];
        if (ymax < current_data->y[loop2])
          ymax=current_data->y[loop2];
      }
      if (loop!=xyplot->data_amount-1){ // Preventing from bugs (to not try to take the next data when the current is the last)
        current_node=current_node->next;
        current_data=(XYPlotData*)current_node->data;
      }
    }
  }

  // Lefting some space between the last points and the edge
  dx=fabs(xmax-xmin);
  dy=fabs(ymax-ymin);
  xmin=xmin-dx*0.1;
  xmax=xmax+dx*0.1;
  ymin=ymin-dy*0.1;
  ymax=ymax+dy*0.1;

  xyplot->xmin=xmin;
  xyplot->xmax=xmax;
  xyplot->ymin=ymin;
  xyplot->ymax=ymax;

  xyplot_free_legends(xyplot);
  xyplot_build_legends(xyplot);
  xyplot_calculate_sizes(xyplot);

  gtk_widget_queue_draw(GTK_WIDGET(xyplot));
}

void
giw_xyplot_get_range (GiwXYPlot *xyplot,
			gdouble *xmin,
			gdouble *xmax,
			gdouble *ymin,
			gdouble *ymax)
{
  g_return_if_fail (xyplot != NULL);
  g_return_if_fail (GIW_IS_XYPLOT (xyplot));

  if (xmin!=NULL)
    *xmin=xyplot->xmin;
  if (xmax!=NULL)
    *xmax=xyplot->xmax;
  if (ymax!=NULL)
    *ymax=xyplot->ymax;
  if (ymin!=NULL)
    *ymin=xyplot->ymin;
}

void
giw_xyplot_get_point (GiwXYPlot *xyplot,
			guint x,
			guint y,
			gdouble *xv,
			gdouble *yv)
{
  g_return_if_fail (xyplot != NULL);
  g_return_if_fail (GIW_IS_XYPLOT (xyplot));

  if ((x < xyplot->plotting_rect.x) ||
      (x > (xyplot->plotting_rect.x + xyplot->plotting_rect.width)) ||
      (y < xyplot->plotting_rect.y) ||
      (y > (xyplot->plotting_rect.y + xyplot->plotting_rect.height))) {
    xv=yv=NULL;
    return;
  }

  // Transforming the point relative to the plotting area origin, because pixel2value works this way
  x=x-xyplot->plotting_rect.x;
  y=xyplot->plotting_rect.y+xyplot->plotting_rect.height-y;

  pixel2value(xyplot, x, y, xv, yv);
}

void
giw_xyplot_set_ticks (GiwXYPlot *xyplot,
			guint hmajor,
			guint hminor,
			guint vmajor,
			guint vminor)
{
  g_return_if_fail (xyplot != NULL);
  g_return_if_fail (GIW_IS_XYPLOT (xyplot));

  xyplot_free_legends(xyplot);

  xyplot->hmajor_ticks=hmajor;
  xyplot->hminor_ticks=hminor;
  xyplot->vmajor_ticks=vmajor;
  xyplot->vminor_ticks=vminor;

  xyplot_build_legends(xyplot);
  xyplot_calculate_sizes(xyplot);
  gtk_widget_queue_draw(GTK_WIDGET(xyplot));
}

void
giw_xyplot_get_ticks (GiwXYPlot *xyplot,
			guint *hmajor,
			guint *hminor,
			guint *vmajor,
			guint *vminor)
{
  g_return_if_fail (xyplot != NULL);
  g_return_if_fail (GIW_IS_XYPLOT (xyplot));

  if (hmajor!=NULL)
    *hmajor=xyplot->hmajor_ticks;
  if (hminor!=NULL)
    *hminor=xyplot->hminor_ticks;
  if (vmajor!=NULL)
    *vmajor=xyplot->vmajor_ticks;
  if (vminor!=NULL)
    *vminor=xyplot->vminor_ticks;
}

void
giw_xyplot_set_legends_digits (GiwXYPlot *xyplot,
			guint digits)
{
  g_return_if_fail (xyplot != NULL);
  g_return_if_fail (GIW_IS_XYPLOT (xyplot));

  if (digits != xyplot->legends_digits){
    xyplot_free_legends(xyplot);

    xyplot->legends_digits=digits;

    xyplot_build_legends(xyplot);
    xyplot_calculate_legends_sizes(xyplot);
    xyplot_calculate_sizes(xyplot);
    gtk_widget_queue_draw(GTK_WIDGET(xyplot));
  }
}

guint
giw_xyplot_get_legends_digits (GiwXYPlot *xyplot)
{
  g_return_val_if_fail (xyplot != NULL, 0);
  g_return_val_if_fail (GIW_IS_XYPLOT (xyplot), 0);

  return(xyplot->legends_digits);
}

void
giw_xyplot_set_background_color (GiwXYPlot *xyplot, GdkColor color)
{
  g_return_if_fail (xyplot != NULL);
  g_return_if_fail (GIW_IS_XYPLOT (xyplot));

  gdk_gc_set_rgb_fg_color(xyplot->back_gc, &color);

  gtk_widget_queue_draw(GTK_WIDGET(xyplot));
}

void
giw_xyplot_set_grids_attributes (GiwXYPlot *xyplot,
			GiwXYPlotGrid grid, // Do not bitwise it!!
			GdkColor color,
			gint line_width,
			GdkLineStyle line_style)
{
  GdkGC *gc;

  g_return_if_fail (xyplot != NULL);
  g_return_if_fail (GIW_IS_XYPLOT (xyplot));

  switch (grid){
    case GIW_XYPLOT_HMAJOR_GRID:
      gc=xyplot->hmajor_grid_gc;
    break;
    case GIW_XYPLOT_HMINOR_GRID:
      gc=xyplot->hminor_grid_gc;
    break;
    case GIW_XYPLOT_VMAJOR_GRID:
      gc=xyplot->vmajor_grid_gc;
    break;
    case GIW_XYPLOT_VMINOR_GRID:
      gc=xyplot->vminor_grid_gc;
    break;
    default:
      return;
  }

  gdk_gc_set_rgb_fg_color(gc, &color);
  gdk_gc_set_line_attributes(gc, line_width, line_style, 0 ,0);

  gtk_widget_queue_draw(GTK_WIDGET(xyplot));
}

void
giw_xyplot_get_grids_attributes (GiwXYPlot *xyplot,
			GiwXYPlotGrid grid, // Do not bitwise it!!
			GdkColor *color,
			gint *line_width,
			GdkLineStyle *line_style)
{
  GdkGC *gc;
  GdkGCValues gc_values;

  g_return_if_fail (xyplot != NULL);
  g_return_if_fail (GIW_IS_XYPLOT (xyplot));
  g_return_if_fail (color != NULL);
  g_return_if_fail (line_width != NULL);
  g_return_if_fail (line_style != NULL);

  switch (grid){
    case GIW_XYPLOT_HMAJOR_GRID:
      gc=xyplot->hmajor_grid_gc;
    break;
    case GIW_XYPLOT_HMINOR_GRID:
      gc=xyplot->hminor_grid_gc;
    break;
    case GIW_XYPLOT_VMAJOR_GRID:
      gc=xyplot->vmajor_grid_gc;
    break;
    case GIW_XYPLOT_VMINOR_GRID:
      gc=xyplot->vminor_grid_gc;
    break;
    default:
      return;
  }

  gdk_gc_get_values(gc, &gc_values);

  *color=gc_values.foreground;
  *line_width=gc_values.line_width;
  *line_style=gc_values.line_style;
}

void
giw_xyplot_enable_grids (GiwXYPlot *xyplot,
			GiwXYPlotGrid grid,
			gboolean enable)
{
  g_return_if_fail (xyplot != NULL);
  g_return_if_fail (GIW_IS_XYPLOT (xyplot));

  switch (grid){
    case GIW_XYPLOT_HMAJOR_GRID:
      xyplot->hmajor_grid=enable;
    break;
    case GIW_XYPLOT_HMINOR_GRID:
      xyplot->hminor_grid=enable;
    break;
    case GIW_XYPLOT_VMAJOR_GRID:
      xyplot->vmajor_grid=enable;
    break;
    case GIW_XYPLOT_VMINOR_GRID:
      xyplot->vminor_grid=enable;
    break;
    default:
      return;
  }

  xyplot_calculate_sizes(xyplot);
  gtk_widget_queue_draw(GTK_WIDGET(xyplot));
}

void
giw_xyplot_add_data(GiwXYPlot *xyplot,
			XYPlotData *data)
{
  g_return_if_fail (xyplot != NULL);
  g_return_if_fail (GIW_IS_XYPLOT (xyplot));
  g_return_if_fail (data != NULL);

  // If the element doesn't exists in the list, add it!!
  if (g_list_find (xyplot->data_list, (gpointer)data)==NULL){
    xyplot->data_list=g_list_append(xyplot->data_list, (gpointer) data);
    xyplot->data_amount++;
  }

  fflush(stdout);
}

void
giw_xyplot_remove_data(GiwXYPlot *xyplot,
			XYPlotData *data)
{
  g_return_if_fail (xyplot != NULL);
  g_return_if_fail (GIW_IS_XYPLOT (xyplot));
  g_return_if_fail (data != NULL);

  // If the element exists in the list, remove it!!
  if (g_list_find (xyplot->data_list, (gpointer)data)!=NULL){
    xyplot->data_list=g_list_remove_all (xyplot->data_list, (gpointer) data);
    xyplot->data_amount--;
  }
}

void
giw_xyplot_configure_mouse_zoom(GiwXYPlot *xyplot,
			gboolean enabled,
			guint button)
{
  g_return_if_fail (xyplot != NULL);
  g_return_if_fail (GIW_IS_XYPLOT (xyplot));

  xyplot->mouse_zoom_enabled=enabled;
  xyplot->mouse_zoom_button=button;
}

void
giw_xyplot_configure_wheel_zoom(GiwXYPlot *xyplot,
			gboolean enabled,
			gdouble factor)
{
  g_return_if_fail (xyplot != NULL);
  g_return_if_fail (GIW_IS_XYPLOT (xyplot));

  xyplot->wheel_zoom_enabled=enabled;
  xyplot->wheel_zoom_factor=factor;
}

void
giw_xyplot_configure_mouse_displace(GiwXYPlot *xyplot,
			gboolean enabled,
			guint button)
{
  g_return_if_fail (xyplot != NULL);
  g_return_if_fail (GIW_IS_XYPLOT (xyplot));

  xyplot->mouse_displace_enabled=enabled;
  xyplot->mouse_displace_button=button;
}

void
giw_xyplot_configure_mouse_autorange(GiwXYPlot *xyplot,
			gboolean enabled,
			guint button)
{
  g_return_if_fail (xyplot != NULL);
  g_return_if_fail (GIW_IS_XYPLOT (xyplot));

  xyplot->mouse_autorange_enabled=enabled;
  xyplot->mouse_autorange_button=button;
}

/******************
* Local Functions *
******************/

static void
xyplot_calculate_sizes (GiwXYPlot *xyplot)
{
  GtkWidget *widget;

  g_return_if_fail (xyplot != NULL);
  g_return_if_fail (GIW_IS_XYPLOT (xyplot));

  widget = GTK_WIDGET(xyplot);

  // Calculating size and position of the plotting area
  xyplot->plotting_rect.x=1.5*((gdouble)xyplot->legends_width);
  xyplot->plotting_rect.y=0.75*((gdouble)xyplot->legends_height);
  xyplot->plotting_rect.width=widget->allocation.width - xyplot->plotting_rect.x - 0.75*xyplot->legends_width;
  xyplot->plotting_rect.height=widget->allocation.height -xyplot->plotting_rect.y- 1.5*xyplot->legends_height;

  // Ticks stuff
  if (xyplot->hmajor_ticks>1){
    xyplot->d_hmajor=((gdouble)xyplot->plotting_rect.width)/((gdouble)xyplot->hmajor_ticks-1.0);
    xyplot->d_hminor=((gdouble)xyplot->d_hmajor)/((gdouble)xyplot->hminor_ticks+1.0);
  }

  if (xyplot->vmajor_ticks>1){
    xyplot->d_vmajor=((gdouble)xyplot->plotting_rect.height)/((gdouble)xyplot->vmajor_ticks-1.0);
    xyplot->d_vminor=((gdouble)xyplot->d_vmajor)/((gdouble)xyplot->vminor_ticks+1.0);
  }

  /* Creating the plotting area (everytime the plotting area's size is changed, the size of the pismap
     must be changed too, so, a new one, with the right size is created*/
  if (xyplot->plotting_area!=NULL) // Freeing the last one (if exists)
    g_object_unref(G_OBJECT(xyplot->plotting_area));
  if (GTK_WIDGET_REALIZED(widget)) // It must be already realized, to create the pixmap
    xyplot->plotting_area=gdk_pixmap_new(widget->window, xyplot->plotting_rect.width, xyplot->plotting_rect.height, -1);
}

static void
value2pixel(GiwXYPlot *xyplot, gdouble xv, gdouble yv, guint *x, guint *y)
{
  g_return_if_fail (xyplot != NULL);
  g_return_if_fail (GIW_IS_XYPLOT (xyplot));

  *x=(xv-xyplot->xmin)*((gdouble)xyplot->plotting_rect.width)/(xyplot->xmax-xyplot->xmin);
  *y=(yv-xyplot->ymin)*((gdouble)xyplot->plotting_rect.height)/(xyplot->ymax-xyplot->ymin);
}

static void
pixel2value(GiwXYPlot *xyplot, guint xp, guint yp, gdouble *x, gdouble *y)
{
  g_return_if_fail (xyplot != NULL);
  g_return_if_fail (GIW_IS_XYPLOT (xyplot));

  *x=xyplot->xmin+((gdouble)xp)*(xyplot->xmax-xyplot->xmin)/((gdouble)xyplot->plotting_rect.width);
  *y=xyplot->ymin+((gdouble)yp)*(xyplot->ymax-xyplot->ymin)/((gdouble)xyplot->plotting_rect.height);
}

void
xyplot_calculate_legends_sizes(GiwXYPlot *xyplot)
{
  GtkWidget *widget;

  g_return_if_fail (xyplot != NULL);

  widget=GTK_WIDGET(xyplot);

  if (xyplot->legends_digits==0){
    xyplot->legends_width=0;
    xyplot->legends_height=0;
    return;
  }

  // Trying with each one
  if (xyplot->h_legends!=NULL){
    pango_layout_set_font_description (xyplot->h_legends[0], widget->style->font_desc);
    pango_layout_get_size(xyplot->h_legends[0], &(xyplot->legends_width), &(xyplot->legends_height));
  }
  if (xyplot->v_legends!=NULL){
    pango_layout_set_font_description (xyplot->v_legends[0], widget->style->font_desc);
    pango_layout_get_size(xyplot->v_legends[0], &(xyplot->legends_width), &(xyplot->legends_height));
  }

  xyplot->legends_width/=PANGO_SCALE;
  xyplot->legends_height/=PANGO_SCALE;
}

void
xyplot_build_legends(GiwXYPlot *xyplot)
{
  GtkWidget *widget;
  gint loop;
  gchar *str;

  g_return_if_fail (xyplot != NULL);

  widget=GTK_WIDGET(xyplot);

  if ( (xyplot->hmajor_ticks > 1) && (xyplot->legends_digits != 0)){  // Preventing from bugs
    xyplot->d_hlegend=fabs(xyplot->xmax-xyplot->xmin)/((gdouble)xyplot->hmajor_ticks-1.0);

    xyplot->h_legends=g_new(PangoLayout*, xyplot->hmajor_ticks);
    str=g_malloc( (sizeof(gchar) * (xyplot->legends_digits+1)) ); // +1 for the '/0'
    for (loop=0; loop<xyplot->hmajor_ticks; loop++){
      snprintf(str, xyplot->legends_digits+1, "%f", xyplot->xmin + (gdouble)loop*xyplot->d_hlegend);
      xyplot->h_legends[loop]=gtk_widget_create_pango_layout (widget, str);
    }
    g_free(str);
  }

  if ((xyplot->vmajor_ticks > 1) && (xyplot->legends_digits != 0) ){  // Preventing from bugs
    xyplot->d_vlegend=fabs(xyplot->ymax-xyplot->ymin)/((gdouble)xyplot->vmajor_ticks-1.0);

    xyplot->v_legends=g_new(PangoLayout*, xyplot->vmajor_ticks);
    str=g_malloc( (sizeof(gchar) * (xyplot->legends_digits+1))); // +1 for the '/0'
    for (loop=0; loop<xyplot->vmajor_ticks; loop++){
      snprintf(str, xyplot->legends_digits+1, "%f", xyplot->ymin + (gdouble)loop*xyplot->d_vlegend);
      xyplot->v_legends[loop]=gtk_widget_create_pango_layout (widget, str);
    }
    g_free(str);
  }

  // Getting the size of the legends
  xyplot_calculate_legends_sizes(xyplot);
}

void
xyplot_free_legends(GiwXYPlot *xyplot)
{
  gint loop;

  g_return_if_fail (xyplot != NULL);

  if (xyplot->h_legends!=NULL){
    for (loop=0; loop<xyplot->hmajor_ticks; loop++)
      if (xyplot->h_legends[loop]!=NULL)
        g_object_unref(G_OBJECT(xyplot->h_legends[loop]));
    g_free(xyplot->h_legends);
    xyplot->h_legends=NULL;
  }

  if (xyplot->v_legends!=NULL){
    for (loop=0; loop<xyplot->vmajor_ticks; loop++)
      if (xyplot->v_legends[loop]!=NULL)
        g_object_unref(G_OBJECT(xyplot->v_legends[loop]));
    g_free(xyplot->v_legends);
    xyplot->v_legends=NULL;
  }
}

