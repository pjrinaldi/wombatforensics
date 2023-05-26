/********************************************************************************
*                                                                               *
*                            L i s t   W i d g e t                              *
*                                                                               *
*********************************************************************************
* Copyright (C) 1997,2022 by Jeroen van der Zijp.   All Rights Reserved.        *
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
#ifndef FXLIST_H
#define FXLIST_H

#ifndef FXSCROLLAREA_H
#include "FXScrollArea.h"
#endif

namespace FX {


/// List styles
enum {
  LIST_EXTENDEDSELECT    = 0,             /// Extended selection mode allows for drag-selection of ranges of items
  LIST_SINGLESELECT      = 0x00100000,    /// Single selection mode allows up to one item to be selected
  LIST_BROWSESELECT      = 0x00200000,    /// Browse selection mode enforces one single item to be selected at all times
  LIST_MULTIPLESELECT    = 0x00300000,    /// Multiple selection mode is used for selection of individual items
  LIST_AUTOSELECT        = 0x00400000,    /// Automatically select under cursor
  LIST_NORMAL            = LIST_EXTENDEDSELECT
  };


class FXIcon;
class FXFont;
class FXList;


/// List item
class FXAPI FXListItem : public FXObject {
  FXDECLARE(FXListItem)
  friend class FXList;
protected:
  FXString  label;
  FXIcon   *icon;
  FXptr     data;
  FXuint    state;
  FXint     x,y;
private:
  FXListItem(const FXListItem&);
  FXListItem& operator=(const FXListItem&);
protected:
  FXListItem():icon(nullptr),data(nullptr),state(0),x(0),y(0){}
  virtual void draw(const FXList* list,FXDC& dc,FXint x,FXint y,FXint w,FXint h) const;
  virtual FXint hitItem(const FXList* list,FXint x,FXint y) const;
public:
  enum {
    SELECTED  = 1,      /// Selected
    FOCUS     = 2,      /// Focus
    DISABLED  = 4,      /// Disabled
    DRAGGABLE = 8,      /// Draggable
    ICONOWNED = 16      /// Icon owned by item
    };
public:

  /// Construct new item with given text, icon, and user-data
  FXListItem(const FXString& text,FXIcon* ic=nullptr,FXptr ptr=nullptr):label(text),icon(ic),data(ptr),state(0),x(0),y(0){}

  /// Change item's text label
  virtual void setText(const FXString& txt);

  /// Return item's text label
  const FXString& getText() const { return label; }

  /// Change item's icon, deleting the old icon if it was owned
  virtual void setIcon(FXIcon* icn,FXbool owned=false);

  /// Return item's icon
  FXIcon* getIcon() const { return icon; }

  /// Change item's user data
  void setData(FXptr ptr){ data=ptr; }

  /// Get item's user data
  FXptr getData() const { return data; }

  /// Make item draw as focused
  virtual void setFocus(FXbool focus);

  /// Return true if item has focus
  FXbool hasFocus() const { return (state&FOCUS)!=0; }

  /// Select item
  virtual void setSelected(FXbool selected);

  /// Return true if this item is selected
  FXbool isSelected() const { return (state&SELECTED)!=0; }

  /// Enable or disable item
  virtual void setEnabled(FXbool enabled);

  /// Return true if this item is enabled
  FXbool isEnabled() const { return (state&DISABLED)==0; }

  /// Make item draggable
  virtual void setDraggable(FXbool draggable);

  /// Return true if this item is draggable
  FXbool isDraggable() const { return (state&DRAGGABLE)!=0; }

  /// Return tip text
  virtual FXString getTipText() const;

  /// Return width of item as drawn in list
  virtual FXint getWidth(const FXList* list) const;

  /// Return height of item as drawn in list
  virtual FXint getHeight(const FXList* list) const;

  /// Create server-side resources
  virtual void create();

  /// Detach server-side resources
  virtual void detach();

  /// Destroy server-side resources
  virtual void destroy();

  /// Save to stream
  virtual void save(FXStream& store) const;

  /// Load from stream
  virtual void load(FXStream& store);

  /// Destroy item and free icons if owned
  virtual ~FXListItem();
  };


/// List item collate function
typedef FXint (*FXListSortFunc)(const FXListItem*,const FXListItem*);


/// List of FXListItem's
typedef FXObjectListOf<FXListItem> FXListItemList;


/**
* A List Widget displays a list of items, each with a text and
* optional icon.  When an item's selected state changes, the list sends
* a SEL_SELECTED or SEL_DESELECTED message.  A change of the current
* item is signified by the SEL_CHANGED message.
* The list sends SEL_COMMAND messages when the user clicks on an item,
* and SEL_CLICKED, SEL_DOUBLECLICKED, and SEL_TRIPLECLICKED when the user
* clicks once, twice, or thrice, respectively.
* When items are inserted or removed, the list sends messages of
* the type SEL_INSERTED or SEL_DELETED.
* In each of these cases, the index to the item, if any, is passed in the
* 3rd argument of the message.
*/
class FXAPI FXList : public FXScrollArea {
  FXDECLARE(FXList)
protected:
  FXListItemList items;             // Item list
  FXint          anchor;            // Anchor item
  FXint          current;           // Current item
  FXint          extent;            // Extent item
  FXint          viewable;          // Viewable item
  FXFont        *font;              // Font
  FXColor        textColor;         // Text color
  FXColor        selbackColor;      // Selected back color
  FXColor        seltextColor;      // Selected text color
  FXint          listWidth;         // List width
  FXint          listHeight;        // List height
  FXint          visible;           // Number of rows high
  FXString       help;              // Help text
  FXListSortFunc sortfunc;          // Item sort function
  FXint          grabx;             // Grab point x
  FXint          graby;             // Grab point y
  FXString       lookup;            // Lookup string
  FXbool         state;             // State of item
protected:
  FXList();
  void recompute();
  virtual FXListItem *createItem(const FXString& text,FXIcon* icon,FXptr ptr);
private:
  FXList(const FXList&);
  FXList &operator=(const FXList&);
public:
  long onPaint(FXObject*,FXSelector,void*);
  long onEnter(FXObject*,FXSelector,void*);
  long onLeave(FXObject*,FXSelector,void*);
  long onUngrabbed(FXObject*,FXSelector,void*);
  long onKeyPress(FXObject*,FXSelector,void*);
  long onKeyRelease(FXObject*,FXSelector,void*);
  long onLeftBtnPress(FXObject*,FXSelector,void*);
  long onLeftBtnRelease(FXObject*,FXSelector,void*);
  long onRightBtnPress(FXObject*,FXSelector,void*);
  long onRightBtnRelease(FXObject*,FXSelector,void*);
  long onMotion(FXObject*,FXSelector,void*);
  long onFocusIn(FXObject*,FXSelector,void*);
  long onFocusOut(FXObject*,FXSelector,void*);
  long onAutoScroll(FXObject*,FXSelector,void*);
  long onClicked(FXObject*,FXSelector,void*);
  long onDoubleClicked(FXObject*,FXSelector,void*);
  long onTripleClicked(FXObject*,FXSelector,void*);
  long onCommand(FXObject*,FXSelector,void*);
  long onQueryTip(FXObject*,FXSelector,void*);
  long onQueryHelp(FXObject*,FXSelector,void*);
  long onTipTimer(FXObject*,FXSelector,void*);
  long onLookupTimer(FXObject*,FXSelector,void*);
  long onCmdSetValue(FXObject*,FXSelector,void*);public:
  long onCmdGetIntValue(FXObject*,FXSelector,void*);
  long onCmdSetIntValue(FXObject*,FXSelector,void*);
public:
  static FXint ascending(const FXListItem* a,const FXListItem* b);
  static FXint descending(const FXListItem* a,const FXListItem* b);
  static FXint ascendingCase(const FXListItem* a,const FXListItem* b);
  static FXint descendingCase(const FXListItem* a,const FXListItem* b);
public:
  enum {
    ID_LOOKUPTIMER=FXScrollArea::ID_LAST,
    ID_LAST
    };
public:

