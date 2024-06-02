#include <parser.h>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <iostream>
#include <cassert>

void test_initialize_states() {
    // 创建测试数据
    nemo::ParserBrain parser(0.1, 0.1, 1.0, 42,
                             {{"hello", 0}, {"world", 1}, {"foo", 2}, {"bar", 3}},
                             {"area1", "area2", "area3"},
                             {"area3"},
                             {"area1"},
                             {{"area1", {"area2"}}, {"area2", {"area1"}}, {"area3", {"area1"}}});

    // 初始化状态
    parser.initialize_states();

    // 检查 fiber_states_ 的初始状态是否正确
    for (const auto& from_area : parser.all_areas_) {
        for (const auto& to_area : parser.all_areas_) {
            assert(parser.fiber_states_[from_area][to_area].count(0) == 1);
        }
    }

    // 检查 area_states_ 的初始状态是否正确
    for (const auto& area : parser.all_areas_) {
        if (area == "area1") {
            assert(parser.area_states_[area].empty());
        } else {
            assert(parser.area_states_[area].count(0) == 1);
        }
    }

    std::cout << "test_initialize_states passed!" << std::endl;
}

void test_applyFiberRule() {
    // 创建测试数据
    nemo::ParserBrain parser(0.1, 0.1, 1.0, 42);

    // 测试 INHIBIT 动作
    parser.applyFiberRule("area1", "area2", "INHIBIT", 1);
    assert(parser.fiber_states_["area1"]["area2"].count(1) == 1);
    assert(parser.fiber_states_["area2"]["area1"].count(1) == 1);

    // 测试 DISINHIBIT 动作
    parser.applyFiberRule("area1", "area2", "DISINHIBIT", 1);
    assert(parser.fiber_states_["area1"]["area2"].count(1) == 0);
    assert(parser.fiber_states_["area2"]["area1"].count(1) == 0);

    std::cout << "test_applyFiberRule passed!" << std::endl;
}

void test_applyAreaRule() {
    // 创建测试数据
    nemo::ParserBrain parser(0.1, 0.1, 1.0, 42);

    // 测试 INHIBIT 动作
    parser.applyAreaRule("area1", "INHIBIT", 1);
    assert(parser.area_states_["area1"].count(1) == 1);

    // 测试 DISINHIBIT 动作
    parser.applyAreaRule("area1", "DISINHIBIT", 1);
    assert(parser.area_states_["area1"].count(1) == 0);

    std::cout << "test_applyAreaRule passed!" << std::endl;
}

void test_applyRule() {
    // 创建测试数据
    nemo::ParserBrain parser(0.1, 0.1, 1.0, 42);

    // 测试 FiberRule
    parser.applyRule("FiberRule", "area1", "area2", "INHIBIT", 1);
    assert(parser.fiber_states_["area1"]["area2"].count(1) == 1);
    parser.applyRule("FiberRule", "area1", "area2", "DISINHIBIT", 1);
    assert(parser.fiber_states_["area1"]["area2"].count(1) == 0);

    // 测试 AreaRule
    parser.applyRule("AreaRule", "area1", "", "INHIBIT", 1);
    assert(parser.area_states_["area1"].count(1) == 1);
    parser.applyRule("AreaRule", "area1", "", "DISINHIBIT", 1);
    assert(parser.area_states_["area1"].count(1) == 0);

    std::cout << "test_applyRule passed!" << std::endl;
}

void test_rememberFibers() {
    // 创建测试数据
    nemo::ParserBrain parser(0.1, 0.1, 1.0, 42);

    nemo::ProjectMap project_map = {
        {"area1", {"area2", "area3"}},
        {"area2", {"area1"}},
        {"area3", {"area2"}}
    };

    // 记忆已激活的纤维束
    parser.rememberFibers(project_map);

    // 检查 activated_fibers_ 是否正确更新
    assert(parser.activated_fibers_["area1"].count("area2") == 1);
    assert(parser.activated_fibers_["area1"].count("area3") == 1);
    assert(parser.activated_fibers_["area2"].count("area1") == 1);
    assert(parser.activated_fibers_["area3"].count("area2") == 1);

    std::cout << "test_rememberFibers passed!" << std::endl;
}

void test_isRecurrent() {
    // 创建测试数据
    nemo::ParserBrain parser(0.1, 0.1, 1.0, 42, {}, {"area1", "area2"}, {"area2"});

    // 检查区域是否为循环区域
    assert(parser.isRecurrent("area2") == true);
    assert(parser.isRecurrent("area1") == false);

    std::cout << "test_isRecurrent passed!" << std::endl;
}

