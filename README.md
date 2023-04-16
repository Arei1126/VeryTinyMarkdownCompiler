# VeryTinyMarkdownCompiler
Very tiny subset of markdown for generating html documents.

## Usage
vtmc [DOCUMENT_TITLE]

It recieves markdown file from standard input and generates html file to standard output. 
Thus, you will normally use like "cat MARKDOWN | vtmc > document.html".

If DOCUMENT_TITLE is given, vtmc generates html header.

## Syntax
\#~\######: \<h1> ~ \<h6>

\*: \<bold>

_: \<hr>

\![ALT_TITLE]\!IMG_URL: \<img src = IMG_URL alt = ALT_TITLE>

[LINK_TITLE]LINK: \<a href = LINK>LINK_TITLE\</a>

blank line: <br>


