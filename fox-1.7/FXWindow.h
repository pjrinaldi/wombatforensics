/********************************************************************************
*                                                                               *
*                            W i n d o w   O b j e c t                          *
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
#ifndef FXWINDOW_H
#define FXWINDOW_H

#ifndef FXDRAWABLE_H
#include "FXDrawable.h"
#endif

namespace FX {


// Forward declarations
class FXIcon;
class FXBitmap;
class FXCursor;
class FXRegion;
class FXDCWindow;
class FXComposite;
class FXAccelTable;
class FXComposeContext;


/// Layout hints for child widgets
enum {
  LAYOUT_NORMAL      = 0,                                   /// Default layout mode
  LAYOUT_SIDE_TOP    = 0,                                   /// Pack on top side (default)
  LAYOUT_SIDE_BOTTOM = 0x00000001,                          /// Pack on bottom side
  LAYOUT_SIDE_LEFT   = 0x00000002,                          /// Pack on left side
  LAYOUT_SIDE_RIGHT  = LAYOUT_SIDE_LEFT|LAYOUT_SIDE_BOTTOM, /// Pack on right side
  LAYOUT_FILL_COLUMN = 0x00000001,                          /// Matrix column is stretchable
  LAYOUT_FILL_ROW    = 0x00000002,                          /// Matrix row is stretchable
  LAYOUT_LEFT        = 0,                                   /// Stick on left (default)
  LAYOUT_RIGHT       = 0x00000004,                          /// Stick on right
  LAYOUT_CENTER_X    = 0x00000008,                          /// Center horizontally
  LAYOUT_FIX_X       = LAYOUT_RIGHT|LAYOUT_CENTER_X,        /// X fixed
  LAYOUT_TOP         = 0,                                   /// Stick on top (default)
  LAYOUT_BOTTOM      = 0x00000010,                          /// Stick on bottom
  LAYOUT_CENTER_Y    = 0x00000020,                          /// Center vertically
  LAYOUT_FIX_Y       = LAYOUT_BOTTOM|LAYOUT_CENTER_Y,       /// Y fixed
  LAYOUT_DOCK_SAME   = 0,                                   /// Dock on same galley if it fits
  LAYOUT_DOCK_NEXT   = 0x00000040,                          /// Dock on next galley
  LAYOUT_RESERVED_1  = 0x00000080,
  LAYOUT_FIX_WIDTH   = 0x00000100,                          /// Width fixed
  LAYOUT_FIX_HEIGHT  = 0x00000200,                          /// height fixed
  LAYOUT_MIN_WIDTH   = 0,                                   /// Minimum width is the default
  LAYOUT_MIN_HEIGHT  = 0,                                   /// Minimum height is the default
  LAYOUT_FILL_X      = 0x00000400,                          /// Stretch or shrink horizontally
  LAYOUT_FILL_Y      = 0x00000800,                          /// Stretch or shrink vertically
  LAYOUT_FILL        = LAYOUT_FILL_X|LAYOUT_FILL_Y,         /// Stretch or shrink in both directions
  LAYOUT_EXPLICIT    = LAYOUT_FIX_X|LAYOUT_FIX_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT   /// Explicit placement
  };


/// Frame border appearance styles (for subclasses)
enum {
  FRAME_NONE   = 0,                                     /// Default is no frame
  FRAME_SUNKEN = 0x00001000,                            /// Sunken border
  FRAME_RAISED = 0x00002000,                            /// Raised border
  FRAME_THICK  = 0x00004000,                            /// Thick border
  FRAME_GROOVE = FRAME_THICK,                           /// A groove or etched-in border
  FRAME_RIDGE  = FRAME_THICK|FRAME_RAISED|FRAME_SUNKEN, /// A ridge or embossed border
  FRAME_LINE   = FRAME_RAISED|FRAME_SUNKEN,             /// Simple line border
  FRAME_NORMAL = FRAME_SUNKEN|FRAME_THICK               /// Regular raised/thick border
  };


/// Packing style (for packers)
enum {
  PACK_NORMAL         = 0,              /// Default is each its own size
  PACK_UNIFORM_HEIGHT = 0x00008000,     /// Uniform height
  PACK_UNIFORM_WIDTH  = 0x00010000      /// Uniform width
  };


/// Data exchange modes
enum FXDNDOrigin {
  FROM_SELECTION  = 0,              /// Primary selection
  FROM_CLIPBOARD  = 1,              /// Clipboard
  FROM_DRAGNDROP  = 2               /// Drag and drop source
  };


/// Drag and drop actions
enum FXDragAction {
  DRAG_REJECT  = 0,             /// Reject all drop actions
  DRAG_ASK     = 1,             /// Ask
  DRAG_COPY    = 2,             /// Copy
  DRAG_MOVE    = 3,             /// Move
  DRAG_LINK    = 4,             /// Link
  DRAG_PRIVATE = 5,             /// Private
  DRAG_ACCEPT  = 6              /// Accept any drop action
  };


/**
* Window is the base class for all widgets in FOX.
* All widgets are organized in a tree structure, with a single root window
* at the top.
* Each window links to its parent window, next and previous siblings windows,
* as well as its first and last child windows.  A window also has an owner window,
* which is the window responsible for its creation and destruction.  In most cases,
* the owner window will be the same as the parent window, except for top-level windows.
* The focus window determines to which child keyboard messages are delegated if the
* window is a composite window (i.e. if it has children).
* Windows are geometrically arranged by their immediate parent window; the actual
* method of placement is determined by the type of layout supported by the parent
* window, as well as the window's layout hints.
* The position of a window is determined relative to its immediate parent window;
* A window can send a message to its target object when an event happens.
*/
class FXAPI FXWindow : public FXDrawable {
  FXDECLARE(FXWindow)
  friend class FXApp;
private:
  FXWindow         *parent;             // Parent Window
  FXWindow         *owner;              // Owner Window
  FXWindow         *first;              // First Child
  FXWindow         *last;               // Last Child
  FXWindow         *next;               // Next Sibling
  FXWindow         *prev;               // Previous Sibling
  FXWindow         *focus;              // Focus Child
  FXuint            wk;                 // Window Key
protected:
  FXComposeContext *composeContext;     // Compose context
  FXCursor         *defaultCursor;      // Normal Cursor
  FXCursor         *dragCursor;         // Cursor during drag
  FXAccelTable     *accelTable;         // Accelerator table
  FXObject         *target;             // Target object
  FXSelector        message;            // Message ID
  FXint             xpos;               // Window X Position
  FXint             ypos;               // Window Y Position
  FXColor           backColor;          // Window background color
  FXString          tag;                // Help tag
  FXuint            flags;              // Window state flags
  FXuint            options;            // Window options
public:
  static FXDragType       octetType;    // Raw octet stream
  static FXDragType       deleteType;   // Delete request
  static FXDragType       textType;     // Ascii text request
  static FXDragType       colorType;    // Color request
  static FXDragType       urilistType;  // URI list request
  static FXDragType       utf8Type;     // UTF-8 text request
  static FXDragType       utf16Type;    // UTF-16 text request
  static FXDragType       actionType;   // Clipboard action
  static const FXDragType stringType;   // Clipboard text type (pre-registered)
  static const FXDragType imageType;    // Clipboard image type (pre-registered)
protected:
  virtual FXbool doesOverrideRedirect() const;
protected:
#ifdef WIN32
  virtual FXID GetDC() const;
  virtual int ReleaseDC(FXID) const;
  virtual const void* GetClass() const;
#else
  void addColormapWindows();
  void remColormapWindows();
#endif
protected:
  FXWindow();
  FXWindow(FXApp* a,FXVisual *vis);
  FXWindow(FXApp* a,FXWindow* own,FXuint opts,FXint x,FXint y,FXint w,FXint h);
private:
  FXWindow(const FXWindow&);
  FXWindow& operator=(const FXWindow&);
protected:

