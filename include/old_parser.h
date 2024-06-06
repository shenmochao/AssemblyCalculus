/*
  先看这里！
  每个函数我都用两行注释来展示
  // 中文介绍，这函数是干啥的
  // 用法介绍，这个函数该怎么使用

  对于部分光看不太知道在干啥的地方，我也写了一点注释
  昨晚尝试着去写了测试，但是写了半天也不知道测试该怎么写
  （因为这是个中间的类，EnglishParserBrain得继承自它，所以测试代码没法用）
  GPT也搞不出来正常的测试代码，所以最后放弃选择睡觉了
  但代码基本上都是照着python代码一比一翻译过来的，应该不会有错
  （测试时有错随时call我）
  所以现在只是在一个编译器不会报错的阶段，可能存在不少隐形bug
  留给测试环节的时间可能得多一点，因为我们写代码的人肯定还得同步改代码

  祝你们顺利~
*/

#ifndef PARSER_BRAIN_H_
#define PARSER_BRAIN_H_

#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "true_brain.h"
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

// 数据结构
struct AreaRule {
    std::string action;
    std::string area;
    int index;
    AreaRule(std::string a, std::string b, int i) 
    : action(a), area(b), index(i) {}
};

struct FiberRule {
    std::string action;
    std::string area1;
    std::string area2;
    int index;

    FiberRule(std::string a, std::string a1, std::string a2, int i) 
        : action(a), area1(a1), area2(a2), index(i) {}
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

typedef std::map<std::string, std::vector<std::string>> ProjectMap;

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
    // 初始化状态
    // parser.initialize_states();
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
    // parser.applyFiberRule("from_area", "to_area", "INHIBIT", 0);
    void applyFiberRule(const std::string &from, const std::string &to, const std::string &action, int index)
    {
        if (action == "INHIBIT")
        {
            fiber_states_[from][to].insert(index);
            fiber_states_[to][from].insert(index);
        }
        else if (action == "DISINHIBIT")
        {
            fiber_states_[from][to].erase(index);
            fiber_states_[to][from].erase(index);
        }
    }
    // 在某个区域上应用抑制或解除抑制
    // parser.applyAreaRule("area_name", "INHIBIT", 0);
    void applyAreaRule(const std::string &area, const std::string &action, int index)
    {
        if (action == "INHIBIT")
        {
            area_states_[area].insert(index);
        }
        else if (action == "DISINHIBIT")
        {
            area_states_[area].erase(index);
        }
    }
    // 根据规则类型决定调用 区块间 还是 单个区块 的应用方法。
    // parser.applyRule("FiberRule", "from_area", "to_area", "INHIBIT", 0);
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
    // parser.parseProject(1, true); // 训练时置为true
    // void parseProject(uint32_t step, bool update_plasticity)
    // {
    //     auto project_map = getProjectMap();
    //     rememberFibers(project_map);
    //     Project(project_map, step, update_plasticity);
    // }
    // parser.parseProject();
    void parseProject()
    {
        auto project_map = getProjectMap();
        rememberFibers(project_map);
        Project(project_map, 1, true);
    }
    // 记忆已经激活的纤维束
    // parser.rememberFibers(project_map);
    void rememberFibers(const ProjectMap &project_map)
    {
        // 遍历投射图中的每一个区域，对于每一个区域，将其与其连接的目标区域存储
        // from_area:起点为字符串(区域名称) to_areas:与其相连的所有终点，字符串数组
        for (const auto &[from_area, to_areas] : project_map)
        {
            activated_fibers_[from_area].insert(to_areas.begin(), to_areas.end());
        }
    }
    // 检查给定的区域是否是循环的
    // bool is_recurrent = parser.isRecurrent("area_name");
    bool isRecurrent(const std::string &area) const
    {
        // 在 recurrent_areas_ 中查找给定的区域名称
        return std::find(recurrent_areas_.begin(), recurrent_areas_.end(), area) != recurrent_areas_.end();
    }
    // 构建一个投射图，包含了在没有被抑制的状态下，处于激活状态的区域之间的连接关系
    // ProjectMap project_map = parser.getProjectMap();
    ProjectMap getProjectMap() const
    {
        ProjectMap proj_map;
        for (const auto &area1 : all_areas_)
        {
            if (area_states_.at(area1).empty())
            {
                for (const auto &area2 : all_areas_)
                {
                    if (area1 == "LEX" && area2 == "LEX")
                        continue;
                    if (area_states_.at(area2).empty())
                    {
                        if (fiber_states_.at(area1).at(area2).empty())
                        {
                            if (!areas_.at(area_by_name_.at(area1)).activated.empty())
                            {
                                proj_map[area1].push_back(area2);
                            }
                            if (!areas_.at(area_by_name_.at(area2)).activated.empty())
                            {
                                proj_map[area2].push_back(area1);
                            }
                        }
                    }
                }
            }
        }
        return proj_map;
    }
    // 激活特定区域中的特定单词
    // parser.activateWord("LEX", "hello");
    void activateWord(const std::string& area_name, const std::string& word) {
        //          区域列表 区域名->区域列表索引  区域名列表
        auto& area = areas_.at(area_by_name_.at(area_name));
        uint32_t k = area.k;
        uint32_t lexeme_index = lexeme_dict_[word].index; // 获取索引，我对其获取方式进行了修改，不会影响函数实现
        uint32_t assembly_start = lexeme_index * k;
        // 在子区间中复制元素，并赋值给另一个容器的特定范围
        area.activated.assign(area.activated.begin() + assembly_start, area.activated.begin() + assembly_start + k);
        area.is_fixed = true;
    }
    // 激活特定区域中的特定索引
    // parser.activateIndex("area_name", 0);
    void activateIndex(const std::string &area_name, uint32_t index)
    {
        //          区域列表 区域名->区域列表索引  区域名列表
        auto &area = areas_.at(area_by_name_.at(area_name));
        uint32_t k = area.k;
        uint32_t assembly_start = index * k;
        area.activated.assign(area.activated.begin() + assembly_start, area.activated.begin() + assembly_start + k);
        area.is_fixed = true;
    }
    // 将区域中的组件解释为字符串
    // std::string interpreted_string = parser.interpretAssemblyAsString("area_name");
    std::string interpretAssemblyAsString(const std::string &area_name) const
    {
        return getWord(area_name, 0.7);
    }
    /*
      当LEX中的一个组件x_w处于活跃状态时，返回对应的单词w。
      x_w是什么？可以理解为w是对应的动词，x_w是w这个单词对应的神经元的集合
    */
    // std::string word = parser.getWord("LEX", 0.7);
    std::string getWord(const std::string &area_name, float min_overlap = 0.7) const
    {
        // 获取与指定区域名称关联的区域对象
        const auto &area = areas_.at(area_by_name_.at(area_name));
        // 将区域中激活的神经元ID存入集合 winners
        std::set<uint32_t> winners(area.activated.begin(), area.activated.end());
        // 获取区域中的神经元数量 k
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
    // std::map<std::string, std::set<std::string>> activated_fibers = parser.getActivatedFibers();
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

}

#endif

