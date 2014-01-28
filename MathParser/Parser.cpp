#include "stdafx.h"
#include "Parser.h"

ExpressionNode* Parser::parse(const std::tstring& ipString)
{
	this->theTokenizer.tokenize(ipString);
	this->lookahead = *this->theTokenizer.tokens.begin();
	return this->expression();
}

void Parser::nextToken()
{
	this->theTokenizer.tokens.pop_front();
	
	if(this->theTokenizer.tokens.empty())
		lookahead = Token(TokenIdentifier::EPSILON, TEXT(""));
	else
	{
		this->lookahead = *this->theTokenizer.tokens.begin();
	}
}

/*
ExpressionNode* Parser::expression()
{
	//expression -> signedTerm sumOp
	ExpressionNode* expr = this->signedTerm();
	return this->sumOp(expr);
}

ExpressionNode* Parser::sumOp(ExpressionNode* expr)
{
	//sumOp -> PLUSMINUS term sumOp
	if(this->lookahead.identifier == TokenIdentifier::PLUSMINUS) {
		AdditionExpressionNode* sum;
		if(expr->getType() == ExpressionNodeIdentifier::ADDITION_NODE) {
			sum = reinterpret_cast<AdditionExpressionNode*>(expr);
		}
		else {
			sum = new AdditionExpressionNode(expr,true);//positive AdditionExpressionNode
		}
		bool isPositive = this->lookahead.sequence == TEXT("+");
		this->nextToken();
		ExpressionNode* theTerm= this->term();
		sum->add(theTerm, isPositive);
		return this->sumOp(sum);
	}
	else {
	//else sumOp->EPSILON
		return expr;
	}
}

ExpressionNode* Parser::signedTerm()
{
	//signed_term -> PLUSMINUS term
	if (this->lookahead.identifier == TokenIdentifier::PLUSMINUS) {
		bool isPositive = this->lookahead.sequence == TEXT("+");
		this->nextToken();
		//bool isPositive = this->stripPlusMinus();
		ExpressionNode* theTerm = this->term();
		if (isPositive) {
			return theTerm;
		}
		else {
			return new AdditionExpressionNode(theTerm,false); //negative AdditionExpressionNode
		}
	}
	else {
		//signed_term->term
		return this->term();
	}
}

ExpressionNode* Parser::term()
{
	//term -> factor termOp
	ExpressionNode* leFactor = this->factor();
	return this->termOp(leFactor);
}

ExpressionNode* Parser::termOp(ExpressionNode* theExpression)
{
	// term_op->MULTDIV factor termOp
	if (this->lookahead.identifier == TokenIdentifier::MULTDIV) {
		MultiplicationExpressionNode* prod;
		if (theExpression->getType() == ExpressionNodeIdentifier::MULTIPLICATION_NODE) {
			prod = reinterpret_cast<MultiplicationExpressionNode*>(theExpression);
		}
		else {
			prod = new MultiplicationExpressionNode(theExpression,true);
		}
		bool isMultiplicationSymbol = this->lookahead.sequence == TEXT("*");
		this->nextToken();
		
		ExpressionNode* leFactor = this->factor();
		prod->add(leFactor, isMultiplicationSymbol);
		return this->termOp(prod);
	}
	else {
		//term_op -> EPSILON
		return theExpression;
	}
}

ExpressionNode* Parser::factor()
{
	//factor -> FUNCTION argument
	if (this->lookahead.identifier == TokenIdentifier::FUNCTION) {
		FunctionExpressionNodeIdentifier theIdentifier = FunctionExpressionNode::stringToFunction(this->lookahead.sequence);
		this->nextToken();
		
		ExpressionNode* expr = this->argument();
		return new FunctionExpressionNode(theIdentifier,expr);
	}
	else {
		//factor -> argument factorOp
		ExpressionNode* arg = this->argument();
		return this->factorOp(arg);
	}
}

ExpressionNode* Parser::factorOp(ExpressionNode* theExpression)
{
	//factorOp -> RAISED factor
	if (this->lookahead.identifier == TokenIdentifier::RAISED) {
		this->nextToken();
		
		ExpressionNode* exponent = this->factor();
		return new ExponentiationExpressionNode(theExpression, exponent);
	}
	else {
	// factor_op->EPSILON
		return theExpression;
	}
}

ExpressionNode* Parser::argument()
{
	if (this->lookahead.identifier == TokenIdentifier::OPEN_BRACKET) {
		//argument -> OPEN_BRACKET sum CLOSE_BRACKET.
		bool isPositiveBracketedExpression = this->lookahead.sequence == TEXT("(") || this->lookahead.sequence == TEXT("+(");
		this->nextToken();
		ExpressionNode* expr;
		if (isPositiveBracketedExpression) {
			expr = this->expression();
		}
		else
			expr = new AdditionExpressionNode(this->expression(), false);

		if (this->lookahead.identifier != TokenIdentifier::CLOSE_BRACKET) {
			std::tcout << TEXT("Closing brackets expected and ") << this->lookahead.sequence << TEXT(" found instead");
		}
		this->nextToken();
		return expr;
	}
	else {
		//argument -> value
		return this->value();
	}
}

ExpressionNode* Parser::value()
{
	ExpressionNode* expr;
	//argument -> NUMBER
	if (this->lookahead.identifier == TokenIdentifier::NUMBER) {
		expr = new ConstantExpressionNode(this->lookahead.sequence);
		this->nextToken();
	}
	// argument -> VARIABLE
	else if (this->lookahead.identifier == TokenIdentifier::VARIABLE) {
		expr = new VariableExpressionNode(this->lookahead.sequence);
		this->nextToken();
	}
	else {
		std::tcout << TEXT("Unexpected sequence ") << this->lookahead.sequence << TEXT(" found");
		expr = new ConstantExpressionNode(0);
	}
	
	return expr;
}
*/
//new functions