  /// Construct a list with initially no items in it
  FXList(FXComposite *p,FXObject* tgt=nullptr,FXSelector sel=0,FXuint opts=LIST_NORMAL,FXint x=0,FXint y=0,FXint w=0,FXint h=0);

  /// Create server-side resources
  virtual void create();

  /// Detach server-side resources
  virtual void detach();

  /// Perform layout
  virtual void layout();

  /// Return default width
  virtual FXint getDefaultWidth();

  /// Return default height
  virtual FXint getDefaultHeight();

  /// Compute and return content width
  virtual FXint getContentWidth();

  /// Return content height
  virtual FXint getContentHeight();

  /// Recalculate layout
  virtual void recalc();

  /// List widget can receive focus
  virtual FXbool canFocus() const;

  /// Move the focus to this window
  virtual void setFocus();

  /// Remove the focus from this window
  virtual void killFocus();

  /// Return the number of items in the list
  FXint getNumItems() const { return (FXint)items.no(); }

  /// Return number of visible items
  FXint getNumVisible() const { return visible; }

  /// Change the number of visible items
  void setNumVisible(FXint nvis);

  /// Return the item at the given index
  FXListItem *getItem(FXint index) const;

  /// Replace the item with a [possibly subclassed] item
  FXint setItem(FXint index,FXListItem* item,FXbool notify=false);

