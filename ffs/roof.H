/*****************************************************************
 * This file is part of jot-lib (or "jot" for short):
 *   <http://code.google.com/p/jot-lib/>
 * 
 * jot-lib is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 * 
 * jot-lib is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with jot-lib.  If not, see <http://www.gnu.org/licenses/>.`
 *****************************************************************/
#ifndef ROOF_H_IS_INCLUDED
#define ROOF_H_IS_INCLUDED

/*!
 *  \file roof.H
 *  \brief Contains the declaration of the ROOF widget.
 *
 *  \ingroup group_FFS
 *  \sa roof.C
 *
 */

#include "mesh/ledge_strip.H"

#include "gest/draw_widget.H"

/*****************************************************************
 * ROOF:
 *****************************************************************/
MAKE_PTR_SUBC(ROOF,DrawWidget);
typedef const ROOF    CROOF;
typedef const ROOFptr CROOFptr;

//! \brief "Widget" that handles creating a roof-like structure based on
//! a chain of quads.
class ROOF : public DrawWidget {
 public:

   //******** MANAGERS ********

   // no public constructor
   static ROOFptr get_instance();

   static ROOFptr get_active_instance() { return upcast(_active); }

   //******** RUN-TIME TYPE ID ********

   DEFINE_RTTI_METHODS3("ROOF", ROOF*, DrawWidget, CDATA_ITEM *);

   //******** SETTING UP ********

   //! \brief If the gesture is valid for activating the widget,
   //! activates it and returns true.
   static bool init(CGESTUREptr& g);

   //******** DRAW FSA METHODS ********

   virtual int  cancel_cb (CGESTUREptr& gest, DrawState*&);
   virtual int  stroke_cb (CGESTUREptr& gest, DrawState*&);

   //******** DrawWidget METHODS ********

   virtual void toggle_active() {
      DrawWidget::toggle_active();
      if (!is_active())
         reset();
   }

   //******** MODE NAME ********

   //! displayed in jot window
   virtual str_ptr      mode_name()     const { return "roof"; }

   //******** GEL METHODS ********

   virtual int draw(CVIEWptr &v);

 protected:
   Bface_list   _bases;         //!< selected chain of quads
   EdgeStrip    _edges;         //!< the edges at the boundary   

   mlib::Wtransf      _plane_xf;      //!< describes draw plane 

   static ROOFptr _instance;

   static void clean_on_exit();

   //******** INTERNAL METHODS ********

   ROOF();
   virtual ~ROOF() {}

   void compute_plane();
   void reset();

   //! \brief for defining callbacks to use with GESTUREs:
   typedef CallMeth_t<ROOF, GESTUREptr> draw_cb_t;
   draw_cb_t* drawCB(draw_cb_t::_method m) {
      return new draw_cb_t(this, m);
   }
};

#endif // ROOF_H_IS_INCLUDED

/* end of file roof.H */
