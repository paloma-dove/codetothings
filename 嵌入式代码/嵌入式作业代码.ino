/*
 * 项目名称：embeddedtest - 智能环境感应控制系统
 * 硬件平台：ESP32-S2 (Lolin S2 Mini)
 * 功能描述：
 * 1. 通过光敏电阻 (LDR) 感应光照强度，控制 LED 亮度。
 * 2. 通过 WiFi 网页可以切换 LED 模式 和 舵机模式。
 *    - LED 模式：光敏电阻控制 LED 亮度（光线越暗，LED 越亮）。
 *    - 舵机模式：光敏电阻控制舵机角度（光线变化带动舵机转动）。
 * 
 * 设计参考：
 * - WiFi 部分参考了 tryesp32-master 中的 WiFiServer 示例。
 * - ADC (LDR) 部分参考了 07analogread_light 示例，使用了 11dB 衰减以支持 0-3.3V 量程。
 */

#include <WiFi.h>

// ==================== 引脚定义 ====================
#define LDR_PIN     4       // 光敏电阻连接到 GPIO 4 (ADC1_CH3)
#define LED_PIN     15      // LED 连接到 GPIO 15
#define SERVO_PIN   5       // 舵机信号线连接到 GPIO 5

// ==================== WiFi 配置 ====================
// 请根据您的实际网络环境修改 SSID 和密码
const char *ssid = "OPPO Find X3 Pro MARS";
const char *password = "c55nb8r2";

// ==================== PWM (LEDC) 配置 ====================
#define LEDC_RESOLUTION      12      // 12位分辨率 (0-4095)
#define LEDC_FREQ_LED        5000    // LED 使用 5kHz 频率
#define LEDC_FREQ_SERVO      50      // 舵机使用 50Hz (20ms 周期)

// ==================== 全局变量 ====================
NetworkServer server(80);
int controlMode = 0;           // 当前模式：0 为 LED，1 为舵机
int ldrValue = 0;              // 存储光敏电阻读数

// 舵机扫掠相关变量
int servoPos = 0;              // 当前角度 (0-180)
int servoDirection = 1;        // 扫掠方向：1 为增加，-1 为减少
unsigned long lastServoUpdate = 0;
const int servoInterval = 15;  // 扫掠速度（毫秒/度）

void setup() {
  Serial.begin(115200);
  
  // 1. 配置 ADC 采样
  analogSetAttenuation(ADC_11db);    // 11dB 衰减，量程 0-3.3V
  analogReadResolution(12);          // 12 位分辨率 (0-4095)

  // 2. 配置 PWM 输出
  ledcAttach(LED_PIN, LEDC_FREQ_LED, LEDC_RESOLUTION);
  ledcAttach(SERVO_PIN, LEDC_FREQ_SERVO, LEDC_RESOLUTION);

  // 3. 初始状态：全部关闭
  ledcWrite(LED_PIN, 0);
  ledcWrite(SERVO_PIN, 0);

  // 4. 连接 WiFi 网络
  Serial.println("\n--- 智能环境感应系统启动 ---");
  Serial.print("正在连接 WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi 已连接！");
  Serial.print("控制页面地址: http://");
  Serial.println(WiFi.localIP());

  server.begin();
}

/**
 * 处理光敏电阻数据及控制 LED
 */
void handleLDR() {
  // 对 ADC 进行多次采样取平均值，过滤噪声
  long sum = 0;
  for(int i = 0; i < 16; i++) {
    sum += analogRead(LDR_PIN);
    delayMicroseconds(100);
  }
  ldrValue = sum / 16;

  // 确保 ldrValue 在有效范围内
  if (ldrValue < 0) ldrValue = 0;
  if (ldrValue > 4095) ldrValue = 4095;

  // 光敏电阻只在 LED 模式下工作，或者作为环境光参考
  if (controlMode == 0) {
    // 【模式 0：LED 自动调光】
    if (ldrValue < 50) {
      ledcWrite(LED_PIN, 0);
    } else {
      // 反向映射：暗 -> 亮
      int brightness = map(ldrValue, 50, 4095, 0, 4095);
      if (brightness < 0) brightness = 0;
      if (brightness > 4095) brightness = 4095;
      int light = 4096 - brightness;
      brightness = light;
      ledcWrite(LED_PIN, brightness);
    }
  } else {
    // 非 LED 模式下关闭 LED
    ledcWrite(LED_PIN, 0);
  }
}

