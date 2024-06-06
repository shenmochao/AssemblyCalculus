
#ifndef PARSER_BRAIN_H_
#define PARSER_BRAIN_H_

#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <memory>
#include <sstream>
#include <functional> 
#include "brain.cc"
#include "brain.h"
namespace nemo
{

// 脑区
const std::string LEX = "LEX";
const std::string DET = "DET";
const std::string SUBJ = "SUBJ";
const std::string OBJ = "OBJ";
const std::string VERB = "VERB";
const std::string PREP = "PREP";
const std::string PREP_P = "PREP_P";
const std::string ADJ = "ADJ";
const std::string ADVERB = "ADVERB";

// 确定 LEX 区域的基础大小，进一步计算确定这个区域的总神经元数量
const int LEX_SIZE = 20;

// 行为
const std::string DISINHIBIT = "DISINHIBIT";
const std::string INHIBIT = "INHIBIT";
const std::string ACTIVATE_ONLY = "ACTIVATE_ONLY";
const std::string CLEAR_DET = "CLEAR_DET";

// 区域划分
const std::vector<std::string> AREAS = {LEX, DET, SUBJ, OBJ, VERB, ADJ, ADVERB, PREP, PREP_P};
const std::vector<std::string> EXPLICIT_AREAS = {LEX};
const std::vector<std::string> RECURRENT_AREAS = {SUBJ, OBJ, VERB, ADJ, ADVERB, PREP, PREP_P};
const std::vector<std::string> INIT = {LEX, SUBJ, VERB};
// 数据结构
struct AreaRule {
    std::string action;
    std::string area;
    int index;
    AreaRule(std::string a, std::string b, int i) 
    : action(a), area(b), index(i) {}
    AreaRule(const AreaRule& areaR): action(areaR.action), area(areaR.area), index(areaR.index) {}
    AreaRule() = default;
};

struct FiberRule {
    std::string action;
    std::string area1;
    std::string area2;
    int index;

    FiberRule(std::string a, std::string a1, std::string a2, int i) 
        : action(a), area1(a1), area2(a2), index(i) {}
    FiberRule(const FiberRule& fiberR): action(fiberR.action), area1(fiberR.area1), area2(fiberR.area2), index(fiberR.index) {}
    FiberRule() = default;
};

struct FiringRule {
    std::string action;
};

struct OtherRule {
    std::string action;
};


// 封装的规则组
struct RuleGroup {
    std::vector<AreaRule> areaRules; // 存储对单一区域的操作
    std::vector<FiberRule> fiberRules; // 存储对区域间的操作
    
    RuleGroup(std::vector<AreaRule> areas, std::vector<FiberRule> fibers) 
        : areaRules(areas), fiberRules(fibers) {}
    RuleGroup(const RuleGroup& rulegroup) 
        : areaRules(rulegroup.areaRules), fiberRules(rulegroup.fiberRules) {}
    RuleGroup() = default;
};

// 封装好的规则类
struct RuleSet {
    uint32_t index; // 索引 & uint32_t 就是无符号int
    std::map<std::string, RuleGroup> ruleGroups; // 只会有"PRE_RULES"和"POST_RULES"
    
