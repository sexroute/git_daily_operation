# -*- coding=utf-8 -*-

#   basestring:     terminal symbol (characters)
#   keyword:        terminal symbol (keyword)
#   matchobj:       terminal symbols (regex, use for scanning symbols)
#   function:       named non-terminal symbol, recursive definition
#                   if you don't want naming in output, precede name with an underscore
#   tuple:          production sequence
#   integer:        count in production sequence:
#                    0: following element is optional
#                   -1: following element can be omitted or repeated endless
#                   -2: following element is required and can be repeated endless
#   list:           options, choose one of them
#   _not:           next element in production sequence is matched only if this would not
#   _and:           next element in production sequence is matched only if this would, too


import os
import sys

import re
import glob

import codecs
import locale

import fileinput

import exceptions

class keyword(unicode): pass
class code(unicode): pass
class ignore(object):
    def __init__(self, regex_text, *args):
        self.regex = re.compile(regex_text, *args)

class _and(object):
    def __init__(self, something):
        self.obj = something

class _not(_and): pass

class Name(unicode):
    def __init__(self, *args):
        self.line = 0
        self.file = u""

class Symbol(list):
    def __init__(self, name, what):
        self.__name__ = name
        self.append(name)
        self.what = what
        self.append(what)
    def __call__(self):
        return self.what
    def __unicode__(self):
        return u'Symbol(' + repr(self.__name__) + ', ' + repr(self.what) + u')'
    def __repr__(self):
        return unicode(self)

word_regex = re.compile(ur"\w+")
rest_regex = re.compile(ur".*")

print_trace = False

def u(text):
    if isinstance(text, exceptions.BaseException):
        text = text.args[0]
    if type(text) is unicode:
        return text
    if isinstance(text, str):
        if sys.stdin.encoding:
            return codecs.decode(text, sys.stdin.encoding)
        else:
            return codecs.decode(text, locale.getpreferredencoding())
    return unicode(text)

def skip(skipper, text, skipWS, skipComments):
    if skipWS:
        t = text.lstrip()
    else:
        t = text
    if skipComments:
        try:
            while True:
                skip, t = skipper.parseLine(t, skipComments, [], skipWS, None)
                if skipWS:
                    t = t.lstrip()
        except: pass
    return t