void test_getProjectMap() {
    // 创建测试数据
    nemo::ParserBrain parser(0.1, 0.1, 1.0, 42, {}, {"area1", "area2"}, {}, {}, {});

    parser.initialize_states();
    parser.activateIndex("area1", 0);

    // 构建投射图
    nemo::ProjectMap project_map = parser.getProjectMap();

    // 检查投射图是否正确
    assert(project_map["area1"].size() == 1);
    assert(project_map["area1"][0] == "area2");

    std::cout << "test_getProjectMap passed!" << std::endl;
}

void test_activateWord() {
    // 创建测试数据
    nemo::ParserBrain parser(0.1, 0.1, 1.0, 42,
                             {{"hello", 0}, {"world", 1}, {"foo", 2}, {"bar", 3}},
                             {"LEX"}, {}, {}, {});

    // 初始化 LEX 区域
    parser.areas_ = std::vector<nemo::Area>(1);
    parser.area_by_name_ = {{"LEX", 0}};
    parser.areas_.at(0).k = 1;
    parser.areas_[0].activated = {1};

    // 激活单词
    parser.activateWord("LEX", "hello");

    // 检查激活状态
    assert(parser.areas_[0].activated.size() == 1);
    assert(parser.areas_[0].activated[0] == 0);

    std::cout << "test_activateWord passed!" << std::endl;
}

void test_activateIndex() {
    // 创建测试数据
    nemo::ParserBrain parser(0.1, 0.1, 1.0, 42, {}, {"area1"}, {}, {}, {});

    // 初始化 area1 区域
    parser.areas_ = std::vector<nemo::Area>(1);
    parser.area_by_name_ = {{"area1", 0}};
    parser.areas_.at(0).k = 1;
    parser.areas_[0].activated = {1};

    // 激活索引
    parser.activateIndex("area1", 0);

    // 检查激活状态
    assert(parser.areas_[0].activated.size() == 1);
    assert(parser.areas_[0].activated[0] == 0);

    std::cout << "test_activateIndex passed!" << std::endl;
}

void test_interpretAssemblyAsString() {
    // 创建测试数据
    nemo::ParserBrain parser(0.1, 0.1, 1.0, 42,
                             {{"hello", 0}, {"world", 1}, {"foo", 2}, {"bar", 3}},
                             {"LEX"}, {}, {}, {});

    // 初始化 LEX 区域
    parser.areas_ = std::vector<nemo::Area>(1);
    parser.area_by_name_ = {{"LEX", 0}};
    parser.areas_.at(0).k = 1;
    parser.areas_[0].activated = {0};

    // 解释装配成字符串
    std::string result = parser.interpretAssemblyAsString("LEX");
    assert(result == "hello");

    std::cout << "test_interpretAssemblyAsString passed!" << std::endl;
}

void test_getWord() {
    // 创建测试数据
    nemo::ParserBrain parser(0.1, 0.1, 1.0, 42,
                             {{"hello", 0}, {"world", 1}, {"foo", 2}, {"bar", 3}},
                             {"LEX"}, {}, {}, {});

    // 初始化 LEX 区域
    parser.areas_ = std::vector<nemo::Area>(1);
    parser.area_by_name_ = {{"LEX", 0}};
    parser.areas_.at(0).k = 1;
    parser.areas_[0].activated = {0};

    // 获取单词
    std::string word = parser.getWord("LEX", 0.7);
    assert(word == "hello");

    std::cout << "test_getWord passed!" << std::endl;
}

void test_getActivatedFibers() {
    // 创建测试数据
    nemo::ParserBrain parser(0.1, 0.1, 1.0, 42, {}, {"area1", "area2"}, {}, {}, {{"area1", {"area2"}}, {"area2", {}}});

    parser.activated_fibers_ = {{"area1", {"area2"}}, {"area2", {}}};

    // 获取已激活的纤维束
    auto activated_fibers = parser.getActivatedFibers();

    // 检查激活的纤维束是否正确
    assert(activated_fibers["area1"].count("area2") == 1);
    assert(activated_fibers["area2"].count("area1") == 0);

    std::cout << "test_getActivatedFibers passed!" << std::endl;
}

int main() {
    test_initialize_states();
    test_applyFiberRule();
    test_applyAreaRule();
    test_applyRule();
    test_rememberFibers();
    test_isRecurrent();
    test_getProjectMap();
    test_activateWord();
    test_activateIndex();
    test_interpretAssemblyAsString();
    test_getWord();
    test_getActivatedFibers();

    std::cout << "All tests passed!" << std::endl;

    return 0;
}