    RuleSet(uint32_t i, RuleGroup pre_rules, RuleGroup post_rules) {
        // emplace() 直接构造一个新的RuleGroup对象, 而不是调用默认构造函数
        index = i;
        ruleGroups.emplace("PRE_RULES", pre_rules);
        ruleGroups.emplace("POST_RULES", post_rules);
    }
    RuleSet() = default;
};


// 创建通用名词的函数
RuleSet generic_noun(uint32_t index) {
    std::vector<AreaRule> preareaRules{

    };
    std::vector<FiberRule> prefiberRules{
        {"DISINHIBIT", LEX, SUBJ, 0}, {"DISINHIBIT", LEX, OBJ, 0}, {"DISINHIBIT", LEX, PREP_P, 0},
        {"DISINHIBIT", DET, SUBJ, 0}, {"DISINHIBIT", DET, OBJ, 0}, {"DISINHIBIT", DET, PREP_P, 0},
        {"DISINHIBIT", ADJ, SUBJ, 0}, {"DISINHIBIT", ADJ, OBJ, 0}, {"DISINHIBIT", ADJ, PREP_P, 0},
        {"DISINHIBIT", VERB, OBJ, 0}, {"DISINHIBIT", PREP_P, PREP, 0}, {"DISINHIBIT", PREP_P, SUBJ, 0},
        {"DISINHIBIT", PREP_P, OBJ, 0}
    };
    std::vector<AreaRule> postareaRules{
        {"INHIBIT", DET, 0}, {"INHIBIT", ADJ, 0}, {"INHIBIT", PREP_P, 0}, {"INHIBIT", PREP, 0}
    };
    std::vector<FiberRule> postfiberRules{
        {"INHIBIT", LEX, SUBJ, 0}, {"INHIBIT", LEX, OBJ, 0}, {"INHIBIT", LEX, PREP_P, 0},
        {"INHIBIT", ADJ, SUBJ, 0}, {"INHIBIT", ADJ, OBJ, 0}, {"INHIBIT", ADJ, PREP_P, 0},
        {"INHIBIT", DET, SUBJ, 0}, {"INHIBIT", DET, OBJ, 0}, {"INHIBIT", DET, PREP_P, 0},
        {"INHIBIT", VERB, OBJ, 0}, {"INHIBIT", PREP_P, PREP, 0}, {"INHIBIT", PREP_P, VERB, 0},
        {"DISINHIBIT", LEX, SUBJ, 1}, {"DISINHIBIT", LEX, OBJ, 1}, {"DISINHIBIT", DET, SUBJ, 1},
        {"DISINHIBIT", DET, OBJ, 1}, {"DISINHIBIT", ADJ, SUBJ, 1}, {"DISINHIBIT", ADJ, OBJ, 1},
        {"INHIBIT", PREP_P, SUBJ, 0}, {"INHIBIT", PREP_P, OBJ, 0}, {"INHIBIT", VERB, ADJ, 0}
    };
    return RuleSet(index, RuleGroup(preareaRules, prefiberRules), RuleGroup(postareaRules, postfiberRules));
}

// 创建通用及物动词的规则集
RuleSet generic_trans_verb(uint32_t index) {
    std::vector<AreaRule> preareaRules{
        {"DISINHIBIT", ADVERB, 1}
    };
    std::vector<FiberRule> prefiberRules{
        {"DISINHIBIT", LEX, VERB, 0}, {"DISINHIBIT", VERB, SUBJ, 0}, {"DISINHIBIT", VERB, ADVERB, 0}
    };
    std::vector<AreaRule> postareaRules{
        {"DISINHIBIT", OBJ, 0}, {"INHIBIT", SUBJ, 0}, {"INHIBIT", ADVERB, 0}
    };
    std::vector<FiberRule> postfiberRules{
        {"INHIBIT", LEX, VERB, 0}, {"DISINHIBIT", PREP_P, VERB, 0}
    };
    return RuleSet(index, RuleGroup(preareaRules, prefiberRules), RuleGroup(postareaRules, postfiberRules));
}


// 创建通用不及物动词的规则集
RuleSet generic_intrans_verb(uint32_t index) {
    std::vector<AreaRule> preareaRules{
        {"DISINHIBIT", ADVERB, 1}
    };
    std::vector<FiberRule> prefiberRules{
        {"DISINHIBIT", LEX, VERB, 0}, {"DISINHIBIT", VERB, SUBJ, 0}, {"DISINHIBIT", VERB, ADVERB, 0}
    };
    std::vector<AreaRule> postareaRules{
        {"INHIBIT", SUBJ, 0}, {"INHIBIT", ADVERB, 0}
    };
    std::vector<FiberRule> postfiberRules{
        {"INHIBIT", LEX, VERB, 0}, {"DISINHIBIT", PREP_P, VERB, 0}
    };
    return RuleSet(index, RuleGroup(preareaRules, prefiberRules), RuleGroup(postareaRules, postfiberRules));
}

// 创建通用系动词的规则集
RuleSet generic_copula(uint32_t index) {
    std::vector<AreaRule> preareaRules{

    };
    std::vector<FiberRule> prefiberRules{
        {"DISINHIBIT", VERB, SUBJ, 0}, {"DISINHIBIT", LEX, VERB, 0}
    };
    std::vector<AreaRule> postareaRules{
        {"DISINHIBIT", OBJ, 0}, {"INHIBIT", SUBJ, 0}
    };
    std::vector<FiberRule> postfiberRules{
        {"INHIBIT", LEX, VERB, 0}, {"DISINHIBIT", ADJ, VERB, 0}
    };
    return RuleSet(index, RuleGroup(preareaRules, prefiberRules), RuleGroup(postareaRules, postfiberRules));
}

// 创建通用副词的规则集
RuleSet generic_adverb(uint32_t index) {
    std::vector<AreaRule> preareaRules{
        {"DISINHIBIT", ADVERB, 0}, 
    };
    std::vector<FiberRule> prefiberRules{
        {"DISINHIBIT", LEX, ADVERB, 0}
    };
    std::vector<AreaRule> postareaRules{
        {"INHIBIT", ADVERB, 1}
    };
    std::vector<FiberRule> postfiberRules{
        {"INHIBIT", LEX, ADVERB, 0}
    };
    return RuleSet(index, RuleGroup(preareaRules, prefiberRules), RuleGroup(postareaRules, postfiberRules));
}

// 创建通用限定词的规则集
RuleSet generic_determinant(uint32_t index) {
    std::vector<AreaRule> preareaRules{
        {"DISINHIBIT", DET, 0}
    };
    std::vector<FiberRule> prefiberRules{
        {"DISINHIBIT", LEX, DET, 0}
    };
    std::vector<AreaRule> postareaRules{
        
    };
    std::vector<FiberRule> postfiberRules{
        {"INHIBIT", LEX, DET, 0}, {"INHIBIT", VERB, ADJ, 0}
    };
    return RuleSet(index, RuleGroup(preareaRules, prefiberRules), RuleGroup(postareaRules, postfiberRules));
}

// 创建通用形容词的规则集
RuleSet generic_adjective(uint32_t index) {
    std::vector<AreaRule> preareaRules{
        {"DISINHIBIT", ADJ, 0}
    };
    std::vector<FiberRule> prefiberRules{
        {"DISINHIBIT", LEX, ADJ, 0}
    };
    std::vector<AreaRule> postareaRules{
        
    };
    std::vector<FiberRule> postfiberRules{
        {"INHIBIT", LEX, ADJ, 0}, {"INHIBIT", VERB, ADJ, 0}
    };
    return RuleSet(index, RuleGroup(preareaRules, prefiberRules), RuleGroup(postareaRules, postfiberRules));
}

// 创建通用介词的规则集
RuleSet generic_preposition(uint32_t index) {
    std::vector<AreaRule> preareaRules{
        {"DISINHIBIT", PREP, 0}
    };
    std::vector<FiberRule> prefiberRules{
        {"DISINHIBIT", LEX, PREP, 0}
    };
    std::vector<AreaRule> postareaRules{
        {"DISINHIBIT", PREP_P, 0}
    };
    std::vector<FiberRule> postfiberRules{
        {"INHIBIT", LEX, PREP, 0}, {"INHIBIT", LEX, SUBJ, 1},
        {"INHIBIT", LEX, OBJ, 1}, {"INHIBIT", DET, SUBJ, 1},
        {"INHIBIT", DET, OBJ, 1}, {"INHIBIT", ADJ, SUBJ, 1},
        {"INHIBIT", ADJ, OBJ, 1}
    };
    return RuleSet(index, RuleGroup(preareaRules, prefiberRules), RuleGroup(postareaRules, postfiberRules));
}

std::map<std::string, RuleSet> LEXEME_DICT = {
    {"the", generic_determinant(0)},
    {"a", generic_determinant(1)},
    {"dogs", generic_noun(2)},
    {"cats", generic_noun(3)},
    {"mice", generic_noun(4)},
    {"people", generic_noun(5)},
    {"chase", generic_trans_verb(6)},
    {"love", generic_trans_verb(7)},
    {"bite", generic_trans_verb(8)},
    {"of", generic_preposition(9)},
    {"big", generic_adjective(10)},
    {"bad", generic_adjective(11)},
    {"run", generic_intrans_verb(12)},
    {"fly", generic_intrans_verb(13)},
    {"quickly", generic_adverb(14)},
    {"in", generic_preposition(15)},
    {"are", generic_copula(16)},
    {"man", generic_noun(17)},
    {"woman", generic_noun(18)},
    {"saw", generic_trans_verb(19)}
};

// 应用：readout_rules_.at(from_area).find(to_area) != readout_rules_.at(from_area).end()
std::map<std::string, std::set<std::string>> ENGLISH_READOUT_RULES = {
    {"VERB", {"LEX", "SUBJ", "OBJ", "PREP_P", "ADVERB", "ADJ"}},
    {"SUBJ", {"LEX", "DET", "ADJ", "PREP_P"}},
    {"OBJ", {"LEX", "DET", "ADJ", "PREP_P"}},
    {"PREP_P", {"LEX", "PREP", "ADJ", "DET"}},
    {"PREP", {"LEX"}},
    {"ADJ", {"LEX"}},
    {"DET", {"LEX"}},
    {"ADVERB", {"LEX"}},
    {"LEX", {}},
};

typedef std::map<std::string, std::set<std::string>> ProjectMap;

class ParserBrain : public Brain
{

public:
    std::map<std::string, RuleSet> lexeme_dict_; // 词素字典，存储每个词素对应的规则集
    std::vector<std::string> all_areas_; // 所有区域的名称列表
    std::vector<std::string> recurrent_areas_; // 循环区域的名称列表
    std::vector<std::string> initial_areas_; // 初始区域的名称列表
    std::map<std::string, std::set<std::string>> readout_rules_; // 读取规则
    std::map<std::string, std::set<std::string>> activated_fibers_; // 激活的纤维，存储每个区域对应的激活纤维集合
    std::map<std::string, std::map<std::string, std::set<int>>> fiber_states_; // 纤维状态，存储每个区域中的纤维状态
    std::map<std::string, std::set<int>> area_states_; // 区域状态，存储每个区域的状态集合
    
