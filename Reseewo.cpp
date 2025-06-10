/*
Reseewo
This program is used to close the seewo easinote process running on the backend to solve the situation that the seewo easinote is running on the backend but cannot be opened!
Copyright (c) 2025 Creaconception Studio.Published by Wajirui. 

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Reseewo
本程序用于关闭在后端运行的希沃白板进程来解决希沃白板在后端运行但打不开的情况！
Copyright (c) 2025 Creaconception Studio.Published by Wajirui. 

本程序为自由软件：你可以根据自由软件基金会发布的
GNU通用公共许可协议第三版或者更新的版本（任选）
的规定对本程序进行再分发和/或修改。

我们希望分发的这款程序对你是有用的，但是我们不提
供任何保证；也不提供任何对可销售性和适用于特定目 的的默示保证。详情请查阅GNU通用公共许可协议。

你应已随同本程序收到一份GNU通用公共许可协议的副本，
如未收到，请查阅http://www.gnu.org/licenses/

https://wajirui.creaconception.dpdns.org
https://github.com/waji-rui/Reseewo

*/

#include <windows.h>
#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <algorithm>
#include <tlhelp32.h>
#include <cstring>
#include <cstdio>
using namespace std;

void setColor(int foreground, int background = 0) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 
        (background << 4) | // 背景色占高4位
        (foreground & 0x0F) // 前景色占低4位
    );
}

// 全局变量声明
std::string ShutdownCmd = "EasiNote.exe";
std::string WindowName = "希沃白板";
std::string OpenCmd = "C:\\Program Files (x86)\\Seewo\\EasiNote5\\swenlauncher\\swenlauncher.exe";

// 辅助函数声明
void trim(std::string &s);
std::string replaceBackslashes(const std::string &input);
void loadConfig();





// 全局变量，记录当前日志条数
int logCount = 0;

const char* windowN = "希沃白板";

// 函数声明
void writeLog(const string& message);
void rotateLogs();
string getCurrentTime();
void logMessage();

void SetReseewoWindowTitle()
{
    SetConsoleTitleA("Reseewo");  // 使用ANSI版本API设置窗口标题
}

// 主日志函数
void logMessage(const string& message) {
	//输出到控制台 
	cout<<message<<endl;
	
    // 获取当前时间
    string timeStr = getCurrentTime();
    
    // 构造完整日志信息
    string fullLog = "[" + timeStr + "] " + message;
    
    // 写入日志文件
    writeLog(fullLog);
    
    // 增加日志计数
    logCount++;
    
    // 检查是否需要轮转日志
    if (logCount >= 1000) {
        rotateLogs();
        logCount = 0;
    }
}

// 获取当前时间字符串
string getCurrentTime() {
    time_t now = time(0);
    tm* localTime = localtime(&now);
    
    char timeStr[20];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localTime);
    
    return string(timeStr);
}

// 写入日志到文件
void writeLog(const string& logEntry) {
    ofstream logFile("reseewo_log.txt", ios::app); // 以追加模式打开
    
    if (logFile.is_open()) {
        logFile << logEntry << endl;
        logFile.close();
    } else {
        cerr << "无法打开日志文件!" << endl;
    }
}

// 日志轮转函数，保留最新的1000条日志
void rotateLogs() {
    const int MAX_LINES = 1000;
    vector<string> logLines;
    string line;
    
    // 读取现有日志
    ifstream inFile("reseewo_log.txt");
    if (inFile.is_open()) {
        while (getline(inFile, line)) {
            logLines.push_back(line);
        }
        inFile.close();
    } else {
        cerr << "无法打开日志文件进行轮转!" << endl;
        return;
    }
    
    // 如果日志行数超过限制，只保留最新的部分
    if (logLines.size() > MAX_LINES) {
        int startIndex = logLines.size() - MAX_LINES;
        logLines.erase(logLines.begin(), logLines.begin() + startIndex);
    }
    
    // 写回文件（使用C++98兼容的循环）
    ofstream outFile("reseewo_log.txt");
    if (outFile.is_open()) {
        for (vector<string>::iterator it = logLines.begin(); it != logLines.end(); ++it) {
            outFile << *it << endl;
        }
        outFile.close();
    } else {
        cerr << "无法写入轮转后的日志文件!" << endl;
    }
}


