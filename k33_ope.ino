void k33_ope() {
  int rc,co2lp,co2icb;
  extern int co2detail,co2bigger;
  
  rc = k33lp.readCO2(co2lp);
  if (rc==0) {
    U_ccmList[CCMID_K33LP].value = co2lp;
    co2detail = co2lp;
  } else {
    U_ccmList[CCMID_K33LP].value = (-1) * rc;
  }
  rc = k33icb.readCO2(co2icb);
  if (rc==0) {
    U_ccmList[CCMID_K33ICB].value = co2icb;
    co2bigger = co2icb;
  } else {
    U_ccmList[CCMID_K33ICB].value = (-1) * rc;
  }
}
