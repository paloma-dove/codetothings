/*
 * 项目名称：middletest - ESP32-S2 WiFi 智能小车控制程序
 * 硬件平台：ESP32-S2 (Lolin S2 Mini) + L9110 四路驱动板
 * 功能描述：
 * 1. 通过 WiFi 网页控制 4 个 TT 马达的运动方向和速度
 * 2. 支持模式：前进、后退、左转、右转、停止
 * 3. 支持速度调节：加速、减速
 * 4. 实时显示：当前状态、当前速度、速度百分比
 *
 * 硬件接线：
 * - L9110 VCC: 接 5V-9V 外部电源 (不能接 3.3V!)
 * - L9110 GND: 接外部电源负极 + ESP32 GND
 * - 左前轮: GPIO 5 (IN1), GPIO 6 (IN2)
 * - 右前轮: GPIO 7 (IN1), GPIO 8 (IN2)
 * - 左后轮: GPIO 9 (IN1), GPIO 10 (IN2)
 * - 右后轮: GPIO 11 (IN1), GPIO 12 (IN2)
 */

#include <WiFi.h>

// ==================== WiFi 配置 ====================
// 请根据您的实际网络环境修改 SSID 和密码
const char *ssid = "OPPO Find X9 Ultra r4kz";
const char *password = "62bu46mfi58e";

// 设置固定的静态 IP 地址
// 根据您的手机热点网段：172.29.9.x
IPAddress staticIP(172, 29, 9, 188);  // 固定 IP：以后直接用 http://172.29.9.188 访问
IPAddress gateway(172, 29, 9, 1);    // 网关 (手机热点通常是网段的 .1)
IPAddress subnet(255, 255, 255, 0); // 子网掩码
IPAddress dns(172, 29, 9, 28);       // DNS 服务器 (用您截图里的)

// ==================== PWM 配置 ====================
#define LEDC_TIMER_12_BIT  12
#define LEDC_BASE_FREQ     200  // L9110 驱动建议使用 1kHz 左右的频率 //1k会啸叫

// ==================== 电机引脚定义 ====================
// 警告：L9110 驱动板 VCC 必须接 5V 或电池，接 3.3V 会导致马达“尖叫”不转

#define MOTOR_LF_IN1   5   // 左前轮 IN1
#define MOTOR_LF_IN2   6   // 左前轮 IN2

#define MOTOR_RF_IN1   7   // 右前轮 IN1
#define MOTOR_RF_IN2   8   // 右前轮 IN2

#define MOTOR_LB_IN1   9   // 左后轮 IN1
#define MOTOR_LB_IN2   10  // 左后轮 IN2

#define MOTOR_RB_IN1   11  // 右后轮 IN1
#define MOTOR_RB_IN2   12  // 右后轮 IN2

// ==================== 全局变量 ====================
NetworkServer server(80);

// 驾驶状态：0=停止, 1=前进, 2=后退, 3=左转, 4=右转
int driveState = 0;
int currentSpeed = 2048;  // 当前速度 (0-4096)
const int MIN_SPEED = 1024;
const int MAX_SPEED = 4096;
const int SPEED_STEP = 512;

const char* stateNames[] = {"停止", "前进", "后退", "左转", "右转"};

void setup() {
  Serial.begin(115200);
  Serial.println("\n[Setup] Starting...");

  // 配置所有电机引脚为 PWM 输出
  ledcAttach(MOTOR_LF_IN1, LEDC_BASE_FREQ, LEDC_TIMER_12_BIT);
  ledcAttach(MOTOR_LF_IN2, LEDC_BASE_FREQ, LEDC_TIMER_12_BIT);
  ledcAttach(MOTOR_RF_IN1, LEDC_BASE_FREQ, LEDC_TIMER_12_BIT);
  ledcAttach(MOTOR_RF_IN2, LEDC_BASE_FREQ, LEDC_TIMER_12_BIT);
  ledcAttach(MOTOR_LB_IN1, LEDC_BASE_FREQ, LEDC_TIMER_12_BIT);
  ledcAttach(MOTOR_LB_IN2, LEDC_BASE_FREQ, LEDC_TIMER_12_BIT);
  ledcAttach(MOTOR_RB_IN1, LEDC_BASE_FREQ, LEDC_TIMER_12_BIT);
  ledcAttach(MOTOR_RB_IN2, LEDC_BASE_FREQ, LEDC_TIMER_12_BIT);

  Serial.println("[Setup] Motors configured");

  // 初始化：所有电机停止
  stopAll();

  // 连接 WiFi
  Serial.print("[Setup] Connecting to WiFi: ");
  Serial.println(ssid);

  // 配置静态 IP (必须在 WiFi.begin() 之前调用)
  WiFi.config(staticIP, gateway, subnet, dns);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n[Setup] WiFi connected!");
  Serial.print("[Setup] Control URL: http://");
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.println("[Setup] Web server started");
}