/**
 * 处理舵机自动扫掠逻辑 (0-180-0)
 */
void handleServo() {
  if (controlMode == 1) {
    // 【模式 1：舵机自动扫掠】
    unsigned long now = millis();
    if (now - lastServoUpdate >= servoInterval) {
      lastServoUpdate = now;

      // 更新位置
      servoPos += servoDirection;

      // 碰到边界时反转方向
      if (servoPos >= 180) {
        servoPos = 180;
        servoDirection = -1;
      } else if (servoPos <= 0) {
        servoPos = 0;
        servoDirection = 1;
      }

      // 将 0-180 度映射为 PWM 占空比
      // 为了防止舵机死区异响，建议稍微收窄范围（例如 115-500 代替 102-512）
      int servoDuty = map(servoPos, 0, 180, 115, 500); 
      ledcWrite(SERVO_PIN, servoDuty);
    }
  } else {
    // 非舵机模式下停止信号输出，防止舵机"叫"
    ledcWrite(SERVO_PIN, 0);
    // 重置位置，以便下次切回模式时重新开始
    servoPos = 0;
    servoDirection = 1;
  }
}

/**
 * 处理 WiFi 网页服务器请求
 */
void handleWebServer() {
  NetworkClient client = server.accept();
  if (client) {
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (currentLine.length() == 0) {
            // 发送 HTTP 响应头
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html; charset=utf-8");
            client.println();

            // 发送 HTML 页面
            client.println("<!DOCTYPE html><html>");
            client.println("<head><title>ESP32-S2 控制中心</title>");
            client.println("<meta name='viewport' content='width=device-width, initial-scale=1' charset='utf-8'>");
            client.println("<style>");
            client.println("body { font-family: Arial; text-align: center; background-color: #f4f4f9; color: #333; }");
            client.println(".card { background: white; border-radius: 10px; padding: 20px; margin: 20px auto; max-width: 400px; box-shadow: 0 4px 6px rgba(0,0,0,0.1); }");
            client.println(".btn { background-color: #4CAF50; color: white; padding: 15px 32px; border: none; border-radius: 5px; cursor: pointer; font-size: 16px; text-decoration: none; display: inline-block; margin-top: 10px; }");
            client.println(".btn:hover { background-color: #45a049; }");
            client.println(".value { font-size: 24px; font-weight: bold; color: #2196F3; }");
            client.println(".warning { color: #ff9800; font-size: 14px; }");
            client.println("</style></head><body>");
            
            client.println("<div class='card'>");
            client.println("<h1>ESP32-S2 控制中心</h1>");
            client.printf("<p>光照传感器读数: <span class='value'>%d</span></p>", 4096-ldrValue);
            
            if (ldrValue < 50) {
              client.println("<p class='warning'>⚠️ 光敏电阻未连接或数值异常</p>");
            }
            
            client.printf("<p>当前工作模式: <strong>%s</strong></p>", controlMode == 0 ? "LED 自动调光" : "舵机 自动扫掠 (0-180)");
            client.println("<hr>");
            client.println("<p>点击下方按钮切换控制目标：</p>");
            client.println("<a href='/toggle' class='btn'>切换模式</a>");
            client.println("</div>");
            
            client.println("</body></html>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        // 检查请求路径
        if (currentLine.endsWith("GET /toggle")) {
          controlMode = !controlMode;
          Serial.printf(">> 模式已切换为: %s\n", controlMode == 0 ? "LED" : "SERVO");
        }
      }
    }
    client.stop();
  }
}

void loop() {
  handleLDR();        // 仅处理 LDR 和 LED
  handleServo();      // 独立处理舵机扫掠逻辑
  handleWebServer();   // WiFi 响应
  
  // 串口调试输出
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 1000) {
    if (controlMode == 0) {
      Serial.printf("LDR: %4d | Mode: LED\n", 4096-ldrValue);
    } else {
      Serial.printf("Servo Pos: %3d | Mode: SWEEP\n", servoPos);
    }
    lastPrint = millis();
  }
}
