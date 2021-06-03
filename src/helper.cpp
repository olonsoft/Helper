#include <arduino.h>
#include <helper.h>
#include <helper_debug>
#include <time_functions.h>

#include <FS.h>

#ifdef ESP32
#include <WiFi.h>
#include <LITTLEFS.h>
#define ESP_FS LITTLEFS
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <LittleFS.h>
#define ESP_FS LittleFS
#endif

String lastErrorStr = "";
bool fileSystemBegun = false;

String getChipIdHex() {
  String mac = WiFi.macAddress();
  mac.replace(":", "");
  mac.remove(1,6);
  return mac;
}

#ifdef ESP32

//String getChipIdHex() {
//  String s = String((uint32_t)(ESP.getEfuseMac() >> 32), HEX);
//  s.toUpperCase();
//  return s;
//}

//String getChipIdStr() {
//  char deviceid[21];
//  uint64_t chipid;
//  chipid = ESP.getEfuseMac();
//  sprintf(deviceid, "%" PRIu64, chipid);
//  String thisID(deviceid);
//  return thisID;
//}

#include <rom/rtc.h>

String getResetReasonString(byte icore) {
  bool isDEEPSLEEP_RESET(false);
  switch (rtc_get_reset_reason((RESET_REASON)icore)) {
    case NO_MEAN:
      return F("NO_MEAN");
    case POWERON_RESET:
      return F("Vbat power on reset");
    case SW_RESET:
      return F("Software reset digital core");
    case OWDT_RESET:
      return F("Legacy watch dog reset digital core");
    case DEEPSLEEP_RESET:
      isDEEPSLEEP_RESET = true;
      break;
    case SDIO_RESET:
      return F("Reset by SLC module, reset digital core");
    case TG0WDT_SYS_RESET:
      return F("Timer Group0 Watch dog reset digital core");
    case TG1WDT_SYS_RESET:
      return F("Timer Group1 Watch dog reset digital core");
    case RTCWDT_SYS_RESET:
      return F("RTC Watch dog Reset digital core");
    case INTRUSION_RESET:
      return F("Instrusion tested to reset CPU");
    case TGWDT_CPU_RESET:
      return F("Time Group reset CPU");
    case SW_CPU_RESET:
      return F("Software reset CPU");
    case RTCWDT_CPU_RESET:
      return F("RTC Watch dog Reset CPU");
    case EXT_CPU_RESET:
      return F("for APP CPU, reseted by PRO CPU");
    case RTCWDT_BROWN_OUT_RESET:
      return F("Reset when the vdd voltage is not stable");
    case RTCWDT_RTC_RESET:
      return F("RTC Watch dog reset digital core and rtc module");
    default:
      break;
  }
  if (isDEEPSLEEP_RESET) {
    String reason = F("Deep Sleep, Wakeup reason (");
    reason += rtc_get_wakeup_cause();
    reason += ')';
    return reason;
  }
  return F("Unknown");
}
#endif

String getResetReasonString() {
#ifdef ESP32
  String reason = F("CPU0: ");
  reason += getResetReasonString(0);
  reason += F(" CPU1: ");
  reason += getResetReasonString(1);
  return reason;
#else
  return ESP.getResetReason();
#endif
}

String getDeviceName(String name) {
  String output = name;
  output.replace(DEVICE_ID_STR, getChipIdHex());
  return output;
}

String getDefaultDeviceName() {
#ifdef ESP8266
  return getDeviceName("ESP8266-"+ String(DEVICE_ID_STR));
#elif defined(ESP32)
  return getDeviceName("ESP32-" + String(DEVICE_ID_STR));
#endif
}

String formatBytes(size_t bytes) {
  const char *sizes[5] = { "B", "KB", "MB", "GB", "TB" };
  int i;
  double dblByte = bytes;
  for (i = 0; i < 5 && bytes >= 1024; i++, bytes /= 1024)
    dblByte = bytes / 1024.0;
  char str[16];
  sprintf(str, "%.2f", dblByte);
  return String(strcat(str, sizes[i])); 
}

