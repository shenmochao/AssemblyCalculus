#include "parser_gpt.h"
#include <iostream>
#include <iostream>
#include <algorithm>
#include <random>
#include <stdexcept>
#include <json/json.h>

namespace parser {

std::unordered_map<std::string, Lexeme> getLexemeDict() {
    return {
        {"the", Lexeme{0,
            {AreaRule(DISINHIBIT, DET, 0), FiberRule(DISINHIBIT, LEX, DET, 0)},
            {FiberRule(INHIBIT, LEX, DET, 0), FiberRule(INHIBIT, VERB, ADJ, 0)}
        }},
        {"a", Lexeme{1,
            {AreaRule(DISINHIBIT, DET, 0), FiberRule(DISINHIBIT, LEX, DET, 0)},
            {FiberRule(INHIBIT, LEX, DET, 0), FiberRule(INHIBIT, VERB, ADJ, 0)}
        }},
        {"dogs", Lexeme{2,
            {
                FiberRule(DISINHIBIT, LEX, SUBJ, 0), FiberRule(DISINHIBIT, LEX, OBJ, 0),
                FiberRule(DISINHIBIT, LEX, PREP_P, 0), FiberRule(DISINHIBIT, DET, SUBJ, 0),
                FiberRule(DISINHIBIT, DET, OBJ, 0), FiberRule(DISINHIBIT, DET, PREP_P, 0),
                FiberRule(DISINHIBIT, ADJ, SUBJ, 0), FiberRule(DISINHIBIT, ADJ, OBJ, 0),
                FiberRule(DISINHIBIT, ADJ, PREP_P, 0), FiberRule(DISINHIBIT, VERB, OBJ, 0),
                FiberRule(DISINHIBIT, PREP_P, PREP, 0), FiberRule(DISINHIBIT, PREP_P, SUBJ, 0),
                FiberRule(DISINHIBIT, PREP_P, OBJ, 0)
            },
            {
                AreaRule(INHIBIT, DET, 0), AreaRule(INHIBIT, ADJ, 0), AreaRule(INHIBIT, PREP_P, 0), AreaRule(INHIBIT, PREP, 0),
                FiberRule(INHIBIT, LEX, SUBJ, 0), FiberRule(INHIBIT, LEX, OBJ, 0), FiberRule(INHIBIT, LEX, PREP_P, 0),
                FiberRule(INHIBIT, ADJ, SUBJ, 0), FiberRule(INHIBIT, ADJ, OBJ, 0), FiberRule(INHIBIT, ADJ, PREP_P, 0),
                FiberRule(INHIBIT, DET, SUBJ, 0), FiberRule(INHIBIT, DET, OBJ, 0), FiberRule(INHIBIT, DET, PREP_P, 0),
                FiberRule(INHIBIT, VERB, OBJ, 0), FiberRule(INHIBIT, PREP_P, PREP, 0), FiberRule(INHIBIT, PREP_P, VERB, 0),
                FiberRule(DISINHIBIT, LEX, SUBJ, 1), FiberRule(DISINHIBIT, LEX, OBJ, 1),
                FiberRule(DISINHIBIT, DET, SUBJ, 1), FiberRule(DISINHIBIT, DET, OBJ, 1),
                FiberRule(DISINHIBIT, ADJ, SUBJ, 1), FiberRule(DISINHIBIT, ADJ, OBJ, 1),
                FiberRule(INHIBIT, PREP_P, SUBJ, 0), FiberRule(INHIBIT, PREP_P, OBJ, 0),
                FiberRule(INHIBIT, VERB, ADJ, 0)
            }
        }},
        {"cats", Lexeme{3,
            {
                FiberRule(DISINHIBIT, LEX, SUBJ, 0), FiberRule(DISINHIBIT, LEX, OBJ, 0),
                FiberRule(DISINHIBIT, LEX, PREP_P, 0), FiberRule(DISINHIBIT, DET, SUBJ, 0),
                FiberRule(DISINHIBIT, DET, OBJ, 0), FiberRule(DISINHIBIT, DET, PREP_P, 0),
                FiberRule(DISINHIBIT, ADJ, SUBJ, 0), FiberRule(DISINHIBIT, ADJ, OBJ, 0),
                FiberRule(DISINHIBIT, ADJ, PREP_P, 0), FiberRule(DISINHIBIT, VERB, OBJ, 0),
                FiberRule(DISINHIBIT, PREP_P, PREP, 0), FiberRule(DISINHIBIT, PREP_P, SUBJ, 0),
                FiberRule(DISINHIBIT, PREP_P, OBJ, 0)
            },
            {
                AreaRule(INHIBIT, DET, 0), AreaRule(INHIBIT, ADJ, 0), AreaRule(INHIBIT, PREP_P, 0), AreaRule(INHIBIT, PREP, 0),
                FiberRule(INHIBIT, LEX, SUBJ, 0), FiberRule(INHIBIT, LEX, OBJ, 0), FiberRule(INHIBIT, LEX, PREP_P, 0),
                FiberRule(INHIBIT, ADJ, SUBJ, 0), FiberRule(INHIBIT, ADJ, OBJ, 0), FiberRule(INHIBIT, ADJ, PREP_P, 0),
                FiberRule(INHIBIT, DET, SUBJ, 0), FiberRule(INHIBIT, DET, OBJ, 0), FiberRule(INHIBIT, DET, PREP_P, 0),
                FiberRule(INHIBIT, VERB, OBJ, 0), FiberRule(INHIBIT, PREP_P, PREP, 0), FiberRule(INHIBIT, PREP_P, VERB, 0),
                FiberRule(DISINHIBIT, LEX, SUBJ, 1), FiberRule(DISINHIBIT, LEX, OBJ, 1),
                FiberRule(DISINHIBIT, DET, SUBJ, 1), FiberRule(DISINHIBIT, DET, OBJ, 1),
                FiberRule(DISINHIBIT, ADJ, SUBJ, 1), FiberRule(DISINHIBIT, ADJ, OBJ, 1),
                FiberRule(INHIBIT, PREP_P, SUBJ, 0), FiberRule(INHIBIT, PREP_P, OBJ, 0),
                FiberRule(INHIBIT, VERB, ADJ, 0)
            }
        }},
        {"mice", Lexeme{4,
            {
                FiberRule(DISINHIBIT, LEX, SUBJ, 0), FiberRule(DISINHIBIT, LEX, OBJ, 0),
                FiberRule(DISINHIBIT, LEX, PREP_P, 0), FiberRule(DISINHIBIT, DET, SUBJ, 0),
                FiberRule(DISINHIBIT, DET, OBJ, 0), FiberRule(DISINHIBIT, DET, PREP_P, 0),
                FiberRule(DISINHIBIT, ADJ, SUBJ, 0), FiberRule(DISINHIBIT, ADJ, OBJ, 0),
                FiberRule(DISINHIBIT, ADJ, PREP_P, 0), FiberRule(DISINHIBIT, VERB, OBJ, 0),
                FiberRule(DISINHIBIT, PREP_P, PREP, 0), FiberRule(DISINHIBIT, PREP_P, SUBJ, 0),
                FiberRule(DISINHIBIT, PREP_P, OBJ, 0)
            },
            {
                AreaRule(INHIBIT, DET, 0), AreaRule(INHIBIT, ADJ, 0), AreaRule(INHIBIT, PREP_P, 0), AreaRule(INHIBIT, PREP, 0),
                FiberRule(INHIBIT, LEX, SUBJ, 0), FiberRule(INHIBIT, LEX, OBJ, 0), FiberRule(INHIBIT, LEX, PREP_P, 0),
                FiberRule(INHIBIT, ADJ, SUBJ, 0), FiberRule(INHIBIT, ADJ, OBJ, 0), FiberRule(INHIBIT, ADJ, PREP_P, 0),
                FiberRule(INHIBIT, DET, SUBJ, 0), FiberRule(INHIBIT, DET, OBJ, 0), FiberRule(INHIBIT, DET, PREP_P, 0),
                FiberRule(INHIBIT, VERB, OBJ, 0), FiberRule(INHIBIT, PREP_P, PREP, 0), FiberRule(INHIBIT, PREP_P, VERB, 0),
                FiberRule(DISINHIBIT, LEX, SUBJ, 1), FiberRule(DISINHIBIT, LEX, OBJ, 1),
                FiberRule(DISINHIBIT, DET, SUBJ, 1), FiberRule(DISINHIBIT, DET, OBJ, 1),
                FiberRule(DISINHIBIT, ADJ, SUBJ, 1), FiberRule(DISINHIBIT, ADJ, OBJ, 1),
                FiberRule(INHIBIT, PREP_P, SUBJ, 0), FiberRule(INHIBIT, PREP_P, OBJ, 0),
                FiberRule(INHIBIT, VERB, ADJ, 0)
            }
        }},
        {"people", Lexeme{5,
            {
                FiberRule(DISINHIBIT, LEX, SUBJ, 0), FiberRule(DISINHIBIT, LEX, OBJ, 0),
                FiberRule(DISINHIBIT, LEX, PREP_P, 0), FiberRule(DISINHIBIT, DET, SUBJ, 0),
                FiberRule(DISINHIBIT, DET, OBJ, 0), FiberRule(DISINHIBIT, DET, PREP_P, 0),
                FiberRule(DISINHIBIT, ADJ, SUBJ, 0), FiberRule(DISINHIBIT, ADJ, OBJ, 0),
                FiberRule(DISINHIBIT, ADJ, PREP_P, 0), FiberRule(DISINHIBIT, VERB, OBJ, 0),
                FiberRule(DISINHIBIT, PREP_P, PREP, 0), FiberRule(DISINHIBIT, PREP_P, SUBJ, 0),
                FiberRule(DISINHIBIT, PREP_P, OBJ, 0)
            },
            {
                AreaRule(INHIBIT, DET, 0), AreaRule(INHIBIT, ADJ, 0), AreaRule(INHIBIT, PREP_P, 0), AreaRule(INHIBIT, PREP, 0),
                FiberRule(INHIBIT, LEX, SUBJ, 0), FiberRule(INHIBIT, LEX, OBJ, 0), FiberRule(INHIBIT, LEX, PREP_P, 0),
                FiberRule(INHIBIT, ADJ, SUBJ, 0), FiberRule(INHIBIT, ADJ, OBJ, 0), FiberRule(INHIBIT, ADJ, PREP_P, 0),
                FiberRule(INHIBIT, DET, SUBJ, 0), FiberRule(INHIBIT, DET, OBJ, 0), FiberRule(INHIBIT, DET, PREP_P, 0),
                FiberRule(INHIBIT, VERB, OBJ, 0), FiberRule(INHIBIT, PREP_P, PREP, 0), FiberRule(INHIBIT, PREP_P, VERB, 0),
                FiberRule(DISINHIBIT, LEX, SUBJ, 1), FiberRule(DISINHIBIT, LEX, OBJ, 1),
                FiberRule(DISINHIBIT, DET, SUBJ, 1), FiberRule(DISINHIBIT, DET, OBJ, 1),
                FiberRule(DISINHIBIT, ADJ, SUBJ, 1), FiberRule(DISINHIBIT, ADJ, OBJ, 1),
                FiberRule(INHIBIT, PREP_P, SUBJ, 0), FiberRule(INHIBIT, PREP_P, OBJ, 0),
                FiberRule(INHIBIT, VERB, ADJ, 0)
            }
        }},
        {"chase", Lexeme{6,
            {
                FiberRule(DISINHIBIT, LEX, VERB, 0), FiberRule(DISINHIBIT, VERB, SUBJ, 0),
                FiberRule(DISINHIBIT, VERB, ADVERB, 0), AreaRule(DISINHIBIT, ADVERB, 1)
            },
            {
                FiberRule(INHIBIT, LEX, VERB, 0), AreaRule(DISINHIBIT, OBJ, 0),
                AreaRule(INHIBIT, SUBJ, 0), AreaRule(INHIBIT, ADVERB, 0),
                FiberRule(DISINHIBIT, PREP_P, VERB, 0)
            }
        }},
        {"love", Lexeme{7,
            {
                FiberRule(DISINHIBIT, LEX, VERB, 0), FiberRule(DISINHIBIT, VERB, SUBJ, 0),
                FiberRule(DISINHIBIT, VERB, ADVERB, 0), AreaRule(DISINHIBIT, ADVERB, 1)
            },
            {
                FiberRule(INHIBIT, LEX, VERB, 0), AreaRule(DISINHIBIT, OBJ, 0),
                AreaRule(INHIBIT, SUBJ, 0), AreaRule(INHIBIT, ADVERB, 0),
                FiberRule(DISINHIBIT, PREP_P, VERB, 0)
            }
        }},
        {"bite", Lexeme{8,
            {
                FiberRule(DISINHIBIT, LEX, VERB, 0), FiberRule(DISINHIBIT, VERB, SUBJ, 0),
                FiberRule(DISINHIBIT, VERB, ADVERB, 0), AreaRule(DISINHIBIT, ADVERB, 1)
            },
            {
                FiberRule(INHIBIT, LEX, VERB, 0), AreaRule(DISINHIBIT, OBJ, 0),
                AreaRule(INHIBIT, SUBJ, 0), AreaRule(INHIBIT, ADVERB, 0),
                FiberRule(DISINHIBIT, PREP_P, VERB, 0)
            }
        }},
        {"of", Lexeme{9,
            {
                AreaRule(DISINHIBIT, PREP, 0), FiberRule(DISINHIBIT, LEX, PREP, 0)
            },
            {
                FiberRule(INHIBIT, LEX, PREP, 0), AreaRule(DISINHIBIT, PREP_P, 0),
                FiberRule(INHIBIT, LEX, SUBJ, 1), FiberRule(INHIBIT, LEX, OBJ, 1),
                FiberRule(INHIBIT, DET, SUBJ, 1), FiberRule(INHIBIT, DET, OBJ, 1),
                FiberRule(INHIBIT, ADJ, SUBJ, 1), FiberRule(INHIBIT, ADJ, OBJ, 1)
            }
        }},
        {"big", Lexeme{10,
            {
                AreaRule(DISINHIBIT, ADJ, 0), FiberRule(DISINHIBIT, LEX, ADJ, 0)
            },
            {
                FiberRule(INHIBIT, LEX, ADJ, 0), FiberRule(INHIBIT, VERB, ADJ, 0)
            }
        }},
        {"bad", Lexeme{11,
            {
                AreaRule(DISINHIBIT, ADJ, 0), FiberRule(DISINHIBIT, LEX, ADJ, 0)
            },
            {
                FiberRule(INHIBIT, LEX, ADJ, 0), FiberRule(INHIBIT, VERB, ADJ, 0)
            }
        }},
        {"run", Lexeme{12,
            {
                FiberRule(DISINHIBIT, LEX, VERB, 0), FiberRule(DISINHIBIT, VERB, SUBJ, 0),
                FiberRule(DISINHIBIT, VERB, ADVERB, 0), AreaRule(DISINHIBIT, ADVERB, 1)
            },
            {
                FiberRule(INHIBIT, LEX, VERB, 0), AreaRule(INHIBIT, SUBJ, 0),
                AreaRule(INHIBIT, ADVERB, 0), FiberRule(DISINHIBIT, PREP_P, VERB, 0)
            }
        }},
        {"fly", Lexeme{13,
            {
                FiberRule(DISINHIBIT, LEX, VERB, 0), FiberRule(DISINHIBIT, VERB, SUBJ, 0),
                FiberRule(DISINHIBIT, VERB, ADVERB, 0), AreaRule(DISINHIBIT, ADVERB, 1)
            },
            {
                FiberRule(INHIBIT, LEX, VERB, 0), AreaRule(INHIBIT, SUBJ, 0),
                AreaRule(INHIBIT, ADVERB, 0), FiberRule(DISINHIBIT, PREP_P, VERB, 0)
            }
        }},
        {"quickly", Lexeme{14,
            {
                AreaRule(DISINHIBIT, ADVERB, 0), FiberRule(DISINHIBIT, LEX, ADVERB, 0)
            },
            {
                FiberRule(INHIBIT, LEX, ADVERB, 0), AreaRule(INHIBIT, ADVERB, 1)
            }
        }},
        {"in", Lexeme{15,
            {
                AreaRule(DISINHIBIT, PREP, 0), FiberRule(DISINHIBIT, LEX, PREP, 0)
            },
            {
                FiberRule(INHIBIT, LEX, PREP, 0), AreaRule(DISINHIBIT, PREP_P, 0),
                FiberRule(INHIBIT, LEX, SUBJ, 1), FiberRule(INHIBIT, LEX, OBJ, 1),
                FiberRule(INHIBIT, DET, SUBJ, 1), FiberRule(INHIBIT, DET, OBJ, 1),
                FiberRule(INHIBIT, ADJ, SUBJ, 1), FiberRule(INHIBIT, ADJ, OBJ, 1)
            }
        }},
        {"are", Lexeme{16,
            {
                FiberRule(DISINHIBIT, LEX, VERB, 0), FiberRule(DISINHIBIT, VERB, SUBJ, 0)
            },
            {
                FiberRule(INHIBIT, LEX, VERB, 0), AreaRule(DISINHIBIT, OBJ, 0),
                AreaRule(INHIBIT, SUBJ, 0), FiberRule(DISINHIBIT, ADJ, VERB, 0)
            }
        }},
        {"man", Lexeme{17,
            {
                FiberRule(DISINHIBIT, LEX, SUBJ, 0), FiberRule(DISINHIBIT, LEX, OBJ, 0),
                FiberRule(DISINHIBIT, LEX, PREP_P, 0), FiberRule(DISINHIBIT, DET, SUBJ, 0),
                FiberRule(DISINHIBIT, DET, OBJ, 0), FiberRule(DISINHIBIT, DET, PREP_P, 0),
                FiberRule(DISINHIBIT, ADJ, SUBJ, 0), FiberRule(DISINHIBIT, ADJ, OBJ, 0),
                FiberRule(DISINHIBIT, ADJ, PREP_P, 0), FiberRule(DISINHIBIT, VERB, OBJ, 0),
                FiberRule(DISINHIBIT, PREP_P, PREP, 0), FiberRule(DISINHIBIT, PREP_P, SUBJ, 0),
                FiberRule(DISINHIBIT, PREP_P, OBJ, 0)
            },
            {
                AreaRule(INHIBIT, DET, 0), AreaRule(INHIBIT, ADJ, 0), AreaRule(INHIBIT, PREP_P, 0), AreaRule(INHIBIT, PREP, 0),
                FiberRule(INHIBIT, LEX, SUBJ, 0), FiberRule(INHIBIT, LEX, OBJ, 0), FiberRule(INHIBIT, LEX, PREP_P, 0),
                FiberRule(INHIBIT, ADJ, SUBJ, 0), FiberRule(INHIBIT, ADJ, OBJ, 0), FiberRule(INHIBIT, ADJ, PREP_P, 0),
                FiberRule(INHIBIT, DET, SUBJ, 0), FiberRule(INHIBIT, DET, OBJ, 0), FiberRule(INHIBIT, DET, PREP_P, 0),
                FiberRule(INHIBIT, VERB, OBJ, 0), FiberRule(INHIBIT, PREP_P, PREP, 0), FiberRule(INHIBIT, PREP_P, VERB, 0),
                FiberRule(DISINHIBIT, LEX, SUBJ, 1), FiberRule(DISINHIBIT, LEX, OBJ, 1),
                FiberRule(DISINHIBIT, DET, SUBJ, 1), FiberRule(DISINHIBIT, DET, OBJ, 1),
                FiberRule(DISINHIBIT, ADJ, SUBJ, 1), FiberRule(DISINHIBIT, ADJ, OBJ, 1),
                FiberRule(INHIBIT, PREP_P, SUBJ, 0), FiberRule(INHIBIT, PREP_P, OBJ, 0),
                FiberRule(INHIBIT, VERB, ADJ, 0)
            }
        }},
        {"woman", Lexeme{18,
            {
                FiberRule(DISINHIBIT, LEX, SUBJ, 0), FiberRule(DISINHIBIT, LEX, OBJ, 0),
                FiberRule(DISINHIBIT, LEX, PREP_P, 0), FiberRule(DISINHIBIT, DET, SUBJ, 0),
                FiberRule(DISINHIBIT, DET, OBJ, 0), FiberRule(DISINHIBIT, DET, PREP_P, 0),
                FiberRule(DISINHIBIT, ADJ, SUBJ, 0), FiberRule(DISINHIBIT, ADJ, OBJ, 0),
                FiberRule(DISINHIBIT, ADJ, PREP_P, 0), FiberRule(DISINHIBIT, VERB, OBJ, 0),
                FiberRule(DISINHIBIT, PREP_P, PREP, 0), FiberRule(DISINHIBIT, PREP_P, SUBJ, 0),
                FiberRule(DISINHIBIT, PREP_P, OBJ, 0)
            },
            {
                AreaRule(INHIBIT, DET, 0), AreaRule(INHIBIT, ADJ, 0), AreaRule(INHIBIT, PREP_P, 0), AreaRule(INHIBIT, PREP, 0),
                FiberRule(INHIBIT, LEX, SUBJ, 0), FiberRule(INHIBIT, LEX, OBJ, 0), FiberRule(INHIBIT, LEX, PREP_P, 0),
                FiberRule(INHIBIT, ADJ, SUBJ, 0), FiberRule(INHIBIT, ADJ, OBJ, 0), FiberRule(INHIBIT, ADJ, PREP_P, 0),
                FiberRule(INHIBIT, DET, SUBJ, 0), FiberRule(INHIBIT, DET, OBJ, 0), FiberRule(INHIBIT, DET, PREP_P, 0),
                FiberRule(INHIBIT, VERB, OBJ, 0), FiberRule(INHIBIT, PREP_P, PREP, 0), FiberRule(INHIBIT, PREP_P, VERB, 0),
                FiberRule(DISINHIBIT, LEX, SUBJ, 1), FiberRule(DISINHIBIT, LEX, OBJ, 1),
                FiberRule(DISINHIBIT, DET, SUBJ, 1), FiberRule(DISINHIBIT, DET, OBJ, 1),
                FiberRule(DISINHIBIT, ADJ, SUBJ, 1), FiberRule(DISINHIBIT, ADJ, OBJ, 1),
                FiberRule(INHIBIT, PREP_P, SUBJ, 0), FiberRule(INHIBIT, PREP_P, OBJ, 0),
                FiberRule(INHIBIT, VERB, ADJ, 0)
            }
        }}
    };
}


ParserBrain::ParserBrain(float p) : nemo::Brain(p, 0.0, 1.0, 42) {
    initializeStates();
}

void ParserBrain::initializeStates() {
    for (const auto& from_area : all_areas) {
        fiber_states[from_area] = std::unordered_map<std::string, std::set<int>>();
        for (const auto& to_area : all_areas) {
            fiber_states[from_area][to_area].insert(0);
        }
    }
    for (const auto& area : all_areas) {
        area_states[area].insert(0);
    }
    for (const auto& area : initial_areas) {
        area_states[area].erase(0);
    }
}

void ParserBrain::applyFiberRule(const FiberRule& rule) {
    if (rule.action == "INHIBIT") {
        fiber_states[rule.area1][rule.area2].insert(rule.index);
        fiber_states[rule.area2][rule.area1].insert(rule.index);
    } else if (rule.action == "DISINHIBIT") {
        fiber_states[rule.area1][rule.area2].erase(rule.index);
        fiber_states[rule.area2][rule.area1].erase(rule.index);
    }
}

void ParserBrain::applyAreaRule(const AreaRule& rule) {
    if (rule.action == "INHIBIT") {
        area_states[rule.area].insert(rule.index);
    } else if (rule.action == "DISINHIBIT") {
        area_states[rule.area].erase(rule.index);
    }
}

void ParserBrain::applyRule(const std::variant<FiberRule, AreaRule>& rule) {
    if (std::holds_alternative<FiberRule>(rule)) {
        applyFiberRule(std::get<FiberRule>(rule));
    } else if (std::holds_alternative<AreaRule>(rule)) {
        applyAreaRule(std::get<AreaRule>(rule));
    }
}

void ParserBrain::parseProject() {
    auto project_map = getProjectMap();
    rememberFibers(project_map);
    project({}, project_map, -2);
}

void ParserBrain::rememberFibers(const std::unordered_map<std::string, std::set<std::string>>& project_map) {
    for (const auto& [from_area, to_areas] : project_map) {
        activated_fibers[from_area].insert(to_areas.begin(), to_areas.end());
    }
}

bool ParserBrain::recurrent(const std::string& area) {
    return std::find(recurrent_areas.begin(), recurrent_areas.end(), area) != recurrent_areas.end();
}

std::unordered_map<std::string, std::set<std::string>> ParserBrain::getProjectMap() {
    std::unordered_map<std::string, std::set<std::string>> proj_map;
    for (const auto& area1 : all_areas) {
        if (area_states[area1].empty()) {
            for (const auto& area2 : all_areas) {
                if (area1 == "LEX" && area2 == "LEX") {
                    continue;
                }
                if (area_states[area2].empty()) {
                    if (fiber_states[area1][area2].empty()) {
                        if (!GetArea(area1).activated.empty()) {
                            proj_map[area1].insert(area2);
                        }
                        if (!GetArea(area2).activated.empty()) {
                            proj_map[area2].insert(area1);
                        }
                    }
                }
            }
        }
    }
    return proj_map;
}

void ParserBrain::activateWord(const std::string& area_name, const std::string& word) {
    auto& area = GetArea(area_name);
    uint32_t k = area.k;
    uint32_t assembly_start = lexeme_dict[word]["index"][0] * k;
    area.activated.resize(k);
    for (uint32_t i = 0; i < k; ++i) {
        area.activated[i] = assembly_start + i;
    }
    area.is_fixed = true;
}

std::string ParserBrain::getWord(const std::string& area_name, float min_overlap) {
    const auto& area = GetArea(area_name);
    if (area.activated.empty()) {
        throw std::runtime_error("Cannot get word because no assembly in " + area_name);
    }
    std::set<uint32_t> winners(area.activated.begin(), area.activated.end());
    uint32_t area_k = area.k;
    uint32_t threshold = min_overlap * area_k;

    for (const auto& [word, lexeme] : lexeme_dict) {
        uint32_t word_index = lexeme.at("index")[0];
        uint32_t word_assembly_start = word_index * area_k;
        std::set<uint32_t> word_assembly;
        for (uint32_t i = 0; i < area_k; ++i) {
            word_assembly.insert(word_assembly_start + i);
        }
        std::vector<uint32_t> intersection;
        std::set_intersection(winners.begin(), winners.end(),
                              word_assembly.begin(), word_assembly.end(),
                              std::back_inserter(intersection));
        if (intersection.size() >= threshold) {
            return word;
        }
    }
    return "<NON-WORD>";
}

std::unordered_map<std::string, std::set<std::string>> ParserBrain::getActivatedFibers() {
    std::unordered_map<std::string, std::set<std::string>> pruned_activated_fibers;
    for (const auto& [from_area, to_areas] : activated_fibers) {
        for (const auto& to_area : to_areas) {
            if (readout_rules[from_area].count(to_area)) {
                pruned_activated_fibers[from_area].insert(to_area);
            }
        }
    }
    return pruned_activated_fibers;
}

// Other methods for EnglishParserBrain, ParserDebugger, and utility functions will follow here...

} // namespace parser

