#ifndef ANALYTICS_H
#define ANALYTICS_H

#include <vector>
#include <string>
#include "Packet.h"

void exportReport(const std::vector<Packet>& history, const std::string& filename);

#endif