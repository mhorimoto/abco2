void init_mcp9600(void) {
  int i;
  //  Serial.begin(115200);
  for(i=0;i<8;i++) {
    if (! mcp[i].begin(mcp96_addr[i])) {
      Serial.print(mcp96_addr[i],HEX);
      Serial.println(" MCP not found");
      mcp96_present[i] = false;
    } else {
      Serial.print("Found MCP address 0x");
      Serial.println(mcp96_addr[i],HEX);
      show_ADCresolution(i);
      mcp[i].setThermocoupleType(MCP9600_TYPE_T);
      show_ThermocoupleType(i);
      mcp[i].setFilterCoefficient(3);
      Serial.print("Filter coefficient value set to: ");
      Serial.println(mcp[i].getFilterCoefficient());
      mcp[i].enable(true);
      mcp96_present[i] = true;
    }
  }
  //  Serial.end();
}

int show_ADCresolution(int id) {
  int ret;
  Serial.print("ADC resolution set to ");
  switch (mcp[id].getADCresolution()) {
    case MCP9600_ADCRESOLUTION_18:
      Serial.print("18");
      ret = 18;
      break;
    case MCP9600_ADCRESOLUTION_16:
      Serial.print("16");
      ret = 16;
      break;
    case MCP9600_ADCRESOLUTION_14:
      Serial.print("14");
      ret = 14;
      break;
    case MCP9600_ADCRESOLUTION_12:
      Serial.print("12");
      ret = 12;
      break;
  }
  Serial.println(" bits");
  return(ret);
}

void show_Sampling_Data(int id) {
  Serial.begin(115200);
  Serial.print("Hot Junction: "); Serial.println(mcp[id].readThermocouple());
  Serial.print("Cold Junction: "); Serial.println(mcp[id].readAmbient());
  Serial.print("ADC: "); Serial.print(mcp[id].readADC() * 2); Serial.println(" uV");
  Serial.end();
}

void show_ThermocoupleType(int id) {
  Serial.print("Thermocouple type set to ");
  switch (mcp[id].getThermocoupleType()) {
    case MCP9600_TYPE_K:  Serial.print("K"); break;
    case MCP9600_TYPE_J:  Serial.print("J"); break;
    case MCP9600_TYPE_T:  Serial.print("T"); break;
    case MCP9600_TYPE_N:  Serial.print("N"); break;
    case MCP9600_TYPE_S:  Serial.print("S"); break;
    case MCP9600_TYPE_E:  Serial.print("E"); break;
    case MCP9600_TYPE_B:  Serial.print("B"); break;
    case MCP9600_TYPE_R:  Serial.print("R"); break;
  }
  Serial.println(" type");
}

void test_mcp9600(void) {
  int aval,smenu;
  float temp;
  extern char lcdtext[];
  aval = analogRead(2);
  smenu = (aval/128)+1;
  Reset_lcdtext();
  sprintf(lcdtext,"PORT-%d         ",smenu);
  lcd.setCursor(0,0);
  lcd.print(lcdtext);
  temp = mcp[smenu-1].readThermocouple();
  lcd.setCursor(0,1);
  lcd.print("           ");
  lcd.setCursor(11,1);
  lcd.print(temp);
}
