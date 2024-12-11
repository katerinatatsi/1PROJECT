#ifndef MEDOID_HPP
#define MEDOID_HPP

#include <unordered_map>
#include <string>


std::unordered_map<int, int> FindMedoid(const std::string& dataset_filename, int τ);

#endif // MEDOID_HPP
