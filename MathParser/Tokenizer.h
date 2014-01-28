#pragma once

#include "stdafx.h"

enum class TokenIdentifier {
	PLUSMINUS, MULTDIV, RAISED, PERMCOMB, FUNCTION, OPEN_BRACKET, CLOSE_BRACKET, NUMBER, VARIABLE, FACTORIAL, EPSILON
};

class Token {
public:
	TokenIdentifier identifier;
	std::tstring sequence;

	Token(const TokenIdentifier& theIdentifier, const std::tstring& theSequence);
	Token();
};
class TokenInfo {
public:
	std::tRegexIterator::regex_type pattern;
	TokenIdentifier identifier;

	TokenInfo(const std::tRegexIterator::regex_type& thePattern, const TokenIdentifier& theIdentifier);
};

class Tokenizer {
private:
	std::deque<TokenInfo> tokenInfos;
	std::deque<Token> tokens;
	void add(const std::tstring& regexString, const TokenIdentifier& theIdentifier);
public:
	Tokenizer();
	void tokenize(std::tstring theString); //make a copy
	const std::deque<Token>& getTokens();

friend class Parser;
};