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
���������ڹر��ں�����е�ϣ�ְװ���������ϣ�ְװ��ں�����е��򲻿��������
Copyright (c) 2025 Creaconception Studio.Published by Wajirui. 

������Ϊ�������������Ը��������������ᷢ����
GNUͨ�ù������Э���������߸��µİ汾����ѡ��
�Ĺ涨�Ա���������ٷַ���/���޸ġ�

����ϣ���ַ�����������������õģ��������ǲ���
���κα�֤��Ҳ���ṩ�κζԿ������Ժ��������ض�Ŀ �ĵ�Ĭʾ��֤�����������GNUͨ�ù������Э�顣

��Ӧ����ͬ�������յ�һ��GNUͨ�ù������Э��ĸ�����
��δ�յ��������http://www.gnu.org/licenses/

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
        (background << 4) | // ����ɫռ��4λ
        (foreground & 0x0F) // ǰ��ɫռ��4λ
    );
}

// ȫ�ֱ�������
std::string ShutdownCmd = "EasiNote.exe";
std::string WindowName = "ϣ�ְװ�";
std::string OpenCmd = "C:\\Program Files (x86)\\Seewo\\EasiNote5\\swenlauncher\\swenlauncher.exe";

// ������������
void trim(std::string &s);
std::string replaceBackslashes(const std::string &input);
void loadConfig();





// ȫ�ֱ�������¼��ǰ��־����
int logCount = 0;

const char* windowN = "ϣ�ְװ�";

// ��������
void writeLog(const string& message);
void rotateLogs();
string getCurrentTime();
void logMessage();

void SetReseewoWindowTitle()
{
    SetConsoleTitleA("Reseewo");  // ʹ��ANSI�汾API���ô��ڱ���
}

// ����־����
void logMessage(const string& message) {
	//���������̨ 
	cout<<message<<endl;
	
    // ��ȡ��ǰʱ��
    string timeStr = getCurrentTime();
    
    // ����������־��Ϣ
    string fullLog = "[" + timeStr + "] " + message;
    
    // д����־�ļ�
    writeLog(fullLog);
    
    // ������־����
    logCount++;
    
    // ����Ƿ���Ҫ��ת��־
    if (logCount >= 1000) {
        rotateLogs();
        logCount = 0;
    }
}

// ��ȡ��ǰʱ���ַ���
string getCurrentTime() {
    time_t now = time(0);
    tm* localTime = localtime(&now);
    
    char timeStr[20];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localTime);
    
    return string(timeStr);
}

// д����־���ļ�
void writeLog(const string& logEntry) {
    ofstream logFile("reseewo_log.txt", ios::app); // ��׷��ģʽ��
    
    if (logFile.is_open()) {
        logFile << logEntry << endl;
        logFile.close();
    } else {
        cerr << "�޷�����־�ļ�!" << endl;
    }
}

// ��־��ת�������������µ�1000����־
void rotateLogs() {
    const int MAX_LINES = 1000;
    vector<string> logLines;
    string line;
    
    // ��ȡ������־
    ifstream inFile("reseewo_log.txt");
    if (inFile.is_open()) {
        while (getline(inFile, line)) {
            logLines.push_back(line);
        }
        inFile.close();
    } else {
        cerr << "�޷�����־�ļ�������ת!" << endl;
        return;
    }
    
    // �����־�����������ƣ�ֻ�������µĲ���
    if (logLines.size() > MAX_LINES) {
        int startIndex = logLines.size() - MAX_LINES;
        logLines.erase(logLines.begin(), logLines.begin() + startIndex);
    }
    
    // д���ļ���ʹ��C++98���ݵ�ѭ����
    ofstream outFile("reseewo_log.txt");
    if (outFile.is_open()) {
        for (vector<string>::iterator it = logLines.begin(); it != logLines.end(); ++it) {
            outFile << *it << endl;
        }
        outFile.close();
    } else {
        cerr << "�޷�д����ת�����־�ļ�!" << endl;
    }
}


