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


#include <wchar.h>
#include "Parser.h"
#include "Scanner.h"


namespace cell {
namespace query {
namespace sql {


void Parser::SynErr(int n) {
	if (errDist >= minErrDist) errors->SynErr(la->line, la->col, n);
	errDist = 0;
}

void Parser::SemErr(const wchar_t* msg) {
	if (errDist >= minErrDist) errors->Error(t->line, t->col, msg);
	errDist = 0;
}

void Parser::Get() {
	for (;;) {
		t = la;
		la = scanner->Scan();
		if (la->kind <= maxT) { ++errDist; break; }

		if (dummyToken != t) {
			dummyToken->kind = t->kind;
			dummyToken->pos = t->pos;
			dummyToken->col = t->col;
			dummyToken->line = t->line;
			dummyToken->next = NULL;
			coco_string_delete(dummyToken->val);
			dummyToken->val = coco_string_create(t->val);
			t = dummyToken;
		}
		la = t;
	}
}

void Parser::Expect(int n) {
	if (la->kind==n) Get(); else { SynErr(n); }
}

void Parser::ExpectWeak(int n, int follow) {
	if (la->kind == n) Get();
	else {
		SynErr(n);
		while (!StartOf(follow)) Get();
	}
}

bool Parser::WeakSeparator(int n, int syFol, int repFol) {
	if (la->kind == n) {Get(); return true;}
	else if (StartOf(repFol)) {return false;}
	else {
		SynErr(n);
		while (!(StartOf(syFol) || StartOf(repFol) || StartOf(0))) {
			Get();
		}
		return StartOf(syFol);
	}
}

void Parser::SQL() {
		Select();
		queries.push_back(
		 query_handle_type(
		    sq_stack.top()
		 )
		); sq_stack.pop();
		
		if (sq_stack.size())
		{
		 errors->Error(t->line, t->col, 
		 L"internal error: select stack not "
		 L"empty at this point.");
		}
		                                        
		Expect(6 /* ";" */);
}

void Parser::Select() {
		Expect(7 /* "SELECT" */);
		sq_stack.push(
		 new sql::select(
		  sq_stack.size() > 0 ? sq_stack.top() : nullptr
		 )
		); 
		
		select::expr_handle_type e;               
		SelectExpr(e);
		sq_stack.top()->add_select_expression(e); 
		while (la->kind == 8 /* "," */) {
			Get();
			SelectExpr(e);
			sq_stack.top()->add_select_expression(e); 
		}
}

void Parser::SelectExpr(select::expr_handle_type &e) {
		if (la->kind == 9 /* "*" */) {
			Get();
		} else if (StartOf(1)) {
			Expression(e);
		} else SynErr(38);
}

void Parser::Expression(select::expr_handle_type &e) {
		AndCondition(e);
		while (la->kind == 10 /* "OR" */) {
			Get();
			select::expr_handle_type r;                          
			AndCondition(r);
			e = select::expr_handle_type(
			       new select::binary_expr(
			        type::BOOL, L"OR", e, r
			       )
			);  
			                                         
		}
}

void Parser::AndCondition(select::expr_handle_type &e) {
		Condition(e);
		while (la->kind == 11 /* "AND" */) {
			Get();
			select::expr_handle_type r;                          
			Condition(r);
			e = select::expr_handle_type(
			       new select::binary_expr(
			         type::BOOL, L"AND", e, r
			       )
			);                                        
		}
}

void Parser::Condition(select::expr_handle_type &e) {
		if (StartOf(2)) {
			Operand(e);
			while (StartOf(3)) {
				select::expr_handle_type r;                          
				ConditionRhs(e, r);
				e = r;                                    
			}
		} else if (la->kind == 12 /* "NOT" */) {
			Get();
			Condition(e);
			e = select::expr_handle_type(
			      new select::unary_expr(
			        type::BOOL, L"NOT", e
			      )
			);                                        
		} else if (la->kind == 13 /* "EXISTS" */) {
			Get();
			Expect(14 /* "(" */);
			Select();
			e = select::expr_handle_type(
			     new select::unary_expr(
			       type::BOOL, L"EXISTS", 
			       select::expr_handle_type(
			          new select::sub_select_expr(
			            sq_stack.top()
			          )
			       )
			     )
			); sq_stack.pop();                       
			Expect(15 /* ")" */);
		} else SynErr(39);
}

void Parser::Operand(select::expr_handle_type &e) {
		Summand(e);
		while (la->kind == 32 /* "||" */) {
			Get();
			select::expr_handle_type r;               
			Summand(r);
			e = select::expr_handle_type(
			    new select::binary_expr(L"||", e, r));          
		}
}

void Parser::ConditionRhs(select::expr_handle_type l, select::expr_handle_type &e) {
		if (StartOf(4)) {
			Compare();
			std::wstring op(t->val);                  
			if (StartOf(2)) {
				Operand(e);
				e = select::expr_handle_type(
				      new select::binary_expr(
				        type::BOOL, op, l, e
				      )
				);                                        
			} else if (la->kind == 16 /* "ALL" */ || la->kind == 17 /* "ANY" */ || la->kind == 18 /* "SOME" */) {
				if (la->kind == 16 /* "ALL" */) {
					Get();
				} else if (la->kind == 17 /* "ANY" */) {
					Get();
				} else {
					Get();
				}
				std::wstring query_op(t->val);            
				Expect(14 /* "(" */);
				Select();
				e = select::expr_handle_type(
				      new select::unary_expr(
				        type::BOOL, op, 
				        select::expr_handle_type(
				          new select::sub_select_expr(
				             sq_stack.top()
				          )
				        )
				      )
				); sq_stack.pop();                       
				Expect(15 /* ")" */);
			} else SynErr(40);
		} else if (la->kind == 19 /* "IS" */) {
			Get();
			bool is_not = false; 
			bool distinct_from = false;               
			if (la->kind == 12 /* "NOT" */) {
				Get();
				is_not = true;                            
			}
			if (la->kind == 20 /* "NULL" */) {
				Get();
				e = select::expr_handle_type(
				     new select::binary_expr(
				         type::BOOL,
				         L"IS", 
				         l, 
				         select::expr_handle_type(
				              new select::null_expr()
				         )
				     )
				);  
				if (is_not)
				{
				e = select::expr_handle_type(
				       new select::unary_expr(
				          type::BOOL, L"NOT", e
				       )
				);
				}                                        
			} else if (StartOf(5)) {
				if (la->kind == 21 /* "DISTINCT" */) {
					Get();
					Expect(22 /* "FROM" */);
					distinct_from = true;                    
				}
				Operand(e);
				e = select::expr_handle_type(
				       new select::binary_expr(
				         type::BOOL,
				         distinct_from    ?
				         L"DISTINCT_FROM" :
				         L"IS",
				         l, e
				       )
				);                                                    
				if (is_not)
				{
				 e = select::expr_handle_type(
				       new select::unary_expr(
				         L"NOT", e
				       )
				 );
				}
				                                        
			} else SynErr(41);
		} else if (la->kind == 23 /* "BETWEEN" */) {
			Get();
			select::expr_handle_type mi, ma;         
			Operand(mi);
			Expect(11 /* "AND" */);
			Operand(ma);
			e = select::expr_handle_type(
			  new select::binary_expr(
			     type::BOOL,
			     L"AND",
			     select::expr_handle_type(
			        new select::binary_expr(
			              type::BOOL, L">=", l, mi
			        )
			     ),
			     select::expr_handle_type(
			        new select::binary_expr(
			           type::BOOL, L"<=", l, ma
			        )
			     )
			  )
			); 
			                                          
		} else if (la->kind == 24 /* "IN" */) {
			Get();
			Expect(14 /* "(" */);
			if (la->kind == 7 /* "SELECT" */) {
				Select();
				e = select::expr_handle_type(
				       new select::sub_select_expr(
				          sq_stack.top()
				       )
				); sq_stack.pop();                        
			} else if (StartOf(1)) {
				Expression(e);
				auto *le = new select::list_expr();
				le->add_expression(e);
				e = select::expr_handle_type(le);                                   
				while (la->kind == 8 /* "," */) {
					Get();
					select::expr_handle_type en;              
					Expression(en);
					le->add_expression(en);                   
				}
			} else SynErr(42);
			Expect(15 /* ")" */);
		} else SynErr(43);
}

void Parser::Compare() {
		switch (la->kind) {
		case 25 /* "<>" */: {
			Get();
			break;
		}
		case 26 /* "<=" */: {
			Get();
			break;
		}
		case 27 /* ">=" */: {
			Get();
			break;
		}
		case 28 /* "=" */: {
			Get();
			break;
		}
		case 29 /* "<" */: {
			Get();
			break;
		}
		case 30 /* ">" */: {
			Get();
			break;
		}
		case 31 /* "!=" */: {
			Get();
			break;
		}
		default: SynErr(44); break;
		}
}

void Parser::Summand(select::expr_handle_type &e) {
		Factor(e);
		while (la->kind == 33 /* "+" */ || la->kind == 34 /* "-" */) {
			if (la->kind == 33 /* "+" */) {
				Get();
			} else {
				Get();
			}
			select::expr_handle_type r; std::wstring op(t->val);  
			Factor(r);
			e = select::expr_handle_type(
			    new select::binary_expr(op, e, r));              
		}
}

void Parser::Factor(select::expr_handle_type &e) {
		Term(e);
		while (la->kind == 9 /* "*" */ || la->kind == 35 /* "/" */ || la->kind == 36 /* "%" */) {
			if (la->kind == 9 /* "*" */) {
				Get();
			} else if (la->kind == 35 /* "/" */) {
				Get();
			} else {
				Get();
			}
			select::expr_handle_type r; std::wstring op(t->val);  
			Term(r);
			e = select::expr_handle_type(
			    new select::binary_expr(op, e, r));              
		}
}

void Parser::Term(select::expr_handle_type &e) {
		if (StartOf(6)) {
			Value(e);
		} else if (la->kind == 7 /* "SELECT" */ || la->kind == 14 /* "(" */) {
			if (la->kind == 14 /* "(" */) {
				Get();
				Expression(e);
				Expect(15 /* ")" */);
			} else {
				Select();
				e = select::expr_handle_type(
				       new select::sub_select_expr(
				          sq_stack.top()
				       )
				 ); sq_stack.pop();                       
			}
		} else SynErr(45);
}

void Parser::Value(select::expr_handle_type &v) {
		select::numeric_expr *nv; 
		bool negate = false;                        
		if (la->kind == 33 /* "+" */ || la->kind == 34 /* "-" */) {
			if (la->kind == 33 /* "+" */) {
				Get();
			} else {
				Get();
				negate = true;                             
			}
		}
		Numeric(nv);
		if (negate)
		{
		 v = select::expr_handle_type(
		       new select::unary_expr(
		          L"NEG", 
		          select::expr_handle_type(nv)
		       )
		 );
		}
		else
		{
		 v = select::expr_handle_type(nv);
		}                                          
}

void Parser::Numeric(select::numeric_expr *&nv) {
		if (la->kind == _decimal) {
			Get();
			nv = new select::numeric_expr(t->val, 10, true);  
		} else if (la->kind == _hex_integer) {
			Get();
			nv = new select::numeric_expr(t->val, 16, false); 
		} else if (la->kind == _integer) {
			Get();
			nv = new select::numeric_expr(t->val, 10, false); 
		} else SynErr(46);
}




// If the user declared a method Init and a mehtod Destroy they should
// be called in the contructur and the destructor respctively.
//
// The following templates are used to recognize if the user declared
// the methods Init and Destroy.

template<typename T>
struct ParserInitExistsRecognizer {
	template<typename U, void (U::*)() = &U::Init>
	struct ExistsIfInitIsDefinedMarker{};

