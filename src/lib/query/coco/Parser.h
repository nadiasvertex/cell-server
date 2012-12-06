/*----------------------------------------------------------------------
Compiler Generator Coco/R,
Copyright (c) 1990, 2004 Hanspeter Moessenboeck, University of Linz
extended by M. Loeberbauer & A. Woess, Univ. of Linz
ported to C++ by Csaba Balazs, University of Szeged
with improvements by Pat Terry, Rhodes University

This program is free software; you can redistribute it and/or modify it 
under the terms of the GNU General Public License as published by the 
Free Software Foundation; either version 2, or (at your option) any 
later version.

This program is distributed in the hope that it will be useful, but 
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License 
for more details.

You should have received a copy of the GNU General Public License along 
with this program; if not, write to the Free Software Foundation, Inc., 
59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

As an exception, it is allowed to write an extension of Coco/R that is
used as a plugin in non-free software.

If not otherwise stated, any source code generated by Coco/R (other than 
Coco/R itself) does not fall under the GNU General Public License.
-----------------------------------------------------------------------*/


#if !defined(cell__query__sql_COCO_PARSER_H__)
#define cell__query__sql_COCO_PARSER_H__

#include <vector>
#include <query/cpp/sql_ast.h>


#include "Scanner.h"

namespace cell {
namespace query {
namespace sql {


class Errors {
public:
	int count;			// number of errors detected

	Errors();
	void SynErr(int line, int col, int n);
	void Error(int line, int col, const wchar_t *s);
	void Warning(int line, int col, const wchar_t *s);
	void Warning(const wchar_t *s);
	void Exception(const wchar_t *s);

}; // Errors

class Parser {
private:
	enum {
		_EOF=0,
		_hex_integer=1,
		_integer=2,
		_decimal=3,
		_name=4,
		_quoted_name=5
	};
	int maxT;

	Token *dummyToken;
	int errDist;
	int minErrDist;

	void SynErr(int n);
	void Get();
	void Expect(int n);
	bool StartOf(int s);
	void ExpectWeak(int n, int follow);
	bool WeakSeparator(int n, int syFol, int repFol);

public:
	Scanner *scanner;
	Errors  *errors;

	Token *t;			// last recognized token
	Token *la;			// lookahead token

public:
   std::vector<query*> queries;
   
private:
   /** The current select query, if any. */
   select *sq;
   
public:

/*******************************************************************************
 *  Character Class Descriptions
 *******************************************************************************/



	Parser(Scanner *scanner);
	~Parser();
	void SemErr(const wchar_t* msg);

	void SQL();
	void Select();
	void SelectExpr(select::expr *&e);
	void Expression(select::expr *&e);
	void AndCondition(select::expr *&e);
	void Condition(select::expr *&e);
	void Operand(select::expr *&e);
	void ConditionRhs(select::expr *&e);
	void Compare();
	void Summand(select::expr *&e);
	void Factor(select::expr *&e);
	void Term(select::expr *&e);
	void Value(select::value_expr *&v);
	void Numeric(select::numeric_expr *&nv);

	void Parse();

}; // end Parser

} // namespace
} // namespace
} // namespace


#endif