namespace parser {

EnglishParserBrain::EnglishParserBrain(float p, int non_LEX_n, int non_LEX_k, int LEX_k,
                                       float default_beta, float LEX_beta, float recurrent_beta, float interarea_beta, bool verbose)
    : ParserBrain(p) {
    this->verbose = verbose;
    all_areas = {LEX, DET, SUBJ, OBJ, VERB, ADJ, ADVERB, PREP, PREP_P};
    recurrent_areas = {SUBJ, OBJ, VERB, ADJ, ADVERB, PREP, PREP_P};
    initial_areas = {LEX, SUBJ, VERB};
    readout_rules = {
        {VERB, {LEX, SUBJ, OBJ, PREP_P, ADVERB, ADJ}},
        {SUBJ, {LEX, DET, ADJ, PREP_P}},
        {OBJ, {LEX, DET, ADJ, PREP_P}},
        {PREP_P, {LEX, PREP, ADJ, DET}},
        {PREP, {LEX}},
        {ADJ, {LEX}},
        {DET, {LEX}},
        {ADVERB, {LEX}},
        {LEX, {}}
    };

    int LEX_n = LEX_SIZE * LEX_k;
    AddArea(LEX, LEX_n, LEX_k, true, true);
    AddArea(SUBJ, non_LEX_n, non_LEX_k, true, false);
    AddArea(OBJ, non_LEX_n, non_LEX_k, true, false);
    AddArea(VERB, non_LEX_n, non_LEX_k, true, false);
    AddArea(ADJ, non_LEX_n, non_LEX_k, true, false);
    AddArea(PREP, non_LEX_n, non_LEX_k, true, false);
    AddArea(PREP_P, non_LEX_n, non_LEX_k, true, false);
    AddArea(DET, non_LEX_n, LEX_k, true, false);
    AddArea(ADVERB, non_LEX_n, non_LEX_k, true, false);

    std::unordered_map<std::string, std::vector<std::pair<std::string, float>>> custom_plasticities;
    for (const auto& area : recurrent_areas) {
        custom_plasticities[LEX].emplace_back(area, LEX_beta);
        custom_plasticities[area].emplace_back(LEX, LEX_beta);
        custom_plasticities[area].emplace_back(area, recurrent_beta);
        for (const auto& other_area : recurrent_areas) {
            if (other_area != area) {
                custom_plasticities[area].emplace_back(other_area, interarea_beta);
            }
        }
    }
    // UpdatePlasticities is a method in Brain class that should be implemented.
    // UpdatePlasticities(custom_plasticities); // You need to define this method
}

std::unordered_map<std::string, std::set<std::string>> EnglishParserBrain::getProjectMap() {
    auto proj_map = ParserBrain::getProjectMap();
    if (proj_map.find(LEX) != proj_map.end() && proj_map[LEX].size() > 2) {
        throw std::runtime_error("LEX projecting into many areas: " + std::to_string(proj_map[LEX].size()));
    }
    return proj_map;
}

std::string EnglishParserBrain::getWord(const std::string& area_name, float min_overlap) {
    std::string word = ParserBrain::getWord(area_name, min_overlap);
    if (!word.empty()) {
        return word;
    }
    return "<NON-WORD>";
}

} // namespace parser


