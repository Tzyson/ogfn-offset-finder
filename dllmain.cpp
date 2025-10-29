#include "pch.h"

uint64_t imageBase = *(uint64_t*)(__readgsqword(0x60) + 0x10);

uint64_t GetPatternOffset(const std::string& pattern) {
    uint64_t patternAddress = Memcury::Scanner::FindPattern(pattern.c_str()).Get();
    return patternAddress - imageBase;
}

void InputLoop() {
    char buffer[256];
    while (true) {
        printf("Enter pattern: ");
        if (fgets(buffer, sizeof(buffer), stdin)) {
            std::string pattern(buffer);
            pattern.erase(std::remove(pattern.begin(), pattern.end(), '\n'), pattern.end());
            try {
                uint64_t offset = GetPatternOffset(pattern);
                printf("Offset: 0x%llX\n", offset);
            }
            catch (...) {
                printf("Error, probably pattern is invalid\n");
            }
        }
    }
}

void Main() {
    AllocConsole();
    SetConsoleTitleA("TZY OFFSET FINDER - https://github.com/tzyson/ogfn-offset-finder");
    FILE* f_out; FILE* f_err; FILE* f_in;
    freopen_s(&f_out, "CONOUT$", "w", stdout);
    freopen_s(&f_err, "NUL", "w", stderr);
    freopen_s(&f_in, "CONIN$", "r", stdin);
    InputLoop();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        std::thread(Main).detach();
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}