void KeepConsoleOnTop() {
    char originalTitle[256];
    GetConsoleTitleA(originalTitle, sizeof(originalTitle)); // ����ԭʼ����

    const char* uniqueTitle = "MyTopMostConsole12345";
    SetConsoleTitleA(uniqueTitle); // ����Ψһ����

    Sleep(100); // �ȴ��������

    HWND hwnd = FindWindowA("ConsoleWindowClass", uniqueTitle);
    if (hwnd) {
        // ���ô����ö�
        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    }

    SetConsoleTitleA(originalTitle); // �ָ�ԭʼ����
}




// ȥ���ַ�����β�հ��ַ�
void trim(std::string &s) {
    size_t start = s.find_first_not_of(" \t");
    if (start == std::string::npos) {
        s = "";
        return;
    }

    size_t end = s.find_last_not_of(" \t");
    s = s.substr(start, end - start + 1);
}

// �滻��б��Ϊ˫��б��
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

// ���������ļ�
void loadConfig() {
    std::ifstream inFile("Open_and_Close.txt");
    
    // �ļ�������ʱ����Ĭ������
    if (!inFile) {
        logMessage("�����ļ�δ�ҵ������ڴ���Ĭ������...");
        
        std::ofstream outFile("Open_and_Close.txt");
        if (outFile) {
            outFile << "Shutdown=EasiNote.exe\n"
                    << "windowName=ϣ�ְװ�\n"
                    << "Open=C:\\Program Files (x86)\\Seewo\\EasiNote5\\swenlauncher\\swenlauncher.exe\n";
            outFile.close();
            logMessage("Ĭ�������ļ��Ѵ���");
            
            // ���³��Դ��ļ�
            inFile.open("Open_and_Close.txt");
        } else {
            logMessage("���������ļ�ʧ��");
            return;
        }
    }

    logMessage("��ʼ��ȡ�����ļ�...");
    std::string line;
    while (std::getline(inFile, line)) {
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            
            // ȥ����β�հ�
            trim(key);
            trim(value);

            if (key == "Shutdown") {
                ShutdownCmd = value;
                logMessage("��ȡ��Shutdown����: " + ShutdownCmd);
            } else if (key == "windowName") {
                WindowName = value;
                logMessage("��ȡ��windowName����: " + WindowName);
            } else if (key == "Open") {
                OpenCmd = replaceBackslashes(value);
                logMessage("��ȡ������Open·��: " + OpenCmd);
            }
        }
    }
    inFile.close();
    logMessage("�����ļ���ȡ��ɣ�δ��ȡ�������ý���Ĭ����������");
}











// ���ڼ��ṹ��
struct WindowCheckInfo {
    DWORD targetPid;
    bool found;
    const char* windowTitle;
};

// ö�ٴ��ڻص�����
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    WindowCheckInfo* info = (WindowCheckInfo*)lParam;
    DWORD pid = 0;
    
    // ��ȡ���ڽ���ID
    GetWindowThreadProcessId(hwnd, &pid);
    if (pid == info->targetPid) {
        char title[256] = {0};
        // ��ȡ���ڱ���
        if (GetWindowTextA(hwnd, title, sizeof(title)-1)) {
            if (strstr(title, info->windowTitle) != NULL) {
                info->found = true;
                return FALSE; // �ҵ�Ŀ�괰�ڣ�ֹͣö��
            }
        }
    }
    return TRUE; // ����ö��
}

// ����ָ������ID
std::vector<DWORD> FindProcessIds(const char* processName) {
    std::vector<DWORD> pids;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    
    if (snapshot == INVALID_HANDLE_VALUE) {
        logMessage("�������̿���ʧ��");
        return pids;
    }

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(snapshot, &pe)) {
        logMessage("��ȡ��һ������ʧ��");
        CloseHandle(snapshot);
        return pids;
    }

    do {
        if (_stricmp(pe.szExeFile, processName) == 0) {
            pids.push_back(pe.th32ProcessID);
            logMessage("�ҵ�Ŀ�����");
        }
    } while (Process32Next(snapshot, &pe));

    CloseHandle(snapshot);
    return pids;
}