    ParserBrain(float p, float beta, float max_weight, uint32_t seed,
                std::map<std::string, RuleSet> lexeme_dict = {},
                std::vector<std::string> all_areas = {},
                std::vector<std::string> recurrent_areas = {},
                std::vector<std::string> initial_areas = {},
                std::map<std::string, std::set<std::string>> readout_rules = {})
        : Brain(p, beta, max_weight, seed), lexeme_dict_(lexeme_dict), all_areas_(all_areas),
          recurrent_areas_(recurrent_areas), initial_areas_(initial_areas), readout_rules_(readout_rules)
    {
        initialize_states();
    }
    // 打印每个区域中的纤维状态
    void printFiberStates(const std::map<std::string, std::map<std::string, std::set<int>>>& fiber_states) {
        std::cout << "area_states_:" << std::endl;
        for (const auto& outer_pair : fiber_states) {
            const std::string& outer_key = outer_pair.first;
            const std::map<std::string, std::set<int>>& inner_map = outer_pair.second;
            
            std::cout << outer_key << ":\n";
            
            for (const auto& inner_pair : inner_map) {
                const std::string& inner_key = inner_pair.first;
                const std::set<int>& inner_set = inner_pair.second;
                
                std::cout << "  " << inner_key << ": {";
                for (auto it = inner_set.begin(); it != inner_set.end(); ++it) {
                    std::cout << *it;
                    if (std::next(it) != inner_set.end()) {
                        std::cout << ", ";
                    }
                }
                std::cout << "}\n";
            }
        }
    }
    // 打印每个区域的状态集合
    void printAreaStates() const {
        std::cout << "area_states_:" << std::endl;
        for (const auto& area : area_states_) {
            std::cout << area.first << ": {";
            for (auto it = area.second.begin(); it != area.second.end(); ++it) {
                std::cout << *it;
                if (std::next(it) != area.second.end()) {
                    std::cout << ", ";
                }
            }
            std::cout << "}" << std::endl;
        }
    }
    void printActivatedFibers(std::map<std::string, std::set<std::string>> activated_fibers_) const
    {
        std::cout << "activated fibers:" << std::endl;
        if(activated_fibers_.size() == 0) {
            std::cout << "activated fibers is empty!" << std::endl;
            return;
        }
        for (const auto& [from_area, to_areas] : activated_fibers_)
        {
            std::cout << "Area: " << from_area << "\nConnected Areas:";
            for (const auto& to_area : to_areas)
            {
                std::cout << " " << to_area;
            }
            std::cout << "\n\n";
        }
    }
    // 初始化状态
    void initialize_states()
    {
        std::cout << "init" << std::endl;
        for (const auto &from_area : all_areas_)
        {
            fiber_states_[from_area] = {};
            for (const auto &to_area : all_areas_)
            {
                fiber_states_[from_area][to_area].insert(0);
            }
        }

        for (const auto &area : all_areas_)
        {
            area_states_[area].insert(0);
        }

        for (const auto &area : initial_areas_)
        {
            area_states_[area].erase(0);
        }
    }
    // 在两个区域之间应用抑制或解除抑制
    void applyFiberRule(const std::string &from, const std::string &to, const std::string &action, int index)
    {
        if (action == "INHIBIT")
        {
            fiber_states_[from][to].insert(index);
            fiber_states_[to][from].insert(index);
            AddFiber(from, to, true);
            InhibitFiber(from, to);
            InhibitFiber(to, from);
        }
        else if (action == "DISINHIBIT")
        {
            fiber_states_[from][to].erase(index);
            fiber_states_[to][from].erase(index);
            AddFiber(from, to, true);
            ActivateFiber(from, to);
            ActivateFiber(to, from);
        }
    }
    // 在某个区域上应用抑制或解除抑制
    void applyAreaRule(const std::string &area, const std::string &action, int index)
    {
        if (action == "INHIBIT")
        {
            area_states_[area].insert(index);
        }
        else if (action == "DISINHIBIT")
        {
            area_states_[area].erase(index);
            ActivateArea(area, area_by_name_[area]);
        }
    }
    // 根据规则类型决定调用 区块间 还是 单个区块 的应用方法。
    void applyRule(const std::string &type, const std::string &from, const std::string &to, const std::string &action, int index)
    {
        if (type == "FiberRule")
        {
            applyFiberRule(from, to, action, index);
        }
        else if (type == "AreaRule")
        {
            applyAreaRule(from, action, index);
        }
    }
    // 解析投射
    // parser.parseProject();
    void parseProject()
    {
        auto project_map = getProjectMap();
        rememberFibers(project_map);
        Project(project_map, 1, true);
    }
    // 记忆已经激活的纤维束
    void rememberFibers(const ProjectMap &project_map)
    {
        // 遍历投射图中的每一个区域，对于每一个区域，将其与其连接的目标区域存储
        // from_area:起点为字符串(区域名称) to_areas:与其相连的所有终点，存有字符串的集合
        for (const auto &[from_area, to_areas] : project_map)
        {
            activated_fibers_[from_area].insert(to_areas.begin(), to_areas.end());
        }
    }
    // 检查给定的区域是否是循环的
    bool isRecurrent(const std::string &area) const
    {
        // 在 recurrent_areas_ 中查找给定的区域名称
        return std::find(recurrent_areas_.begin(), recurrent_areas_.end(), area) != recurrent_areas_.end();
    }
    // 构建一个投射图，包含了在没有被抑制的状态下，处于激活状态的区域之间的连接关系
    ProjectMap getProjectMap() {
        ProjectMap proj_map;
        for (const auto& area1 : all_areas_) {
            if (area_states_.at(area1).empty()) {
                for (const auto& area2 : all_areas_) {
                    if (area1 == "LEX" && area2 == "LEX")
                        continue;
                    if (area_states_.at(area2).empty()) {
                        if (fiber_states_.at(area1).at(area2).empty()) {
                            if (!areas_.at(area_by_name_.at(area1)).activated.empty()) {
                                proj_map[area1].insert(area2);
                            }
                            if (!areas_.at(area_by_name_.at(area2)).activated.empty()) {
                                proj_map[area2].insert(area1);
                            }
                        }
                    }
                }
            }
        }
        return proj_map;
    }
    // 激活特定区域中的特定单词
    void activateWord(const std::string& area_name, const std::string& word) {
        // auto& area = areas_.at(area_by_name_.at(area_name));
        auto& area = GetArea(area_name);
        uint32_t k = area.k;
        uint32_t lexeme_index = lexeme_dict_[word].index;
        uint32_t assembly_start = lexeme_index * k;
        std::vector<int> temp(k);
        std::iota(temp.begin(), temp.end(), assembly_start);
        area.activated.assign(temp.begin(), temp.end());
        area.is_fixed = true;
        //std::cout << "Exiting activateWord function..." << std::endl;
    }
    // 将区域中的组件解释为字符串
    std::string interpretAssemblyAsString(const std::string &area_name) const
    {
        return getWord(area_name, 0.7);
    }
    /*
      当LEX中的一个组件x_w处于活跃状态时，返回对应的单词w。
      x_w是什么？可以理解为w是对应的动词，x_w是w这个单词对应的神经元的集合
    */
    std::string getWord(const std::string &area_name, float min_overlap = 0.7) const
    {
        // 获取与指定区域名称关联的区域对象
        const auto &area = areas_.at(area_by_name_.at(area_name));
        // 将区域中激活的神经元ID存入集合 winners
        std::set<uint32_t> winners(area.activated.begin(), area.activated.end());
        // 获取区域中的激活的神经元数量 k
        uint32_t area_k = area.k;
        // 计算重叠阈值
        float threshold = min_overlap * area_k;

        for (const auto &[word, lexeme_] : lexeme_dict_)
        {
            // 计算词素在区域中的起始索引
            uint32_t word_assembly_start = lexeme_.index * area_k; // 在这里获取索引，算法实现不会被影响
            // 构建该词素对应的神经元集合
            std::set<uint32_t> word_assembly;
            for (uint32_t i = word_assembly_start; i < word_assembly_start + area_k; ++i)
            {
                word_assembly.insert(i);
            }
            // 计算 winners 与 word_assembly 的交集
            std::set<uint32_t> intersection;
            std::set_intersection(winners.begin(), winners.end(), word_assembly.begin(), word_assembly.end(),
                                std::inserter(intersection, intersection.begin()));
            // 如果交集大小大于等于阈值，则返回对应的单词
            if (intersection.size() >= threshold)
            {
                return word;
            }
        }
        // 如果没有找到符合条件的单词，返回空字符串
        return "";
    }
    // 获取已激活的纤维束
    std::map<std::string, std::set<std::string>> getActivatedFibers() const
    {
        std::map<std::string, std::set<std::string>> pruned_activated_fibers;
        for (const auto &[from_area, to_areas] : activated_fibers_)
        {
            for (const auto &to_area : to_areas)
            {
                if (readout_rules_.at(from_area).find(to_area) != readout_rules_.at(from_area).end())
                {
                    pruned_activated_fibers[from_area].insert(to_area);
                }
            }
        }
        return pruned_activated_fibers;
    }
};


class EnglishParserBrain : public ParserBrain
{
public:
    bool verbose;
    EnglishParserBrain(float p, int non_LEX_n = 10000, int non_LEX_k = 100, int LEX_k = 20,
                       float default_beta = 0.2, float LEX_beta = 1.0, float recurrent_beta = 0.05, float interarea_beta = 0.5, bool verbose = false)
        :
         ParserBrain(p, default_beta, 10000.0, 7777,
            LEXEME_DICT, 
			AREAS, 
		    RECURRENT_AREAS, 
			INIT,
			ENGLISH_READOUT_RULES),
            verbose(verbose)
    {
        int LEX_n = LEX_SIZE * LEX_k;
        AddArea(LEX, LEX_n, LEX_k, false, true);

        int DET_k = LEX_k;
        AddArea(SUBJ, non_LEX_n, non_LEX_k, true, false);
        AddArea(OBJ, non_LEX_n, non_LEX_k, true, false);
        AddArea(VERB, non_LEX_n, non_LEX_k, true, false);
        AddArea(ADJ, non_LEX_n, non_LEX_k, true, false);
        AddArea(PREP, non_LEX_n, non_LEX_k, true, false);
        AddArea(PREP_P, non_LEX_n, non_LEX_k, true, false);
        AddArea(DET, non_LEX_n, DET_k, false, false);
        AddArea(ADVERB, non_LEX_n, non_LEX_k, true, false);
    }
        //有一个更新beta的函数，无从下手
    ProjectMap getProjectMap() 
    {
        ProjectMap proj_map = ParserBrain::getProjectMap();
        if (proj_map.find(LEX) != proj_map.end() && proj_map[LEX].size() > 2) // because LEX->LEX
        {
            throw std::runtime_error("Got that LEX projecting into many areas");
        }
        return proj_map;
    }