String intToString(int32_t i) {
	char buf[16];
  //sprintf(buf, "%" PRId32, i);
	sprintf(buf, "%d", i);
	return String(buf);
}

/*
  ps_malloc(550 * 1024);  
  uint8_t * buf = (uint8_t*)ps_malloc(500 * 1024);
  if (buf == NULL) 
    Serial.println("can't allocate memory with malloc\n");

    Serial.print("ESP32 SDK: "); Serial.println(ESP.getSdkVersion());
    Serial.print("ESP32 CPU FREQ: "); Serial.print(getCpuFrequencyMhz()); Serial.println("MHz");
    Serial.print("ESP32 APB FREQ: "); Serial.print(getApbFrequency() / 1000000.0, 1); Serial.println("MHz");
    Serial.print("ESP32 FLASH SIZE: "); Serial.print(ESP.getFlashChipSize() / (1024.0 * 1024), 2); Serial.println("MB");
    Serial.print("ESP32 RAM SIZE: "); Serial.print(ESP.getHeapSize() / 1024.0, 2); Serial.println("KB");
    Serial.print("ESP32 FREE RAM: "); Serial.print(ESP.getFreeHeap() / 1024.0, 2); Serial.println("KB");
    Serial.print("ESP32 MAX RAM ALLOC: "); Serial.print(ESP.getMaxAllocHeap() / 1024.0, 2); Serial.println("KB");
    Serial.print("ESP32 FREE PSRAM: "); Serial.print(ESP.getFreePsram() / 1024.0, 2); Serial.println("KB");
ESP32 SDK: v3.2.3
ESP32 CPU FREQ: 240MHz
ESP32 APB FREQ: 80.0MHz
ESP32 FLASH SIZE: 4.00MB
ESP32 RAM SIZE: 370.00KB
ESP32 FREE RAM: 344.70KB
ESP32 MAX RAM ALLOC: 122.09KB
ESP32 FREE PSRAM: 3545.93KB
*/

bool beginFS() {
  if (!fileSystemBegun) {
#ifdef ESP32    
    fileSystemBegun = ESP_FS.begin(FORMAT_LITTLEFS_IF_FAILED); // format on fail.
#else
    fileSystemBegun = ESP_FS.begin();
    if (!fileSystemBegun) {
      DEBUG_PRINTF_P(PSTR("Formatting FS.\n"));
      fileSystemBegun = ESP_FS.format();
    }
#endif  
  }
  return (fileSystemBegun);
}

