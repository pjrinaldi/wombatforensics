%{
/* $Id: expr.l,v 1.4 2006-11-05 04:42:43 ganzhorn Exp $
 * This file is part of lfhex.
 * Copyright (C) 2006 Salem Ganzhorn <eyekode@yahoo.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include <math.h>

#include <string>
#include <stdexcept>
#include <iostream>

using namespace std;

#include "expr.h"
#include "expr_yacc.h"

int exprparse();

extern off_t expr_value;

static off_t hex2l( const string &str );

#define yylval exprlval

%}

%option noyywrap

hexnum   [0-9a-fA-F]+

%%

[1-9][0-9]* {
               yylval = atol(yytext);
               return NUMBER;
            }
{hexnum} |
0x{hexnum}  {
               string hexnum = yytext;
               if( hexnum.substr(0,2) == "0x" ) {
                 hexnum = hexnum.substr(2);
               }
               yylval = hex2l(hexnum);
               return NUMBER;
            }
[-+*()]     {  return yytext[0]; }
[ \r\n\t]   {/*nop*/}
.           {  return yytext[0]; }

%%

static off_t 
hex2l( 
  const string &str
  )
{
  //
  off_t ret = 0;
  size_t scale = 1;
  int i = str.size()-1;
  while( i > -1 ) {
    switch( str[i] ) {
      case '0':
      break;
      case '1':
        ret += 1*scale;
      break;
      case '2':
        ret += 2*scale;
      break;
      case '3':
        ret += 3*scale;
      break;
      case '4':
        ret += 4*scale;
      break;
      case '5': 
        ret += 5*scale;
      break;
      case '6':
        ret += 6*scale;
      break;
      case '7':
        ret += 7*scale;
      break;
      case '8':
        ret += 8*scale;
      break;
      case '9':
        ret += 9*scale;
      break;
      case 'a': 
      case 'A':
        ret += 10*scale;
      break;
      case 'b':
      case 'B':
        ret += 11*scale;
      break;
      case 'c': 
      case 'C':
        ret += 12*scale;
      break;
      case 'd':
      case 'D':
        ret += 13*scale;
      break;
      case 'e':
      case 'E':
        ret += 14*scale;
      break;
      case 'f':
      case 'F':
        ret += 15*scale;
      break;
      default: 
        char buf[1024];
        sprintf(buf,"Unknown hex char: \"%s\"[%d]",str.c_str(),i);
        throw runtime_error(buf);
    }
    --i;
    scale *= 16;
  }
  return ret;
}

bool
expr_eval(
  const string &str,
  off_t     &retval
  )
{
//  exprrestart(NULL);
  expr_scan_string(str.c_str());
  expr_value = 0;
  bool status = !exprparse();
  retval = expr_value;
  return status;
}


