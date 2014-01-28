#include "stdafx.h"
#include "ExpressionNode.h"

ExpressionNode::~ExpressionNode()
{
	//std::tcout << TEXT("Expression node destroyed") << std::endl;
}
ConstantExpressionNode::ConstantExpressionNode(const double& theValue) : value(theValue)
{

}

ConstantExpressionNode::ConstantExpressionNode(const std::tstring& theStrValue)
{
	std::size_t numIJ = std::count(theStrValue.begin(), theStrValue.end(), TEXT('i')) + std::count(theStrValue.begin(),theStrValue.end(),TEXT('j'));
	std::tstring formattedString = std::regex_replace(theStrValue, std::tRegexIterator::regex_type(TEXT("[ij]+")), TEXT(""));
	double numericVal = formattedString == TEXT("") ? 1 : _tstof(formattedString.c_str());

	switch(numIJ % 4)
	{
		case 0:
			this->value= std::complex<double>(numericVal,0);
			break;
		case 1:
			this->value= std::complex<double>(0,numericVal);
			break;
		case 2:
			this->value = std::complex<double>(-numericVal,0);
			break;
		default:
			this->value = std::complex<double>(0,-numericVal);
			break;
	}
}

std::complex<double> ConstantExpressionNode::getValue() const
{
	return this->value;
}

ExpressionNodeIdentifier ConstantExpressionNode::getType() const
{
	return ExpressionNodeIdentifier::CONSTANT_NODE;
}

VariableExpressionNode::VariableExpressionNode(const std::tstring& theName) : name(theName), valueSet(false)
{
}

ExpressionNodeIdentifier VariableExpressionNode::getType() const
{
	return ExpressionNodeIdentifier::VARIABLE_NODE;
}

void VariableExpressionNode::setValue(const std::complex<double>& theValue)
{
	this->value = theValue;
	this->valueSet = true;
}

std::complex<double> VariableExpressionNode::getValue() const
{
	if (this->valueSet)
		return this->value;
	else {
		std::tcout << TEXT("Variable ") << this->name << TEXT(" was not initialized.");
		return 0;
	}
}

Term::Term(const bool& theDescriptor, ExpressionNode* theExpression) : tDescriptor(theDescriptor), expression(theExpression)
{
}

Term::Term(Term&& otherTerm) : tDescriptor(otherTerm.tDescriptor), expression(std::move(otherTerm.expression))
{
}
SequenceExpressionNode::SequenceExpressionNode()
{
}

SequenceExpressionNode::SequenceExpressionNode(ExpressionNode* theNode, const bool& theDescriptor)
{
	this->terms.push_back(Term(theDescriptor, theNode));
}

void SequenceExpressionNode::add(ExpressionNode* theNode, const bool& theDescriptor)
{
	this->terms.push_back(Term(theDescriptor,theNode));
}

AdditionExpressionNode::AdditionExpressionNode() : SequenceExpressionNode()
{
}

AdditionExpressionNode::AdditionExpressionNode(ExpressionNode* theNode, const bool& theDescriptor) : SequenceExpressionNode(theNode, theDescriptor)
{
}

ExpressionNodeIdentifier AdditionExpressionNode::getType() const
{
	return ExpressionNodeIdentifier::ADDITION_NODE;
}

std::complex<double> AdditionExpressionNode::getValue() const
{
	std::complex<double> sum = 0.0;

	for (const Term& theTerm : this->terms) {
		if (theTerm.tDescriptor == true) { //positive
			sum += theTerm.expression->getValue();
		}
		else {
			sum -= theTerm.expression->getValue();
		}
	}
	return sum;
}

MultiplicationExpressionNode::MultiplicationExpressionNode() : SequenceExpressionNode()
{

}

MultiplicationExpressionNode::MultiplicationExpressionNode(ExpressionNode* theNode, const bool& theDescriptor) : SequenceExpressionNode(theNode, theDescriptor)
{
}

ExpressionNodeIdentifier MultiplicationExpressionNode::getType() const
{
	return ExpressionNodeIdentifier::MULTIPLICATION_NODE;
}

