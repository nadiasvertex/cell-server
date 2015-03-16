# Introduction #

One of the most important parts of a database server is fast transaction processing. The overhead of parsing the SQL is an integral part of the transaction, so a slow parser is a problem.

In addition, for cell the parser is used to rewrite the query and to recognize queries that we've seen before. So it really must be as fast as possible.

Without getting into parser theory too deeply, there are just a few common algorithms: LALR, LR(k), LL(k), GLR, and PEG. Some parsers implement LR or LL where k=1, which restricts the amount of ambiguity and sometimes improves the performance of the parser.

All of these parser algorithms can be linear for the appropriate grammar. The small print is that linear just means that for each basic operation, the time it takes for the parser to work depends only on the length of the input. What is not usually made clear is that the time of each basic operation may be quite large. Consequently, a linear LALR parser may be significantly faster than a linear LL(k) or PEG.

Other than speed, the most important thing in a production parser is error handling. You would think it would be grammar features, but error recovery is more important. Users will make mistakes, and you have to be able to recover from those mistakes and provide useful diagnostics.

Finally, it would be best if the generator supported EBNF-like operations.

# Evaluations #

I really like PEGs. They make sense to me on a fundamental level. Why pay for the complexity of a grammar generator, when you just want a _recognizer_? (All CFG algorithms are actually grammar generators, not grammar recognizers. They can be used to recognize by running them backwards, but that was not the purpose for which they were invented.)

I looked at many more parsers than I show on the list below. However, these were my top picks. Note that I specifically excluded ANTLR because its just too much stuff. I know its meant to make things simpler, but for me, it just complicates everything.

So I started with pegtl:

## pegtl ##

http://code.google.com/p/pegtl/

I really like this library. It is simple to integrate into your project, and has good documentation. It is also a scannerless system, which I like. I really wanted to use it, and in fact did use it for the prototype version of this project. However:

  * To deal with tree generation, you have to build a stack machine, which post processes the stack and generates your AST. Not the end of the world, but it adds a level of irritation.
  * The parser doesn't appear to perform much in the way of optimization, and instead relies on your C++ compiler to optimize. This may be fine, but it seems like the compiler doesn't really have the ability to "see" into the grammar in a useful way, so I am dubious about the speed of this parser. Also, the benchmarks show parsing of a "large" scheme file and the results are given in _seconds_. Even a large file should not take 300 milliseconds (the fastest listed time) to parse on a modern computer.
  * There is no ability to determine where you are in the input stream, and associate that with grammar pieces. During the AST analysis, you have no way of telling the user where exactly things went wrong. This is a show stopper.

Consequently, I had to discard it. I was unable to find any other PEG parsers for C++, so I had to move on to CFGs. (I did find YARD, but no documentation for it, also unmaintained?)

## Lemon ##

http://www.hwaci.com/sw/lemon/

The first CFG I looked at closely was the Lemon parser. This is a nice, fast LALR(1) parser with an enhanced grammar specification as compared to bison/yacc. It is the tool used to generate the parser for SQLite, so it is likely to see some maintenance love, and very likely has been looked at by several sets of eyes. In other words, it is mature and probably well-implemented.

Pros:

  * Better grammar specification
  * Re-entrant
  * Fast
  * Small
  * Well documented

Cons:

  * LALR
  * Not scannerless
  * Only generates C code

## MSTA ##

http://cocom.sourceforge.net/msta.html

The documentation for this project indicates that quite a bit of thought went into it. The generator claims to perform a number of optimizations on the grammar, which can transform non-LALR specified productions into faster LALR productions.

Pros:

  * Highly optimized generator
  * Extended grammar features for ease of use

Cons:

  * Not sure about the maintenance of the parser
  * Not sure about the portability of the system

## CppCC ##

http://cppcc.sourceforge.net/

I have used this parser before, and I liked it. The grammar specification is sane, has useful extensions, and it has all of the features I'm interested in.

Pros:

  * Not LALR
  * EBNF
  * Scannerless
  * Complete documentation

Cons:

  * Some concerns about actual performance.
  * Does not appear to be well maintained.
  * Some of the forums claim that the parser leaks memory, which is a real problem in a long-running program.

## Dragon ##

http://www.lemke-it.com/lit_opensource.html

Another LR(1) parser. This one perked my interest because it appears to do various analyses on the grammar to transform it into a potentially faster parser. It seems to have modest maintenance, however I couldn't find much documentation. In fact, I tried to download it and couldn't even find the archive, so...

## LLgennext ##

http://os.ghalkes.nl/LLnextgen/

An Extended-LL(1) parser. This is a rewrite of the LLgen generator from the AMK. It has a few useability improvements over LLgen. The Extended-LL(1) means it can deal with some conflicts that strict LL(1) generators cannot.

Pros:

  * Some useability enhancements
  * Small
  * Good documentation

Cons:

  * Only generates C code
  * YACC-ish grammar

## Coco/R ##

http://www.ssw.uni-linz.ac.at/coco/

I was very impressed by this generator. It appears to have a well-thought-out grammar, and designed to be used in production parsers.

Pros:

  * Supports UTF-8!
  * EBNF
  * Tons of ease-of-use features
  * Well maintained
  * Good documentation
  * Integrated scanner

Cons:

  * CFG instead of PEG, but that's minor really.

## Elkhound ##

http://scottmcpeak.com/elkhound/sources/elkhound/index.html

This parser uses the wonderful GLR algorithm, which makes the grammar specification much simpler. It also has numerous improvements to some prior art in GLR space. Performance-wise, it is not quite as fast as bison, but it might be worth it.

Pros:

  * Fast GLR
  * Careful implementation
  * Used to implement the Elsa C++ parser, so it has seen real use.

Cons:

  * Uncertain maintenance
  * Spotty documentation
  * Primitive error recovery

## Dparser ##

http://dparser.sourceforge.net/

This is a great parser which I have used on several occasions, including one very high-performance production system. However, getting at the recognized production values is not well documented and is frankly infuriating. As much as I like it, that one problem alone is enough to make me steer clear of it.

Pros:

  * Fast
  * Excellent grammar format
  * Generally quite easy to use

Cons:

  * Annoying production value access.

## bison ##

http://www.gnu.org/software/bison/

The veritable bison parser. The modern version implements GLR if you want it. Which I do.

Pros:

  * Very well documented
  * Mature, often-used, well-maintained.
  * Clearly very portable

Cons:

  * Frustrating experiences in the past leave me wondering if its a good idea to use it now.

# Decisions, Decisions #

After investigating all of the options, it came down to Lemon, Coco/R, CppCC, and bison/GLR. I decided to go with Coco/R, but quickly discovered that it didn't compile for mingw. After a little bit of patching, it worked fine.

I'm currently evaluating Coco/R. I chose it primarily because it had usability features that appear promising. It also seems to be fast and well maintained.