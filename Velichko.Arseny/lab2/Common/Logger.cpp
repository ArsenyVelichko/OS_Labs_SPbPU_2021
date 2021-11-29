#include <iostream>
#include <fstream>

#include "Logger.h"

//Initialization with dummy deleter
Logger::Logger() : m_os(&std::cout, [](const std::ostream*) {}) {}

Logger::Logger(const std::string& filename) : m_os(new std::ofstream(filename)) {}

void Logger::initLevelNames() {
	if (!m_levelNames.empty()) { return; }

	m_levelNames.resize(Level_Count);

	m_levelNames[Level_Debug] = "Debug";
	m_levelNames[Level_Info] = "Info";
	m_levelNames[Level_Warning] = "Warning";
	m_levelNames[Level_Error] = "Error";
	m_levelNames[Level_WhatATerribleFailure] = "Wtf";
}

std::string_view Logger::getLevelName(Level level) {
	initLevelNames();
	return m_levelNames[level];
}

void Logger::log(Level level, const std::string& message,
				 const std::string& file, const std::string& function, int line) {

	*m_os << getLevelName(level) << ": " << message << '\n'
		  << "File: " << file << '\n'
		  << "Function: " << function << '\n'
		  << "Line: " << line << '\n';
};