std::complex<double> MultiplicationExpressionNode::getValue() const
{
	std::complex<double> prod = 1.0;

	for (const Term& theTerm : this->terms) {
		if (theTerm.tDescriptor == true)//multiplication
			prod *= theTerm.expression->getValue();
		else
			prod /= theTerm.expression->getValue();
	}
	return prod;
}

/*
ExponentiationExpressionNode::ExponentiationExpressionNode(ExpressionNode* theBase, ExpressionNode* theExponent) : base(theBase), exponent(theExponent)
{

}

ExpressionNodeIdentifier ExponentiationExpressionNode::getType() const
{
	return ExpressionNodeIdentifier::EXPONENTIATION_NODE;
}

double ExponentiationExpressionNode::getValue() const
{
	return std::pow(this->base->getValue(), this->exponent->getValue());
}
*/

ExponentiationExpressionNode::ExponentiationExpressionNode() : SequenceExpressionNode()
{
}

ExponentiationExpressionNode::ExponentiationExpressionNode(ExpressionNode* baseNode) : SequenceExpressionNode(baseNode, true)
{
}

ExpressionNodeIdentifier ExponentiationExpressionNode::getType() const
{
	return ExpressionNodeIdentifier::EXPONENTIATION_NODE;
}

std::complex<double> ExponentiationExpressionNode::getValue() const
{
	//if there are less than two terms return 0 else return result of the lambda expression.
	return this->terms.size() < 2 ? 0 
		: [this]()->std::complex<double>
	{
			std::complex<double> base = this->terms.begin()->expression->getValue();

			std::for_each(this->terms.begin()+1, this->terms.end(), [&](const Term& leTerm)->void{
				base = std::pow(base, leTerm.expression->getValue());
			});
			return base;
	}();
}

PCExpressionNode::PCExpressionNode() : SequenceExpressionNode()
{
}

PCExpressionNode::PCExpressionNode(ExpressionNode* theNode, const bool& theDescriptor) : SequenceExpressionNode(theNode,theDescriptor)
{
}

ExpressionNodeIdentifier PCExpressionNode::getType() const
{
	return ExpressionNodeIdentifier::PERM_COMB_NODE;
}

std::complex<double> PCExpressionNode::getValue() const
{
	//
	return this->terms.size() < 2 ? 0
		: [this]()->std::complex<double>
	{
		double base = this->terms.begin()->expression->getValue().real();
		double result;

		for (auto itr = this->terms.begin()+1; itr != this->terms.end(); ++itr) {
			result = 1;
			if (itr->tDescriptor) { //permutation
				for (double idx = 1, multiplier = base; idx <= itr->expression->getValue().real(); ++idx,--multiplier) {
					result *= multiplier;
				}
			}
			else {
				for (double idx = 1, multiplier = base; idx <= itr->expression->getValue().real(); ++idx,--multiplier) {
					result *= multiplier/idx;
				}
			}
			base = result;
		}
		return base;
	}();
}
/*
FunctionExpressionNode::FunctionExpressionNode(const FunctionExpressionNodeIdentifier& theFunction, ExpressionNode* theArgument) : function(theFunction), argument(theArgument)
{

}

ExpressionNodeIdentifier FunctionExpressionNode::getType() const
{
	return ExpressionNodeIdentifier::FUNCTION_NODE;
}

double FunctionExpressionNode::getValue() const
{
	typedef FunctionExpressionNodeIdentifier fIdentifier;
	switch(this->function)
	{
		case fIdentifier::SIN:
			return std::sin(this->argument->getValue());
		case fIdentifier::COS:
			return std::cos(this->argument->getValue());
		case fIdentifier::TAN:
			return std::tan(this->argument->getValue());
		case fIdentifier::ASIN:
			return std::asin(this->argument->getValue());
		case fIdentifier::ATAN:
			return std::atan(this->argument->getValue());
		case fIdentifier::SQRT:
			return std::sqrt(this->argument->getValue());
		case fIdentifier::EXP:
			return std::exp(this->argument->getValue());
		case fIdentifier::LN:
			return std::log(this->argument->getValue());
		case fIdentifier::LOG:
			return std::log10(this->argument->getValue());
		case fIdentifier::LOG2:
			return std::log(this->argument->getValue()) / std::log(2);
		default:
		{
			std::tcout << TEXT("Invalid function id");
			return 0;
		}
	}
}

FunctionExpressionNodeIdentifier FunctionExpressionNode::stringToFunction(std::tstring str)
{
	typedef FunctionExpressionNodeIdentifier fIdentifier;
	if (str == TEXT("sin"))
		return fIdentifier::SIN;
	else if (str == TEXT("cos"))
		return fIdentifier::COS;
	else if (str == TEXT("tan"))
		return fIdentifier::TAN;
	else if (str == TEXT("asin"))
		return fIdentifier::ASIN;
	else if (str == TEXT("acos"))
		return fIdentifier::ACOS;
	else if (str == TEXT("atan"))
		return fIdentifier::ATAN;
	else if (str == TEXT("sqrt"))
		return fIdentifier::SQRT;
	else if (str == TEXT("exp"))
		return fIdentifier::EXP;
	else if (str == TEXT("ln"))
		return fIdentifier::LN;
	else if (str == TEXT("log"))
		return fIdentifier::LOG;
	else if (str == TEXT("log2"))
		return fIdentifier::LOG2;
	else {
		std::tcout << TEXT("Unexpected function ") << str << TEXT(" found");
		return fIdentifier::NONE;
	}
}

std::tstring FunctionExpressionNode::getAllFunctions()
{
	return TEXT("sin|cos|tan|asin|acos|atan|sqrt|exp|ln|log|log2");
}
*/