class parser(object):
    def __init__(self, another = False, p = False):
        self.restlen = -1
        if not(another):
            self.skipper = parser(True, p)
            self.skipper.packrat = p
        else:
            self.skipper = self
        self.lines = None
        self.textlen = 0
        self.memory = {}
        self.packrat = p

    # parseLine():
    #   textline:       text to parse
    #   pattern:        pyPEG language description
    #   resultSoFar:    parsing result so far (default: blank list [])
    #   skipWS:         Flag if whitespace should be skipped (default: True)
    #   skipComments:   Python functions returning pyPEG for matching comments
    #
    #   returns:        pyAST, textrest
    #
    #   raises:         SyntaxError(reason) if textline is detected not being in language
    #                   described by pattern
    #
    #                   SyntaxError(reason) if pattern is an illegal language description

    def parseLine(self, textline, pattern, resultSoFar = [], skipWS = True, skipComments = None):
        name = None
        _textline = textline
        _pattern = pattern

        def R(result, text):
            if __debug__:
                if print_trace:
                    try:
                        if _pattern.__name__ != "comment":
                            sys.stderr.write(u"match: " + _pattern.__name__ + u"\n")
                    except: pass

            if self.restlen == -1:
                self.restlen = len(text)
            else:
                self.restlen = min(self.restlen, len(text))
            res = resultSoFar
            if name and result:
                name.line = self.lineNo()
                res.append(Symbol(name, result))
            elif name:
                name.line = self.lineNo()
                res.append(Symbol(name, []))
            elif result:
                if type(result) is type([]):
                    res.extend(result)
                else:
                    res.extend([result])
            if self.packrat:
                self.memory[(len(_textline), id(_pattern))] = (res, text)
            return res, text

        def syntaxError():
            if self.packrat:
                self.memory[(len(_textline), id(_pattern))] = False
            raise SyntaxError()

        if self.packrat:
            try:
                result = self.memory[(len(textline), id(pattern))]
                if result:
                    return result
                else:
                    raise SyntaxError()
            except: pass

        if callable(pattern):
            if __debug__:
                if print_trace:
                    try:
                        if pattern.__name__ != "comment":
                            sys.stderr.write(u"testing with " + pattern.__name__ + u": " + textline[:40] + u"\n")
                    except: pass

            if pattern.__name__[0] != "_":
                name = Name(pattern.__name__)

            pattern = pattern()
            if callable(pattern):
                pattern = (pattern,)

        text = skip(self.skipper, textline, skipWS, skipComments)

        pattern_type = type(pattern)

        if pattern_type is str or pattern_type is unicode:
            if text[:len(pattern)] == pattern:
                text = skip(self.skipper, text[len(pattern):], skipWS, skipComments)
                return R(None, text)
            else:
                syntaxError()

        elif pattern_type is keyword:
            m = word_regex.match(text)
            if m:
                if m.group(0) == pattern:
                    text = skip(self.skipper, text[len(pattern):], skipWS, skipComments)
                    return R(None, text)
                else:
                    syntaxError()
            else:
                syntaxError()

        elif pattern_type is _not:
            try:
                r, t = self.parseLine(text, pattern.obj, [], skipWS, skipComments)
            except:
                return resultSoFar, textline
            syntaxError()

        elif pattern_type is _and:
            r, t = self.parseLine(text, pattern.obj, [], skipWS, skipComments)
            return resultSoFar, textline

        elif pattern_type is type(word_regex) or pattern_type is ignore:
            if pattern_type is ignore:
                pattern = pattern.regex
            m = pattern.match(text)
            if m:
                text = skip(self.skipper, text[len(m.group(0)):], skipWS, skipComments)
                if pattern_type is ignore:
                    return R(None, text)
                else:
                    return R(m.group(0), text)
            else:
                syntaxError()

        elif pattern_type is tuple:
            result = []
            n = 1
            for p in pattern:
                if type(p) is type(0):
                    n = p
                else:
                    if n>0:
                        for i in range(n):
                            result, text = self.parseLine(text, p, result, skipWS, skipComments)
                    elif n==0:
                        if text == "":
                            pass
                        else:
                            try:
                                newResult, newText = self.parseLine(text, p, result, skipWS, skipComments)
                                result, text = newResult, newText
                            except SyntaxError:
                                pass
                    elif n<0:
                        found = False
                        while True:
                            try:
                                newResult, newText = self.parseLine(text, p, result, skipWS, skipComments)
                                result, text, found = newResult, newText, True
                            except SyntaxError:
                                break
                        if n == -2 and not(found):
                            syntaxError()
                    n = 1
            return R(result, text)

        elif pattern_type is list:
            result = []
            found = False
            for p in pattern:
                try:
                    result, text = self.parseLine(text, p, result, skipWS, skipComments)
                    found = True
                except SyntaxError:
                    pass
                if found:
                    break
            if found:
                return R(result, text)
            else:
                syntaxError()

        else:
            raise SyntaxError(u"illegal type in grammar: " + u(pattern_type))

    def lineNo(self):
        if not(self.lines): return u""
        if self.restlen == -1: return u""
        parsed = self.textlen - self.restlen

        left, right = 0, len(self.lines)

        while True:
            mid = int((right + left) / 2)
            if self.lines[mid][0] <= parsed:
                try:
                    if self.lines[mid + 1][0] >= parsed:
                        try:
                            return u(self.lines[mid + 1][1]) + u":" + u(self.lines[mid + 1][2])
                        except:
                            return u""
                    else:
                        left = mid + 1
                except:
                    try:
                        return u(self.lines[mid + 1][1]) + u":" + u(self.lines[mid + 1][2])
                    except:
                        return u""
            else:
                right = mid - 1
            if left > right:
                return u""

# plain module API

