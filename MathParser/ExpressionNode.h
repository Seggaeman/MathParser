#pragma once
#include "stdafx.h"

enum class ExpressionNodeIdentifier {
	VARIABLE_NODE, CONSTANT_NODE, ADDITION_NODE, MULTIPLICATION_NODE, EXPONENTIATION_NODE, PERM_COMB_NODE, FUNCTION_NODE, FACTORIAL_NODE
};

/*enum class FunctionExpressionNodeIdentifier {
	SIN, COS, TAN, ASIN, ACOS, ATAN, SQRT, EXP, LN, LOG, LOG2, NONE
};*/

class ExpressionNode {
public:
	virtual ExpressionNodeIdentifier getType() const = 0;
	virtual std::complex<double> getValue() const = 0;
	virtual ~ExpressionNode();
};

class ConstantExpressionNode : public ExpressionNode {
private:
	std::complex<double> value;

public:
	ConstantExpressionNode(const double& theValue);
	ConstantExpressionNode(const std::tstring& theStrValue);
	virtual std::complex<double> getValue() const override;
	virtual ExpressionNodeIdentifier getType() const override;
};

class VariableExpressionNode : public ExpressionNode {
private:
	std::tstring name;
	std::complex<double> value;
	bool valueSet;

public:
	VariableExpressionNode(const std::tstring& theName);
	virtual ExpressionNodeIdentifier getType() const override;
	virtual std::complex<double> getValue() const override;
	void setValue(const std::complex<double>& theValue);
};

class Term {
public:
	bool tDescriptor;	//positive = multiplication = true; negative = division = false;
	std::unique_ptr<ExpressionNode> expression;

public:
	Term(const bool& theDescriptor, ExpressionNode* theExpression);
	Term(Term&& otherTerm);
};

class SequenceExpressionNode : public ExpressionNode {
protected:
	std::vector<Term> terms;

public:
	SequenceExpressionNode();
	SequenceExpressionNode(ExpressionNode* theNode, const bool& theDescriptor);
	void add(ExpressionNode* theNode, const bool& theDescriptor);
};

class AdditionExpressionNode : public SequenceExpressionNode {
public:	
	AdditionExpressionNode();
	AdditionExpressionNode(ExpressionNode* theNode, const bool& theDescriptor);
	virtual ExpressionNodeIdentifier getType() const override;
	virtual std::complex<double> getValue() const override;
};

class MultiplicationExpressionNode : public SequenceExpressionNode {
public:
	MultiplicationExpressionNode();
	MultiplicationExpressionNode(ExpressionNode* theNode, const bool& theDescriptor);
	virtual ExpressionNodeIdentifier getType() const override;
	virtual std::complex<double> getValue() const override;
};

/*
class ExponentiationExpressionNode : public ExpressionNode {
private:
	std::unique_ptr<ExpressionNode> base, exponent;

public:
	ExponentiationExpressionNode(ExpressionNode* theBase, ExpressionNode* theExponent);
	virtual ExpressionNodeIdentifier getType() const override;
	virtual std::complex<double> getValue() const override;
};

class FunctionExpressionNode : public ExpressionNode {
private:
	FunctionExpressionNodeIdentifier function;
	std::unique_ptr<ExpressionNode> argument;

public:
	FunctionExpressionNode(const FunctionExpressionNodeIdentifier& theFunction, ExpressionNode* theArgument);
	ExpressionNodeIdentifier getType() const override;
	std::complex<double> getValue() const override;
	static FunctionExpressionNodeIdentifier stringToFunction(std::tstring str);
	static std::tstring getAllFunctions();
};
*/

class ExponentiationExpressionNode : public SequenceExpressionNode {
public:
	ExponentiationExpressionNode();
	ExponentiationExpressionNode(ExpressionNode* baseNode);
	virtual ExpressionNodeIdentifier getType() const override;
	virtual std::complex<double> getValue() const override;
};

class PCExpressionNode : public SequenceExpressionNode {
public:
	PCExpressionNode();
	PCExpressionNode(ExpressionNode* theNode, const bool& theDescriptor);
	virtual ExpressionNodeIdentifier getType() const override;
	virtual std::complex<double> getValue() const override;
};

class FunctionExpressionNode : public ExpressionNode {
private:
	std::deque<std::tstring> fIdentifierList;

public:
	std::unique_ptr<ExpressionNode> argument;	//the function argument as in cos(arg)
	void addFuncSequence(const std::tstring& tokenSequence);
	virtual ExpressionNodeIdentifier getType() const override;
	virtual std::complex<double> getValue() const override;
};

class FactorialExpressionNode : public ExpressionNode {
public:
	std::unique_ptr<ExpressionNode> base;
	int numRepeats;
	FactorialExpressionNode(ExpressionNode* baseNode);
	virtual ExpressionNodeIdentifier getType() const override;
	virtual std::complex<double> getValue() const override;
};