  /// Replace items text, icon, and user-data pointer
  FXint setItem(FXint index,const FXString& text,FXIcon *icon=nullptr,FXptr ptr=nullptr,FXbool notify=false);

  /// Fill list by appending items from array of strings
  FXint fillItems(const FXchar *const *strings,FXIcon *icon=nullptr,FXptr ptr=nullptr,FXbool notify=false);

  /// Fill list by appending items from array of strings
  FXint fillItems(const FXString* strings,FXIcon *icon=nullptr,FXptr ptr=nullptr,FXbool notify=false);

  /// Fill list by appending items from newline separated strings
  FXint fillItems(const FXString& strings,FXIcon *icon=nullptr,FXptr ptr=nullptr,FXbool notify=false);

  /// Insert a new [possibly subclassed] item at the give index
  FXint insertItem(FXint index,FXListItem* item,FXbool notify=false);

  /// Insert item at index with given text, icon, and user-data pointer
  FXint insertItem(FXint index,const FXString& text,FXIcon *icon=nullptr,FXptr ptr=nullptr,FXbool notify=false);

  /// Append a [possibly subclassed] item to the list
  FXint appendItem(FXListItem* item,FXbool notify=false);

  /// Append new item with given text and optional icon, and user-data pointer
  FXint appendItem(const FXString& text,FXIcon *icon=nullptr,FXptr ptr=nullptr,FXbool notify=false);

  /// Prepend a [possibly subclassed] item to the list
  FXint prependItem(FXListItem* item,FXbool notify=false);

  /// Prepend new item with given text and optional icon, and user-data pointer
  FXint prependItem(const FXString& text,FXIcon *icon=nullptr,FXptr ptr=nullptr,FXbool notify=false);

  /// Move item from oldindex to newindex
  FXint moveItem(FXint newindex,FXint oldindex,FXbool notify=false);

  /// Extract item from list
  FXListItem* extractItem(FXint index,FXbool notify=false);

  /// Remove item from list
  void removeItem(FXint index,FXbool notify=false);

  /// Remove all items from list
  void clearItems(FXbool notify=false);

  /// Return item width
  FXint getItemWidth(FXint index) const;

  /// Return item height
  FXint getItemHeight(FXint index) const;

  /// Return index of item at x,y, if any
  virtual FXint getItemAt(FXint x,FXint y) const;

  /// Return item hit code: 0 no hit; 1 hit the icon; 2 hit the text
  FXint hitItem(FXint index,FXint x,FXint y) const;

  /// Scroll to bring item into view
  virtual void makeItemVisible(FXint index);