  // Window state flags
  enum {
    FLAG_SHOWN        = 0x00000001,     // Is shown
    FLAG_ENABLED      = 0x00000002,     // Able to receive input
    FLAG_UPDATE       = 0x00000004,     // Is subject to GUI update
    FLAG_DROPTARGET   = 0x00000008,     // Drop target
    FLAG_FOCUSED      = 0x00000010,     // Has focus
    FLAG_DIRTY        = 0x00000020,     // Needs layout
    FLAG_RECALC       = 0x00000040,     // Needs recalculation
    FLAG_TIP          = 0x00000080,     // Show tip
    FLAG_HELP         = 0x00000100,     // Show help
    FLAG_DEFAULT      = 0x00000200,     // Default widget
    FLAG_INITIAL      = 0x00000400,     // Initial widget
    FLAG_SHELL        = 0x00000800,     // Shell window
    FLAG_ACTIVE       = 0x00001000,     // Window is active
    FLAG_PRESSED      = 0x00002000,     // Button has been pressed
    FLAG_KEY          = 0x00004000,     // Keyboard key pressed
    FLAG_CARET        = 0x00008000,     // Caret is on
    FLAG_CHANGED      = 0x00010000,     // Window data changed
    FLAG_LASSO        = 0x00020000,     // Lasso mode
    FLAG_TRYDRAG      = 0x00040000,     // Tentative drag mode
    FLAG_DODRAG       = 0x00080000,     // Doing drag mode
    FLAG_SCROLLINSIDE = 0x00100000,     // Scroll only when inside
    FLAG_SCROLLING    = 0x00200000,     // Right mouse scrolling
    FLAG_OWNED        = 0x00400000,     // Owned window handle
    FLAG_CURSOR       = 0x00800000      // Showing cursor
    };

public:

