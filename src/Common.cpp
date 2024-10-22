#include <Arduino.h>
// Get Chip ID
String getChipNumber()
{
  uint32_t chipId = 0;
  for (int i = 0; i < 17; i = i + 8)
    chipId ^= ((ESP.getEfuseMac() >> (40 - i)) & 0xff0) << i;
  String s = String(chipId);
  while (s.length() > 16)
    s = s.substring(1);
  while (s.length() < 4)
    s = String("0") + s;
  return s;
}
String getChipNumberShort()
{
  String s = getChipNumber();
  return s.substring(s.length() - 4, s.length());
}
