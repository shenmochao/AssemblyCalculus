#ifndef PARSER_H
#define PARSER_H

#include "brain.h"
#include <map>
#include <set>
#include <vector>
#include <string>
#include <unordered_map>

namespace parser {

struct TreeNode {
    std::string word;
    std::vector<TreeNode*> children;
    TreeNode(const std::string& w) : word(w) {}
};

struct AreaRule {
    std::string action;
    std::string area;
    int index;
    AreaRule(const std::string& a, const std::string& ar, int i) : action(a), area(ar), index(i) {}
};

struct FiberRule {
    std::string action;
    std::string area1;
    std::string area2;
    int index;
    FiberRule(const std::string& a, const std::string& ar1, const std::string& ar2, int i) : action(a), area1(ar1), area2(ar2), index(i) {}
};

struct FiringRule {
    std::string action;
    FiringRule(const std::string& a) : action(a) {}
};

struct OtherRule {
    std::string action;
    OtherRule(const std::string& a) : action(a) {}
};

class ParserBrain : public nemo::Brain {
public:
    std::unordered_map<std::string, std::unordered_map<std::string, std::set<int>>> fiber_states;
    std::unordered_map<std::string, std::set<int>> area_states;
    std::unordered_map<std::string, std::set<std::string>> activated_fibers;
    std::unordered_map<std::string, std::map<std::string, std::vector<int>>> lexeme_dict;
    std::vector<std::string> all_areas;
    std::vector<std::string> recurrent_areas;
    std::vector<std::string> initial_areas;
    std::unordered_map<std::string, std::vector<std::string>> readout_rules;

    ParserBrain(float p);

    void initializeStates();
    void applyFiberRule(const FiberRule& rule);
    void applyAreaRule(const AreaRule& rule);
    void applyRule(const std::variant<FiberRule, AreaRule>& rule);
    void parseProject();
    void rememberFibers(const std::unordered_map<std::string, std::set<std::string>>& project_map);
    bool recurrent(const std::string& area);
    std::unordered_map<std::string, std::set<std::string>> getProjectMap();
    void activateWord(const std::string& area_name, const std::string& word);
    std::string getWord(const std::string& area_name, float min_overlap = 0.7);
    std::unordered_map<std::string, std::set<std::string>> getActivatedFibers();
};

class EnglishParserBrain : public ParserBrain {
public:
    EnglishParserBrain(float p, int non_LEX_n = 2000, int non_LEX_k = 100, int LEX_k = 20,
                       float default_beta = 0.2, float LEX_beta = 1.0, float recurrent_beta = 0.05, float interarea_beta = 0.5, bool verbose = false);

    std::unordered_map<std::string, std::set<std::string>> getProjectMap();
    std::string getWord(const std::string& area_name, float min_overlap = 0.7);
};

class ParserDebugger {
public:
    ParserBrain& b;
    std::vector<std::string> all_areas;
    std::vector<std::string> explicit_areas;

    ParserDebugger(ParserBrain& brain, const std::vector<std::string>& all_areas, const std::vector<std::string>& explicit_areas);

    void run();
    void peak();
};

void potentiateWordInLEX(ParserBrain& b, const std::string& word, int rounds = 20);

enum class ReadoutMethod {
    FIXED_MAP_READOUT,
    FIBER_READOUT,
    NATURAL_READOUT
};

void parse(const std::string& sentence = "people bite dogs", const std::string& language = "English", float p = 0.1, int LEX_k = 20,
           int project_rounds = 20, bool verbose = true, bool debug = false, ReadoutMethod readout_method = ReadoutMethod::FIBER_READOUT);

} // namespace parser

#endif // PARSER_H
