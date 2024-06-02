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

namespace nemo
{

typedef std::map<std::string, std::vector<std::string>> ProjectMap;

class ParserBrain : public Brain
{
public:
    std::map<std::string, uint32_t> lexeme_dict_;
    std::vector<std::string> all_areas_;
    std::vector<std::string> recurrent_areas_;
    std::vector<std::string> initial_areas_;
    std::map<std::string, std::set<std::string>> readout_rules_;
    std::map<std::string, std::set<std::string>> activated_fibers_;
    std::map<std::string, std::map<std::string, std::set<int>>> fiber_states_;
    std::map<std::string, std::set<int>> area_states_;

    ParserBrain(float p, float beta, float max_weight, uint32_t seed,
                std::map<std::string, uint32_t> lexeme_dict = {},
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
        auto& area = areas_.at(area_by_name_.at(area_name));
        uint32_t k = area.k;
        uint32_t lexeme_index = lexeme_dict_.at(word);
        uint32_t assembly_start = lexeme_index * k;
        area.activated.assign(area.activated.begin() + assembly_start, area.activated.begin() + assembly_start + k);
        area.is_fixed = true;
    }
    // 激活特定区域中的特定索引
    // parser.activateIndex("area_name", 0);
    void activateIndex(const std::string &area_name, uint32_t index)
    {
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
        const auto &area = areas_.at(area_by_name_.at(area_name));
        std::set<uint32_t> winners(area.activated.begin(), area.activated.end());
        uint32_t area_k = area.k;
        float threshold = min_overlap * area_k;

        for (const auto &[word, lexeme_index] : lexeme_dict_)
        {
            uint32_t word_assembly_start = lexeme_index * area_k;
            std::set<uint32_t> word_assembly;
            for (uint32_t i = word_assembly_start; i < word_assembly_start + area_k; ++i)
            {
                word_assembly.insert(i);
            }
            std::set<uint32_t> intersection;
            std::set_intersection(winners.begin(), winners.end(), word_assembly.begin(), word_assembly.end(),
                                  std::inserter(intersection, intersection.begin()));
            if (intersection.size() >= threshold)
            {
                return word;
            }
        }
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

