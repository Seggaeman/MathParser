========================================================================
	MATHPARSER
========================================================================
A  math parser coded in Visual C++. Based on this java parser:  http://cogitolearning.co.uk/?p=565. Written primarily as a learning project.
Features include standard arithmetic, logarithms, permutations/combinations and complex numbers.

Usage
-----
MathParser.exe <expression> [<additional options>]

<expression> can contain any of the following symbols.

Symbol table
------------
+	add						(		opening parenthesis
-	subtract				)		closing parenthesis
*	multiply				cos,sin,tan,acos,asin,atan trigonometric functions
/	divide					sqrt	square root
^	exponentiation			exp		base e exponentiation
e,E	*10^					ln,log2,log		logarithms to bases e, 2 and 10 respectively
!	factorial				P		permutation
C	combination				i,j		square root of -1 (imaginary number)

Precedence of operations(in descending order of priority)
---------------------------------------------------------
parenthesis, factorial, trigonometric/logarithm/square root/exp functions,  permutation/combination, exponentiation,
multiplication/division, addition/subtraction.

Some of the above operations have been grouped into sets. If they follow each other they are performed left to right.

Additional options
------------------
<additional options> are specified as follows
/[R][I]

R	Output real part of solution.
I	Output imaginary part of solution.

Ignoring <additional options> is equivalent to /RI

Sample usage:
------------
MathParser.exe "3+21"
(24,0)

MathParser.exe "(3*(12-3))^(1/3)" /R
3

MathParser.exe "(3P2C3)*23.11j-302i"
(0,160.2)

========================================================================
    CONSOLE APPLICATION : MathParser Project Overview
========================================================================

AppWizard has created this MathParser application for you.

This file contains a summary of what you will find in each of the files that
make up your MathParser application.


MathParser.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

MathParser.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

MathParser.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named MathParser.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////
