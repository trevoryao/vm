#include "regex.h"

#include <iostream>
#include <sstream>
#include <string>

#include "exp.h"

using namespace std;
using namespace Utils;

namespace {
inline bool IsUpperCase(char c) {
    return 'A' <= c && c <= 'Z';
}

Exp **CompilePattern(const string &pattern, bool ignoreCase) {
    Exp **compiled = new Exp*[pattern.size() + 1];

    int j = 0;

    Exp *last = nullptr;
    stringstream scanned;

    for (size_t i = 0; i < pattern.size(); ++i, ++j) {
        switch (pattern[i]) {
            case '|': {
                if (i - 1 < 0) {
                    cerr << "regex pattern not well formed." << endl;
                    return nullptr;
                }

                // keep running string of existing chars, then create
                // new exp with string of stuff, then new

                delete compiled[0];
                delete compiled[1];

                compiled[0] = new Exp{ExpType::two, new Exp*[2] {
                    new Exp{ExpType::str, 
                    scanned.str()}, 
                    new Exp{ExpType::str, 
                    pattern.substr(i + 1, pattern.size() - i - 1)}
                }};

                compiled[1] = new Exp{ExpType::null};

                return compiled;
            }
            case '.': compiled[j] = new Exp{ExpType::dot}; break;
            case '^': compiled[j] = new Exp{ExpType::start}; break;
            case '$': compiled[j] = new Exp{ExpType::end}; break;
            case '*': {
                if (i - 1 < 0) {
                    cerr << "regex pattern not well formed." << endl;
                    return nullptr;
                }

                compiled[--j] = new Exp{ExpType::star, new Exp*[1] {last}};
                break;
            }
            case '+': {
                if (i - 1 < 0) {
                    cerr << "regex pattern not well formed." << endl;
                    return nullptr;
                }

                compiled[--j] = new Exp{ExpType::plus, new Exp*[1] {last}};
                break;
            }
            case '?': {
                if (i - 1 < 0) {
                    cerr << "regex pattern not well formed." << endl;
                    return nullptr;
                }

                compiled[--j] = new Exp{ExpType::question, new Exp*[1] {last}};
                break;
            }
            case '\\': {
                if (++i == pattern.size()) {
                    cerr << "regex pattern not well formed." << endl;
                    return nullptr;
                }

                switch (pattern[i]) {
                    case '|': compiled[j] = new Exp{'|'}; break;
                    case '*': compiled[j] = new Exp{'*'}; break;
                    case '+': compiled[j] = new Exp{'+'}; break;
                    case '?': compiled[j] = new Exp{'?'}; break;
                    case '.': compiled[j] = new Exp{'.'}; break;
                    case '\\': compiled[j] = new Exp{'\\'}; break;
                    case '^': compiled[j] = new Exp{'^'}; break;
                    case '$': compiled[j] = new Exp{'$'}; break;
                    case '[': compiled[j] = new Exp{'['}; break;
                    case ']': compiled[j] = new Exp{'['}; break;
                    default: {
                        compiled[j] = new Exp{pattern[i]}; break;
                    }
                }
            }
            case '[': {
                cerr << "pattern=" << pattern << endl;
                stringstream rangeChars; // buffer for copying characters
                compiled[j] = new Exp{ExpType::range};
                if (pattern[i + 1] == '^') {
                    compiled[j]->type = ExpType::notRange;
                    ++i;
                }

                while (pattern[++i] != ']') {
                    if (i == pattern.size()) {
                        cerr << "regex pattern not well formed." << endl;
                        return nullptr;
                    }
                    if (pattern[i] == '\\') {
                        if (++i == pattern.size()) {
                            cerr << "regex pattern not well formed." << endl;
                            return nullptr;
                        }
                    }

                    rangeChars.put(ignoreCase && IsUpperCase(pattern[i]) ? 
                        pattern[i] - 'A' + 'a' : pattern[i]);
                }

                compiled[j]->strValues = rangeChars.str();
                cerr << "strValues=" << compiled[j]->strValues << endl;
                break;
            }
            default: {
                const char c_sen = ignoreCase && IsUpperCase(pattern[i]) ? 
                    pattern[i] - 'A' + 'a' : pattern[i];

                compiled[j] = new Exp{c_sen};
                scanned << c_sen;

                break;
            }
        }

        last = compiled[j];
    }
    
    compiled[j] = new Exp{ExpType::null};

    return compiled;
}

// should not be used for start or end anchor.
bool MatchOne(const Exp *exp, const char c, bool ignoreCase) {
    const char c_sen = ignoreCase && IsUpperCase(c) ? c - 'A' + 'a' : c; // case insensitive/sensitive char

    switch (exp->type) {
        case ExpType::one:
            return c_sen == exp->value;
        case ExpType::dot:
            return true;
        case ExpType::star:
        case ExpType::question:
        case ExpType::plus:
            return MatchOne(exp->values[0], c_sen, ignoreCase);
        case ExpType::range:
        case ExpType::notRange: {
            for (size_t i = 0; i < exp->strValues.size(); ++i) {
                if (c_sen == exp->strValues[i]) {
                    return exp->type == ExpType::range;
                }
            }

            return exp->type == ExpType::notRange;
        }
        case ExpType::null: 
            return false;
    }

    return false;
}

bool MatchMany(const Exp *exp, const char c, bool ignoreCase) {
    const char c_sen = ignoreCase && IsUpperCase(c) ? c - 'A' + 'a' : c; // case insensitive/sensitive char

    switch (exp->type) {
        case ExpType::one:
            return c_sen == exp->value;
        case ExpType::dot:
        case ExpType::question:
            return true;
        
        case ExpType::star:
        case ExpType::plus:
            return MatchOne(exp->values[0], c_sen, ignoreCase);
        case ExpType::range:
        case ExpType::notRange: {
            for (size_t i = 0; i < exp->strValues.size(); ++i) {
                if (c_sen == exp->strValues[i]) {
                    return exp->type == ExpType::range;
                }
            }

            return exp->type == ExpType::notRange;
        }
        case ExpType::null: 
            return false;
    }

    return false;
}
}