// �������Ƿ���Ҫ������ֹ
bool ShouldSkipTermination(DWORD pid) {
    // ���ǰ̨����
    HWND foregroundWnd = GetForegroundWindow();
    if (foregroundWnd) {
        DWORD foregroundPid;
        GetWindowThreadProcessId(foregroundWnd, &foregroundPid);
        
        if (foregroundPid == pid) {
            logMessage("����ӵ��ǰ̨���ڣ�����");
            return true;
        }
    }

    // ����ض����ڱ���
    WindowCheckInfo info = { pid, false, windowN };
    EnumWindows(EnumWindowsProc, (LPARAM)&info);

    if (info.found) {
        logMessage("��⵽�ض����ڣ�������ֹ");
        return true;
    }

    return false;
}

// ��ֹ���̺���
void TerminateTargetProcess(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (!hProcess) {
        logMessage("�򿪽���ʧ��");
        return;
    }

    if (TerminateProcess(hProcess, 0)) {
        logMessage("�ɹ���ֹ����");
    } else {
        logMessage("��ֹ����ʧ��");
    }

    CloseHandle(hProcess);
}

// �����ܺ���
void CloseBackgroundProcess(const char* processName, const char* windowNames = "ϣ�ְװ�") {
	windowN = windowNames;
    logMessage("��ʼ���ҽ���...");
    std::vector<DWORD> pids = FindProcessIds(processName);

    if (pids.empty()) {
        logMessage("δ�ҵ�Ŀ�����");
        return;
    }

    for (size_t i = 0; i < pids.size(); ++i) {
        logMessage("���ڴ������ʵ��...");
        
        if (!ShouldSkipTermination(pids[i])) {
            logMessage("ִ����ֹ����...");
            TerminateTargetProcess(pids[i]);
        } else {
            logMessage("����������ֹ...");
            return;
        }
    }
}



bool launchApplication(const std::string& exePath) {
    if (exePath.empty()) {
        logMessage("���󣺳���·������Ϊ��");
        return false;
    }

    // ��������ŵ������У�����·���еĿո�
    std::string commandLine = "\"" + exePath + "\"";
    std::vector<char> cmdLineBuf(commandLine.begin(), commandLine.end());
    cmdLineBuf.push_back('\0');

    STARTUPINFOA startupInfo;
    PROCESS_INFORMATION processInfo;
    ZeroMemory(&startupInfo, sizeof(startupInfo));
    ZeroMemory(&processInfo, sizeof(processInfo));
    startupInfo.cb = sizeof(startupInfo);

    logMessage("������������: " + exePath);

    // ��������
    BOOL result = CreateProcessA(
        NULL,                   // Ӧ�ó������ƣ��������������У�
        &cmdLineBuf[0],         // ������
        NULL,                   // ���̰�ȫ����
        NULL,                   // �̰߳�ȫ����
        FALSE,                  // ���̳о��
        0,                      // �����ⴴ����־
        NULL,                   // ʹ�ø����̻���
        NULL,                   // ʹ�ø����̹���Ŀ¼
        &startupInfo,           // ������Ϣ
        &processInfo            // ������Ϣ
    );

    if (result) {
        logMessage("���������ɹ�");
        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);
        return true;
    } else {
        // ��ȡ������Ϣ
        DWORD error = GetLastError();
        std::ostringstream oss;
        oss << "����ʧ�ܣ��������: 0x" << std::hex << error;
        logMessage(oss.str());
        
        return false;
    }
}







// ����Ƿ��Ѿ���ʵ��������
bool IsAnotherInstanceRunning() {
    // ʹ��Ψһ�Ļ��������ƣ��������Ӧ�ó����޸�
    HANDLE hMutex = CreateMutexA(NULL, FALSE, "MyAppSingleInstanceMutex");
    DWORD lastError = GetLastError();

    if (hMutex == NULL) {
        // ����������ʧ�ܣ�Ĭ����Ϊ����ʵ������
        return true;
    }

    // ������������Ѵ��ڣ���˵������ʵ��
    if (lastError == ERROR_ALREADY_EXISTS) {
        CloseHandle(hMutex);
        return true;
    }

    // �״δ��������壬���رվ���Ա��ֻ�������Ч
    return false;
}

