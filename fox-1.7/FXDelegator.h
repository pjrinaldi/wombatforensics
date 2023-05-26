/********************************************************************************
*                                                                               *
*                       D e l e g a t o r   T a r g e t                         *
*                                                                               *
*********************************************************************************
* Copyright (C) 2000,2022 by Jeroen van der Zijp.   All Rights Reserved.        *
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
#ifndef FXDELEGATOR_H
#define FXDELEGATOR_H

#ifndef FXOBJECT_H
#include "FXObject.h"
#endif

namespace FX {


/**
* A delegator forwards messages to a delegate object.
* Delegators are used when you need to multiplex messages
* toward any number of target objects.
* For example, many controls may be connected to FXDelegator,
* instead of directly to the document object.  Changing the
* delegate in FXDelegator will then reconnect the controls with their
* new target.
*/
class FXAPI FXDelegator : public FXObject {
  FXDECLARE(FXDelegator)
protected:
  FXObject   *delegate;
private:
  FXDelegator(const FXDelegator&);
  FXDelegator &operator=(const FXDelegator&);
public:
  virtual long onDefault(FXObject*,FXSelector,void*);
public:

  /// Construct a delegator
  FXDelegator(FXObject* target=nullptr):delegate(target){ }

  /// Return delegate object
  FXObject* getDelegate() const { return delegate; }

  /// Change delegate object
  void setDelegate(FXObject* target){ delegate=target; }

  /// Always trash during detroy
  virtual ~FXDelegator(){ delegate=(FXObject*)-1L; }
  };

}

#endif