  /// Window classes
  enum WindowClass {
    ClassNormal,        /// Normal child window
    ClassGraphic,       /// Graphics child window
    ClassSpecial,       /// Special child window
    ClassRoot,          /// Root window
    ClassShell,         /// Shell window
    ClassPopup,         /// Popup window
    ClassToolTip,       /// Tooltip window
    ClassMain,          /// Main application window
    ClassDialog,        /// Dialog box window
    ClassToolBar,       /// Toolbar window
    ClassSplash,        /// Splash window
    ClassNotify,        /// Notify window
    ClassPanel,         /// Panel window
    ClassDesktop        /// Desktop backdrop window
    };

public:
  long onPaint(FXObject*,FXSelector,void*);
  long onMap(FXObject*,FXSelector,void*);
  long onUnmap(FXObject*,FXSelector,void*);
  long onConfigure(FXObject*,FXSelector,void*);
  long onUpdate(FXObject*,FXSelector,void*);
  long onMotion(FXObject*,FXSelector,void*);
  long onMouseWheel(FXObject*,FXSelector,void*);
  long onEnter(FXObject*,FXSelector,void*);
  long onLeave(FXObject*,FXSelector,void*);
  long onLeftBtnPress(FXObject*,FXSelector,void*);
  long onLeftBtnRelease(FXObject*,FXSelector,void*);
  long onMiddleBtnPress(FXObject*,FXSelector,void*);
  long onMiddleBtnRelease(FXObject*,FXSelector,void*);
  long onRightBtnPress(FXObject*,FXSelector,void*);
  long onRightBtnRelease(FXObject*,FXSelector,void*);
  long onSpaceBallMotion(FXObject*,FXSelector,void*);
  long onSpaceBallButtonPress(FXObject*,FXSelector,void*);
  long onSpaceBallButtonRelease(FXObject*,FXSelector,void*);
  long onBeginDrag(FXObject*,FXSelector,void*);
  long onEndDrag(FXObject*,FXSelector,void*);
  long onDragged(FXObject*,FXSelector,void*);
  long onKeyPress(FXObject*,FXSelector,void*);
  long onKeyRelease(FXObject*,FXSelector,void*);
  long onUngrabbed(FXObject*,FXSelector,void*);
  long onDestroy(FXObject*,FXSelector,void*);
  long onFocusSelf(FXObject*,FXSelector,void*);
  long onFocusIn(FXObject*,FXSelector,void*);
  long onFocusOut(FXObject*,FXSelector,void*);
  long onSelectionLost(FXObject*,FXSelector,void*);
  long onSelectionGained(FXObject*,FXSelector,void*);
  long onSelectionRequest(FXObject*,FXSelector,void*);
  long onClipboardLost(FXObject*,FXSelector,void*);
  long onClipboardGained(FXObject*,FXSelector,void*);
  long onClipboardRequest(FXObject*,FXSelector,void*);
  long onDNDEnter(FXObject*,FXSelector,void*);
  long onDNDLeave(FXObject*,FXSelector,void*);
  long onDNDMotion(FXObject*,FXSelector,void*);
  long onDNDDrop(FXObject*,FXSelector,void*);
  long onDNDRequest(FXObject*,FXSelector,void*);
  long onQueryHelp(FXObject*,FXSelector,void*);
  long onQueryTip(FXObject*,FXSelector,void*);
  long onCmdShow(FXObject*,FXSelector,void*);
  long onCmdHide(FXObject*,FXSelector,void*);
  long onUpdToggleShown(FXObject*,FXSelector,void*);
  long onCmdToggleShown(FXObject*,FXSelector,void*);
  long onCmdRaise(FXObject*,FXSelector,void*);
  long onCmdLower(FXObject*,FXSelector,void*);
  long onCmdEnable(FXObject*,FXSelector,void*);
  long onCmdDisable(FXObject*,FXSelector,void*);
  long onUpdToggleEnabled(FXObject*,FXSelector,void*);
  long onCmdToggleEnabled(FXObject*,FXSelector,void*);
  long onCmdUpdate(FXObject*,FXSelector,void*);
  long onUpdYes(FXObject*,FXSelector,void*);
  long onCmdDelete(FXObject*,FXSelector,void*);
public:
  enum {
    ID_NONE,
    ID_HIDE,
    ID_SHOW,
    ID_TOGGLESHOWN,
    ID_LOWER,
    ID_RAISE,
    ID_DELETE,
    ID_DISABLE,
    ID_ENABLE,
    ID_TOGGLEENABLED,
    ID_UNCHECK,
    ID_CHECK,
    ID_UNKNOWN,
    ID_UPDATE,
    ID_TIPTIMER,
    ID_SETVALUE,
    ID_SETINTVALUE,
    ID_SETLONGVALUE,
    ID_SETREALVALUE,
    ID_SETSTRINGVALUE,
    ID_SETICONVALUE,
    ID_SETINTRANGE,
    ID_SETREALRANGE,
    ID_GETINTVALUE,
    ID_GETLONGVALUE,
    ID_GETREALVALUE,
    ID_GETSTRINGVALUE,
    ID_GETICONVALUE,
    ID_GETINTRANGE,
    ID_GETREALRANGE,
    ID_SETHELPSTRING,
    ID_GETHELPSTRING,
    ID_SETTIPSTRING,
    ID_GETTIPSTRING,
    ID_QUERY_MENU,
    ID_HOTKEY,
    ID_ACCEL,
    ID_UNPOST,
    ID_POST,
    ID_MDI_TILEHORIZONTAL,
    ID_MDI_TILEVERTICAL,
    ID_MDI_CASCADE,
    ID_MDI_MAXIMIZE,
    ID_MDI_MINIMIZE,
    ID_MDI_RESTORE,
    ID_MDI_CLOSE,
    ID_MDI_WINDOW,
    ID_MDI_MENUWINDOW,
    ID_MDI_MENUMINIMIZE,
    ID_MDI_MENURESTORE,
    ID_MDI_MENUCLOSE,
    ID_MDI_NEXT,
    ID_MDI_PREV,
    ID_LAST
    };
public:

  // Predefined DND type names
  static const FXchar octetTypeName[];
  static const FXchar deleteTypeName[];
  static const FXchar textTypeName[];
  static const FXchar colorTypeName[];
  static const FXchar urilistTypeName[];
  static const FXchar utf8TypeName[];
  static const FXchar utf16TypeName[];
  static const FXchar actionTypeName[];

public:

  /// Constructor
  FXWindow(FXComposite* p,FXuint opts=0,FXint x=0,FXint y=0,FXint w=0,FXint h=0);

  /// Return a pointer to the parent window
  FXWindow* getParent() const { return parent; }

  /// Return a pointer to the owner window
  FXWindow* getOwner() const { return owner; }

  /// Return a pointer to the shell window
  FXWindow* getShell() const;

  /// Return a pointer to the root window
  FXWindow* getRoot() const;

  /// Return a pointer to the next (sibling) window, if any
  FXWindow* getNext() const { return next; }

  /// Return a pointer to the previous (sibling) window , if any
  FXWindow* getPrev() const { return prev; }

  /// Return a pointer to this window's first child window , if any
  FXWindow* getFirst() const { return first; }

  /// Return a pointer to this window's last child window, if any
  FXWindow* getLast() const { return last; }

  /// Return a pointer to the currently focused child window
  FXWindow* getFocus() const { return focus; }