def parseLine(textline, pattern, resultSoFar = [], skipWS = True, skipComments = None, packrat = False):
    p = parser(p=packrat)
    text = skip(p.skipper, textline, skipWS, skipComments)
    ast, text = p.parseLine(text, pattern, resultSoFar, skipWS, skipComments)
    return ast, text

# parse():
#   language:       pyPEG language description
#   lineSource:     a fileinput.FileInput object
#   skipWS:         Flag if whitespace should be skipped (default: True)
#   skipComments:   Python function which returns pyPEG for matching comments
#   packrat:        use memoization
#   lineCount:      add line number information to AST
#
#   returns:        pyAST
#
#   raises:         SyntaxError(reason), if a parsed line is not in language
#                   SyntaxError(reason), if the language description is illegal

def parse(language, lineSource, skipWS = True, skipComments = None, packrat = False, lineCount = True, encoding=None):
    lines, lineNo = [], 0

    if encoding is None:
      encoding = locale.getpreferredencoding()

    while callable(language):
        language = language()

    orig, ld = "", 0
    for line in lineSource:
        if lineSource.isfirstline():
            ld = 1
        else:
            ld += 1
        lines.append((len(orig), lineSource.filename(), lineSource.lineno() - 1))
        orig += line

    textlen = len(orig)

    try:
        p = parser(p=packrat)
        p.textlen = len(orig)
        if lineCount:
            p.lines = lines
        else:
            p.line = None
        text = skip(p.skipper, orig, skipWS, skipComments)
        result, text = p.parseLine(text, language, [], skipWS, skipComments)
        if text:
            raise SyntaxError()

    except SyntaxError, msg:
        parsed = textlen - p.restlen
        textlen = 0
        nn, lineNo, filename = 0, 0, u""
        for n, ld, l in lines:
            if n >= parsed:
                break
            else:
                lineNo = l
                nn += 1
                filename = ld

        lineNo += 1
        nn -= 1
        lineContent = orig.splitlines()[nn]

        msg = u'''File "%s", line %s, %s''' % (u(os.path.abspath(filename)),
                                               u(lineNo),
                                               lineContent.decode(encoding))

        raise SyntaxError(msg.encode(locale.getpreferredencoding()))

    return result

def comment():
  return [re.compile(r"//.*"), re.compile("/\*.*?\*/", re.S)]

# symbol <- r"\w+"
def name():
  return re.compile(r"\w+")

def value():
  return re.compile(r"\d+")

# file inclusive boiler-plate
def ifndef_statement():
  return [('#ifndef', name), ('#if', '!defined', '(', name, ')')]

def define_statement():
  return '#define', name

def endif_statement():
  return '#endif'

def filename():
  return re.compile(r'\w+\.\w+')

def include_statement():
  return '#include', '"', name, '.', name, '"'

def pragma_statement():
  return '#pragma', 'once'


# name value definition
def define_field_statement():
  return '#define', name, value

def enum_item():
  return name, 0, ('=', value)

def enum_item_list():
  return enum_item, -1, (',', enum_item)

def enum_block():
  return keyword('enum'), '{', enum_item_list, 0, ',', '}', ';'

def statement():
  return [define_field_statement, enum_block]

def lang():
  return (0, pragma_statement,
          0, ifndef_statement,
          0, define_statement,
          -1, include_statement,
          -1, statement,
          0, endif_statement)

def extract_name_list(result):
  for i in result:
    if isinstance(i, Symbol) and i.__name__ == 'statement':
      for j in i[1]:
        if isinstance(j, Symbol) and j.__name__ == 'define_field_statement':
          k,v = j[1]
          yield k.what, v.what

if __name__ == '__main__':
  files = []
  for i in sys.argv[1:]:
    files.extend(glob.glob(i))
  
  for filename in files:
    default_encoding = locale.getpreferredencoding()
    result = parse(lang(), fileinput.input(filename), True, comment, encoding=default_encoding)
  
    with open(os.path.basename(filename) + '.csv', 'w') as f:
      for k,v in extract_name_list(result):
        f.write('%s,%s\n' % (k,v))