// ==================== 电机控制函数 ====================

void stopAll() {
  ledcWrite(MOTOR_LF_IN1, 0);
  ledcWrite(MOTOR_LF_IN2, 0);
  ledcWrite(MOTOR_RF_IN1, 0);
  ledcWrite(MOTOR_RF_IN2, 0);
  ledcWrite(MOTOR_LB_IN1, 0);
  ledcWrite(MOTOR_LB_IN2, 0);
  ledcWrite(MOTOR_RB_IN1, 0);
  ledcWrite(MOTOR_RB_IN2, 0);
  driveState = 0;
  Serial.println("[Motor] Stop all");
}

void moveForward() {
  driveState = 1;
  ledcWrite(MOTOR_LF_IN1, currentSpeed);
  ledcWrite(MOTOR_LF_IN2, 0);
  ledcWrite(MOTOR_RF_IN1, currentSpeed);
  ledcWrite(MOTOR_RF_IN2, 0);
  ledcWrite(MOTOR_LB_IN1, currentSpeed);
  ledcWrite(MOTOR_LB_IN2, 0);
  ledcWrite(MOTOR_RB_IN1, currentSpeed);
  ledcWrite(MOTOR_RB_IN2, 0);
  Serial.println("[Motor] Forward");
}

void moveBackward() {
  driveState = 2;
  ledcWrite(MOTOR_LF_IN1, 0);
  ledcWrite(MOTOR_LF_IN2, currentSpeed);
  ledcWrite(MOTOR_RF_IN1, 0);
  ledcWrite(MOTOR_RF_IN2, currentSpeed);
  ledcWrite(MOTOR_LB_IN1, 0);
  ledcWrite(MOTOR_LB_IN2, currentSpeed);
  ledcWrite(MOTOR_RB_IN1, 0);
  ledcWrite(MOTOR_RB_IN2, currentSpeed);
  Serial.println("[Motor] Backward");
}

void turnLeft() {
  driveState = 3;
  ledcWrite(MOTOR_LF_IN1, 0);
  ledcWrite(MOTOR_LF_IN2, currentSpeed);
  ledcWrite(MOTOR_RF_IN1, currentSpeed);
  ledcWrite(MOTOR_RF_IN2, 0);
  ledcWrite(MOTOR_LB_IN1, 0);
  ledcWrite(MOTOR_LB_IN2, currentSpeed);
  ledcWrite(MOTOR_RB_IN1, currentSpeed);
  ledcWrite(MOTOR_RB_IN2, 0);
  Serial.println("[Motor] Turn Left");
}

void turnRight() {
  driveState = 4;
  ledcWrite(MOTOR_LF_IN1, currentSpeed);
  ledcWrite(MOTOR_LF_IN2, 0);
  ledcWrite(MOTOR_RF_IN1, 0);
  ledcWrite(MOTOR_RF_IN2, currentSpeed);
  ledcWrite(MOTOR_LB_IN1, currentSpeed);
  ledcWrite(MOTOR_LB_IN2, 0);
  ledcWrite(MOTOR_RB_IN1, 0);
  ledcWrite(MOTOR_RB_IN2, currentSpeed);
  Serial.println("[Motor] Turn Right");
}

void speedUp() {
  currentSpeed = min(currentSpeed + SPEED_STEP, MAX_SPEED);
  Serial.print("[Speed] Up: ");
  Serial.println(currentSpeed);
  // 重新执行当前指令以应用新速度
  applyCurrentState();
}