  /// Get window class
  virtual WindowClass getWindowClass() const;

  /// Change window key
  void setKey(FXuint k){ wk=k; }

  /// Return window key
  FXuint getKey() const { return wk; }

  /// Return child window with given window key
  FXWindow* getChildWithKey(FXuint k) const;

  /// Set the message target object for this window
  void setTarget(FXObject *t){ target=t; }

  /// Get the message target object for this window, if any
  FXObject* getTarget() const { return target; }

  /// Set the message identifier for this window
  void setSelector(FXSelector sel){ message=sel; }

  /// Get the message identifier for this window
  FXSelector getSelector() const { return message; }

  /// Get this window's x-coordinate, in the parent's coordinate system
  FXint getX() const { return xpos; }

  /// Get this window's y-coordinate, in the parent's coordinate system
  FXint getY() const { return ypos; }

  /// Return the default width of this window
  virtual FXint getDefaultWidth();

  /// Return the default height of this window
  virtual FXint getDefaultHeight();

  /// Return width for given height
  virtual FXint getWidthForHeight(FXint givenheight);

  /// Return height for given width
  virtual FXint getHeightForWidth(FXint givenwidth);

  /// Set this window's x-coordinate, in the parent's coordinate system
  void setX(FXint x);

  /// Set this window's y-coordinate, in the parent's coordinate system
  void setY(FXint y);

  /**
  * Set the window width; and flag the widget as being in need of
  * layout by its parent.  This does not immediately update the server-
  * side representation of the widget.
  */
  void setWidth(FXint w);

  /**
  * Set the window height; and flag the widget as being in need of
  * layout by its parent.  This does not immediately update the server-
  * side representation of the widget.
  */
  void setHeight(FXint h);

  /// Set layout hints for this window
  void setLayoutHints(FXuint lout);

  /// Get layout hints for this window
  FXuint getLayoutHints() const;

  /// Return a pointer to the accelerator table
  FXAccelTable* getAccelTable() const { return accelTable; }

  /// Set the accelerator table
  void setAccelTable(FXAccelTable* acceltable){ accelTable=acceltable; }

  /// Add a hot key
  void addHotKey(FXHotKey code);

  /// Remove a hot key
  void remHotKey(FXHotKey code);

  /// Change help tag for this widget
  void setHelpTag(const FXString&  text){ tag=text; }

  /// Get the help tag for this widget
  const FXString& getHelpTag() const { return tag; }