    std::string getWord(const std::string &area_name, float min_overlap = 0.7) 
    {

        std::string word = ParserBrain::getWord(area_name, min_overlap);
        if (!word.empty())
        {
            return word;
        }
        return "<NON-WORD>";
    }

};


class ParserDebugger
{
public:
    ParserDebugger(EnglishParserBrain& brain, const std::vector<std::string>& all_areas, const std::vector<std::string>& explicit_areas)
        : b(brain), all_areas(all_areas), explicit_areas(explicit_areas) {}

    void run() {
        std::string command;
        std::cout << "DEBUGGER: ENTER to continue, 'P' for PEAK" << std::endl;
        std::getline(std::cin, command);
        while (!command.empty()) {
            if (command == "P") {
                peak();
                return;
            } else if (!command.empty()) {
                std::cout << "DEBUGGER: Command not recognized..." << std::endl;
            }
            std::cout << "DEBUGGER: ENTER to continue, 'P' for PEAK" << std::endl;
            std::getline(std::cin, command);
        }
    }

private:
    EnglishParserBrain& b;
    //EnglishParserBrain b;
    std::vector<std::string> all_areas;
    std::vector<std::string> explicit_areas;

    void peak() {
        std::map<std::string, int> remove_map;

        for (const std::string& area_name : all_areas) {
            auto &area = b.GetArea(area_name);
            area.is_fixed = false;
        }

        while (true) {
            std::string test_proj_map_string;
            std::cout << "DEBUGGER: enter projection map, eg. {\"VERB\": [\"LEX\"]}, or ENTER to quit" << std::endl;
            std::getline(std::cin, test_proj_map_string);
            if (test_proj_map_string.empty()) {
                break;
            }
            std::map<std::string, std::set<std::string>> test_proj_map = parseProjectionMap(test_proj_map_string);

            // Important: save winners to later "remove" this test project round
            std::set<std::string> to_area_set;
            for (const auto& [_, to_area_list] : test_proj_map) {
                for (const std::string& to_area : to_area_list) {
                    to_area_set.insert(to_area);
                }
            }

            for (const std::string& to_area : to_area_set) {
                remove_map[to_area]++;
            }
            b.Project(test_proj_map, 1, false);

            for (const std::string& area : explicit_areas) {
                if (to_area_set.count(area)) {
                    std::string area_word = b.interpretAssemblyAsString(area);
                    std::cout << "DEBUGGER: in explicit area " << area << ", got: " << area_word << std::endl;
                }
            }

            std::string print_assemblies;
            std::cout << "DEBUGGER: print assemblies in areas? Eg. 'LEX,VERB' or ENTER to cont" << std::endl;
            std::getline(std::cin, print_assemblies);
            if (!print_assemblies.empty()) {
                for (const std::string& print_area : splitString(print_assemblies, ',')) {
                    std::cout << "DEBUGGER: Printing assembly in area " << print_area << std::endl;
                    // std::cout << b->area_by_name[print_area].winners << std::endl;
                    if (std::find(explicit_areas.begin(), explicit_areas.end(), print_area) != explicit_areas.end()) {
                        std::string word = b.interpretAssemblyAsString(print_area);
                        std::cout << "DEBUGGER: in explicit area got assembly = " << word << std::endl;
                    }
                }
            }
        }
    }
    std::map<std::string, std::set<std::string>> parseProjectionMap(const std::string& json_str) {
        std::map<std::string, std::set<std::string>> result;
        std::string key;
        std::string value;
        bool is_key = true;
        for (char c : json_str) {
            if (c == ':') {
                is_key = false;
            } else if (c == ',') {
                if (!key.empty() && !value.empty()) {
                    std::stringstream ss(value);
                    std::string item;
                    std::set<std::string> values;
                    while (std::getline(ss, item, ',')) {
                        values.insert(item);
                    }
                    result[key] = values;
                }
                key.clear();
                value.clear();
                is_key = true;
            } else if (c != '{' && c != '}' && c != ' ' && c != '"' && c != '[' && c != ']') {
                if (is_key) {
                    key += c;
                } else {
                    value += c;
                }
            }
        }
        if (!key.empty() && !value.empty()) {
            std::stringstream ss(value);
            std::string item;
            std::set<std::string> values;
            while (std::getline(ss, item, ',')) {
                values.insert(item);
            }
            result[key] = values;
        }
        return result;
    }

