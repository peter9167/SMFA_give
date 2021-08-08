#include <TimerOne.h> // TimeOne라이브러리 사용
#include <DHT.h> // DHT라이브러리 사용
#define DHTPIN 4 //핀 번호 설정
#define DHTTYPE DHT11 // DHT 사용 번호 설정 (DHT11)
DHT dht(DHTPIN, DHTTYPE); // DHT 설정



//-------타이머 인터럽트를 위한 타이머 카운트------------
int time_count = 0;
int hum_count = 0;
int temp_count = 0;



//--------초기함수(setup)----------------------------
void setup(){
  Serial.begin(9600); // 시리얼 모니터 통신속도 9600 설정
  
  dht.begin(); // DHT 초기화
  
  Timer1.initialize(1000); //1000000μs = 1s, 1000us = 0.001s, 1000us = 1ms
  Timer1.attachInterrupt(timerIsr);
}



//--------루프함수(loop)----------------------------
void loop(){
    //--------------온도/습도 값 읽기 및 변수 선언-----------------
    
    int h = dht.readHumidity(); //습도 값 저장 변수 선언
    int t = dht.readTemperature(); //온도 값 저장 변수 선언
    int h_pwm, t_pwm; // 온도/습도 값 PWM 전달 값 변수 선언



    //------map함수를 이용한 온도 습도 PWM 신호 값 변환---------
    
    h_pwm = map(h, 20, 90, 0, 255); // 습도 값 PWM 신호 값 변환
    t_pwm = map(t, 0, 50, 0, 255); // 온도 값 PWM 신호 값 변환



    //------습도 값 읽기 카운터-------
    if(hum_count == 1000){
      hum(h, h_pwm);
      hum_count = 0;
    }

    
    //------온도 값 읽기 카운터-------
    if(temp_count == 1100){
      temp(t, t_pwm);
      temp_count = 0;
    }
}

//------------인터럽트를 위한 카운터 호출 함수----------------------
void timerIsr(){
  time_count += 1;
  hum_count += 1;
  temp_count += 1;
}



//------------습도 값 시리얼 출력 및 PWM 값 전달 호출 함수----------------------
void hum(int h, int h_pwm){
    int h_a = 3; //PWM 습도 출력 핀
    Serial.print("습도:");
    Serial.println(h); // 습도 값 출력
    Serial.print("PWM 신호 습도:");
    Serial.println(h_pwm); // PWM 신호 습도 값 출력
    analogWrite(h_a, h_pwm); //PWM 출력
}



//------------온도 값 시리얼 출력 및 PWM 값 전달 호출 함수----------------------
void temp(int t, int t_pwm){
    int t_a = 5; //PWM 온도 출력 핀
    Serial.print("온도:");
    Serial.println(t); // 온도 값 출력
    Serial.print("PWM 신호 온도:");
    Serial.println(t_pwm); // PWM 신호 온도 값 출력
    analogWrite(t_a, t_pwm); //PWM 출력
}
