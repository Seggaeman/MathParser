#include "stdafx.h"
#include "Tokenizer.h"

Token::Token() : identifier(TokenIdentifier::EPSILON), sequence(TEXT(""))
{
}
Token::Token(const TokenIdentifier& theIdentifier, const std::tstring& theSequence)
	: identifier(theIdentifier), sequence(theSequence)
{

}

TokenInfo::TokenInfo(const std::tRegexIterator::regex_type& thePattern, const TokenIdentifier& theIdentifier)
	: pattern(thePattern), identifier(theIdentifier)
{
}

Tokenizer::Tokenizer()
{
	this->add(_TEXT("(\\+|-)"), TokenIdentifier::PLUSMINUS);
	this->add(_TEXT("(\\*|/)"), TokenIdentifier::MULTDIV);
	this->add(TEXT("\\^"), TokenIdentifier::RAISED);
	this->add(TEXT("(P|C)"), TokenIdentifier::PERMCOMB);
	this->add(_TEXT("(sin|cos|tan|asin|acos|atan|sqrt|exp|ln|log|log2)"), TokenIdentifier::FUNCTION);
	this->add(TEXT("\\("), TokenIdentifier::OPEN_BRACKET);
	this->add(TEXT("(\\+|-)\\("), TokenIdentifier::OPEN_BRACKET);
	this->add(_TEXT("\\)"), TokenIdentifier::CLOSE_BRACKET);
	this->add(_TEXT("(\\+|-)?([ij]+)?[0-9]+(\\.[0-9]+)?([ij]+)?([eE][\\+\\-]?[0-9]+)?"), TokenIdentifier::NUMBER);
	this->add(TEXT("(\\+|-)?[ij]+"), TokenIdentifier::NUMBER);
	this->add(_TEXT("[a-zA-Z][a-zA-Z0-9_]*"), TokenIdentifier::VARIABLE);
	this->add(TEXT("!+"), TokenIdentifier::FACTORIAL);
}

void Tokenizer::add(const std::tstring& regexString, const TokenIdentifier& theIdentifier)
{
	this->tokenInfos.push_back(TokenInfo(std::tRegexIterator::regex_type(_TEXT("^")+regexString), theIdentifier));
}

const std::deque<Token>& Tokenizer::getTokens()
{
	return this->tokens;
}

void Tokenizer::tokenize(std::tstring theString)
{
	//remove spaces, newlines and tabs from string.
	theString= std::regex_replace(theString, std::tRegexIterator::regex_type(TEXT("[\\n\\r\\s\\t]")), TEXT(""));
	//strip consecutive +-
	auto replacementFunc = [&](const std::tRegexIterator::regex_type& pattern, const std::tstring& negativeReplace, const std::tstring& positiveReplace)->void {
		std::map<std::size_t, std::tstring> toBeReplaced;
		int posAdjust = 0;
		for (std::tRegexIterator iter(theString.begin(), theString.end(), pattern),end; iter != end; ++iter)
		{
			toBeReplaced.insert(std::make_pair(iter->position(), iter->str()));
		}

		for (auto itr = toBeReplaced.begin(); itr != toBeReplaced.end(); ++itr)
		{
			if (std::count(itr->second.begin(), itr->second.end(), TEXT('-')) % 2)	//odd number of minuses implies negative.
			{
				theString.replace(itr->first-posAdjust, itr->second.length(), negativeReplace);
				posAdjust += itr->second.length()-negativeReplace.length();
			}
			else 
			{
				theString.replace(itr->first-posAdjust, itr->second.length(), positiveReplace);
				posAdjust += itr->second.length()-positiveReplace.length();
			}
		}
	};

	//replacementFunc(std::tRegexIterator::regex_type(TEXT("(?<=[0-9!])[\\+\\-]+")), TEXT("+-"), TEXT("+"));
	//replacementFunc(std::tRegexIterator::regex_type(TEXT("(?<=[eE\\*/\\^])[\\+\\-]+")), TEXT("-"),TEXT(""));
	//replacementFunc(std::tRegexIterator::regex_type(TEXT("(?<=log2)[\\+\\-]+")), TEXT("-"),TEXT(""));
	replacementFunc(std::tRegexIterator::regex_type(TEXT("[\\+\\-]+")), TEXT("-"),TEXT("+"));
	this->tokens.clear();

	while (theString != TEXT(""))
	{
		bool matchedToken = false;
		for (TokenInfo info : this->tokenInfos) 
		{
			std::match_results<std::tstring::const_iterator> lesResultats;
			matchedToken = std::regex_search(theString, lesResultats, info.pattern);
			if (matchedToken)
			{
				Token testToken(info.identifier, lesResultats.str(0));
				if (testToken.identifier == TokenIdentifier::PLUSMINUS)
				{
					//if this is the start of the string treat it as a number.
					if (this->tokens.size() == 0
						|| (this->tokens.size() > 0 && (this->tokens.end()-1)->identifier != TokenIdentifier::NUMBER) && (this->tokens.end()-1)->identifier != TokenIdentifier::FACTORIAL)
					{
						continue;
					}
				}
				this->tokens.push_back(testToken);
				theString.replace(lesResultats.position(), lesResultats.str().length(), TEXT(""));
				break;
			}
		}
		if (!matchedToken)
		{
			std::tcout << TEXT("Unexpected character in input: ") << theString;
			break;
		}
	}
}