namespace parser {

ParserDebugger::ParserDebugger(ParserBrain& brain, const std::vector<std::string>& all_areas, const std::vector<std::string>& explicit_areas)
    : b(brain), all_areas(all_areas), explicit_areas(explicit_areas) {}

void ParserDebugger::run() {
    std::string command;
    std::cout << "DEBUGGER: ENTER to continue, 'P' for PEAK \n";
    std::getline(std::cin, command);
    while (!command.empty()) {
        if (command == "P") {
            peak();
            return;
        } else {
            std::cout << "DEBUGGER: Command not recognized...\n";
        }
        std::cout << "DEBUGGER: ENTER to continue, 'P' for PEAK \n";
        std::getline(std::cin, command);
    }
}

void ParserDebugger::peak() {
    std::unordered_map<std::string, int> remove_map;
    b.disablePlasticity(true); // Assuming this method is defined in Brain class
    b.saveWinners(true); // Assuming this method is defined in Brain class

    for (const auto& area : all_areas) {
        b.GetArea(area).is_fixed = false;
    }
    while (true) {
        std::string test_proj_map_string;
        std::cout << "DEBUGGER: enter projection map, eg. {\"VERB\": [\"LEX\"]}, or ENTER to quit\n";
        std::getline(std::cin, test_proj_map_string);
        if (test_proj_map_string.empty()) {
            break;
        }
        Json::Value test_proj_map;
        Json::Reader reader;
        reader.parse(test_proj_map_string, test_proj_map);
        std::set<std::string> to_area_set;
        for (const auto& from_area : test_proj_map.getMemberNames()) {
            for (const auto& to_area : test_proj_map[from_area]) {
                to_area_set.insert(to_area.asString());
                if (b.GetArea(to_area.asString()).saved_winners.empty()) {
                    b.GetArea(to_area.asString()).saved_winners.push_back(b.GetArea(to_area.asString()).activated);
                }
            }
        }
        for (const auto& to_area : to_area_set) {
            remove_map[to_area]++;
        }
        b.Project({}, test_proj_map, -1); // Assuming Project method can handle Json::Value
        for (const auto& area : explicit_areas) {
            if (to_area_set.find(area) != to_area_set.end()) {
                std::string area_word = b.getWord(area);
                std::cout << "DEBUGGER: in explicit area " << area << ", got: " << area_word << std::endl;
            }
        }

        std::string print_assemblies;
        std::cout << "DEBUGGER: print assemblies in areas? Eg. 'LEX,VERB' or ENTER to cont\n";
        std::getline(std::cin, print_assemblies);
        if (print_assemblies.empty()) {
            continue;
        }
        std::stringstream ss(print_assemblies);
        std::string print_area;
        while (std::getline(ss, print_area, ',')) {
            std::cout << "DEBUGGER: Printing assembly in area " << print_area << std::endl;
            for (const auto& winner : b.GetArea(print_area).activated) {
                std::cout << winner << " ";
            }
            std::cout << std::endl;
            if (std::find(explicit_areas.begin(), explicit_areas.end(), print_area) != explicit_areas.end()) {
                std::string word = b.getWord(print_area);
                std::cout << "DEBUGGER: in explicit area got assembly = " << word << std::endl;
            }
        }
    }

    // Restore assemblies and weights
    for (const auto& [area, num_test_projects] : remove_map) {
        auto& area_obj = b.GetArea(area);
        area_obj.activated = area_obj.saved_winners[0];
        area_obj.saved_winners.erase(area_obj.saved_winners.begin(), area_obj.saved_winners.begin() + num_test_projects);
    }
    b.disablePlasticity(false); // Assuming this method is defined in Brain class
    b.saveWinners(false); // Assuming this method is defined in Brain class
    for (const auto& area : all_areas) {
        b.GetArea(area).saved_winners.clear();
    }
}

void potentiateWordInLEX(ParserBrain& b, const std::string& word, int rounds) {
    b.activateWord(LEX, word);
    for (int i = 0; i < rounds; ++i) {
        b.Project({}, {{LEX, {LEX}}});
    }
}

}