ExpressionNode* Parser::expression()
{
	ExpressionNode* theMultComponent = this->multComponent();
	return this->sumOp(theMultComponent);
}

ExpressionNode* Parser::sumOp(ExpressionNode* theMultComponent)
{
	//sumOp -> PLUSMINUS term sumOp
	if(this->lookahead.identifier == TokenIdentifier::PLUSMINUS) {
		AdditionExpressionNode* sum;
		if(theMultComponent->getType() == ExpressionNodeIdentifier::ADDITION_NODE) {
			sum = reinterpret_cast<AdditionExpressionNode*>(theMultComponent);
		}
		else {
			sum = new AdditionExpressionNode(theMultComponent,true);//positive AdditionExpressionNode
		}
		bool isPositive = this->lookahead.sequence == TEXT("+");
		this->nextToken();
		ExpressionNode* nextMultComponent = this->multComponent();
		sum->add(nextMultComponent, isPositive);
		return this->sumOp(sum);
	}
	else {
	//else sumOp->EPSILON
		return theMultComponent;
	}
}

ExpressionNode* Parser::multComponent()
{
	ExpressionNode* theExpComponent = this->expComponent();
	return this->multOp(theExpComponent);
}

ExpressionNode* Parser::multOp(ExpressionNode* theExpComponent)
{
	if(this->lookahead.identifier == TokenIdentifier::MULTDIV) {
		MultiplicationExpressionNode* product;
		if(theExpComponent->getType() == ExpressionNodeIdentifier::MULTIPLICATION_NODE) {
			product = reinterpret_cast<MultiplicationExpressionNode*>(theExpComponent);
		}
		else {
			product = new MultiplicationExpressionNode(theExpComponent,true);//positive AdditionExpressionNode
		}
		bool isMultiplicationSymbol = this->lookahead.sequence == TEXT("*");
		this->nextToken();
		ExpressionNode* nextExpComponent = this->expComponent();
		product->add(nextExpComponent, isMultiplicationSymbol);
		return this->multOp(product);
	}
	else {
	//else sumOp->EPSILON
		return theExpComponent;
	}
}

ExpressionNode* Parser::expComponent()
{
	ExpressionNode* thePermCombComponent = this->permCombComponent();
	return this->expOp(thePermCombComponent);
}

