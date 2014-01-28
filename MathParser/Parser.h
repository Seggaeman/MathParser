#pragma once

#include "stdafx.h"
#include "Tokenizer.h"
#include "ExpressionNode.h"
class Parser {
private:
	Tokenizer theTokenizer;
	Token lookahead;
	void nextToken();
/*
	ExpressionNode* expression();
	ExpressionNode* sumOp(ExpressionNode* expr);
	ExpressionNode* signedTerm();
	ExpressionNode* term();
	ExpressionNode* termOp(ExpressionNode* expr);
	ExpressionNode* factor();
	ExpressionNode* factorOp(ExpressionNode* expr);
	ExpressionNode* argument();
	ExpressionNode* value();
*/
	ExpressionNode* expression();
	ExpressionNode* sumOp(ExpressionNode* theMultComponent);
	ExpressionNode* multComponent();
	ExpressionNode* multOp(ExpressionNode* theExpComponent);
	ExpressionNode* expComponent();
	ExpressionNode* expOp(ExpressionNode* thePermCombComponent);
	ExpressionNode* permCombComponent();
	ExpressionNode* permCombOp(ExpressionNode* theFuncComponent);
	ExpressionNode* funcComponent();
	ExpressionNode* factComponent(ExpressionNode* baseNode);
	ExpressionNode* valBracketComponent();
	ExpressionNode* value();


public:
	ExpressionNode* parse(const std::tstring& ipString);
};