void FunctionExpressionNode::addFuncSequence(const std::tstring& tokenSequence)
{
	this->fIdentifierList.push_front(tokenSequence);
}

ExpressionNodeIdentifier FunctionExpressionNode::getType() const
{
	return ExpressionNodeIdentifier::FUNCTION_NODE;
}

std::complex<double> FunctionExpressionNode::getValue() const
{
	std::complex<double> result = this->argument->getValue();

	std::for_each(this->fIdentifierList.rbegin(), this->fIdentifierList.rend(), [&](const std::tstring& theIdentifier)->void {
		if (theIdentifier == TEXT("sin"))
			result = std::sin(result);
		else if (theIdentifier == TEXT("cos"))
			result = std::cos(result);
		else if (theIdentifier == TEXT("tan"))
			result = std::tan(result);
		else if (theIdentifier == TEXT("asin"))
			result = std::asin(result);
		else if (theIdentifier == TEXT("acos"))
			result = std::acos(result);
		else if (theIdentifier == TEXT("atan"))
			result = std::atan(result);
		else if (theIdentifier == TEXT("sqrt"))
			result = std::sqrt(result);
		else if (theIdentifier == TEXT("exp"))
			result = std::exp(result);
		else if (theIdentifier == TEXT("ln"))
			result = std::log(result);
		else if (theIdentifier == TEXT("log"))
			result = std::log10(result);
		else if (theIdentifier == TEXT("log2"))
			result = std::log(result)/std::log(2);
		else {
			std::tcout << TEXT("Unexpected function ") << theIdentifier << TEXT(" found");
		}
	});
	return result;
}

FactorialExpressionNode::FactorialExpressionNode(ExpressionNode* baseNode) : numRepeats(0), base(baseNode)
{
}

ExpressionNodeIdentifier FactorialExpressionNode::getType() const
{
	return ExpressionNodeIdentifier::FACTORIAL_NODE;
}

std::complex<double> FactorialExpressionNode::getValue() const
{
	auto factorial = [](const double& theValue)->double {
		double retVal = 1;
		for (int idx = 1; idx <= theValue; ++idx) {
			retVal *= idx;
		}
		return retVal;
	};

	double result = this->base->getValue().real();
	for (int idx=0; idx < this->numRepeats; ++idx) {
		result = factorial(result);
	}
	return result;
}