void speedDown() {
  currentSpeed = max(currentSpeed - SPEED_STEP, MIN_SPEED);
  Serial.print("[Speed] Down: ");
  Serial.println(currentSpeed);
  applyCurrentState();
}

void applyCurrentState() {
  switch (driveState) {
    case 1: moveForward(); break;
    case 2: moveBackward(); break;
    case 3: turnLeft(); break;
    case 4: turnRight(); break;
    default: stopAll(); break;
  }
}

// ==================== WiFi 网页服务器 ====================

void handleCommand(const String& cmd) {
  Serial.print("[Web] Command: ");
  Serial.println(cmd);

  if (cmd == "forward") {
    moveForward();
  } else if (cmd == "backward") {
    moveBackward();
  } else if (cmd == "left") {
    turnLeft();
  } else if (cmd == "right") {
    turnRight();
  } else if (cmd == "stop") {
    stopAll();
  } else if (cmd == "speedup") {
    speedUp();
  } else if (cmd == "speeddown") {
    speedDown();
  }
}

void sendWebPage(NetworkClient& client) {
  float speedPercent = (currentSpeed * 100.0) / 4096.0;

  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html; charset=utf-8");
  client.println("Connection: close"); // 重要：明确告诉浏览器连接会关闭
  client.println();

  // 使用 F() 宏将字符串放入 Flash，节省 RAM
  client.println(F("<!DOCTYPE html><html>"));
  client.println(F("<head>"));
  client.println(F("<meta charset='utf-8'>"));
  client.println(F("<meta name='viewport' content='width=device-width, initial-scale=1'>"));
  client.println(F("<title>ESP32-S2 智能小车控制</title>"));
  client.println(F("<style>"));
  client.println(F("* { margin: 0; padding: 0; box-sizing: border-box; }"));
  client.println(F("body { font-family: 'Segoe UI', Arial, sans-serif; background: linear-gradient(135deg, #1a1a2e 0%, #16213e 100%); min-height: 100vh; display: flex; justify-content: center; align-items: center; color: #fff; }"));
  client.println(F(".container { background: rgba(255,255,255,0.1); backdrop-filter: blur(10px); border-radius: 20px; padding: 30px; width: 90%; max-width: 500px; box-shadow: 0 8px 32px rgba(0,0,0,0.3); border: 1px solid rgba(255,255,255,0.1); }"));
  client.println(F("h1 { text-align: center; margin-bottom: 20px; font-size: 24px; color: #4fc3f7; }"));
  client.println(F(".status-panel { background: rgba(0,0,0,0.3); border-radius: 15px; padding: 20px; margin-bottom: 20px; }"));
  client.println(F(".status-row { display: flex; justify-content: space-between; margin: 10px 0; padding: 10px; background: rgba(255,255,255,0.05); border-radius: 8px; }"));
  client.println(F(".status-label { color: #90a4ae; font-size: 14px; }"));
  client.println(F(".status-value { color: #fff; font-weight: bold; font-size: 16px; }"));
  client.println(F(".state-active { color: #4caf50 !important; }"));
  client.println(F(".state-stop { color: #f44336 !important; }"));
  client.println(F(".speed-bar { width: 100%; height: 20px; background: rgba(255,255,255,0.1); border-radius: 10px; margin-top: 10px; overflow: hidden; }"));
  client.println(F(".speed-fill { height: 100%; background: linear-gradient(90deg, #4caf50, #8bc34a); border-radius: 10px; transition: width 0.3s ease; }"));
  client.println(F(".control-grid { display: grid; grid-template-columns: repeat(3, 1fr); gap: 10px; margin-bottom: 20px; }"));
  client.println(F(".btn { padding: 20px; border: none; border-radius: 12px; font-size: 16px; font-weight: bold; cursor: pointer; transition: all 0.2s; color: white; }"));
  client.println(F(".btn:active { transform: scale(0.95); }"));
  client.println(F(".btn-forward { background: linear-gradient(135deg, #4caf50, #2e7d32); grid-column: 2; }"));
  client.println(F(".btn-left { background: linear-gradient(135deg, #ff9800, #f57c00); grid-row: 2; }"));
  client.println(F(".btn-stop { background: linear-gradient(135deg, #f44336, #c62828); grid-row: 2; }"));
  client.println(F(".btn-right { background: linear-gradient(135deg, #ff9800, #f57c00); grid-row: 2; }"));
  client.println(F(".btn-backward { background: linear-gradient(135deg, #2196f3, #1565c0); grid-column: 2; grid-row: 3; }"));
  client.println(F(".speed-control { display: flex; gap: 10px; justify-content: center; }"));
  client.println(F(".btn-speed { flex: 1; padding: 15px; background: linear-gradient(135deg, #9c27b0, #7b1fa2); border: none; border-radius: 10px; color: white; font-size: 18px; font-weight: bold; cursor: pointer; }"));
  client.println(F(".btn-speed:active { transform: scale(0.95); }"));
  client.println(F(".info { text-align: center; margin-top: 15px; font-size: 12px; color: #78909c; }"));
  client.println(F("</style>"));
  client.println(F("</head>"));
  client.println(F("<body>"));
  client.println(F("<div class='container'>"));
  client.println(F("<h1>🚗 ESP32-S2 智能小车</h1>"));

  // 状态面板
  client.println(F("<div class='status-panel'>"));
  client.print(F("<div class='status-row'><span class='status-label'>当前状态</span><span class='status-value "));
  client.print(driveState == 0 ? "state-stop" : "state-active");
  client.print(F("'>"));
  client.print(stateNames[driveState]);
  client.println(F("</span></div>"));
  client.print(F("<div class='status-row'><span class='status-label'>当前速度</span><span class='status-value'>"));
  client.print(currentSpeed);
  client.println(F(" / 4096</span></div>"));
  client.print(F("<div class='status-row'><span class='status-label'>速度百分比</span><span class='status-value'>"));
  client.print(speedPercent, 1);
  client.println(F("%</span></div>"));
  client.print(F("<div class='speed-bar'><div class='speed-fill' style='width:"));
  client.print((int)speedPercent);
  client.println(F("%'></div></div>"));
  client.println(F("</div>"));

  // 方向控制
  client.println(F("<div class='control-grid'>"));
  client.println(F("<a href='/forward'><button class='btn btn-forward'>⬆️ 前进</button></a>"));
  client.println(F("<a href='/left'><button class='btn btn-left'>⬅️ 左转</button></a>"));
  client.println(F("<a href='/stop'><button class='btn btn-stop'>⏹️ 停止</button></a>"));
  client.println(F("<a href='/right'><button class='btn btn-right'>➡️ 右转</button></a>"));
  client.println(F("<a href='/backward'><button class='btn btn-backward'>⬇️ 后退</button></a>"));
  client.println(F("</div>"));

  // 速度控制
  client.println(F("<div class='speed-control'>"));
  client.println(F("<a href='/speeddown'><button class='btn-speed'>➖ 减速</button></a>"));
  client.println(F("<a href='/speedup'><button class='btn-speed'>➕ 加速</button></a>"));
  client.println(F("</div>"));

  client.println(F("<div class='info'>ESP32-S2 WiFi 智能小车控制中心</div>"));
  client.println(F("</div>"));
  client.println(F("</body></html>"));
}

void handleWebServer() {
  NetworkClient client = server.accept();
  if (client) {
    Serial.println("[Web] New client connected");
    String currentLine = "";
    bool requestDone = false;
    
    while (client.connected() && !requestDone) {
      if (client.available()) {
        char c = client.read();
        //Serial.write(c); // 取消注释可查看原始请求

        if (c == '\n') {
          if (currentLine.length() == 0) {
            // 空行表示请求头结束，准备发送响应
            requestDone = true;
            break;
          } else {
            // 解析请求行
            if (currentLine.startsWith("GET /")) {
              String cmd = currentLine.substring(5);
              // 找第一个空格，只取路径部分
              int spaceIndex = cmd.indexOf(' ');
              if (spaceIndex > 0) {
                cmd = cmd.substring(0, spaceIndex);
              }
              cmd.trim();
              handleCommand(cmd);
            }
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }

    // 发送网页
    sendWebPage(client);
    
    // 关闭连接
    client.stop();
    Serial.println("[Web] Client disconnected");
  }
}

void loop() {
  handleWebServer();
  delay(5);
}