  /// Return true if window is a shell window
  FXbool isShell() const;

  /// Return true if specified window is owned by this window
  FXbool isOwnerOf(const FXWindow* window) const;

  /// Return true if specified window is ancestor of this window
  FXbool isChildOf(const FXWindow* window) const;

  /// Return true if this window contains child in its subtree
  FXbool containsChild(const FXWindow* child) const;

  /// Return the child window at specified coordinates
  FXWindow* getChildAt(FXint x,FXint y) const;

  /// Return the number of child windows for this window
  FXint numChildren() const;

  /**
  * Return the index (starting from zero) of the specified child window,
  * or -1 if the window is not a child or NULL
  */
  FXint indexOfChild(const FXWindow *window) const;

  /**
  * Return the child window at specified index,
  * or NULL if the index is negative or out of range
  */
  FXWindow* childAtIndex(FXint index) const;

  /// Return the common ancestor of window a and window b
  static FXWindow* commonAncestor(FXWindow* a,FXWindow* b);

  /// Return true if sibling a <= sibling b in list
  static FXbool before(const FXWindow *a,const FXWindow* b);

  /// Return true if sibling a >= sibling b in list
  static FXbool after(const FXWindow *a,const FXWindow* b);

  /// Return compose context
  FXComposeContext* getComposeContext() const { return composeContext; }

  /// Create compose context
  void createComposeContext();

  /// Destroy compose context
  void destroyComposeContext();

  /// Is cursor shown
  FXbool cursorShown() const;

  /// Show or hide the cursor
  void showCursor(FXbool flag=true);

  /// Set the default cursor for this window
  void setDefaultCursor(FXCursor* cur);

  /// Return the default cursor for this window
  FXCursor* getDefaultCursor() const { return defaultCursor; }

  /// Set the drag cursor for this window
  void setDragCursor(FXCursor* cur);

  /// Return the drag cursor for this window
  FXCursor* getDragCursor() const { return dragCursor; }

  /// Return the cursor position and mouse button-state
  FXbool getCursorPosition(FXint& x,FXint& y,FXuint& buttons) const;

  /// Warp the cursor to the new position
  FXbool setCursorPosition(FXint x,FXint y);

  /// Return true if the window is active
  FXbool isActive() const;

  /// Return true if this window is a control capable of receiving the focus
  virtual FXbool canFocus() const;

  /// Return true if this window has the focus
  FXbool hasFocus() const;

  /// Return true if this window is in focus chain
  FXbool inFocusChain() const;

  /// Move the focus to this window
  virtual void setFocus();

  /// Remove the focus from this window
  virtual void killFocus();

  /// Return true if this is the default window
  FXbool isDefault() const;

  /**
  * This changes the default window which responds to the Return
  * key in a dialog. If the flag is true, this window becomes the default
  * window; when the flag is false, this window will be no longer the
  * default window.  Finally, when the flag is maybe, the default window
  * will revert to the initial default window.
  */
  virtual void setDefault(FXuchar flag=true);

  /// Find default window among this window's children
  FXWindow* findDefault() const;

  /// Return true if this is the initial default window
  FXbool isInitial() const;

  /// Make this window the initial default window
  void setInitial(FXbool flag=true);

  /// Find inital default window among this window's children
  FXWindow* findInitial() const;

  /// Return true if this window is able to receive mouse and keyboard events
  FXbool isEnabled() const;

  /// Enable the window to receive mouse and keyboard events
  virtual void enable();

  /// Disable the window from receiving mouse and keyboard events
  virtual void disable();

  /// Create all of the server-side resources for this window
  virtual void create();

  /// Attach foreign window handle to this window
  virtual void attach(FXID w);

  /// Detach the server-side resources for this window
  virtual void detach();

  /// Destroy the server-side resources for this window
  virtual void destroy();

  /// Set window shape by means of region
  virtual void setShape(const FXRegion& region);

  /// Set window shape by means of bitmap
  virtual void setShape(FXBitmap* bitmap);