void KeepConsoleOnTop() {
    char originalTitle[256];
    GetConsoleTitleA(originalTitle, sizeof(originalTitle)); // 保存原始标题

    const char* uniqueTitle = "MyTopMostConsole12345";
    SetConsoleTitleA(uniqueTitle); // 设置唯一标题

    Sleep(100); // 等待标题更新

    HWND hwnd = FindWindowA("ConsoleWindowClass", uniqueTitle);
    if (hwnd) {
        // 设置窗口置顶
        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    }

    SetConsoleTitleA(originalTitle); // 恢复原始标题
}




// 去除字符串首尾空白字符
void trim(std::string &s) {
    size_t start = s.find_first_not_of(" \t");
    if (start == std::string::npos) {
        s = "";
        return;
    }

    size_t end = s.find_last_not_of(" \t");
    s = s.substr(start, end - start + 1);
}

// 替换反斜杠为双反斜杠
std::string replaceBackslashes(const std::string &input) {
    std::string output;
    for (size_t i = 0; i < input.size(); ++i) {
        if (input[i] == '\\') {
            output += "\\\\";
        } else {
            output += input[i];
        }
    }
    return output;
}

// 加载配置文件
void loadConfig() {
    std::ifstream inFile("Open_and_Close.txt");
    
    // 文件不存在时创建默认配置
    if (!inFile) {
        logMessage("配置文件未找到，正在创建默认配置...");
        
        std::ofstream outFile("Open_and_Close.txt");
        if (outFile) {
            outFile << "Shutdown=EasiNote.exe\n"
                    << "windowName=希沃白板\n"
                    << "Open=C:\\Program Files (x86)\\Seewo\\EasiNote5\\swenlauncher\\swenlauncher.exe\n";
            outFile.close();
            logMessage("默认配置文件已创建");
            
            // 重新尝试打开文件
            inFile.open("Open_and_Close.txt");
        } else {
            logMessage("创建配置文件失败");
            return;
        }
    }

    logMessage("开始读取配置文件...");
    std::string line;
    while (std::getline(inFile, line)) {
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            
            // 去除首尾空白
            trim(key);
            trim(value);

            if (key == "Shutdown") {
                ShutdownCmd = value;
                logMessage("读取到Shutdown配置: " + ShutdownCmd);
            } else if (key == "windowName") {
                WindowName = value;
                logMessage("读取到windowName配置: " + WindowName);
            } else if (key == "Open") {
                OpenCmd = replaceBackslashes(value);
                logMessage("读取并处理Open路径: " + OpenCmd);
            }
        }
    }
    inFile.close();
    logMessage("配置文件读取完成，未读取到的配置将以默认配置运行");
}











// 窗口检查结构体
struct WindowCheckInfo {
    DWORD targetPid;
    bool found;
    const char* windowTitle;
};

// 枚举窗口回调函数
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    WindowCheckInfo* info = (WindowCheckInfo*)lParam;
    DWORD pid = 0;
    
    // 获取窗口进程ID
    GetWindowThreadProcessId(hwnd, &pid);
    if (pid == info->targetPid) {
        char title[256] = {0};
        // 获取窗口标题
        if (GetWindowTextA(hwnd, title, sizeof(title)-1)) {
            if (strstr(title, info->windowTitle) != NULL) {
                info->found = true;
                return FALSE; // 找到目标窗口，停止枚举
            }
        }
    }
    return TRUE; // 继续枚举
}