  /**
  * Search items by name, beginning from item start.  If the start
  * item is -1 the search will start at the first item in the list.
  * Flags may be SEARCH_FORWARD or SEARCH_BACKWARD to control the
  * search direction; this can be combined with SEARCH_NOWRAP or SEARCH_WRAP
  * to control whether the search wraps at the start or end of the list.
  * The option SEARCH_IGNORECASE causes a case-insensitive match.  Finally,
  * passing SEARCH_PREFIX causes searching for a prefix of the item name.
  * Return -1 if no matching item is found.
  */
  FXint findItem(const FXString& string,FXint start=-1,FXuint flags=SEARCH_FORWARD|SEARCH_WRAP) const;

  /**
  * Search items by associated user data, beginning from item start. If the
  * start item is -1 the search will start at the first item in the list.
  * Flags may be SEARCH_FORWARD or SEARCH_BACKWARD to control the
  * search direction; this can be combined with SEARCH_NOWRAP or SEARCH_WRAP
  * to control whether the search wraps at the start or end of the list.
  */
  FXint findItemByData(FXptr ptr,FXint start=-1,FXuint flags=SEARCH_FORWARD|SEARCH_WRAP) const;

  /// Change item text
  void setItemText(FXint index,const FXString& text);

  /// Return item text
  FXString getItemText(FXint index) const;

  /// Change item icon, deleting the old icon if it was owned
  void setItemIcon(FXint index,FXIcon* icon,FXbool owned=false);

  /// Return item icon, if any
  FXIcon* getItemIcon(FXint index) const;

  /// Change item user-data pointer
  void setItemData(FXint index,FXptr ptr);

  /// Return item user-data pointer
  FXptr getItemData(FXint index) const;

  /// Return true if item is selected
  FXbool isItemSelected(FXint index) const;

  /// Return true if item is current
  FXbool isItemCurrent(FXint index) const;

  /// Return true if item is visible
  FXbool isItemVisible(FXint index) const;

  /// Return true if item is enabled
  FXbool isItemEnabled(FXint index) const;

  /// Repaint item
  void updateItem(FXint index) const;

  /// Enable item
  virtual FXbool enableItem(FXint index);

  /// Disable item
  virtual FXbool disableItem(FXint index);

  /// Select item
  virtual FXbool selectItem(FXint index,FXbool notify=false);

  /// Deselect item
  virtual FXbool deselectItem(FXint index,FXbool notify=false);

  /// Toggle item selection state
  virtual FXbool toggleItem(FXint index,FXbool notify=false);

  /// Extend selection from anchor item to index
  virtual FXbool extendSelection(FXint index,FXbool notify=false);

  /// Select all items
  virtual FXbool selectAll(FXbool notify=false);

  /// Deselect all items
  virtual FXbool killSelection(FXbool notify=false);

  /// Change current item
  virtual void setCurrentItem(FXint index,FXbool notify=false);

  /// Return current item, if any
  FXint getCurrentItem() const { return current; }

  /// Change anchor item
  void setAnchorItem(FXint index);

  /// Return anchor item, if any
  FXint getAnchorItem() const { return anchor; }

  /// Sort items using current sort function
  void sortItems();

  /// Return sort function
  FXListSortFunc getSortFunc() const { return sortfunc; }

  /// Change sort function
  void setSortFunc(FXListSortFunc func){ sortfunc=func; }

  /// Change text font
  void setFont(FXFont* fnt);

  /// Return text font
  FXFont* getFont() const { return font; }

  /// Return normal text color
  FXColor getTextColor() const { return textColor; }

  /// Change normal text color
  void setTextColor(FXColor clr);

  /// Return selected text background
  FXColor getSelBackColor() const { return selbackColor; }

  /// Change selected text background
  void setSelBackColor(FXColor clr);

  /// Return selected text color
  FXColor getSelTextColor() const { return seltextColor; }

  /// Change selected text color
  void setSelTextColor(FXColor clr);

  /// Return list style
  FXuint getListStyle() const;

  /// Change list style
  void setListStyle(FXuint style);

  /// Set the status line help text for this list
  void setHelpText(const FXString& text);

  /// Get the status line help text for this list
  const FXString& getHelpText() const { return help; }

  /// Save list to a stream
  virtual void save(FXStream& store) const;

  /// Load list from a stream
  virtual void load(FXStream& store);

  /// Destructor
  virtual ~FXList();
  };

}

#endif