  /// Set window shape by means of icon
  virtual void setShape(FXIcon* icon);

  /// Clear window shape
  virtual void clearShape();

  /// Raise this window to the top of the stacking order
  virtual void raise();

  /// Lower this window to the bottom of the stacking order
  virtual void lower();

  /**
  * Move the window immediately, in the parent's coordinate system.
  * Update the server representation as well if the window is realized.
  * Perform layout of the children when necessary.
  */
  virtual void move(FXint x,FXint y);

  /**
  * Resize the window to the specified width and height immediately,
  * updating the server representation as well, if the window was realized.
  * Perform layout of the children when necessary.
  */
  virtual void resize(FXint w,FXint h);

  /**
  * Move and resize the window immediately, in the parent's coordinate system.
  * Update the server representation as well if the window is realized.
  * Perform layout of the children when necessary.
  */
  virtual void position(FXint x,FXint y,FXint w,FXint h);

  /// Mark this window's layout as dirty for later layout
  virtual void recalc();

  /// Perform layout immediately
  virtual void layout();

  /// Generate a SEL_UPDATE message for the window and its children
  void forceRefresh();

  /// Reparent this window under new father before other
  virtual void reparent(FXWindow* father,FXWindow *other=nullptr);

  /// Scroll rectangle x,y,w,h by a shift of dx,dy
  void scroll(FXint x,FXint y,FXint w,FXint h,FXint dx,FXint dy) const;

  /// Mark the specified rectangle to be repainted later
  void update(FXint x,FXint y,FXint w,FXint h) const;

  /// Mark the entire window to be repainted later
  void update() const;

  /// Process any outstanding repaint messages immediately, for the given rectangle
  void repaint(FXint x,FXint y,FXint w,FXint h) const;

  /// Process any outstanding repaint messages immediately
  void repaint() const;

  /// Return true if the window has been grabbed
  FXbool grabbed() const;

  /**
  * Grab the mouse to this window; future mouse events will be
  * reported to this window even while the cursor goes outside of this window
  */
  void grab();

  /// Release the mouse grab
  void ungrab();

  /// Return true if active grab is in effect
  FXbool grabbedKeyboard() const;

  /// Grab keyboard device
  void grabKeyboard();

  /// Ungrab keyboard device
  void ungrabKeyboard();

  /// Return true if the window is shown
  FXbool shown() const;

  /// Show this window
  virtual void show();

  /// Hide this window
  virtual void hide();

  /// Return true if the window is composite
  virtual FXbool isComposite() const;

  /// Return true if the window is under the cursor
  FXbool underCursor() const;

  /// Return true if this window owns the primary selection
  FXbool hasSelection() const;

  /// Try to acquire the primary selection, given a list of drag types
  FXbool acquireSelection(const FXDragType *types,FXuint numtypes);

  /// Release the primary selection
  FXbool releaseSelection();

  /// Return true if this window owns the clipboard
  FXbool hasClipboard() const;

  /// Try to acquire the clipboard, given a list of drag types
  FXbool acquireClipboard(const FXDragType *types,FXuint numtypes);

  /// Release the clipboard
  FXbool releaseClipboard();

  /// Return true if this window is able to receive drops
  FXbool isDropEnabled() const;

  /// Enable this window to receive drops
  virtual void dropEnable();

  /// Disable this window from receiving drops
  virtual void dropDisable();

  /// Return true if a drag operaion has been initiated from this window
  FXbool isDragging() const;

  /// Initiate a drag operation with a list of previously registered drag types
  FXbool beginDrag(const FXDragType *types,FXuint numtypes);

  /**
  * When dragging, inform the drop-target of the new position and
  * the drag action
  */
  FXbool handleDrag(FXint x,FXint y,FXDragAction action=DRAG_COPY);

  /**
  * Terminate the drag operation with or without actually dropping the data
  * Returns the action performed by the target
  */
  FXDragAction endDrag(FXbool drop=true);