// 查找指定进程ID
std::vector<DWORD> FindProcessIds(const char* processName) {
    std::vector<DWORD> pids;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    
    if (snapshot == INVALID_HANDLE_VALUE) {
        logMessage("创建进程快照失败");
        return pids;
    }

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(snapshot, &pe)) {
        logMessage("获取第一个进程失败");
        CloseHandle(snapshot);
        return pids;
    }

    do {
        if (_stricmp(pe.szExeFile, processName) == 0) {
            pids.push_back(pe.th32ProcessID);
            logMessage("找到目标进程");
        }
    } while (Process32Next(snapshot, &pe));

    CloseHandle(snapshot);
    return pids;
}

// 检查进程是否需要跳过终止
bool ShouldSkipTermination(DWORD pid) {
    // 检查前台窗口
    HWND foregroundWnd = GetForegroundWindow();
    if (foregroundWnd) {
        DWORD foregroundPid;
        GetWindowThreadProcessId(foregroundWnd, &foregroundPid);
        
        if (foregroundPid == pid) {
            logMessage("进程拥有前台窗口，跳过");
            return true;
        }
    }

    // 检查特定窗口标题
    WindowCheckInfo info = { pid, false, windowN };
    EnumWindows(EnumWindowsProc, (LPARAM)&info);

    if (info.found) {
        logMessage("检测到特定窗口，跳过终止");
        return true;
    }

    return false;
}

// 终止进程函数
void TerminateTargetProcess(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (!hProcess) {
        logMessage("打开进程失败");
        return;
    }

    if (TerminateProcess(hProcess, 0)) {
        logMessage("成功终止进程");
    } else {
        logMessage("终止进程失败");
    }

    CloseHandle(hProcess);
}

// 主功能函数
void CloseBackgroundProcess(const char* processName, const char* windowNames = "希沃白板") {
	windowN = windowNames;
    logMessage("开始查找进程...");
    std::vector<DWORD> pids = FindProcessIds(processName);

    if (pids.empty()) {
        logMessage("未找到目标进程");
        return;
    }

    for (size_t i = 0; i < pids.size(); ++i) {
        logMessage("正在处理进程实例...");
        
        if (!ShouldSkipTermination(pids[i])) {
            logMessage("执行终止操作...");
            TerminateTargetProcess(pids[i]);
        } else {
            logMessage("跳过进程终止...");
            return;
        }
    }
}



bool launchApplication(const std::string& exePath) {
    if (exePath.empty()) {
        logMessage("错误：程序路径不能为空");
        return false;
    }

    // 构造带引号的命令行（处理路径中的空格）
    std::string commandLine = "\"" + exePath + "\"";
    std::vector<char> cmdLineBuf(commandLine.begin(), commandLine.end());
    cmdLineBuf.push_back('\0');

    STARTUPINFOA startupInfo;
    PROCESS_INFORMATION processInfo;
    ZeroMemory(&startupInfo, sizeof(startupInfo));
    ZeroMemory(&processInfo, sizeof(processInfo));
    startupInfo.cb = sizeof(startupInfo);

    logMessage("正在启动程序: " + exePath);

    // 创建进程
    BOOL result = CreateProcessA(
        NULL,                   // 应用程序名称（包含在命令行中）
        &cmdLineBuf[0],         // 命令行
        NULL,                   // 进程安全属性
        NULL,                   // 线程安全属性
        FALSE,                  // 不继承句柄
        0,                      // 无特殊创建标志
        NULL,                   // 使用父进程环境
        NULL,                   // 使用父进程工作目录
        &startupInfo,           // 启动信息
        &processInfo            // 进程信息
    );

    if (result) {
        logMessage("程序启动成功");
        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);
        return true;
    } else {
        // 获取错误信息
        DWORD error = GetLastError();
        std::ostringstream oss;
        oss << "启动失败，错误代码: 0x" << std::hex << error;
        logMessage(oss.str());
        
        return false;
    }
}