ExpressionNode* Parser::expOp(ExpressionNode* thePermCombComponent)
{
	if(this->lookahead.identifier == TokenIdentifier::RAISED) {
		ExponentiationExpressionNode* expTerms;
		if(thePermCombComponent->getType() == ExpressionNodeIdentifier::EXPONENTIATION_NODE) {
			expTerms = reinterpret_cast<ExponentiationExpressionNode*>(thePermCombComponent);
		}
		else {
			expTerms = new ExponentiationExpressionNode(thePermCombComponent);
		}
		this->nextToken();
		ExpressionNode* nextPermCombComponent = this->permCombComponent();
		expTerms->add(nextPermCombComponent,true);
		return this->expOp(expTerms);
	}
	else {
		return thePermCombComponent;
	}
}

ExpressionNode* Parser::permCombComponent()
{
	ExpressionNode* theFuncComponent = this->funcComponent();
	return this->permCombOp(theFuncComponent);
}

ExpressionNode* Parser::permCombOp(ExpressionNode* theFuncComponent)
{
	if(this->lookahead.identifier == TokenIdentifier::PERMCOMB) {
		PCExpressionNode* pcSequence;
		if(theFuncComponent->getType() == ExpressionNodeIdentifier::PERM_COMB_NODE) {
			pcSequence = reinterpret_cast<PCExpressionNode*>(theFuncComponent);
		}
		else {
			pcSequence = new PCExpressionNode(theFuncComponent,true);//
		}
		bool isPermutationSymbol = this->lookahead.sequence == TEXT("P");
		this->nextToken();
		ExpressionNode* nextPermCombComponent = this->funcComponent();
		pcSequence->add(nextPermCombComponent, isPermutationSymbol);
		return this->permCombOp(pcSequence);
	}
	else {
		return theFuncComponent;
	}
}

ExpressionNode* Parser::funcComponent()
{
	if (this->lookahead.identifier == TokenIdentifier::FUNCTION)
	{
			FunctionExpressionNode* funcNode = new FunctionExpressionNode;
			while(this->lookahead.identifier == TokenIdentifier::FUNCTION) {
				funcNode->addFuncSequence(this->lookahead.sequence);
				this->nextToken();
			}
			funcNode->argument = std::unique_ptr<ExpressionNode>(this->valBracketComponent());
			return funcNode;
	}
	else
		return this->factComponent(this->valBracketComponent());
}

ExpressionNode* Parser::valBracketComponent()
{
	ExpressionNode* expr;
	if (this->lookahead.identifier == TokenIdentifier::OPEN_BRACKET) {
		//argument -> OPEN_BRACKET sum CLOSE_BRACKET.
		bool isPositiveBracketedExpression = this->lookahead.sequence == TEXT("(") || this->lookahead.sequence == TEXT("+(");
		this->nextToken();
		if (isPositiveBracketedExpression) {
			expr = this->expression();
		}
		else
			expr = new AdditionExpressionNode(this->expression(), false);

		if (this->lookahead.identifier != TokenIdentifier::CLOSE_BRACKET) {
			std::tcout << TEXT("Closing brackets expected and ") << this->lookahead.sequence << TEXT(" found instead");
		}
		this->nextToken();
	}
	else {
		//argument -> value
		expr= this->value();
	}
	return expr;
}

ExpressionNode* Parser::value()
{
	ExpressionNode* expr;
	//argument -> NUMBER
	if (this->lookahead.identifier == TokenIdentifier::NUMBER) {
		expr = new ConstantExpressionNode(this->lookahead.sequence);
		this->nextToken();
	}
	// argument -> VARIABLE
	else if (this->lookahead.identifier == TokenIdentifier::VARIABLE) {
		expr = new VariableExpressionNode(this->lookahead.sequence);
		this->nextToken();
	}
	else {
		std::tcout << TEXT("Unexpected sequence ") << this->lookahead.sequence << TEXT(" found");
		expr = new ConstantExpressionNode(0);
	}
	
	return expr;
}

ExpressionNode* Parser::factComponent(ExpressionNode* baseNode)
{
	if (this->lookahead.identifier == TokenIdentifier::FACTORIAL) {
		FactorialExpressionNode* factNode = new FactorialExpressionNode(baseNode);
		factNode->numRepeats = std::count(this->lookahead.sequence.begin(), this->lookahead.sequence.end(), TEXT('!'));
		this->nextToken();
		return factNode;
	}
	else {
		return baseNode;
	}
}