  /// Return true if this window is the target of a drop
  FXbool isDropTarget() const;

  /**
  * When being dragged over, indicate that no further SEL_DND_MOTION messages
  * are required while the cursor is inside the given rectangle
  */
  void setDragRectangle(FXint x,FXint y,FXint w,FXint h,FXbool wantupdates=true) const;

  /**
  * When being dragged over, indicate we want to receive SEL_DND_MOTION messages
  * every time the cursor moves
  */
  void clearDragRectangle() const;

  /// When being dragged over, indicate acceptance or rejection of the dragged data
  void acceptDrop(FXDragAction action=DRAG_ACCEPT) const;

  /// The target accepted our drop
  FXDragAction didAccept() const;

  /**
  * Sent by the drop target in response to SEL_DND_DROP.  The drag action
  * should be the same as the action the drop target reported to the drag
  * source in reponse to the SEL_DND_MOTION message.
  * This function notifies the drag source that its part of the drop transaction
  * is finished, and that it is free to release any resources involved in the
  * drag operation.
  * Calling dropFinished() is advisable in cases where the drop target needs
  * to perform complex processing on the data received from the drag source,
  * prior to returning from the SEL_DND_DROP message handler.
  */
  void dropFinished(FXDragAction action=DRAG_REJECT) const;

  /// When being dragged over, inquire the drag types which are being offered
  FXbool inquireDNDTypes(FXDNDOrigin origin,FXDragType*& types,FXuint& numtypes) const;

  /// When being dragged over, return true if we are offered the given drag type
  FXbool offeredDNDType(FXDNDOrigin origin,FXDragType type) const;

  /// When being dragged over, return the drag action
  FXDragAction inquireDNDAction() const;

  /**
  * Set DND data; the array must be allocated with allocElms() and ownership is
  * transferred to the system.
  */
  FXbool setDNDData(FXDNDOrigin origin,FXDragType type,FXuchar* data,FXuint size) const;

  /**
  * Set DND data from string value.
  */
  FXbool setDNDData(FXDNDOrigin origin,FXDragType type,const FXString& string) const;

  /**
  * Get DND data; the caller becomes the owner of the array and must free it
  * with freeElms().
  */
  FXbool getDNDData(FXDNDOrigin origin,FXDragType type,FXuchar*& data,FXuint& size) const;

  /**
  * Get DND data into string value.
  */
  FXbool getDNDData(FXDNDOrigin origin,FXDragType type,FXString& string) const;

  /// Return true if window logically contains the given point
  virtual FXbool contains(FXint parentx,FXint parenty) const;

  /// Translate coordinates from fromwindow's coordinate space to this window's coordinate space
  void translateCoordinatesFrom(FXint& tox,FXint& toy,const FXWindow* fromwindow,FXint fromx,FXint fromy) const;

  /// Translate coordinates from this window's coordinate space to towindow's coordinate space
  void translateCoordinatesTo(FXint& tox,FXint& toy,const FXWindow* towindow,FXint fromx,FXint fromy) const;

  /// Set window background color
  virtual void setBackColor(FXColor clr);

  /// Get background color
  FXColor getBackColor() const { return backColor; }

  /// Does save-unders
  virtual FXbool doesSaveUnder() const;

  /**
  * Translate message for localization; using the current FXTranslator,
  * an attempt is made to translate the given message into the current
  * language.  An optional hint may be passed to break any ties in case
  * more than one tranlation is possible for the given message text.
  * In addition, the name of the widget is passed as context name so
  * that translations for a single dialog may be grouped together.
  */
  virtual const FXchar* tr(const FXchar* text,const FXchar* hint=nullptr,FXint count=-1) const FX_FORMAT(2) ;

  /// Save window to stream
  virtual void save(FXStream& store) const;

  /// Restore window from stream
  virtual void load(FXStream& store);

  /// Destroy window
  virtual ~FXWindow();
  };

}

#endif