int main() {
	SetReseewoWindowTitle();
	
	//��Ȩ��Ϣ�� 
	// ����ԭʼ��ɫ���ã����ڻָ���
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO origInfo;
    GetConsoleScreenBufferInfo(hConsole, &origInfo);
    WORD origColor = origInfo.wAttributes;

    // ������ɫ������ɫ���� + ��ɫ����
    setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    






                                                          

    // ASCII������"RESEEWO"���ֶ���ư汾��
    cout<<"--------------------------------------"<<endl;
    std::cout << "__________                                                \n";
    std::cout << "\\______   \\  ____    ______  ____   ____ __  _  __ ____   \n"; 
    std::cout << " |       _/_/ __ \\  /  ___/_/ __ \\_/ __ \\\\ \\/ \\/ //  _ \\  \n";
    std::cout << " |    |   \\\\  ___/  \\___ \\ \\  ___/\\  ___/ \\     /(  <_> ) \n";
    std::cout << " |____|_  / \\___  >/____  > \\___  >\\___  > \\/\\_/  \\____/  \n";
	std::cout << "        \\/      \\/      \\/      \\/     \\/                 \n";
	cout<<"Copyright (c) 2025 Creaconception Studio. All Rights Reserved."<<endl; 
	cout<<"���⹤���� (c) 2025 ��������Ȩ��"<<endl;
	cout<<"Produced by Waji_rui"<<endl;
	cout<<"�߼�_rui ��Ʒ"<<endl<<endl<<"https://wajirui.creaconception.dpdns.org"<<endl<<"https://github.com/waji-rui/Reseewo"<<endl<<endl;
	cout<<"--------------------------------------"<<endl;
	// �ָ�ԭʼ��ɫ
    SetConsoleTextAttribute(hConsole, origColor);
	cout<<"�������ڹر��ں�����е�ϣ�ְװ���������ϣ�ְװ��ں�����е��򲻿��������"<<endl;
	cout<<"���Ҹó���Ϊ���ɳ��򲢿�Դ���������棡��ԴЭ�飺GNU General Public License v3.0"<<endl;
	cout<<"--------------------------------------"<<endl;
	cout<<"���޸�ͬĿ¼�µ�Open_and_Close.txt�����ļ�ʵ���Զ��幦��"<<endl;
	cout<<"��־�����ͬĿ¼�µ�reseewo_log"<<endl;
	cout<<"�ڳ�����־�У��������0x2��ʾ�ļ�δ�ҵ���0x5��ʾȨ�޲��㣬���������ͨ��΢���ĵ���ѯ���庬��"<<endl;
	cout<<"--------------------------------------"<<endl<<endl;
    // ����Ƿ��������ʵ��
    if (IsAnotherInstanceRunning()) {
    	logMessage("�����ʵ�����û����˶��ʵ�����������󾯸�");
        MessageBoxA(NULL, "�����Ѿ��������У�", "����", MB_OK | MB_ICONERROR);
        logMessage("�����ʵ���������ʵ���ѹر�");
        return 0;
    }
    KeepConsoleOnTop();

    // �������߼�
    logMessage("��������ʼ��������������");
    Sleep(1500);
    
    cout<<"-----"<<endl;
    logMessage("�����ȡ����������ļ�");
    loadConfig();
    
    cout<<"-----"<<endl;
    logMessage("�����ȡ���ʼִ�йرս��̳�������ǰ̨�������н�����");
	CloseBackgroundProcess(ShutdownCmd.c_str(), WindowName.c_str());
	
	
	cout<<"-----"<<endl;
	logMessage("�����ȡ��رս��̳���ִ����ɣ���ʼ����Ҫ�ĳ���");
	launchApplication(OpenCmd);
	
	
	cout<<"-----"<<endl;
	logMessage("�����ȡ����г���ִ����ɣ�������Զ��˳�");
	Sleep(3000); 
    logMessage("���������������������˳�");
    return 0;
}