// 检查是否已经有实例在运行
bool IsAnotherInstanceRunning() {
    // 使用唯一的互斥体名称，建议根据应用程序修改
    HANDLE hMutex = CreateMutexA(NULL, FALSE, "MyAppSingleInstanceMutex");
    DWORD lastError = GetLastError();

    if (hMutex == NULL) {
        // 创建互斥体失败，默认认为已有实例运行
        return true;
    }

    // 如果错误码是已存在，则说明已有实例
    if (lastError == ERROR_ALREADY_EXISTS) {
        CloseHandle(hMutex);
        return true;
    }

    // 首次创建互斥体，不关闭句柄以保持互斥体有效
    return false;
}

int main() {
	SetReseewoWindowTitle();
	
	//版权信息等 
	// 保存原始颜色设置（用于恢复）
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO origInfo;
    GetConsoleScreenBufferInfo(hConsole, &origInfo);
    WORD origColor = origInfo.wAttributes;

    // 设置颜色：亮绿色字体 + 黑色背景
    setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    






                                                          

    // ASCII艺术字"RESEEWO"（手动设计版本）
    cout<<"--------------------------------------"<<endl;
    std::cout << "__________                                                \n";
    std::cout << "\\______   \\  ____    ______  ____   ____ __  _  __ ____   \n"; 
    std::cout << " |       _/_/ __ \\  /  ___/_/ __ \\_/ __ \\\\ \\/ \\/ //  _ \\  \n";
    std::cout << " |    |   \\\\  ___/  \\___ \\ \\  ___/\\  ___/ \\     /(  <_> ) \n";
    std::cout << " |____|_  / \\___  >/____  > \\___  >\\___  > \\/\\_/  \\____/  \n";
	std::cout << "        \\/      \\/      \\/      \\/     \\/                 \n";
	cout<<"Copyright (c) 2025 Creaconception Studio. All Rights Reserved."<<endl; 
	cout<<"创意工作室 (c) 2025 保留所有权利"<<endl;
	cout<<"Produced by Waji_rui"<<endl;
	cout<<"瓦吉_rui 出品"<<endl<<endl<<"https://wajirui.creaconception.dpdns.org"<<endl<<"https://github.com/waji-rui/Reseewo"<<endl<<endl;
	cout<<"--------------------------------------"<<endl;
	// 恢复原始颜色
    SetConsoleTextAttribute(hConsole, origColor);
	cout<<"程序用于关闭在后端运行的希沃白板进程来解决希沃白板在后端运行但打不开的情况！"<<endl;
	cout<<"并且该程序为自由程序并开源，谨防盗版！开源协议：GNU General Public License v3.0"<<endl;
	cout<<"--------------------------------------"<<endl;
	cout<<"可修改同目录下的Open_and_Close.txt配置文件实现自定义功能"<<endl;
	cout<<"日志输出在同目录下的reseewo_log"<<endl;
	cout<<"在程序日志中，错误代码0x2表示文件未找到，0x5表示权限不足，其他代码可通过微软文档查询具体含义"<<endl;
	cout<<"--------------------------------------"<<endl<<endl;
    // 检查是否存在其他实例
    if (IsAnotherInstanceRunning()) {
    	logMessage("【多个实例】用户打开了多个实例，发出错误警告");
        MessageBoxA(NULL, "程序已经在运行中！", "错误", MB_OK | MB_ICONERROR);
        logMessage("【多个实例】多出的实例已关闭");
        return 0;
    }
    KeepConsoleOnTop();

    // 程序主逻辑
    logMessage("【——开始——】程序启动");
    Sleep(1500);
    
    cout<<"-----"<<endl;
    logMessage("【进度】加载配置文件");
    loadConfig();
    
    cout<<"-----"<<endl;
    logMessage("【进度】开始执行关闭进程程序，若有前台窗口运行将跳过");
	CloseBackgroundProcess(ShutdownCmd.c_str(), WindowName.c_str());
	
	
	cout<<"-----"<<endl;
	logMessage("【进度】关闭进程程序执行完成，开始打开需要的程序");
	launchApplication(OpenCmd);
	
	
	cout<<"-----"<<endl;
	logMessage("【进度】所有程序执行完成，三秒后自动退出");
	Sleep(3000); 
    logMessage("【——结束——】程序退出");
    return 0;
}
