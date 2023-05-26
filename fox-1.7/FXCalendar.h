/********************************************************************************
*                                                                               *
*                       C a l e n d a r   W i d g e t                           *
*                                                                               *
*********************************************************************************
* Copyright (C) 2006,2022 by Sander Jansen.   All Rights Reserved.              *
*********************************************************************************
* This library is free software; you can redistribute it and/or modify          *
* it under the terms of the GNU Lesser General Public License as published by   *
* the Free Software Foundation; either version 3 of the License, or             *
* (at your option) any later version.                                           *
*                                                                               *
* This library is distributed in the hope that it will be useful,               *
* but WITHOUT ANY WARRANTY; without even the implied warranty of                *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                 *
* GNU Lesser General Public License for more details.                           *
*                                                                               *
* You should have received a copy of the GNU Lesser General Public License      *
* along with this program.  If not, see <http://www.gnu.org/licenses/>          *
********************************************************************************/
#ifndef FXCALENDAR_H
#define FXCALENDAR_H

#ifndef FXPACKER_H
#include "FXPacker.h"
#endif

namespace FX {


class FXLabel;
class FXPopup;
class FXOptionMenu;
class FXHorizontalFrame;
class FXCalendarView;


/**
* The Calendar Widget. In addition of displaying the calendar, also provides month and year
* controls and optional frame.
*/
class FXAPI FXCalendar : public FXPacker {
FXDECLARE(FXCalendar)
protected:
  FXCalendarView    *view;
  FXHorizontalFrame *frame;
  FXLabel           *year;
  FXOptionMenu      *month;
  FXPopup           *monthpopup;
  FXOption          *months[12];
  FXArrowButton     *arrows[4];
protected:
  FXCalendar();
private:
  FXCalendar(const FXCalendar&);
  FXCalendar &operator=(const FXCalendar&);
public:
  enum{
    ID_CALENDAR=FXPacker::ID_LAST,
    ID_NEXTYEAR,
    ID_PREVYEAR,
    ID_NEXTMONTH,
    ID_PREVMONTH,
    ID_MONTH_START,
    ID_MONTH_END=ID_MONTH_START+12,
    ID_MONTH
    };
public:
  long onCmdDate(FXObject*,FXSelector,void*);
  long onCmdMonth(FXObject*,FXSelector,void*);
  long onCmdNextYear(FXObject*,FXSelector,void*);
  long onCmdPrevYear(FXObject*,FXSelector,void*);
  long onCmdNextMonth(FXObject*,FXSelector,void*);
  long onCmdPrevMonth(FXObject*,FXSelector,void*);
  long onCmdSelectMonth(FXObject*,FXSelector,void*);
  long onFwdToView(FXObject*,FXSelector,void*);
  long onFwdToTarget(FXObject*,FXSelector,void*);
public:

  /// Constructor
  FXCalendar(FXComposite *p,FXObject* tgt=nullptr,FXSelector sel=0,FXuint opts=0,FXint x=0,FXint y=0,FXint w=0,FXint h=0);

  /// Create
  void create();

  /// Enable the window to receive mouse and keyboard events
  virtual void enable();

  /// Disable the window from receiving mouse and keyboard events
  virtual void disable();

  /// Set date
  void setCurrentDate(FXDate date,FXbool notify=false);

  /// Get the current date
  FXDate getCurrentDate() const;

  /// Set the current month; current day will be properly updated for the choosen month
  void setCurrentMonth(FXint mo,FXbool notify=false);

  /**
  * Return the current month shown. The month may be different than
  * the current date if a day in a sibling month is current.
  */
  FXint getCurrentMonth() const;

  /// Return calendar view control
  FXCalendarView* calendarView() const { return view; }

  /// Set the first day of the week [0 -> 6]
  void setFirstDay(FXint d);

  /// Get the first day of the week [0 -> 6]
  FXint getFirstDay() const;

  /// Change the Frame Style
  void setFrameStyle(FXuint);

  /// Set the Calendar Style
  void setCalendarStyle(FXuint);

  /// Get the Calendar Style
  FXuint getCalendarStyle() const;

  /// Set the back color
  void setBackColor(FXColor c);

  /// Get the back color
  FXColor getBackColor() const;

  /// Set the display color of titles
  void setTitleColor(FXColor c);

  /// Get the display color of titles
  FXColor getTitleColor() const;

  /// Set the display color of titles
  void setTitleBackColor(FXColor c);

  /// Get the display color of titles
  FXColor getTitleBackColor() const;

  /// Set the display color of non-weekend days
  void setDayColor(FXColor c);

  /// Get the display color of non-weekend days
  FXColor getDayColor() const;

  /// Set the display color of non-weekend days not in the current month
  void setOtherDayColor(FXColor c);

  /// Get the display color of non-weekend days not in the current month
  FXColor getOtherDayColor() const;

  /// Set the display color of today
  void setTodayColor(FXColor c);

  /// Get the display color of today
  FXColor getTodayColor() const;

  /// Set the display color of days in the weekend
  void setWeekendColor(FXColor c);

  /// Get the display color of days in the weekend
  FXColor getWeekendColor() const;

  /// Set the display color of days in the weekend not in the current month
  void setOtherWeekendColor(FXColor c);

  /// Get the display color of days in the weekend not in the current month
  FXColor getOtherWeekendColor() const;

  /// Set font used by the header
  void setHeaderFont(FXFont *fnt);

  /// Get font used by the header
  FXFont* getHeaderFont() const;

  /// Set font used by the calendar
  void setCalendarFont(FXFont *fnt);

  /// Get font used by the calendar
  FXFont* getCalendarFont() const;

  /// Destructor
  virtual ~FXCalendar();
  };

}

#endif