    std::vector<std::string> splitString(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::istringstream iss(str);
        std::string token;
        while (std::getline(iss, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }
};


enum class ReadoutMethod {
    FIXED_MAP_READOUT = 1,
    FIBER_READOUT = 2,
    NATURAL_READOUT = 3
};


class TreeNode {
public:
    std::string name;
    std::vector<std::shared_ptr<TreeNode>> children;

    TreeNode(const std::string& name) : name(name) {}

    void addChild(std::shared_ptr<TreeNode> child) {
        children.push_back(child);
    }

    void printTree(int depth = 0) {
        for (int i = 0; i < depth; ++i) std::cout << "  ";
        std::cout << name << std::endl;
        for (const auto& child : children) {
            child->printTree(depth + 1);
        }
    }
};

class NodeValue {
public:
    enum class Type { String, Map };
    Type type;
    std::string str_value;
    std::map<std::string, NodeValue> map_value;

    NodeValue(const std::string& value) : type(Type::String), str_value(value) {}
    NodeValue(const std::map<std::string, NodeValue>& value) : type(Type::Map), map_value(value) {}

    bool isString() const {
        return type == Type::String;
    }

    bool isMap() const {
        return type == Type::Map;
    }

    const std::string& getString() const {
        return str_value;
    }

    const std::map<std::string, NodeValue>& getMap() const {
        return map_value;
    }
};


void read_out(const std::string& area, const std::map<std::string, std::set<std::string>>& mapping, std::vector<std::vector<std::string>>& dependencies, EnglishParserBrain& b, std::vector<std::string>& visited) {
    auto it = mapping.find(area);
    if (it != mapping.end()) {
        std::vector<std::string> to_areas(it->second.begin(), it->second.end());
        std::string this_word = b.getWord("LEX", 0.7);

        if (this_word.empty()) {
            std::cerr << "Error: this_word is empty for area: " << area << std::endl;
            return;
        }

        int k = 0;
        for (const auto& to_area : to_areas) {
            if (to_area == "LEX") continue;

            b.Project({{to_area, {"LEX"}}}, 1, true);
            std::string other_word = b.getWord("LEX", 0.7);
            dependencies.push_back({this_word, other_word, to_area});
        }
    }
    // 添加一个visit来遍历已经访问过的结点
    for (const auto& [area_key, _] : mapping) {
        int skip = 0;
        for(std::string name : visited){
            if(name==area_key) skip = 1;
        }
        if (area_key != "LEX" && skip == 0) {
            visited.push_back(area_key);
            read_out(area_key, mapping, dependencies, b, visited);
        }
    }
}


void parseHelper(EnglishParserBrain& b, std::string sentence, int project_rounds, 
                 bool verbose, bool debug, std::map<std::string, RuleSet> lexeme_dict, 
                 std::vector<std::string> all_areas, std::vector<std::string> explicit_areas, 
                 ReadoutMethod readout_method, std::map<std::string, std::set<std::string>> readout_rules) {

    ParserDebugger debugger(b, all_areas, explicit_areas);
    std::vector<std::string> sentence_words;
    std::istringstream iss(sentence);
    for (std::string word; iss >> word; ) {
        sentence_words.push_back(word);
    }
    bool extreme_debug = false;
    for (const auto& word : sentence_words) {   
        auto lexeme = lexeme_dict[word];
        b.activateWord("LEX", word);
        if (verbose) {
            std::cout << "Activated word: " << word << std::endl << std::endl;
            // std::cout << b.area_by_name[LEX_k].winners << std::endl;
        }
        RuleGroup prerules(lexeme.ruleGroups["PRE_RULES"]);
        int i = 1;
        for (const auto& rule : prerules.areaRules) {
            //std::cout << "AreaRule" << i << ": " << std::endl << "area: " << rule.area << "  action: " << rule.action << " index: " << rule.index << std::endl;
            b.applyAreaRule(rule.area, rule.action,rule.index);
        }
        for (const auto& rule : prerules.fiberRules) {
            //std::cout << "FiberRule" << i++ << ": " << std::endl << "area1: " << rule.area1 << "  area2: " << rule.area2 << "  action: " << rule.action << std::endl << std::endl;
            b.applyFiberRule(rule.area1,rule.area2, rule.action,rule.index);
        }
        //b.printAreaStates();
        //b.printFiberStates(b.fiber_states_);
        nemo::ProjectMap proj_map = b.getProjectMap();
        printProjectMap(proj_map);
        for (const auto& area : proj_map) {
            std::cout << area.first << std::endl;
            if (std::find(proj_map["LEX"].begin(), proj_map["LEX"].end(), area.first) == proj_map["LEX"].end()) {
                b.GetArea(area.first).is_fixed=true;
                if (verbose) {
                    //固定组件，因为该区域不和LEX相连
                    std::cout << "FIXED assembly bc not LEX->this area in: " << area.first << std::endl << std::endl;
                }
            } else if (area.first != "LEX") {
                b.GetArea(area.first).is_fixed=false;
                //b.GetArea(area.first).activated.clear();
                if (verbose) {
                    // 清除组件，因为该区域和LEX相连
                    std::cout << "ERASED assembly because LEX->this area in " << area.first << std::endl << std::endl;
                }
            }
        }
        proj_map = b.getProjectMap();
        if (verbose) {
            std::cout << "Got proj_map = " << std::endl << std::endl;
            printProjectMap(proj_map);
        }
        for (int i = 0; i < project_rounds; ++i) {
            b.parseProject();
            if (verbose) {
                proj_map = b.getProjectMap();
                std::cout << "Got proj_map = " << std::endl << std::endl;
                printProjectMap(proj_map);
            }
            if (extreme_debug && word == "a") {
                std::cout << "Starting debugger after round " << i << " for word " << word << std::endl << std::endl;
                debugger.run();
            }
        }
        RuleGroup postrules(lexeme.ruleGroups["POST_RULES"]);
        for (const auto& rule : prerules.areaRules) {
            b.applyAreaRule(rule.area, rule.action,rule.index);
        }
        for (const auto& rule : prerules.fiberRules) {
            b.applyFiberRule(rule.area1,rule.area2, rule.action,rule.index);
        }
        if (debug) {
            std::cout << "Starting debugger after the word " << word << std::endl << std::endl;
            debugger.run();
        }
        //b.printAreaStates();
        //b.printFiberStates(b.fiber_states_);
    }
    for (const auto& area : all_areas) {
        b.GetArea(area).is_fixed = false;
    }
    std::vector<std::vector<std::string>> dependencies;

    std::function<void(const std::map<std::string, NodeValue>& , std::shared_ptr<TreeNode> )> treeify;
    treeify=[&](const std::map<std::string, NodeValue>& parsed_dict, std::shared_ptr<TreeNode> parent) {
        for (const auto& [key, values] : parsed_dict) {
            auto key_node = std::make_shared<TreeNode>(key);
            parent->addChild(key_node);

            if (values.isString()) {
                auto value_node = std::make_shared<TreeNode>(values.getString());
                key_node->addChild(value_node);
            } else if (values.isMap()) {
                treeify(values.getMap(), key_node);
            }
        }
    };

    if (readout_method == ReadoutMethod::FIXED_MAP_READOUT) {
        // 从 VERB 开始解析
        std::map<std::string, NodeValue> parsed;
        std::vector<std::string> visited;
        read_out("VERB", b.getProjectMap(), dependencies, b, visited);

        // 输出解析结果
        std::cout << "Final parse dict: " << std::endl;
        for (const auto& dep : dependencies) {
            std::cout << dep[0] << " -> " << dep[1] << " in " << dep[2] << std::endl;
        }

        auto root = std::make_shared<TreeNode>("VERB");
        treeify(parsed, root);

        // 打印树
        root->printTree();
    }

    if (readout_method == ReadoutMethod::FIBER_READOUT) {
        auto activated_fibers = b.getActivatedFibers();
        if (verbose) {
            std::cout << "Got activated fibers for readout:" << std::endl;
            b.printActivatedFibers(activated_fibers);
        }
        std::vector<std::string> visited;
        read_out("VERB", b.getProjectMap(), dependencies, b, visited);
        std::cout << "Got dependencies: " << std::endl;
        if(dependencies.size() == 0){
            std::cout << "dependencies is empty!" << std::endl;
        }
        for (const auto& dep : dependencies) {
            std::cout << dep[0] << " -> " << dep[1] << " in " << dep[2] << std::endl;
        }
    }
}

void parse(std::string sentence = "cats chase mice", float p = 0.1, int LEX_k = 20,
           int project_rounds = 20, bool verbose = true, bool debug = false, ReadoutMethod readout_method = ReadoutMethod::FIBER_READOUT) {
    EnglishParserBrain b(p, LEX_k, verbose);
    std::map<std::string, RuleSet> lexeme_dict = LEXEME_DICT;
    std::vector<std::string> all_areas = AREAS;
    std::vector<std::string> explicit_areas = EXPLICIT_AREAS;
    std::map<std::string, std::set<std::string>> readout_rules = ENGLISH_READOUT_RULES;

    parseHelper(b, sentence,  project_rounds, verbose, debug, 
		lexeme_dict, all_areas, explicit_areas, readout_method, readout_rules);}
}
#endif