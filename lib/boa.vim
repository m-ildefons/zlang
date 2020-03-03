" Vim syntax file
" Language: Boa general purpose programming language
" Maintainer: Moritz Röhrich
" Latest Revision: 03 Mar 2020

if exists("b:current_syntax")
  finish
endif

syn keyword basicLanguageKeywords void
syn keyword basicLanguageKeywords int
syn keyword basicLanguageKeywords real
syn keyword basicLanguageKeywords complex
syn keyword basicLanguageKeywords imaginary
syn keyword basicLanguageKeywords char
syn keyword basicLanguageKeywords string

syn keyword basicLanguageKeywords for
syn keyword basicLanguageKeywords while
syn keyword basicLanguageKeywords if
syn keyword basicLanguageKeywords else

syn keyword basicLanguageKeywords return
syn keyword basicLanguageKeywords continue
syn keyword basicLanguageKeywords break

syn keyword basicLanguageKeywords NULL
syn keyword basicLanguageKeywords struct
syn keyword basicLanguageKeywords union

syn keyword basicLanguageKeywords equal
syn keyword basicLanguageKeywords unequal

syn keyword basicLanguageKeywords not
syn keyword basicLanguageKeywords and
syn keyword basicLanguageKeywords xor
syn keyword basicLanguageKeywords or

syn keyword basicLanguageKeywords exp

" Integer with - + or nothing in front
syn match number '\d\+'
syn match number '[-+]\d\+'

" Floating point number with decimal no E or e
syn match number '[-+]\d\+\.\d*'

" Floating point like number with E and no decimal point (+,-)
syn match number '[-+]\=\d[[:digit:]]*[eE][\-+]\=\d\+'
syn match number '\d[[:digit:]]*[eE][\-+]\=\d\+'

" Floating point like number with E and decimal point (+,-)
syn match number '[-+]\=\d[[:digit:]]*\.\d*[eE][\-+]\=\d\+'
syn match number '\d[[:digit:]]*\.\d*[eE][\-+]\=\d\+'

syn region string start='"' end='"' contained
