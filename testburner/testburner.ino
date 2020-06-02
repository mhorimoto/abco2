#define MIS 19
#define BUP 27
int b,m,vb,vm;

void setup() {
  pinMode(MIS,INPUT_PULLUP);
  pinMode(BUP,INPUT_PULLUP);
  Serial.begin(115200);
  Serial.println("BEGIN");
}
void loop() {
  vb = digitalRead(BUP);
  vm = digitalRead(MIS);
  if (vb!=b) {
    Serial.print("BURNER=");
    if (vb==HIGH) {
      Serial.println("HIGH");
    } else {
      Serial.println("LOW");
    }
  }
  b=vb;
  if (vm!=m) {
    Serial.print("MISS=");
    if (vm==HIGH) {
      Serial.println("HIGH");
    } else {
      Serial.println("LOW");
    }
  }
  m=vm;
}