namespace parser {
void parse(const std::string& sentence, const std::string& language, float p, int LEX_k,
           int project_rounds, bool verbose, bool debug, ReadoutMethod readout_method) {
    EnglishParserBrain b(p, 2000, 100, LEX_k, 0.2, 1.0, 0.05, 0.5, verbose);
    ParserDebugger debugger(b, AREAS, EXPLICIT_AREAS);

    std::vector<std::string> sentence_words;
    std::istringstream iss(sentence);
    for (std::string s; iss >> s; ) {
        sentence_words.push_back(s);
    }

    std::vector<std::tuple<std::string, std::string, std::string>> dependencies;

    auto read_out = [&](const std::string& area, const std::map<std::string, std::set<std::string>>& mapping) {
        auto to_areas = mapping.at(area);
        b.Project({}, {{area, to_areas}}, -1);

        std::string this_word = b.getWord(LEX);
        for (const auto& to_area : to_areas) {
            if (to_area == LEX) continue;
            b.Project({}, {{to_area, {LEX}}}, -1);
            std::string other_word = b.getWord(LEX);
            dependencies.emplace_back(this_word, other_word, to_area);
        }
        for (const auto& to_area : to_areas) {
            if (to_area != LEX) {
                read_out(to_area, mapping);
            }
        }
    };

    for (const auto& word : sentence_words) {
        b.activateWord(LEX, word);

        const auto& lexeme = b.lexeme_dict.at(word);
        for (const auto& rule : lexeme.pre_rules) {
            b.applyRule(rule);
        }

        auto proj_map = b.getProjectMap();
        for (const auto& area : proj_map) {
            if (proj_map[LEX].find(area.first) == proj_map[LEX].end() && area.first != LEX) {
                b.GetArea(area.first).is_fixed = true;
            } else {
                b.GetArea(area.first).is_fixed = false;
                b.GetArea(area.first).activated.clear();
            }
        }

        for (int i = 0; i < project_rounds; ++i) {
            b.parseProject();
        }
        for (const auto& rule : lexeme.post_rules) {
            b.applyRule(rule);
        }

        if (debug) {
            std::cout << "Starting debugger after the word " << word << std::endl;
            debugger.run();
        }
    }

    b.disablePlasticity(true);
    for (const auto& area : AREAS) {
        b.GetArea(area).is_fixed = false;
    }

    if (readout_method == ReadoutMethod::FIXED_MAP_READOUT) {
        read_out(VERB, ENGLISH_READOUT_RULES);
    } else if (readout_method == ReadoutMethod::FIBER_READOUT) {
        auto activated_fibers = b.getActivatedFibers();
        read_out(VERB, activated_fibers);
    }

    TreeNode* parse_tree = buildDependencyTree(dependencies, std::get<0>(dependencies[0]));
    printTree(parse_tree);
}

int main() {
    parse("people bite dogs");
    return 0;
}

}