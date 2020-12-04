#include <helper_wifi.h>

bool wifiIsAP(WiFiMode_t wifimode)
{
  #if defined(ESP32)
  return (wifimode == WIFI_MODE_AP) || (wifimode == WIFI_MODE_APSTA);
  #else // if defined(ESP32)
  return (wifimode == WIFI_AP) || (wifimode == WIFI_AP_STA);
  #endif // if defined(ESP32)
}

bool wifiIsSTA(WiFiMode_t wifimode)
{
  #if defined(ESP32)
  return (wifimode & WIFI_MODE_STA) != 0;
  #else // if defined(ESP32)
  return (wifimode & WIFI_STA) != 0;
  #endif // if defined(ESP32)
}

bool wifiValidIP() {  // todo Return false for 169.254.x.x or fe80::/64
  IPAddress ip = WiFi.localIP();
  if ((ip[0] == 169) && (ip[1] == 254)) return false;
  return (static_cast<uint32_t>(WiFi.localIP()) != 0);
}

bool wifiConnected() {
  return (WiFi.RSSI() < 0) && WiFi.isConnected() && wifiValidIP();
}

int wifiGetRssiAsQuality(int rssi) {
  int quality = 0;

  if (rssi <= -100) {
    quality = 0;
  } else if (rssi >= -50) {
    quality = 100;
  } else {
    quality = 2 * (rssi + 100);
  }
  return quality;
}

String getWifiModeString(WiFiMode_t wifimode) {
  switch (wifimode) {
    case WIFI_OFF:    return F("OFF");
    case WIFI_STA:    return F("STA");
    case WIFI_AP:     return F("AP");
    case WIFI_AP_STA: return F("AP+STA");
    default:
      break;
  }
  return F("Unknown");
}

String getEncryptionTypeStr(uint8_t authmode) {
  switch (authmode) {
#ifdef ESP32
    case WIFI_AUTH_OPEN:
      return F("open");
      break;
    case WIFI_AUTH_WEP:
      return F("WEP");
      break;
    case WIFI_AUTH_WPA_PSK:
      return F("WPA/PSK");
      break;
    case WIFI_AUTH_WPA2_PSK:
      return F("WPA2/PSK");
      break;
    case WIFI_AUTH_WPA_WPA2_PSK:
      return F("WPA/WPA2/PSK");
      break;
    case WIFI_AUTH_WPA2_ENTERPRISE:
      return F("WPA2 Enterprise");
      break;
#else  // ifdef ESP32
    case ENC_TYPE_WEP:
      return F("WEP");
      break;
    case ENC_TYPE_TKIP:
      return F("WPA/PSK");
      break;
    case ENC_TYPE_CCMP:
      return F("WPA2/PSK");
      break;
    case ENC_TYPE_NONE:
      return F("Open");
      break;
    case ENC_TYPE_AUTO:
      return F("WPA/WPA2/PSK");
      break;
#endif
    default:
      break;
  }
  return "Unknown";
}
