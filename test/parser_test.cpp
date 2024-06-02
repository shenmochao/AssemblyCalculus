#include <iostream>
#include <cassert>
#include <parser.h>

using namespace nemo;

// 测试 ParserBrain 对象的状态初始化功能
void testInitializeStates() {
    std::vector<std::string> all_areas = {"A", "B", "C"};
    std::vector<std::string> initial_areas = {"A"};

    // 使用指定参数初始化 ParserBrain
    ParserBrain pb(0.1, 0.2, 0.3, 42, {}, all_areas, {}, initial_areas);

    // 检查纤维状态
    for (const auto& from_area : all_areas) {
        assert(pb.fiber_states_.count(from_area) > 0);
        for (const auto& to_area : all_areas) {
            assert(pb.fiber_states_[from_area].count(to_area) > 0);
            assert(pb.fiber_states_[from_area][to_area].count(0) > 0);
        }
    }

    // 检查区域状态
    for (const auto& area : all_areas) {
        assert(pb.area_states_.count(area) > 0);
        if (std::find(initial_areas.begin(), initial_areas.end(), area) != initial_areas.end()) {
            assert(pb.area_states_[area].count(0) == 0);
        } else {
            assert(pb.area_states_[area].count(0) > 0);
        }
    }

    std::cout << "testInitializeStates passed!" << std::endl;
}

// 测试 ParserBrain 对象的规则应用功能
void testApplyRules() {
    std::vector<std::string> all_areas = {"A", "B"};
    ParserBrain pb(0.1, 0.2, 0.3, 42, {}, all_areas);

    // 应用纤维规则并检查状态
    if (pb.fiber_states_.count("A") && pb.fiber_states_["A"].count("B")) {
        pb.applyFiberRule("A", "B", "INHIBIT", 1);
        assert(pb.fiber_states_.at("A").at("B").count(1) > 0);
        assert(pb.fiber_states_.at("B").at("A").count(1) > 0);

        pb.applyFiberRule("A", "B", "DISINHIBIT", 1);
        assert(pb.fiber_states_.at("A").at("B").count(1) == 0);
        assert(pb.fiber_states_.at("B").at("A").count(1) == 0);
    }

    // 应用区域规则并检查状态
    if (pb.area_states_.count("A")) {
        pb.applyAreaRule("A", "INHIBIT", 1);
        assert(pb.area_states_.at("A").count(1) > 0);

        pb.applyAreaRule("A", "DISINHIBIT", 1);
        assert(pb.area_states_.at("A").count(1) == 0);
    }

    std::cout << "testApplyRules passed!" << std::endl;
}

// 测试 ParserBrain 对象的 getProjectMap 功能
void testGetProjectMap() {
    std::cout << "testGetProjectMap()" << std::endl;
    std::vector<std::string> all_areas = {"A", "B"};
    std::map<std::string, uint32_t> lexeme_dict = {{"word1", 0}};
    ParserBrain pb(0.1, 0.2, 0.3, 42, lexeme_dict, all_areas);

    std::cout << "Checking area states before activation:" << std::endl;
    for (const auto& area : all_areas) {
        std::cout << "Area: " << area << ", State count: " << pb.area_states_.count(area) << std::endl;
    }

    pb.activateWord("A", "word1");

    std::cout << "Checking area states after activation:" << std::endl;
    for (const auto& area : all_areas) {
        std::cout << "Area: " << area << ", State count: " << pb.area_states_.count(area) << std::endl;
    }

    ProjectMap proj_map = pb.getProjectMap();
    assert(proj_map.count("A") > 0);
    assert(proj_map["A"].size() == 1);
    assert(proj_map["A"][0] == "B");

    std::cout << "testGetProjectMap passed!" << std::endl;
}


// 测试 ParserBrain 对象的单词激活和解释功能
void testActivateAndInterpret() {
    std::vector<std::string> all_areas = {"A"};
    std::map<std::string, uint32_t> lexeme_dict = {{"word1", 0}};
    ParserBrain pb(0.1, 0.2, 0.3, 42, lexeme_dict, all_areas);

    // 激活单词并检查解释结果
    pb.activateWord("A", "word1");
    std::string interpreted_word = pb.interpretAssemblyAsString("A");
    assert(interpreted_word == "word1");

    std::cout << "testActivateAndInterpret passed!" << std::endl;
}

// 测试 ParserBrain 对象的 getActivatedFibers 功能
void testGetActivatedFibers() {
    std::vector<std::string> all_areas = {"A", "B"};
    std::map<std::string, std::set<std::string>> readout_rules = {{"A", {"B"}}};
    ParserBrain pb(0.1, 0.2, 0.3, 42, {}, all_areas, {}, {}, readout_rules);

    pb.activated_fibers_["A"].insert("B");

    auto activated_fibers = pb.getActivatedFibers();
    assert(activated_fibers.count("A") > 0);
    assert(activated_fibers["A"].count("B") > 0);

    std::cout << "testGetActivatedFibers passed!" << std::endl;
}

int main() {
    testInitializeStates();
    testApplyRules();
    testGetProjectMap();
    testActivateAndInterpret();
    testGetActivatedFibers();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