String getSystemInfoJson() {  
  String c = ",";
  String s = "{";
  // ===== CPU =====
  s +=     jsonPair(F("CPU chip id"),   getChipIdHex());
  s += c + jsonPair(F("CPU freq"),      intToString(ESP.getCpuFreqMHz()) + "MHz");
#ifdef ESP32
  s += c + jsonPair(F("Chip revision"), ESP.getChipRevision());
#endif
  s += c + jsonPair(F("SDK version"),   String(ESP.getSdkVersion())); // esp32 ??? ESP.getFullVersion()
#ifdef ESP8266
  s += c + jsonPair(F("Core version"),  ESP.getCoreVersion());
#endif
  const char compile_date[] = __DATE__ " " __TIME__;
  s += c + jsonPair(F("Built"),  String(compile_date));
  // ====== RAM =====
#ifdef ESP32  
  s += c + jsonPair(F("Heap size"),     formatBytes(ESP.getHeapSize()));
  s += c + jsonPair(F("Heap free"),     formatBytes(ESP.getFreeHeap()));
  s += c + jsonPair(F("Heap min"),      formatBytes(ESP.getMinFreeHeap()));
  s += c + jsonPair(F("Heap max alloc"),formatBytes(ESP.getMaxAllocHeap()));
#endif
#ifdef ESP8266
  s += c + jsonPair(F("Heap free"),      formatBytes(ESP.getFreeHeap()));
  s += c + jsonPair(F("Heap free block"),formatBytes(ESP.getMaxFreeBlockSize()));
  s += c + jsonPair(F("Heap fragm"),     String(ESP.getHeapFragmentation()) + "%");
#endif
  // ====== SPI ======
#ifdef ESP32
  s += c + jsonPair(F("SPI size"),      formatBytes(ESP.getPsramSize()));
  s += c + jsonPair(F("SPI Free"),      formatBytes(ESP.getFreePsram()));
  s += c + jsonPair(F("SPI min"),       formatBytes(ESP.getMinFreePsram()));
  s += c + jsonPair(F("SPI max alloc"), formatBytes(ESP.getMaxAllocPsram()));
#endif  
  // ====== Flash ======
  s += c + jsonPair(F("Flash chip speed"), String(ESP.getFlashChipSpeed()/1000000.0) + "MHz");  // ? on esp32 return 0
#ifdef ESP8266
  s += c + jsonPair(F("Flash chip id"), int32_t((ESP.getFlashChipId())));   
  s += c + jsonPair(F("Flash size"),    formatBytes(ESP.getFlashChipRealSize()));
#else
  s += c + jsonPair(F("Flash size"),    formatBytes(ESP.getFlashChipSize()));
#endif
  s += c + jsonPair(F("Firmware size"), formatBytes(ESP.getSketchSize()));         // esp32
  s += c + jsonPair(F("Free FW space"),  formatBytes(ESP.getFreeSketchSpace()));   // esp32
 
  // ================= LittleFS ================
  if (!beginFS() ) {
    lastErrorStr = F("LittleFS Mount Failed");
  }

#ifdef ESP8266
  FSInfo fs_info;  
  bool fs = ESP_FS.info(fs_info);
  if (fs) {
    s += c + jsonPair(F("FS size"),      formatBytes(fs_info.totalBytes));
    s += c + jsonPair(F("FS used size"), formatBytes(fs_info.usedBytes));
  } else {
    s += c + jsonPair(F("FS"), "No SPIFFS partition");
  }
#elif defined(ESP32)
  s += c + jsonPair(F("FS size"),      formatBytes(ESP_FS.totalBytes()));
  s += c + jsonPair(F("FS used size"), formatBytes(ESP_FS.usedBytes()));
#endif
  // ================= WiFi ================
  if (WiFi.getMode() == WIFI_STA) {
    s += c + jsonPair(F("SSID"),    WiFi.SSID());
    // s += c + jsonPair("psk",  WiFi.psk());
    s += c + jsonPair(F("BSSID"),   WiFi.BSSIDstr());
    s += c + jsonPair(F("RSSI"),    WiFi.RSSI());
    s += c + jsonPair(F("IP"),      WiFi.localIP().toString());
    s += c + jsonPair(F("MAC"),     WiFi.macAddress());
  } else {
    s += c + jsonPair(F("IP"),      WiFi.softAPIP().toString());
    s += c + jsonPair(F("MAC"),     WiFi.softAPmacAddress());
  }
  s += c + jsonPair(F("UpTime"),    getUpTimeString());
  s += c + jsonPair(F("Reset Reason"), getResetReasonString());
  s += c + jsonPair(F("Last Error"), lastErrorStr);
  s += F("}");
  return s;
}

String getSystemStatusJson(int errWiFi, int errMqtt) {
  String c = ",";
  String s = "{";

  s +=     jsonPair(F("Time"),      timeToString());
  s += c + jsonPair(F("UpTime"),    getUpTimeString());
  s += c + jsonPair(F("Free heap"), formatBytes(ESP.getFreeHeap()));  // esp32
  s += c + jsonPair(F("SSID"),      WiFi.SSID());
  // s c + += jsonPair(F("psk"),    WiFi.psk());
  s += c + jsonPair(F("BSSID"),     WiFi.BSSIDstr());
  s += c + jsonPair(F("RSSI"),      WiFi.RSSI());
  s += c + jsonPair(F("IP"),        WiFi.localIP().toString());
  s += c + "\"Disconnections\":{";
    s +=     jsonPair(F("WiFi"),  errWiFi);
    s += c + jsonPair(F("MQTT"),  errMqtt);
  s += "}";

  s += "}";

return s;
}

