void vlv_ctrl(int ope,int ccmid) {
  extern UECSCCM U_ccmList[];
  switch(ope) {
  case VLV1_OPEN:
    U_ccmList[ccmid].value |= 0b1;  // OPEN
    digitalWrite(D_VLV1_NORM,HIGH);
    digitalWrite(D_VLV1_REV,LOW);
    break;
  case VLV2_OPEN:
    U_ccmList[ccmid].value |= 0b10; // OPEN
    digitalWrite(D_VLV2_NORM,HIGH);
    digitalWrite(D_VLV2_REV,LOW);
    break;
  case VLV3_OPEN:
    U_ccmList[ccmid].value |= 0b100;  // CLOSE
    digitalWrite(D_VLV3_NORM,HIGH);
    digitalWrite(D_VLV3_REV,LOW);
    break;
  case VLV4_OPEN:
    U_ccmList[ccmid].value |= 0b1000;  // CLOSE
    digitalWrite(D_VLV4_NORM,HIGH);
    digitalWrite(D_VLV4_REV,LOW);
    break;
  case VLV5_OPEN:
    U_ccmList[ccmid].value |= 0b10000;  // CLOSE
    digitalWrite(D_VLV5_NORM,HIGH);
    digitalWrite(D_VLV5_REV,LOW);
    break;
  case VLV6_OPEN:
    U_ccmList[ccmid].value |= 0b100000;  // CLOSE
    digitalWrite(D_VLV6_NORM,HIGH);
    digitalWrite(D_VLV6_REV,LOW);
    break;
  case VLV7_OPEN:
    U_ccmList[ccmid].value |= 0b1000000;  // CLOSE
    digitalWrite(D_VLV7_NORM,HIGH);
    digitalWrite(D_VLV7_REV,LOW);
    break;
  case VLV1_CLOSE:
    U_ccmList[ccmid].value &= 0b01111111111111110000111111111110;  // CLOSE
    digitalWrite(D_VLV1_NORM,LOW);
    digitalWrite(D_VLV1_REV,HIGH);
    break;
  case VLV2_CLOSE:
    U_ccmList[ccmid].value &= 0b01111111111111110000111111111101;  // CLOSE
    digitalWrite(D_VLV2_NORM,LOW);
    digitalWrite(D_VLV2_REV,HIGH);
    break;
  case VLV3_CLOSE:
    U_ccmList[ccmid].value &= 0b01111111111111110000111111111011;  // CLOSE
    digitalWrite(D_VLV3_NORM,LOW);
    digitalWrite(D_VLV3_REV,HIGH);
    break;
  case VLV4_CLOSE:
    U_ccmList[ccmid].value &= 0b01111111111111110000111111110111;  // CLOSE
    digitalWrite(D_VLV4_NORM,LOW);
    digitalWrite(D_VLV4_REV,HIGH);
    break;
  case VLV5_CLOSE:
    U_ccmList[ccmid].value &= 0b01111111111111110000111111101111;  // CLOSE
    digitalWrite(D_VLV5_NORM,LOW);
    digitalWrite(D_VLV5_REV,HIGH);
    break;
  case VLV6_CLOSE:
    U_ccmList[ccmid].value &= 0b01111111111111110000111111011111;  // CLOSE
    digitalWrite(D_VLV6_NORM,LOW);
    digitalWrite(D_VLV6_REV,HIGH);
    break;
  case VLV7_CLOSE:
    U_ccmList[ccmid].value &= 0b01111111111111110000111110111111;  // CLOSE
    digitalWrite(D_VLV7_NORM,LOW);
    digitalWrite(D_VLV7_REV,HIGH);
    break;
  case VLV1_UNKNOWN:
    digitalWrite(D_VLV1_NORM,LOW);
    digitalWrite(D_VLV1_REV,LOW);
    break;
  case VLV2_UNKNOWN:
    digitalWrite(D_VLV2_NORM,LOW);
    digitalWrite(D_VLV2_REV,LOW);
    break;
  case VLV3_UNKNOWN:
    digitalWrite(D_VLV3_NORM,LOW);
    digitalWrite(D_VLV3_REV,LOW);
    break;
  case VLV4_UNKNOWN:
    digitalWrite(D_VLV4_NORM,LOW);
    digitalWrite(D_VLV4_REV,LOW);
    break;
  case VLV5_UNKNOWN:
    digitalWrite(D_VLV5_NORM,LOW);
    digitalWrite(D_VLV5_REV,LOW);
    break;
  case VLV6_UNKNOWN:
    digitalWrite(D_VLV6_NORM,LOW);
    digitalWrite(D_VLV6_REV,LOW);
    break;
  case VLV7_UNKNOWN:
    digitalWrite(D_VLV7_NORM,LOW);
    digitalWrite(D_VLV7_REV,LOW);
    break;
  }
}