namespace Utils {
Regex::Regex(const bool ignoreCase, const string &regex): 
    pattern{CompilePattern(regex, ignoreCase)}, ignoreCase{ignoreCase} { }

Regex::~Regex() { 
    for (int i = 0; ; ++i) {
        ExpType type = pattern[i]->type;
        delete pattern[i];

        if (type == ExpType::null) break;
    }

    delete[] pattern;
}

// should be called after starting anchor is found
bool Regex::MatchPat(const string &str, bool skipFirst) {
    size_t i = 0; // index of string

    for (int j = skipFirst ? 1 : 0; pattern[j]->type != ExpType::null; ++j) {
        if (pattern[j]->type == ExpType::end) {
            return i == str.size();
        }

        if (i >= str.size()) {
            return pattern[j]->type == ExpType::star ||
                pattern[j]->type == ExpType::question;
        }

        const char c_sen = ignoreCase && IsUpperCase(str[i]) ? str[i] - 'A' + 'a' : str[i]; // case insensitive/sensitive char

        switch (pattern[j]->type) {
            case ExpType::one: {
                if (c_sen != pattern[j]->value) return false;
                ++i;
                break;
            }
            case ExpType::plus: {
                if (!MatchOne(pattern[j], str[i], ignoreCase)) {
                    return false;
                }
                ++i;
            } // fallthrough as will be same
            case ExpType::star: {
                // keep advancing matches until next pattern element matches
                do {
                    if (i + 1 < str.size() && !MatchOne(pattern[j + 1], str[i + 1], ignoreCase)) {
                        break;
                    }
                    if (!MatchMany(pattern[j], str[i], ignoreCase)) {
                        break;
                    }
                    ++i;
                } while (i < str.size());
                break;
            }
            case ExpType::question: {
                if (MatchOne(pattern[j], str[i], ignoreCase)) {
                    ++i;
                }
                break;
            }
            case ExpType::dot: ++i; break;
            case ExpType::range:
            case ExpType::notRange: {
                for (size_t k = 0; k < pattern[j]->strValues.size(); ++k) {
                    if (c_sen == pattern[j]->strValues[k]) {
                        return pattern[j]->type == ExpType::range;
                    }
                }

                return pattern[j]->type == ExpType::notRange;
            }
        }
    }

    return true;
}

bool Regex::Match(const string &str) {
    /* 
    iterate over string until we find a char matching the
    start of the pattern, then can match pattern
    each time matching fails, search from the last point of entry for new starting anchor
    If no starting anchor is found, can return false
    */

    if (pattern[0]->type == ExpType::start) {
        return MatchPat(str, true);
    }

    if (pattern[0]->type == ExpType::end) {
        return true;
    }

    if (pattern[0]->type == ExpType::two) {
        string &str1 = pattern[0]->values[0]->strValues;
        string &str2 = pattern[0]->values[1]->strValues;

        for (size_t i = 0; i < str.size(); ++i) {
            if (str[i] == str1[0] || str[i] == str2[0]) {
                bool match1 = str[i] == str1[0], 
                    match2 = str[i] == str2[0];
                
                for (size_t j = i, k = 0, l = 0; j < str.size() && (match1 || match2); ++j) {
                    if (match1 && k < str1.size()) {
                        if (str1[k] != str[j]) {
                            match1 = false;
                        } else {
                            ++k;
                        }
                    }
                    if (match2 && l < str2.size()) {
                        if (str2[l] != str[j]) {
                            match2 = false;
                        } else {
                            ++l;
                        }
                    }

                    if (k == str1.size() || l == str2.size()) {
                        return true;
                    }
                }
            }
        }

        return false;
    }

    // all special cases finished
    for (size_t i = 0; i < str.size(); ++i) {
        if (pattern[0]->type == ExpType::question ||
            pattern[0]->type == ExpType::star) {
            // if matching element is there, will return true, otherwise false
            if (MatchPat(str.substr(i, str.size() - i), 
                MatchOne(pattern[0], str[i], ignoreCase))) {
                return true;
            }
        }
        

        if (MatchOne(pattern[0], str[i], ignoreCase)) {
            if (MatchPat(str.substr(i, str.size() - i))) {
                return true;
            }
        }
    }

    return false;
}
}
