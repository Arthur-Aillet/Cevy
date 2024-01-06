/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** input
*/

#include "Input.hpp"
#include "ecs.hpp"
#include "Commands.hpp"
#include "EntityCommands.hpp"

void update_all_inputs(cevy::ecs::Query<Input<Int>> inputs) {
    for (auto input : inputs) {
        std::get<0>(input).update_keys();
    }
}