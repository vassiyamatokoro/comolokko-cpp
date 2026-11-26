#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>
#include <raylib.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

using json = nlohmann::json;

int main() {
    try {
        std::ifstream logCfgFile("config/logger_config.json");
        if (!logCfgFile.is_open()) {
            std::cerr << "Hata: config/logger_config.json acilamadi!\n";
            return 1;
        }
        json logCfg = json::parse(logCfgFile);

        const std::string loggerName = logCfg.at("logger_name").get<std::string>();
        const std::string logFile = logCfg.at("log_file").get<std::string>();
        const std::string logPattern = logCfg.at("log_pattern").get<std::string>();

        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFile, true);
        std::vector<spdlog::sink_ptr> sinks{consoleSink, fileSink};

        auto logger = std::make_shared<spdlog::logger>(loggerName, sinks.begin(), sinks.end());
        logger->set_pattern(logPattern);
        spdlog::set_default_logger(logger);

        spdlog::info("Log sistemi baslatildi.");

        std::ifstream gameCfgFile("config/game_config.json");
        if (!gameCfgFile.is_open()) {
            spdlog::error("config/game_config.json bulunamadi!");
            return 1;
        }
        json gameCfg = json::parse(gameCfgFile);

        const int screenWidth = gameCfg.at("window").at("width").get<int>();
        const int screenHeight = gameCfg.at("window").at("height").get<int>();
        const int targetFps = gameCfg.at("window").at("target_fps").get<int>();
        const std::string windowTitle = gameCfg.at("window").at("title").get<std::string>();

        const Color bgColor{
            gameCfg.at("colors").at("background_r").get<unsigned char>(),
            gameCfg.at("colors").at("background_g").get<unsigned char>(),
            gameCfg.at("colors").at("background_b").get<unsigned char>(),
            gameCfg.at("colors").at("background_a").get<unsigned char>()
        };

        const std::string welcomeText = gameCfg.at("ui").at("welcome_text").get<std::string>();
        const int textX = gameCfg.at("ui").at("text_pos_x").get<int>();
        const int textY = gameCfg.at("ui").at("text_pos_y").get<int>();
        const int fontSize = gameCfg.at("ui").at("font_size").get<int>();

        InitWindow(screenWidth, screenHeight, windowTitle.c_str());
        SetTargetFPS(targetFps);
        spdlog::info("Raylib penceresi acildi ({}x{}).", screenWidth, screenHeight);

        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(bgColor);
            DrawText(welcomeText.c_str(), textX, textY, fontSize, DARKGRAY);
            EndDrawing();
        }

        CloseWindow();
        spdlog::info("Oyun penceresi guvenle kapatildi.");
    } catch (const std::exception& e) {
        std::cerr << "Kritik hata: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
