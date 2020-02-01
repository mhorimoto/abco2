void vlv_ctrl(int ope,int ccmid) {
  extern UECSCCM U_ccmList[];
  switch(ope) {
  case VLV1_OPEN:
    U_ccmList[ccmid].value |= 0b1;  // OPEN
    set_VLV_SELECT[0] = 2;
    digitalWrite(D_VLV1_NORM,HIGH);
    digitalWrite(D_VLV1_REV,LOW);
    break;
  case VLV2_OPEN:
    U_ccmList[ccmid].value |= 0b10; // OPEN
    set_VLV_SELECT[1] = 2;
    digitalWrite(D_VLV2_NORM,HIGH);
    digitalWrite(D_VLV2_REV,LOW);
    break;
  case VLV3_OPEN:
    U_ccmList[ccmid].value |= 0b100;  // OPEN
    set_VLV_SELECT[2] = 2;
    digitalWrite(D_VLV3_NORM,HIGH);
    digitalWrite(D_VLV3_REV,LOW);
    break;
  case VLV4_OPEN:
    U_ccmList[ccmid].value |= 0b1000;  // OPEN
    set_VLV_SELECT[3] = 2;
    digitalWrite(D_VLV4_NORM,HIGH);
    digitalWrite(D_VLV4_REV,LOW);
    break;
  case VLV5_OPEN:
    U_ccmList[ccmid].value |= 0b10000;  // OPEN
    set_VLV_SELECT[4] = 2;
    digitalWrite(D_VLV5_NORM,HIGH);
    digitalWrite(D_VLV5_REV,LOW);
    break;
  case VLV6_OPEN:
    U_ccmList[ccmid].value |= 0b100000;  // OPEN
    set_VLV_SELECT[5] = 2;
    digitalWrite(D_VLV6_NORM,HIGH);
    digitalWrite(D_VLV6_REV,LOW);
    break;
  case VLV7_OPEN:
    U_ccmList[ccmid].value |= 0b1000000;  // OPEN
    set_VLV_SELECT[6] = 2;
    digitalWrite(D_VLV7_NORM,HIGH);
    digitalWrite(D_VLV7_REV,LOW);
    break;
  case VLV1_CLOSE:
    U_ccmList[ccmid].value &= 0b01111111111111110000111111111110;  // CLOSE
    set_VLV_SELECT[0] = 1;
    digitalWrite(D_VLV1_NORM,LOW);
    digitalWrite(D_VLV1_REV,HIGH);
    break;
  case VLV2_CLOSE:
    U_ccmList[ccmid].value &= 0b01111111111111110000111111111101;  // CLOSE
    set_VLV_SELECT[1] = 1;
    digitalWrite(D_VLV2_NORM,LOW);
    digitalWrite(D_VLV2_REV,HIGH);
    break;
  case VLV3_CLOSE:
    U_ccmList[ccmid].value &= 0b01111111111111110000111111111011;  // CLOSE
    set_VLV_SELECT[2] = 1;
    digitalWrite(D_VLV3_NORM,LOW);
    digitalWrite(D_VLV3_REV,HIGH);
    break;
  case VLV4_CLOSE:
    U_ccmList[ccmid].value &= 0b01111111111111110000111111110111;  // CLOSE
    set_VLV_SELECT[3] = 1;
    digitalWrite(D_VLV4_NORM,LOW);
    digitalWrite(D_VLV4_REV,HIGH);
    break;
  case VLV5_CLOSE:
    U_ccmList[ccmid].value &= 0b01111111111111110000111111101111;  // CLOSE
    set_VLV_SELECT[4] = 1;
    digitalWrite(D_VLV5_NORM,LOW);
    digitalWrite(D_VLV5_REV,HIGH);
    break;
  case VLV6_CLOSE:
    U_ccmList[ccmid].value &= 0b01111111111111110000111111011111;  // CLOSE
    set_VLV_SELECT[5] = 1;
    digitalWrite(D_VLV6_NORM,LOW);
    digitalWrite(D_VLV6_REV,HIGH);
    break;
  case VLV7_CLOSE:
    U_ccmList[ccmid].value &= 0b01111111111111110000111110111111;  // CLOSE
    set_VLV_SELECT[6] = 1;
    digitalWrite(D_VLV7_NORM,LOW);
    digitalWrite(D_VLV7_REV,HIGH);
    break;
  case VLV1_UNKNOWN:
    set_VLV_SELECT[0] = 0;
    digitalWrite(D_VLV1_NORM,LOW);
    digitalWrite(D_VLV1_REV,LOW);
    break;
  case VLV2_UNKNOWN:
    set_VLV_SELECT[1] = 0;
    digitalWrite(D_VLV2_NORM,LOW);
    digitalWrite(D_VLV2_REV,LOW);
    break;
  case VLV3_UNKNOWN:
    set_VLV_SELECT[2] = 0;
    digitalWrite(D_VLV3_NORM,LOW);
    digitalWrite(D_VLV3_REV,LOW);
    break;
  case VLV4_UNKNOWN:
    set_VLV_SELECT[3] = 0;
    digitalWrite(D_VLV4_NORM,LOW);
    digitalWrite(D_VLV4_REV,LOW);
    break;
  case VLV5_UNKNOWN:
    set_VLV_SELECT[4] = 0;
    digitalWrite(D_VLV5_NORM,LOW);
    digitalWrite(D_VLV5_REV,LOW);
    break;
  case VLV6_UNKNOWN:
    set_VLV_SELECT[5] = 0;
    digitalWrite(D_VLV6_NORM,LOW);
    digitalWrite(D_VLV6_REV,LOW);
    break;
  case VLV7_UNKNOWN:
    set_VLV_SELECT[6] = 0;
    digitalWrite(D_VLV7_NORM,LOW);
    digitalWrite(D_VLV7_REV,LOW);
    break;
  }
}