String addTrailingSlash(String str) {
  String tmpStr = str;
  tmpStr.trim();

  // Get rid of any initial /
  if (tmpStr.substring(0, 1) == "/") tmpStr = tmpStr.substring(1);

  // Add trailing / if required
  int lenTopic = tmpStr.length();
  if (tmpStr.substring(lenTopic - 1, lenTopic) != "/") tmpStr += F("/");
  return tmpStr;
}

bool isdigit(char n) { return (n >= '0' && n <= '9') ? true : false; }

String boolToString(bool b, String t, String f) { return b ? t : f; }

String boolToString(bool b) { return boolToString(b, "true", "false"); }

String boolSuccess(bool b) { return boolToString(b, SUCCESS_STR, FAIL_STR); }

String quotedText(String s) { return "\"" + s + "\""; }

String quotedText(const char* s) { return "\"" + String(s) + "\""; }




String jsonPair(String s1, String s2) {
  return (quotedText(s1) + ":" + quotedText(s2));
}

String jsonPair(String s1, int32_t i) {
  return (quotedText(s1) + ":" + intToString(i));
}

String jsonPair(String s1, bool b) {
  return (quotedText(s1) + ":" + boolToString(b));
}
String jsonPair(String s1, float f) {
  char buf[16];
  sprintf(buf, "%.2f", f);
  return (quotedText(s1) + ":" + String(buf));
}

String jsonPair(const char* s1, const char* s2) {
  return quotedText(s1) + ":" + quotedText(s2);
}

bool isTrue(String s) {
  s.toLowerCase();
  return (s == "true");
}



String TwoDigitNumber(int n) {
  if (n >= 0 && n < 10)
    return '0' + String(n);
  else
    return String(n);
}

/*
//https://github.com/esp8266/Arduino/blob/master/libraries/esp8266/examples/RTCUserMemory/RTCUserMemory.ino
// read write to RTC memory

// next functions were found at espEasy/misc.ino
int calc_CRC16(const String& text) {
  return calc_CRC16(text.c_str(), text.length());
}

int calc_CRC16(const char *ptr, int count)
{
    int  crc;
    crc = 0;
    while (--count >= 0)
    {
        crc = crc ^ (int) *ptr++ << 8;
        char i = 8;
        do
        {
            if (crc & 0x8000)
                crc = crc << 1 ^ 0x1021;
            else
                crc = crc << 1;
        } while(--i);
    }
    return crc;
}

uint32_t calc_CRC32(const uint8_t *data, size_t length) {
  uint32_t crc = 0xffffffff;
  while (length--) {
    uint8_t c = *data++;
    for (uint32_t i = 0x80; i > 0; i >>= 1) {
      bool bit = crc & 0x80000000;
      if (c & i) {
        bit = !bit;
      }
      crc <<= 1;
      if (bit) {
        crc ^= 0x04c11db7;
      }
    }
  }
  return crc;
}



String getWifiModeString(WiFiMode_t wifimode)
{
  switch (wifimode) {
    case WIFI_OFF:   return F("OFF");
    case WIFI_STA:   return F("STA");
    case WIFI_AP:    return F("AP");
    case WIFI_AP_STA: return F("AP+STA");
    default:
      break;
  }
  return F("Unknown");
}

// taken fron H4
vector<string> split(const string& s, const char* delimiter){
	vector<string> vt;
	string delim(delimiter);
	auto len=delim.size();
	auto start = 0U;
	auto end = s.find(delim);
	while (end != string::npos){
		vt.push_back(s.substr(start, end - start));
		start = end + len;
		end = s.find(delim, start);
	}
	string tec=s.substr(start, end);
	if(tec.size()) vt.push_back(tec);		
	return vt;
}

*/