	struct InitIsMissingType {
		char dummy1;
	};
	
	struct InitExistsType {
		char dummy1; char dummy2;
	};

	// exists always
	template<typename U>
	static InitIsMissingType is_here(...);

	// exist only if ExistsIfInitIsDefinedMarker is defined
	template<typename U>
	static InitExistsType is_here(ExistsIfInitIsDefinedMarker<U>*);

	enum { InitExists = (sizeof(is_here<T>(NULL)) == sizeof(InitExistsType)) };
};

template<typename T>
struct ParserDestroyExistsRecognizer {
	template<typename U, void (U::*)() = &U::Destroy>
	struct ExistsIfDestroyIsDefinedMarker{};

	struct DestroyIsMissingType {
		char dummy1;
	};
	
	struct DestroyExistsType {
		char dummy1; char dummy2;
	};

	// exists always
	template<typename U>
	static DestroyIsMissingType is_here(...);

	// exist only if ExistsIfDestroyIsDefinedMarker is defined
	template<typename U>
	static DestroyExistsType is_here(ExistsIfDestroyIsDefinedMarker<U>*);

	enum { DestroyExists = (sizeof(is_here<T>(NULL)) == sizeof(DestroyExistsType)) };
};

// The folloing templates are used to call the Init and Destroy methods if they exist.

// Generic case of the ParserInitCaller, gets used if the Init method is missing
template<typename T, bool = ParserInitExistsRecognizer<T>::InitExists>
struct ParserInitCaller {
	static void CallInit(T *t) {
		// nothing to do
	}
};

// True case of the ParserInitCaller, gets used if the Init method exists
template<typename T>
struct ParserInitCaller<T, true> {
	static void CallInit(T *t) {
		t->Init();
	}
};

// Generic case of the ParserDestroyCaller, gets used if the Destroy method is missing
template<typename T, bool = ParserDestroyExistsRecognizer<T>::DestroyExists>
struct ParserDestroyCaller {
	static void CallDestroy(T *t) {
		// nothing to do
	}
};

// True case of the ParserDestroyCaller, gets used if the Destroy method exists
template<typename T>
struct ParserDestroyCaller<T, true> {
	static void CallDestroy(T *t) {
		t->Destroy();
	}
};

void Parser::Parse() {
	t = NULL;
	la = dummyToken = new Token();
	la->val = coco_string_create(L"Dummy Token");
	Get();
	SQL();
	Expect(0);
}

Parser::Parser(Scanner *scanner) {
	maxT = 37;

	ParserInitCaller<Parser>::CallInit(this);
	dummyToken = NULL;
	t = la = NULL;
	minErrDist = 2;
	errDist = minErrDist;
	this->scanner = scanner;
	errors = new Errors();
}

bool Parser::StartOf(int s) {
	const bool T = true;
	const bool x = false;

	static bool set[7][39] = {
		{T,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x},
		{x,T,T,T, x,x,x,T, x,x,x,x, T,T,T,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,T,T,x, x,x,x},
		{x,T,T,T, x,x,x,T, x,x,x,x, x,x,T,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,T,T,x, x,x,x},
		{x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,T, x,x,x,T, T,T,T,T, T,T,T,T, x,x,x,x, x,x,x},
		{x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,T,T,T, T,T,T,T, x,x,x,x, x,x,x},
		{x,T,T,T, x,x,x,T, x,x,x,x, x,x,T,x, x,x,x,x, x,T,x,x, x,x,x,x, x,x,x,x, x,T,T,x, x,x,x},
		{x,T,T,T, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,T,T,x, x,x,x}
	};



	return set[s][la->kind];
}

Parser::~Parser() {
	ParserDestroyCaller<Parser>::CallDestroy(this);
	delete errors;
	delete dummyToken;
}

Errors::Errors() {
	count = 0;
}

void Errors::SynErr(int line, int col, int n) {
	wchar_t* s;
	switch (n) {
			case 0: s = coco_string_create(L"EOF expected"); break;
			case 1: s = coco_string_create(L"hex_integer expected"); break;
			case 2: s = coco_string_create(L"integer expected"); break;
			case 3: s = coco_string_create(L"decimal expected"); break;
			case 4: s = coco_string_create(L"name expected"); break;
			case 5: s = coco_string_create(L"quoted_name expected"); break;
			case 6: s = coco_string_create(L"\";\" expected"); break;
			case 7: s = coco_string_create(L"\"SELECT\" expected"); break;
			case 8: s = coco_string_create(L"\",\" expected"); break;
			case 9: s = coco_string_create(L"\"*\" expected"); break;
			case 10: s = coco_string_create(L"\"OR\" expected"); break;
			case 11: s = coco_string_create(L"\"AND\" expected"); break;
			case 12: s = coco_string_create(L"\"NOT\" expected"); break;
			case 13: s = coco_string_create(L"\"EXISTS\" expected"); break;
			case 14: s = coco_string_create(L"\"(\" expected"); break;
			case 15: s = coco_string_create(L"\")\" expected"); break;
			case 16: s = coco_string_create(L"\"ALL\" expected"); break;
			case 17: s = coco_string_create(L"\"ANY\" expected"); break;
			case 18: s = coco_string_create(L"\"SOME\" expected"); break;
			case 19: s = coco_string_create(L"\"IS\" expected"); break;
			case 20: s = coco_string_create(L"\"NULL\" expected"); break;
			case 21: s = coco_string_create(L"\"DISTINCT\" expected"); break;
			case 22: s = coco_string_create(L"\"FROM\" expected"); break;
			case 23: s = coco_string_create(L"\"BETWEEN\" expected"); break;
			case 24: s = coco_string_create(L"\"IN\" expected"); break;
			case 25: s = coco_string_create(L"\"<>\" expected"); break;
			case 26: s = coco_string_create(L"\"<=\" expected"); break;
			case 27: s = coco_string_create(L"\">=\" expected"); break;
			case 28: s = coco_string_create(L"\"=\" expected"); break;
			case 29: s = coco_string_create(L"\"<\" expected"); break;
			case 30: s = coco_string_create(L"\">\" expected"); break;
			case 31: s = coco_string_create(L"\"!=\" expected"); break;
			case 32: s = coco_string_create(L"\"||\" expected"); break;
			case 33: s = coco_string_create(L"\"+\" expected"); break;
			case 34: s = coco_string_create(L"\"-\" expected"); break;
			case 35: s = coco_string_create(L"\"/\" expected"); break;
			case 36: s = coco_string_create(L"\"%\" expected"); break;
			case 37: s = coco_string_create(L"??? expected"); break;
			case 38: s = coco_string_create(L"invalid SelectExpr"); break;
			case 39: s = coco_string_create(L"invalid Condition"); break;
			case 40: s = coco_string_create(L"invalid ConditionRhs"); break;
			case 41: s = coco_string_create(L"invalid ConditionRhs"); break;
			case 42: s = coco_string_create(L"invalid ConditionRhs"); break;
			case 43: s = coco_string_create(L"invalid ConditionRhs"); break;
			case 44: s = coco_string_create(L"invalid Compare"); break;
			case 45: s = coco_string_create(L"invalid Term"); break;
			case 46: s = coco_string_create(L"invalid Numeric"); break;

		default:
		{
			wchar_t format[20];
			coco_swprintf(format, 20, L"error %d", n);
			s = coco_string_create(format);
		}
		break;
	}
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
	coco_string_delete(s);
	count++;
}

void Errors::Error(int line, int col, const wchar_t *s) {
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
	count++;
}

void Errors::Warning(int line, int col, const wchar_t *s) {
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
}

void Errors::Warning(const wchar_t *s) {
	wprintf(L"%ls\n", s);
}

void Errors::Exception(const wchar_t* s) {
	wprintf(L"%ls", s); 
	exit(1);
}

} // namespace
} // namespace
} // namespace

